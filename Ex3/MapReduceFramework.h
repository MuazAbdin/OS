#ifndef MAPREDUCEFRAMEWORK_H
#define MAPREDUCEFRAMEWORK_H

#include "MapReduceClient.h"

/**
 * an identifier of a running job.
 * Returned when starting a job and used by other framework functions
 * (for example to get the state of a job).
 */
typedef void* JobHandle;

enum stage_t {UNDEFINED_STAGE=0, MAP_STAGE=1, SHUFFLE_STAGE=2, REDUCE_STAGE=3};

/**
 * @brief a struct which quantizes the state of a job, including:
 */
typedef struct {
	stage_t stage;
	float percentage;
} JobState;

/**
 * @brief This function saves the intermediary element (K2*, V2*) in the context
 * 	      data structures (intermediate vector).
 * 	   &  updates the number of intermediary elements using atomic counter.
 * @param key key of intermediary element.
 * @param value value of intermediary element.
 * @param context contains data structure (intermediate vector) of the thread that
 * 	      created the intermediary element
 */
void emit2 (K2* key, V2* value, void* context);

/**
 * @brief This function saves the output element (K3*, V3*) in the context
 * 	      data structures (output vector).
 * 	   &  updates the number of output elements using atomic counter.
 * @param key key of output element.
 * @param value value of output element.
 * @param context contains data structure (output vector) of the thread that
 * 	      created the output element.
 */
void emit3 (K3* key, V3* value, void* context);

/**
 * @brief This function starts running the MapReduce algorithm (with several threads)
 * 		  and returns a JobHandle.
 * 		  You can assume that the input to this function is valid.
 * @param client The implementation of MapReduceClient or in other words the task that
 * 		  the framework should run.
 * @param inputVec a vector of type std::vector<std::pair<K1*, V1*>>, the input elements.
 * @param outputVec a vector of type std::vector<std::pair<K3*, V3*>>, to which the output
 * 		  elements will be added before returning.
 * 		  You can assume that outputVec is empty.
 * @param multiThreadLevel the number of worker threads to be used for running the algorithm.
 *        You can assume multiThreadLevel argument is valid (greater or equal to 1).
 * @return The function returns JobHandle that will be used for monitoring the job.
 */
JobHandle startMapReduceJob(const MapReduceClient& client,
	const InputVec& inputVec, OutputVec& outputVec,
	int multiThreadLevel);

/**
 * @brief a function gets JobHandle returned by startMapReduceFramework
 * 		  and waits until it is finished
 *
 * @param job JobHandle returned by startMapReduceFramework
 */
void waitForJob(JobHandle job);

/**
 * @brief this function gets a JobHandle and updates the state of the job
 * 	      into the given JobState struct.
 * @param job JobHandle
 * @param state JobState
 */
void getJobState(JobHandle job, JobState* state);

/**
 * @brief Releasing all resources of a job.
 * 		  You should prevent releasing resources before the job finished.
 * 		  After this function is called the job handle will be invalid.
 * @param job JobHandle
 */
void closeJobHandle(JobHandle job);

	
	
#endif //MAPREDUCEFRAMEWORK_H
