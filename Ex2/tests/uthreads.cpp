/**
 * @file: uthreads.cpp
 * @authors: Muaz.Abdeen (300575297)
 *
 * @brief:
 */

// ------------------------------ includes ------------------------------------------

#include "uthreads.h"
#include <iostream>
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <list>
#include <unordered_set>

// ------------------------------- ADDRESS TRANSLATION -------------------------------

#ifdef __x86_64__
/* code for 64 bit Intel arch */

typedef unsigned long address_t;
#define JB_SP 6
#define JB_PC 7

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
	address_t ret;
	asm volatile("xor    %%fs:0x30,%0\n"
				 "rol    $0x11,%0\n"
	: "=g" (ret)
	: "0" (addr));
	return ret;
}

#else
/* code for 32 bit Intel arch */

typedef unsigned int address_t;
#define JB_SP 4
#define JB_PC 5

/* A translation is required when using an address of a variable.
   Use this as a black box in your code. */
address_t translate_address(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%gs:0x18,%0\n"
		"rol    $0x9,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}

#endif

// ------------------------------ macros & constants --------------------------------

#define FAILURE -1
#define SUCCESS 0
#define MAIN_THREAD concurrentThreads[0]
#define MAIN_TID 0
#define NO_THREAD -1

// ------------------------------ GLOBAL VARIABLES -----------------------------------

class Uthread {
public:
	int tid;
	int quanta = 0;
	char* tStack;
	sigjmp_buf env = {0};

	explicit Uthread(int tid=MAIN_TID, void (*f)()=nullptr): tid(tid) {
		tStack = new char[STACK_SIZE];
		address_t sp, pc;
		sp = (address_t) tStack + STACK_SIZE - sizeof(address_t);
		pc = (address_t) f;
		sigsetjmp(env, 1);
		(env->__jmpbuf)[JB_SP] = translate_address(sp);
		(env->__jmpbuf)[JB_PC] = translate_address(pc);
		sigemptyset(&env->__saved_mask);
	}

	~Uthread() { delete[] tStack; }
};

struct Mutex {
	bool isLocked;
	int tid;
};

static Uthread* concurrentThreads[MAX_THREAD_NUM]{nullptr};
static std::unordered_set<int> blockedThreads; // no defined order between blocked threads
static std::list<int> readyThreads;	// FIFO for Round-Robin algorithm
static std::list<int> mutexWaitingThreads;	// FIFO to achieve starvation freedom

static Uthread* runningThread;

static int totalThreads;
static int totalQuanta;

static sigset_t alarmSet;
static struct sigaction sa;
static struct itimerval timer;

static struct sigaction ignore = {.sa_handler= SIG_IGN};
static struct itimerval stopTimer = {0};

static Mutex mutex{false, NO_THREAD};

// ------------------------------ HELPER FUNCTIONS ----------------------------------

void setQuantumTimer(int quantum_usecs);
void timerHandler(int sig);
int setThreadID();
int getReadyThread();
bool isReady(int tid);
bool isBlocked(int tid);
bool isWaiting(int tid);
void terminateProcess();

// ----------------------------------------------------------------------------------

/* TODO: signal race solution:
 * I. disable timer		II. remove pending
 * III. set SIGVTALRM handler to be SIG_IGN */

void printInfo() {
	std::cout << "ALL THREADS" << std::endl;
	for (auto const &thread : concurrentThreads) {
		if (thread != nullptr) { std::cout << thread << std::endl; }
	}
}

void setQuantumTimer(int quantum_usecs) {
	// first time interval
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = quantum_usecs;

	// following time intervals
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = quantum_usecs;

//	// Start a virtual timer. It counts down whenever this process is executing.
//	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
//		std::cerr << "system error: setitimer error." << std::endl;
//		exit(EXIT_FAILURE);
//	}
}


void timerHandler(int sig) {
	// save the running thread context
	int ret_val = sigsetjmp(runningThread->env, 1);
	/* if returned from previous saved context, that means it is in the
	 * RUNNING now, so return from SIGVTALRM handler, and continue processing
	 * the thread (i.e. executing the function it points to) */
	if (ret_val != 0) {
//		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return;
	}

	const int nextTID = getReadyThread();
	/* In case SIGVTALRM is raised after thread self blocking, or moved to mutex waiting,
	 * checking isBlocked and isWaiting is needed in case the READY queue IS empty,
	 * because getReadyThread() returns the running tid in this case.
	 * (extreme case: main in mutexWaiting and the running thread blocked itself,
	 * or waiting for mutex which locked by a blocked thread !!!) */
	if ((nextTID == runningThread->tid) &&
		isBlocked(runningThread->tid) && isWaiting(runningThread->tid))
	{
		std::cerr << "DEADLOCK: READY & RUNNING are empty" << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}
	/* In case SIGVTALRM is raised after the quantum expired, then if READY is NOT empty,
	 * push running thread into READY queue if it is not blocked or waiting mutex, and
	 * get the next READY thread, else if READY queue IS empty, just keep executing the
	 * running thread for another quantum. */
	if ((nextTID != runningThread->tid) &&
		!isBlocked(runningThread->tid) && !isWaiting(runningThread->tid))
	{
		readyThreads.push_back(runningThread->tid);
		runningThread = concurrentThreads[nextTID];
	}

	runningThread->quanta++;
	++totalQuanta;

	// jump to the next ready thread (=running thread now)
	siglongjmp(runningThread->env, 1);
}

int setThreadID() {
	if (totalThreads <= MAX_THREAD_NUM) {
		for (int i=0; i<MAX_THREAD_NUM; ++i) {
			if (concurrentThreads[i] == nullptr) { return i; }
		}
	}
	return FAILURE;
}

int getReadyThread() {
	if (!readyThreads.empty()) {
		const int ret = readyThreads.front();
		readyThreads.pop_front();
		return ret;
	}
	return runningThread->tid;
}

bool isReady(int tid) {
	for (auto &i : readyThreads) {
		if (i == tid) { return true; }
	}
	return false;
//	return any_of(readyThreads.begin(), readyThreads.end(),
//				  [tid] (int i) { return i==tid; });
}

bool isBlocked(int tid) {
	for (auto &i : blockedThreads) {
		if (i == tid) { return true; }
	}
	return false;
//	return any_of(blockedThreads.begin(), blockedThreads.end(),
//				  [tid] (int i) { return i==tid; });
}

bool isWaiting(int tid) {
	for (auto &i : mutexWaitingThreads) {
		if (i == tid) { return true; }
	}
	return false;
//	return any_of(mutexWaitingThreads.begin(), mutexWaitingThreads.end(),
//				  [tid] (int i) { return i==tid; });
}

void terminateProcess() {
	for (auto &thread : concurrentThreads) {
		delete thread;
	}
//	exit(EXIT_SUCCESS);
}


// ================================================================================== //
// ============================== LIBRARY API ======================================= //
// ================================================================================== //

int uthread_init (int quantum_usecs)
{
	if (quantum_usecs <= 0) {
		std::cerr << "thread library error: non-positive quantum" << std::endl;
		return FAILURE;
	}

	// create a set of signals to be blocked, add SIGVTALRM to it.
	if (sigemptyset(&alarmSet) != SUCCESS) {
		std::cerr << "system error: sigemptyset failed." << std::endl;
		exit(EXIT_FAILURE);
	}
	if (sigaddset(&alarmSet, SIGVTALRM) != SUCCESS) {
		std::cerr << "system error: sigaddset failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	// specify the action to be associated with SIGVTALRM. (i.e the handler)
	sa.sa_handler = &timerHandler;
	if (sigaction(SIGVTALRM, &sa, nullptr) != SUCCESS) {
		std::cerr << "system error: sigaction failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	// schedule the main thread
	try {
		MAIN_THREAD = new Uthread();
		MAIN_THREAD->quanta++;
		runningThread = MAIN_THREAD;
		++totalThreads;
		++totalQuanta;
	} catch (std::bad_alloc&) {
		std::cerr << "system error: Memory allocation failed." << std::endl;
		exit(EXIT_FAILURE);
	}

	// setup the quanta timer (sends SIGVTALRM signal over intervals).
	setQuantumTimer(quantum_usecs);

	return SUCCESS;
}

int uthread_spawn (void (*f) (void))
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}
	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (f == nullptr) {
		std::cerr << "thread library error: invalid thread entry." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	const int tid = setThreadID();
	if (tid == FAILURE) {
		std::cerr << "thread library error: threads out of limit." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}

	// schedule the spawned thread
	try {
		concurrentThreads[tid] = new Uthread(tid, f);
		++totalThreads;
		readyThreads.push_back(tid);
	} catch (std::bad_alloc&) {
		std::cerr << "system error: Memory allocation failed." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return tid;
}

int uthread_terminate (int tid)
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (tid < 0 || tid >= MAX_THREAD_NUM || concurrentThreads[tid] == nullptr) {
		std::cerr << "thread library error: no such a thread" << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	// terminate the main thread
	if (tid == MAIN_THREAD->tid) {
		terminateProcess();
		exit(EXIT_SUCCESS);
	}

	// remove the terminated thread from all thread categories.
	mutexWaitingThreads.remove(tid);
	readyThreads.remove(tid);
	blockedThreads.erase(tid);
	/* if the terminated thread acquires the mutex, then free it and move one
	 * of the waiting threads to READY if it is not blocked .*/
	if (tid == mutex.tid) {
		mutex = {false, NO_THREAD};
		int waiting = mutexWaitingThreads.front();
		mutexWaitingThreads.pop_front();
		if (!isBlocked(waiting)) { readyThreads.push_back(waiting); }
	}
	delete concurrentThreads[tid];
	concurrentThreads[tid] = nullptr;
	--totalThreads;

	// the thread terminates itself
	if (tid == runningThread->tid) {
		const int nextTID = getReadyThread();
		// if READY is empty
		if (nextTID == runningThread->tid) {
			std::cerr << "DEADLOCK: READY & RUNNING are empty" << std::endl;
			terminateProcess();
			exit(EXIT_FAILURE);
		}
		// fetch and run the next ready thread
		runningThread = concurrentThreads[nextTID];
		runningThread->quanta++;
		++totalQuanta;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		siglongjmp(runningThread->env, 1);
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return SUCCESS;
}

int uthread_block (int tid)
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}
	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (tid < 0 || tid >= MAX_THREAD_NUM || concurrentThreads[tid] == nullptr) {
		std::cerr << "thread library error: no such a thread" << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	// try to block the main thread
	if (tid == MAIN_THREAD->tid) {
		std::cerr << "thread library error: can not block main thread" << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}

	/* I.   BLOCKED: has no effect.
	 * II.  READY: move to BLOCKED.
	 * III. RUNNING: a scheduling decision should be made.
	 * 				 generate SIGVTALRM, which will be handled by timerHandler() */
	if (isReady(tid)) {	readyThreads.remove(tid); }
	blockedThreads.insert(tid);
	if (tid == runningThread->tid) {
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		raise(SIGVTALRM);
	}

	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
		std::cerr << "system error: sigprocmask failed." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return SUCCESS;
}

int uthread_resume (int tid)
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (tid < 0 || tid >= MAX_THREAD_NUM || concurrentThreads[tid] == nullptr) {
		std::cerr << "thread library error: no such a thread." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}

	// resume affects only blocked threads
	if (isBlocked(tid)) {
		blockedThreads.erase(tid);
		// move to READY if not waiting the mutex
		if (!isWaiting(tid)) { readyThreads.push_back(tid); }
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return SUCCESS;
}

int uthread_mutex_lock ()
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	// the mutex is unlocked
	if (!mutex.isLocked) {
		mutex = {true, runningThread->tid};
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return SUCCESS;
	}
	// the mutex is already locked by this thread
	if (mutex.tid == runningThread->tid) {
		std::cerr << "thread library error: the mutex is already locked by "
			         "this thread." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	/* the mutex is already locked by different thread, push it to mutex waiting
	 * queue, and keep trying to acquire it, until succeed */
	mutexWaitingThreads.push_back(runningThread->tid);
	while (mutex.tid != runningThread->tid) {
		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		/* thread is waiting, so make a schedule decision, when its next turn in
		 * READY queue comes it will try again to acquire the mutex, but the mutex
		 * may be locked, because this thread was previously doubly blocked
		 * (directly and by mutex), and it removed from waiting queue but still in
		 * BLOCKED, so the mutex was acquired by another waiting thread, in this
		 * case move this thread again to the waiting queue. */
		raise(SIGVTALRM);
		sigprocmask(SIG_BLOCK, &alarmSet, nullptr);
		if (!mutex.isLocked) {
			mutex = {true, runningThread->tid};
		} else {
			mutexWaitingThreads.push_back(runningThread->tid);
		}
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}
	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return SUCCESS;
}

int uthread_mutex_unlock ()
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}
	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	// the mutex is already unlocked
	if (!mutex.isLocked) {
		std::cerr << "thread library error: the mutex is already unlocked." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	// only the thread which locked the mutex can release it
	if (mutex.tid != runningThread->tid) {
		std::cerr << "thread library error: only the thread which locked the "
			   		 "mutex can release it." << std::endl;
//		sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr);
		setitimer (ITIMER_VIRTUAL, &timer, nullptr);
		return FAILURE;
	}
	/* release the mutex, if the next waiting thread is NOT blocked directly
	 * by uthread_block(), move it to READY (but do NOT make it acquire the mutex
	 * immediately, it will try to acquire it when it back to RUNNING state).
	 * otherwise, move to the next waiting thread. */
	mutex = {false, NO_THREAD};
	int next;
	while (!mutexWaitingThreads.empty()) {
		next = mutexWaitingThreads.front();
		mutexWaitingThreads.pop_front();
		if (!isBlocked(next)) {
			readyThreads.push_back(next);
			/* In the future when this thread will be back to RUNNING state,
			 * it will try again to acquire the mutex.
			 * XX  mutex = {true, next};  XX */
			break;
		}
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}
	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return SUCCESS;
}

int uthread_get_tid ()
{
	return runningThread->tid;
}

int uthread_get_total_quantums ()
{
	return totalQuanta;
}

int uthread_get_quantums (int tid)
{
//	if (sigprocmask(SIG_BLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &stopTimer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	if (tid < 0 || tid >= MAX_THREAD_NUM || concurrentThreads[tid] == nullptr) {
		std::cerr << "thread library error: no such a thread." << std::endl;
		return FAILURE;
	}

//	if (sigprocmask(SIG_UNBLOCK, &alarmSet, nullptr) != SUCCESS) {
//		std::cerr << "system error: sigprocmask failed." << std::endl;
//		terminateProcess();
//		exit(EXIT_FAILURE);
//	}

	if (setitimer (ITIMER_VIRTUAL, &timer, nullptr)) {
		std::cerr << "system error: setitimer error." << std::endl;
		terminateProcess();
		exit(EXIT_FAILURE);
	}

	return concurrentThreads[tid]->quanta;
}
