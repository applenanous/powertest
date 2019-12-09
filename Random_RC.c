#include <stdio.h>  
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
#include "seqTime.h"
//#include "getch.c"
#include "scanKey.c"
#include "sendIR.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0
#define POWER_OFF 175
#define POWER_ON 174
#define POWER_Toggle 149

const int IRPin = 22; // Set GPIO pin 22
void random_RC (int test_cycle, status *testStatus);
int randomSeq[2000][3], count = 0, cmd_type;

int main ()
{
    int input_check, test_cycle = 0;
    status testStatus = {YES, RUN};
    pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(IRPin, OUTPUT); 		// Set pin as output
    
    printf("RC - Random Test is running! Press CTRL+C to quit. \n\n"); 
    
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
	
	do {
		printf("Command type ([1]On-Off, [2]Toggle): ");
		fflush(stdout);
		input_check = scanf("%d", &cmd_type);
		while(getchar() != '\n');
		//fflush(stdin);
		if (input_check == 1 && (cmd_type == 1 || cmd_type == 2))
			printf("OK. RC use %s type command. \n", (cmd_type == 1)?"On-Off":"Toggle");
		else 
			printf("Invalid input. \n");
	}while(input_check == 0 || (cmd_type != 1 && cmd_type != 2));
		
	testStatus.runTest = YES;
	testStatus.state = RUN;
	pthread_create(&thread_scanKey, NULL, (void*) scan_key, &testStatus);
	printf("Test will start. Use SPACE or P keys to pause test. \n");
	//printf("To stop test, press Q. \n");
	random_RC(test_cycle, &testStatus);
	
	testStatus.runTest = NO;
	printf("\nTest is %s. Press any key to exit..\n", (testStatus.state == STOP)?"stopped":"completed");
	pthread_join(thread_scanKey, NULL);
	getch();
}

void random_RC (int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
	{
		printf("\nCycle %d starts! \n", n);
		
		for (int i=1; i<=count; i++)	
		{
			if (cmd_type == 1)
			{
				if (randomSeq[i-1][1] < 120)
					sendIR_PowerON(IRPin, randomSeq[i-1][1]);
				else
					sendIR_cmd(IRPin, POWER_ON);
			}
			else
			{
				if (randomSeq[i-1][1] < 120)
					sendIR_PowerToggle(IRPin, randomSeq[i-1][1]);
				else
					sendIR_cmd(IRPin, POWER_Toggle);
			}
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: ON \t", i, randomSeq[i-1][2], randomSeq[i-1][1]);
			fflush(stdout);
			if (randomSeq[i-1][1] > 110)
				wait_us(randomSeq[i-1][1]*1000 - ((cmd_type == 1)?duration_PowerON_3:duration_PowerToggle_3), testStatus);
			if (testStatus->state == STOP)
				return; 
		
			if (cmd_type == 1)
			{
				if (randomSeq[i-1][2] < 120)
					sendIR_PowerOFF(IRPin, randomSeq[i-1][2]);
				else
					sendIR_cmd(IRPin, POWER_OFF);
			}
			else
			{
				if (randomSeq[i-1][2] < 120)
					sendIR_PowerToggle(IRPin, randomSeq[i-1][2]);
				else
					sendIR_cmd(IRPin, POWER_Toggle);
			}
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: OFF \t", i, randomSeq[i-1][2], randomSeq[i-1][1]);
			fflush(stdout);
			if (randomSeq[i-1][2] > 110)
				wait_us(randomSeq[i-1][2]*1000 - ((cmd_type == 1)?duration_PowerOFF_3:duration_PowerToggle_3), testStatus);
			if (testStatus->state == STOP)
				return;
		}

		if (cmd_type == 1)
			sendIR_cmd(IRPin, POWER_ON);
		else
			sendIR_cmd(IRPin, POWER_Toggle);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_us((310000 - ((cmd_type == 1)?duration_PowerON_3:duration_PowerToggle_3)), testStatus);
		if (testStatus->state == STOP)
			return;
		printf("\nCycle %d is finished! \n", n);
	}
	return;
}
