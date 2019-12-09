#include <stdio.h> 
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
//#include "getch.c"
//#include <conio.h>
#include "scanKey.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0
#define timeON 12000	// milliseconds
#define timeOFF_step 10	//milliseconds

const int relayPin = 17; 	// Set GPIO pin 17
void seqA_AC (int test_cycle, status *testStatus);

int main (void)
{
	int input_check, test_cycle = 0;
	status testStatus = {NO, RUN};
	pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(relayPin, OUTPUT); 		// Set pin as output   
    
    printf("AC - Sequential A Test is running! Press CTRL+C to quit. \n\n");
    do {
		printf("Number of cycles: ");
		fflush(stdout);
		input_check = scanf("%d", &test_cycle);
		while(getchar() != '\n');
		//fflush(stdin);
		if (input_check == 1 && test_cycle > 0)
			printf("OK. Test cycle: %d \n", test_cycle);
		else if (input_check == 1 && test_cycle <= 0)
			printf("Number entered is invalid. \n");
		else 
			printf("Wrong input. \n");
	}while(input_check == 0 || test_cycle <= 0);	// do-while loop to check valid total cycles that need to be performed
	
	testStatus.runTest = YES;
	testStatus.state = RUN;
	pthread_create(&thread_scanKey, NULL, (void*) scan_key, &testStatus);
	printf("Test will start. Use SPACE or P keys to pause test. \n");
	//printf("To stop test, press Q. \n");
	seqA_AC(test_cycle, &testStatus);
	
	testStatus.runTest = NO;
    printf("\nTest is %s. Press any key to exit..\n", (testStatus.state == STOP)?"stopped":"completed");
    pthread_join(thread_scanKey, NULL);
    getch();
}

void seqA_AC (int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
    {
		printf("\nCycle %d starts! \n", n);
		
		digitalWrite(relayPin, HIGH); 		// Turn ON
		printf("\rRound 0: \t\t\tON = 60s \tState: ON  \t");
		fflush(stdout);
		wait_ms(60000, testStatus);
		//delay(60000);
		if (testStatus->state == STOP)
			return;

		for (int i=1; i<=100; i++)	
		{
			digitalWrite(relayPin, HIGH); 	// Turn ON
			printf("\rRound %d: \tOFF = %dms \tON = %ds \tState: ON  \t", i, i*timeOFF_step, timeON/1000);
			fflush(stdout);
			wait_ms(timeON, testStatus);
			//delay(timeON); 					// Wait 12000ms
			if (testStatus->state == STOP)
				return;

			digitalWrite(relayPin, LOW); 	// Turn OFF
			printf("\rRound %d: \tOFF = %dms \tON = %ds \tState: OFF \t", i, i*timeOFF_step, timeON/1000);
			fflush(stdout);
			wait_ms(i*timeOFF_step, testStatus);
			//delay(i*timeOFF_step); 			// Wait 
			if (testStatus->state == STOP)
				return;
		}

		digitalWrite(relayPin, HIGH);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_ms(9.5*1000, testStatus);
		//delay(9.5*1000);
		if (testStatus->state == STOP)
			return;
		printf("Cycle %d is finished! \n", n);
	}
	return;
}
