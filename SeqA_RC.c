#include <stdio.h>  
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
//#include "getch.c"
#include "scanKey.c"
#include "sendIR.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0
#define timeON 			12000	// milliseconds
#define timeOFF_step 	10		//milliseconds
#define POWER_OFF 175
#define POWER_ON 174
#define POWER_Toggle 149

const int IRPin = 22; // Set GPIO pin 22
void seqA_RC (int test_cycle, status *testStatus);
int cmd_type;

int main (void)
{
    int input_check, test_cycle = 0;
    status testStatus = {YES, RUN};
    pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(IRPin, OUTPUT); 		// Set pin as output
    
    printf("RC - Sequential B Test is running! Press CTRL+C to quit. \n\n");
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
	seqA_RC(test_cycle, &testStatus);

	testStatus.runTest = 0;
    //printf("\nTest is %s. Press any key to continue..\n", (testStatus.state == STOP)?"stopped":"completed");
    //getch();
	printf("\nTest is %s. Press Enter to continue..\n", (testStatus.state == STOP)?"stopped":"completed");
    getchar();
}

void seqA_RC (int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
    {
		printf("\nCycle %d starts! \n", n);
		
		if (cmd_type == 1)
			sendIR_cmd(IRPin, POWER_ON);
		printf("\rRound 0: \t\t\tON = 60s \tState: ON  \t");
		fflush(stdout);
		wait_ms(60000, testStatus);
		//delay(60000);
		if (testStatus->state == STOP)
			return;
			
		for (int i=1; i<=100; i++)
		{
			if (cmd_type == 1)
			{
				if (i*timeON < 120)
					sendIR_PowerON(IRPin, timeON);
				else
					sendIR_cmd(IRPin, POWER_ON);
			}
			else
			{
				if (i*timeON < 120)
					sendIR_PowerToggle(IRPin, timeON);
				else
					sendIR_cmd(IRPin, POWER_Toggle);
			}
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: ON \t", i, i*timeOFF_step, timeON);
			fflush(stdout);
			if (i*timeON > 110)
				wait_us(timeON*1000 - ((cmd_type == 1)?duration_PowerON_3:duration_PowerToggle_3), testStatus);
			if (testStatus->state == STOP)
				return; 
		
			if (cmd_type == 1)
			{
				if (i*timeOFF_step < 120)
					sendIR_PowerOFF(IRPin, i*timeOFF_step);
				else
					sendIR_cmd(IRPin, POWER_OFF);
			}
			else
			{
				if (i*timeOFF_step < 120)
					sendIR_PowerToggle(IRPin, i*timeOFF_step);
				else
					sendIR_cmd(IRPin, POWER_Toggle);
			}
			printf("\rRound %d: \tOFF = %dms \tON = %dms \tState: OFF \t", i, i*timeOFF_step, timeON);
			fflush(stdout);
			if (i*timeOFF_step > 110)
				wait_us(i*timeOFF_step*1000 - ((cmd_type == 1)?duration_PowerOFF_3:duration_PowerToggle_3), testStatus);
			if (testStatus->state == STOP)
				return;
		}

		if (cmd_type == 1)
			sendIR_cmd(IRPin, POWER_ON);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_ms(54000, testStatus);
		//delay(54000); 		// Wait 
		if (testStatus->state == STOP)
			return;
		printf("\nCycle %d is finished! \n", n);
	}
	return;
}
