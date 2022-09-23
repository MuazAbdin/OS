/**
 * @file: MapReduceFramework.cpp
 * @authors: Muaz.Abdeen (300575297)
 *           almogco (207005950)
 * @brief:
 */

// ------------------------------ includes ------------------------------------------
#include <pthread.h>
#include <semaphore.h>
#include <cstdlib>
#include <cstdio>
#include <atomic>
#include <algorithm>
#include <vector>

#include "Barrier.h"
#include "MapReduceFramework.h"

// ------------------------------ macros & constants --------------------------------

#define SHUFFLE_T 0

// ------------------------------ GLOBAL VARIABLES -----------------------------------

// ------------------------------ HELPER STRUCTURES ----------------------------------

typedef struct ThreadContext ThreadContext;
typedef struct JobContext JobContext;
typedef std::vector<IntermediateVec> ShuffledVec;

/**
 * A struct includes all parameters which are relevant to a thread
 */
struct ThreadContext {
	int threadID;
	JobContext* jc;

	const MapReduceClient* client;
	const InputVec* inputVec;
	IntermediateVec intermediateVec;
	OutputVec* outputVec;

	pthread_mutex_t* mutex;
	std::atomic<uint64_t>* counter;
	Barrier* barrier;
};

/**
 * A srtuct which includes all the parameters which are relevant to the job.
 * The pointer to this struct can be casted to JobHandle.
 */
struct JobContext {
	size_t totalThreads;
	pthread_t* threads;
	ThreadContext* contexts;

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_t waitJobMutex = PTHREAD_MUTEX_INITIALIZER;
	std::atomic<uint64_t> counter{0};
	Barrier* barrier;

	bool waiting = false;
	ShuffledVec shuffledVec;
	uint64_t totalIntermediatePairs;
};

// ------------------------------ HELPER FUNCTIONS ----------------------------------

void initJobContext(JobContext* context, const MapReduceClient& client,
					const InputVec& inputVec, OutputVec& outputVec,
					int multiThreadLevel);
void* threadRoutine(void* arg);
void lockMutex(pthread_mutex_t *mutex);
void unlockMutex(pthread_mutex_t *mutex);
void freeContext(JobContext* jobContext);
void destroyMutexes(JobContext* jobContext);

// ----------------------------------------------------------------------------------
/**
 * @brief initializes the JobContext struct
 * @param context The JobContext to be initialized.
 * @param outputVec The output vector.
 * @param multiThreadLevel max number of working threads.
 */
void initJobContext(JobContext* context, const MapReduceClient& client,
					const InputVec& inputVec, OutputVec& outputVec,
					int multiThreadLevel) {
	context->totalThreads = multiThreadLevel;
	context->threads = new pthread_t[multiThreadLevel];
	context->contexts = new ThreadContext[multiThreadLevel];
	context->barrier = new Barrier(multiThreadLevel);

	for (int i = 0; i < multiThreadLevel; ++i) {
		context->contexts[i].threadID = i;
		context->contexts[i].jc = context;
		context->contexts[i].client = &client;
		context->contexts[i].inputVec = &inputVec;
		context->contexts[i].outputVec = &outputVec;
		context->contexts[i].mutex = &(context->mutex);
		context->contexts[i].counter = &(context->counter);
		context->contexts[i].barrier = context->barrier;
	}
}

/**
 * @brief The start routine each one of the threads will call
 * @param arg argument for this routine
 * @return
 */
void* threadRoutine(void* arg) {
	auto tc =  static_cast<ThreadContext*>(arg);

	// (1) The MAP phase
	if ((*(tc->counter)) >> 62 != 1) { (*(tc->counter)) = (uint64_t) 1 << 62; }
	uint64_t inputElements = (*(tc->inputVec)).size();
	uint64_t oldValue = (*(tc->counter))++;
	while((oldValue & (0x7FFFFFFF)) < inputElements) {
		InputPair inputPair = (*(tc->inputVec))[oldValue & (0x7FFFFFFF)];
		tc->client->map(inputPair.first, inputPair.second, tc);
		oldValue = (*(tc->counter))++;
	}
	tc->barrier->barrier();

	// (2) The SORT & SHUFFLE phase
	if (tc->threadID == SHUFFLE_T) {
		lockMutex(tc->mutex);
		tc->jc->totalIntermediatePairs = (*(tc->counter))>>31 & (0x7FFFFFFF);
		(*(tc->counter)) = (uint64_t) 2 << 62;

		IntermediateVec allPairs;
		for (size_t i = 0; i < tc->jc->totalThreads; ++i) {
			while (!tc->jc->contexts[i].intermediateVec.empty()) {
				IntermediatePair pair = tc->jc->contexts[i].intermediateVec.back();
				tc->jc->contexts[i].intermediateVec.pop_back();
				allPairs.push_back(pair);
			}
		}

		// sorting the allPairs vector
		std::sort(allPairs.begin(), allPairs.end(),
				  [](IntermediatePair p1, IntermediatePair p2) { return *(p1.first) < *(p2.first); });

		while (!allPairs.empty()) {
			IntermediateVec uniKeyVec;
			IntermediatePair uniPair = allPairs.back();
			while (!(*(uniPair.first) < *(allPairs.back().first)) &&
				   !(*(allPairs.back().first) < *(uniPair.first))) {
				IntermediatePair pair = allPairs.back();
				allPairs.pop_back();
				uniKeyVec.push_back(pair);
				/* update the number of already processed keys */
				(*(tc->counter)) += (uint64_t) 1 << 31;
				if  (allPairs.empty()) { break; }
			}
			tc->jc->shuffledVec.push_back(uniKeyVec);
		}

		(*(tc->counter)) = (uint64_t) 3 << 62;
		unlockMutex(tc->mutex);
	}
	tc->barrier->barrier();

	// (3) The REDUCE phase
	uint64_t shuffled = tc->jc->shuffledVec.size();
	oldValue = (*(tc->counter))++;
	while((oldValue & (0x7FFFFFFF)) < shuffled) {
		IntermediateVec intermediateVec = tc->jc->shuffledVec[oldValue & (0x7FFFFFFF)];
		tc->client->reduce(&intermediateVec, tc);
		(*(tc->counter)) += (uint64_t) intermediateVec.size() << 31;
		oldValue = (*(tc->counter))++;
	}

	return nullptr;
}

