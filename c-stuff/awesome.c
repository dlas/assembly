#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <math.h>


sigjmp_buf threads[10];
int next_thread_id=0;
int current_thread_id=0;

// A thread: print some stuff.
void thread1(void * x, int tid) {
	int t = 0;
	while (1) {
		printf("HELLO WORLD[%i]: %s %i\n", tid, x,t);
		schedule();
		t++;
	}
}

// A thread: calculate all prime numbers
void thread2(void *x, int id) {
	unsigned int i;
	for (i = 10000; ; i++) {
		unsigned int try;
		unsigned int max = sqrt(i);
		for (try = 2; try < max; try++) {
			if (i % try == 0) {
				printf("Thread[%i] %i is divisible by %i\n", id, i, try);
				break;
			}
		}
		if ( try == max) {
				printf("Thread[%i] %i is prime\n", id, i);
		}
		schedule();
	}
}

// Scheduler. Call to choose a new task to execute for a while.
// Eventually "returns" to its caller, giving its caller a chance to run.
void schedule(void) {
	if (setjmp(threads[current_thread_id]) == 0) {
		current_thread_id = (current_thread_id+1)%next_thread_id;
		longjmp(threads[current_thread_id], 1);
	}
}


// This is the preamble to run in thread-context whenever we startup a new
// thread.  It prepares its own thread context structure to all the
// start function as soon as the thread is scheduled.
void thread_preamble(void (*start)(void * x, int id),  void * cookie) {
	int thread_id;
	// Allocate the next thread id for the my thread id
	thread_id = next_thread_id++;
	
	// Save the entry point for this thread 
	if (setjmp(threads[thread_id]) != 0) {
		// *** This is the real entry point for this thread the first
		// time schedule runs ***
		start(cookie, thread_id);
		// This should never be reached because we do not support thread
		// termination
	}
	// We end up here (and return to makethread) when thread_preamble is call
	// to create the thread. 
}

// Start a new thread. This works by allocating some stack space for
// the thread. And calling the thread_preamle function on that stack.
// The preabmle will return and then we restore the initial stack.
void makethread(void (*start)(void * x, int id), void * cookie) {
	void * newesp = malloc(1024*1024) + (1024*1024-16);
	asm("movq %%rsp, %%rax\n"	// Put rsp somewhere safe
	    "movq %2, %%rdi\n"		// Copy cookie parameter into 2nd arg slot
	    "movq %3, %%rsi\n"		// Copy start parameter into 1st arg slot
	    "movq %0, %%rsp\n"          // Setup new stack
	    "pushq    %%rax\n"          // Save old esp (now at rax) 
	    "call *%1\n"		// Call thread_preamble
	    "popq %%rsp"		// restore old stack
	    : :"r"(newesp),  "r"(thread_preamble),"r"(start), "r"(cookie) : "rax");
}


int main(int argc, char ** argv) {

	makethread(thread1, "meow");
	makethread(thread2, NULL);
	makethread(thread1, "kidney");
	longjmp(threads[0], 1);

}

