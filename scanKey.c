#include "getch.c"
#include <stdio.h>
//#include <unistd.h>

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0

typedef struct {
    int runTest;
    int state;
} status;

void scan_key (status *testStatus);
void wait_ms(int n, status *testStatus);
void wait_us(int n, status *testStatus);

void scan_key (status *testStatus)	//check key pressed
{
	int pressedKey;
	char choice;
	while (testStatus->runTest == YES)		//check key pressed when test is running
	{
		pressedKey = getch();
		if (testStatus->runTest == YES && (pressedKey == ' ' || pressedKey == 'P' || pressedKey == 'p'))	//key pressed is SPACE or 'P'
		{
			if (testStatus->state == PAUSE)			//if test running, pause test
			{
				printf("\nTest is continued.\n");
				testStatus->state = RUN;
			}
			else if (testStatus->state == RUN)		//if test pausing, run test
			{
				printf("\nTest is paused.");
				testStatus->state = PAUSE;
			}
		}
		else if (testStatus->runTest == YES && (pressedKey == 'q' || pressedKey == 'Q'))	//key pressed is 'Q'
		{
			if (testStatus->state == RUN)			//pause test if test running
			{
				printf("\nTest is paused.");
				testStatus->state = PAUSE;
			}
			do {
				printf("\nDo you want to quit? (y/n): ");
				fflush(stdout);
				choice = getchar();
				if (choice != '\n')
					while(getchar() != '\n');
				if (choice == 'Y' || choice == 'y')			//if enter 'Y', stop test
				{
					testStatus->state = STOP;
				}
				else if (choice == 'N' || choice == 'n')	//if enter 'N', do not stop test and continue
				{
					printf("Test is continued.\n");
					testStatus->state = RUN;
				}
			}while(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n');		//do-while loop to get confirmation to stop test
		}
	}
}

void wait_ms(int n, status *testStatus)	//splits delay into multiple 100ms while check test's state. E.g.: n = 350ms
{
	for (int i=1; i<=n/100; i++)		// e.g.: n/100 = 350/100 = 3
	{
		//delay(100);					// e.g.: delay 100ms for 3 times
		usleep(100000);
		if (testStatus->state != RUN)	//check test's state during delay for each 100ms
		{
			while (testStatus->state == PAUSE)	//if test's state is PAUSE, keep delay loop
				//delay(1);
				usleep(1);
			if (testStatus->state == STOP)		//if test's state is STOP, exit for loop
				return;
		}
	}
	//delay(n%100);						// e.g.: delay for n%100 = 350/100 = 50 ms
	usleep(n%100000);
	if (testStatus->state != RUN)		//check test's state 
		while (testStatus->state == PAUSE)		//if test's state is PAUSE, keep delay loop
			//delay(1);
			usleep(1);
}

void wait_us(int n, status *testStatus)	//splits delay into multiple 100ms while check test's state. E.g.: n = 350500us
{
	for (int i=1; i<=n/100000; i++)		// e.g.: n/100000 = 350500/100000 = 3
	{
		//delay(100);					// e.g.: delay 100ms for 3 times
		usleep(100000);
		if (testStatus->state != RUN)	//check test's state during delay for each 100ms
		{
			while (testStatus->state == PAUSE)	//if test's state is PAUSE, keep delay loop
				//delay(1);
				usleep(1);
			if (testStatus->state == STOP)		//if test's state is STOP, exit for loop
				return;
		}
	}
	//delayMicroseconds(n%100000);		// e.g.: delay for n%100000 = 350500/100000 = 50500 us
	usleep(n%100000);
	if (testStatus->state != RUN)	//check test's state 
		while (testStatus->state == PAUSE)	//if test's state is PAUSE, keep delay loop
			//delay(1);
			usleep(1);
}