void lockMutex (pthread_mutex_t *mutex) {
	if (pthread_mutex_lock(mutex) != 0){
		fprintf(stderr, "system error: error on pthread_mutex_lock\n");
		exit(EXIT_FAILURE);
	}
}

void unlockMutex (pthread_mutex_t *mutex) {
	if (pthread_mutex_unlock(mutex) != 0) {
		fprintf(stderr, "system error: error on pthread_mutex_unlock\n");
		exit(EXIT_FAILURE);
	}
}

void freeContext(JobContext* jobContext) {
	if (jobContext != nullptr) {
		delete[] jobContext->threads;
		jobContext->threads = nullptr;
		delete[] jobContext->contexts;
		jobContext->contexts = nullptr;
		delete jobContext->barrier;
		jobContext->barrier = nullptr;
		delete jobContext;
		jobContext = nullptr;
	}
}

void destroyMutexes(JobContext* jobContext) {
	if (pthread_mutex_destroy(&(jobContext->mutex)) != 0 ||
		pthread_mutex_destroy(&(jobContext->waitJobMutex)) != 0) {
		fprintf(stderr, "system error: error on pthread_mutex_destroy\n");
		freeContext(jobContext);
		exit(EXIT_FAILURE);
	}
}

// ================================================================================== //
// ============================== LIBRARY API ======================================= //
// ================================================================================== //

void emit2 (K2* key, V2* value, void* context) {
	auto tc =  static_cast<ThreadContext*>(context);
	// I. saves the intermediary element in the context data structures (intermediate vector).
	IntermediatePair pair(key, value);
	tc->intermediateVec.push_back(pair);
	// II. updates the number of intermediary elements using atomic counter.
	*(tc->counter) += (uint64_t) 1 << 31;
}

void emit3 (K3* key, V3* value, void* context) {
	auto tc =  static_cast<ThreadContext*>(context);;
	lockMutex(tc->mutex);
	// I. saves the output element in the context data structures (output vector).
	OutputPair pair(key, value);
	tc->outputVec->push_back(pair);
	// II. updates the number of output elements using atomic counter.
	/** WE UPDATE IT OUTSIDE emit3 **/
	unlockMutex(tc->mutex);
}

JobHandle startMapReduceJob(const MapReduceClient& client,
							const InputVec& inputVec, OutputVec& outputVec,
							int multiThreadLevel) {
	// (1) create the job context
	JobContext* context;
	try {
		context = new JobContext();
		initJobContext(context, client, inputVec, outputVec, multiThreadLevel);
	} catch (std::bad_alloc& ba) {
		fprintf(stderr, "system error: error std::bad_alloc\n");
		destroyMutexes(context);
		freeContext(context);
		exit(EXIT_FAILURE);
	}

	// (2) create the job threads
	for (int i = 0; i < multiThreadLevel; ++i) {
		if (pthread_create(context->threads+i, NULL, threadRoutine, context->contexts+i) != 0) {
			fprintf(stderr, "system error: error on pthread_create\n");
			destroyMutexes(context);
			freeContext(context);
			exit(EXIT_FAILURE);
		}
	}

	return context;
}

void waitForJob(JobHandle job) {
	auto jc = static_cast<JobContext*>(job);
	pthread_mutex_lock((&jc->waitJobMutex));
	if (!jc->waiting) {
		jc->waiting = true;
		for (size_t i = 0; i < jc->totalThreads; ++i) {
			if (pthread_join(jc->threads[i], NULL) != 0) {
				fprintf(stderr, "system error: could not join threads number %d\n",
						jc->contexts[i].threadID);
				destroyMutexes(jc);
				freeContext(jc);
				exit(EXIT_FAILURE);
			}
		}
	}
	pthread_mutex_unlock((&jc->waitJobMutex));
}

void getJobState(JobHandle job, JobState* state) {
	auto jc =  static_cast<const JobContext*>(job);
	pthread_mutex_lock(jc->contexts[0].mutex);
	state->stage = static_cast<stage_t>(jc->counter >> 62);
	uint processed_keys;
	uint total_keys;
	switch (state->stage) {
		case MAP_STAGE:
			total_keys = jc->contexts[0].inputVec->size();
			processed_keys = jc->counter & (0x7FFFFFFF);
			state->percentage = (processed_keys < total_keys) ?
								100 * static_cast<float>(processed_keys)/total_keys : 100;
			break;
		case SHUFFLE_STAGE:
		case REDUCE_STAGE:
			total_keys = jc->totalIntermediatePairs;
			processed_keys = jc->counter>>31 & (0x7FFFFFFF);
			state->percentage = 100 * static_cast<float>(processed_keys)/total_keys;
//			state->percentage = (processed_keys < total_keys) ?
//								100 * static_cast<float>(processed_keys)/total_keys : 100;
			break;
		default:
			state->percentage = 0;
	}
	pthread_mutex_unlock(jc->contexts[0].mutex);
}

void closeJobHandle(JobHandle job) {
	/* In case that the function is called and the job is not finished yet
	 * wait until the job is finished to close it. */
	waitForJob(job);
	auto jc =  static_cast<JobContext*>(job);
	destroyMutexes(jc);
	freeContext(jc);
}