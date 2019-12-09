#include <stdio.h>  
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
#include "seqTime.h"
//#include "getch.c"
#include "scanKey.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0

const int relayPin = 17; 	// Set GPIO pin 17
void random_AC(int test_cycle, status *testStatus);
int randomSeq[2000][3], count = 0;

int main (void)
{
    int input_check, test_cycle = 0;
    status testStatus = {YES, RUN};
    pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(relayPin, OUTPUT); 		// Set pin as output
    
   	printf("AC - Random Test is running! Press CTRL+C to quit. \n\n");
   	
    FILE *fp;
    fp = fopen("randomSeqTiming.txt", "r");
    if (fp == NULL) 
    {
		printf("Default timing will be used. \n");
		for (int i=0; i<1560;i++)
		{
			for (int j=0; j<3;j++)
			{
				randomSeq[i][j] = randomSeq_timing[i][j];
			}
			count++;
		}
    }
    else
    {
		printf("Custom timing file \"randomSeqTiming.txt\" is found. New timing will be used. \n");
        while (fscanf(fp, "%d %d %d ", &randomSeq[count][0], &randomSeq[count][1], &randomSeq[count][2]) == 3) 
            count++;
	}
       
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
	random_AC(test_cycle, &testStatus);
	
	testStatus.runTest = NO;
	printf("\nTest is %s. Press any key to exit..\n", (testStatus.state == STOP)?"stopped":"completed");
	pthread_join(thread_scanKey, NULL);
	getch();
}

void random_AC (int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
	{
		printf("\nCycle %d starts! \n", n);
		
		for (int i=1; i<=count; i++)	
		{
			digitalWrite(relayPin, HIGH);       // Turn ON
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: ON \t", i, randomSeq[i-1][2], randomSeq[i-1][1]);
			fflush(stdout);
			wait_ms(randomSeq[i-1][1], testStatus);
			//delay(randomSeq[i-1][1]);
			if (testStatus->state == STOP)
				return; 
		
			digitalWrite(relayPin, LOW);        // Turn OFF
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: OFF \t", i, randomSeq[i-1][2], randomSeq[i-1][1]);
			fflush(stdout);
			wait_ms(randomSeq[i-1][2], testStatus);
			//delay(randomSeq[i-1][2]);
			if (testStatus->state == STOP)
				return;
		}

		digitalWrite(relayPin, HIGH);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_ms(310, testStatus);
		//delay(310);
		if (testStatus->state == STOP)
			return;
		printf("\nCycle %d is finished! \n", n);
	}
	return;
}
