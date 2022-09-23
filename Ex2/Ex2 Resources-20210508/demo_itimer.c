/*
 * Interval-timer demo program.
 * Hebrew University OS course.
 * Author: OS, os@cs.huji.ac.il
 */

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

struct itimerval timer;
struct itimerval stopTimer={0};
int gotit = 0;
int num = 0;

void thread1(void);

void timer_handler(int sig)
{
  gotit = 1;
  printf("Timer expired\n");
  ++num;
  if (num%5==0) {
  	thread1();

  }
}

void ignore_signal(int sig)
{
	gotit = 2;
	printf("TIMER EXPIRED\n");
}

void thread1(void) {
	printf(" -- thread (1) --\n");
	setitimer (ITIMER_VIRTUAL, &stopTimer, NULL);
	printf("stopTimer\n");
	for (int i = 0; i < 1e6; ++i)
	{
		if (i % 100000 == 0) {printf(" # ");}
	}
	printf("\n");
	for (int i = 0; i < 1e6; ++i)
	{
		if (i == 5e5) {
			setitimer (ITIMER_VIRTUAL, &timer, NULL);
			printf("resumeTimer & raise SIG\n");
			raise(SIGVTALRM);
//			setitimer (ITIMER_VIRTUAL, &timer, NULL);
//			printf("resumeTimer & return\n");
			return;
		}
	}

	setitimer (ITIMER_VIRTUAL, &timer, NULL);
	printf("resumeTimer\n");
}

void thread2(void) {
	printf(" -- thread (2) --");
	setitimer (ITIMER_VIRTUAL, &stopTimer, NULL);
	printf("stopTimer\n");

}


int main(void) {
	struct sigaction sa = {0};
	struct sigaction ignore = {.sa_handler = ignore_signal};
//	struct itimerval timer;
//	struct itimerval stopTimer={0};

	// Install timer_handler as the signal handler for SIGVTALRM.
	sa.sa_handler = &timer_handler;
	if (sigaction(SIGVTALRM, &sa,NULL) < 0) {
		printf("sigaction error.");
	}

	// Configure the timer to expire after 1 sec... */
	timer.it_value.tv_sec = 1;		// first time interval, seconds part
	timer.it_value.tv_usec = 0;		// first time interval, microseconds part

	// configure the timer to expire every 3 sec after that.
	timer.it_interval.tv_sec = 3;	// following time intervals, seconds part
	timer.it_interval.tv_usec = 0;	// following time intervals, microseconds part

	// Start a virtual timer. It counts down whenever this process is executing.
	if (setitimer (ITIMER_VIRTUAL, &timer, NULL)) {
		printf("setitimer error.");
	}


	int counter = 1;
//	int i = timer.it_interval.tv_sec;
	struct itimerval oldtimer;
	for (int i = 0; i < 1e15; ++i)
	{
		if (i % 10000000 == 0) {
			getitimer(ITIMER_VIRTUAL, &oldtimer);
//			printf("time = %ld\n", oldtimer.it_interval.tv_sec);
			printf("time = %ld\n", oldtimer.it_value.tv_sec);
		}

	}

	for(;;) {
		if (gotit==1) {
			printf("(%d) Got it!\n", counter++);
//			printf("gotit = (%d)\n", gotit);
//			timer.it_value.tv_sec = 0;
//			setitimer (ITIMER_VIRTUAL, &timer, NULL);
//			setitimer (ITIMER_VIRTUAL, &stopTimer, NULL);
//			printf("stopTimer\n");
//			sigaction(SIGVTALRM, &ignore, NULL);
			gotit = 0;
//			setitimer (ITIMER_VIRTUAL, &timer, NULL);
		}
//		else {
//			printf("(%d) Got it!\n", counter++);
//			printf("gotit = (%d)\n", gotit);
//			setitimer (ITIMER_VIRTUAL, &timer, NULL);
//			printf("timer\n");
//		}
//		else if (gotit==2) {
//			printf("(%d) IGNORE IT!!!??\n", counter++);
//			sigaction(SIGVTALRM, &sa, NULL);
//			gotit = 0;
//		}
	}
}

