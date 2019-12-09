
#define pulse_Guide 			2400			// microseconds
#define pulse_oneON 			1200			// microseconds
#define pulse_zeroON 			600				// microseconds
#define pulse_OFF 				(600-60)		// microseconds
#define duration_PowerON		(19800-990)		// microseconds
#define duration_PowerOFF 		(20400-1020)	// microseconds
#define duration_PowerToggle	(19200-960)	// microseconds
#define duration_PowerON_3 		109800			// complete 3 frames (45000 + 45000 + 19800)
#define duration_PowerOFF_3 	110400			// complete 3 frames (45000 + 45000 + 20400)
#define duration_PowerToggle_3 	109200			// complete 3 frames (45000 + 45000 + 19200)

void static sendIR_Guide(int IRPin);
void static sendIRPulse(int IRPin, int pulse);
void static sendIR(int IRPin, int code);
void sendIR_PowerOFF (int IRPin, int duration);
void sendIR_PowerON (int IRPin, int duration);
void sendIR_PowerToggle (int IRPin, int duration);
void sendIR_cmd (int IRPin,int code);

void sendIR_cmd (int IRPin, int code)
{
	int t = 2400 + 600;
	for (int  i=0; i<=11; i++)
	{
		if(((code >> i) & 1) == 1)
		    t = t + (1200 + 600);
		else
		    t = t + (600 + 600);
	}
	sendIR(IRPin, code);
	delayMicroseconds(45000 - t);
	sendIR(IRPin, code);
	delayMicroseconds(45000 - t);
	sendIR(IRPin, code);
}

void static sendIR (int IRPin, int code)
{
	sendIR_Guide(IRPin);
	for (int  i=0; i<=11; i++)
	{
		sendIRPulse(IRPin, (code >> i) & 1);
	}
}

void static sendIR_Guide (int IRPin)	// Duration = 2.4ms
{
	for (int duration = 0; duration < pulse_Guide/24; duration++)
	{	
		digitalWrite(IRPin, HIGH);
		delayMicroseconds(12);		
		digitalWrite(IRPin, LOW);
		delayMicroseconds(12);
	}
	digitalWrite(IRPin, LOW);
	delayMicroseconds(pulse_OFF);
}

void static sendIRPulse (int IRPin, int pulse)
{
	if  (pulse == 1)
	{
		for (int duration = 0; duration < pulse_oneON/24; duration++)
		{	
			digitalWrite(IRPin, HIGH);
			delayMicroseconds(12);		
			digitalWrite(IRPin, LOW);
			delayMicroseconds(12);
		}
	}
	else if (pulse == 0)
	{
		for (int duration = 0; duration < pulse_zeroON/24; duration++)
		{	
			digitalWrite(IRPin, HIGH);
			delayMicroseconds(12);		
			digitalWrite(IRPin, LOW);
			delayMicroseconds(12);
		}
	}
	digitalWrite(IRPin, LOW);
	delayMicroseconds(pulse_OFF);
}

void sendIR_PowerOFF (int IRPin, int duration)	
{
	// start of 1st frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	if (duration == 10) 
	{
		delayMicroseconds(10000 - 9600);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	if (duration == 20) 
	{
		delayMicroseconds(20000 - 19200);
		return;
	}
	sendIRPulse(IRPin, 0);
	
	if (duration == 30) 
	{
		delayMicroseconds(30000 - duration_PowerOFF);
		return;
	}
	if (duration == 40) 
	{
		delayMicroseconds(40000 - duration_PowerOFF);
		return;
	}
	delayMicroseconds(45000 - duration_PowerOFF);
	
	// start of 2nd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	if (duration == 50) 
	{
		delayMicroseconds(50000-45000-4200);
		return;
	}
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	if (duration == 60) 
	{
		delayMicroseconds(60000-45000-13800);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);

	if (duration == 70) 
	{
		delayMicroseconds(70000-45000-duration_PowerOFF);
		return;
	}
	if (duration == 80) 
	{
		delayMicroseconds(80000-45000-duration_PowerOFF);
		return;
	}
	if (duration == 90) 
	{
		delayMicroseconds(90000-45000-duration_PowerOFF);
		return;
	}
	delayMicroseconds(45000 - duration_PowerOFF);
	
	// start of 3rd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	if (duration == 100) 
	{
		delayMicroseconds(100000-90000-9600);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	if (duration == 110) 
	{
		delayMicroseconds(20000 - 19200);
		return;
	}
	sendIRPulse(IRPin, 0);
}

void sendIR_PowerON (int IRPin, int duration)
{
	// start of 1st frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	if (duration == 10) 
	{
		delayMicroseconds(10000 - 9000);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	
	if (duration == 20)
	{
		delayMicroseconds(20000 - duration_PowerON);
		return;
	}
	if (duration == 30) 
	{
		delayMicroseconds(30000 - duration_PowerON);
		return;
	}
	if (duration == 40) 
	{
		delayMicroseconds(40000 - duration_PowerON);
		return;
	}
	delayMicroseconds(45000 - duration_PowerON);
	
	// start of 2nd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 0);
	if (duration == 50) 
	{
		delayMicroseconds(50000 - 45000 - 3600);
		return;
	}
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	if (duration == 60) 
	{
		delayMicroseconds(60000 - 45000 - 13200);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);

	if (duration == 70) 
	{
		delayMicroseconds(70000 - 45000 - duration_PowerON);
		return;
	}
	if (duration == 80) 
	{
		delayMicroseconds(80000 - 45000 - duration_PowerON);
		return;
	}
	if (duration == 90) 
	{
		delayMicroseconds(90000 - 55000 - duration_PowerON);
		return;
	}
	delayMicroseconds(45000 - duration_PowerON);
	
	// start of 3rd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 1);
	if (duration == 100) 
	{
		delayMicroseconds(100000 - 90000 - 9000);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);	
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
}

void sendIR_PowerToggle (int IRPin, int duration)
{
	// start of 1st frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	if (duration == 10) 
	{
		delayMicroseconds(10000 - 8400);
		return;
	}
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	
	if (duration == 20)
	{
		delayMicroseconds(20000 - duration_PowerToggle);
		return;
	}
	if (duration == 30) 
	{
		delayMicroseconds(30000 - duration_PowerToggle);
		return;
	}
	if (duration == 40) 
	{
		delayMicroseconds(40000 - duration_PowerToggle);
		return;
	}
	delayMicroseconds(45000 - duration_PowerON);
	
	// start of 2nd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	if (duration == 50) 
	{
		delayMicroseconds(50000 - 45000 - 4200);
		return;
	}
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	if (duration == 60) 
	{
		delayMicroseconds(60000 - 45000 - 12600);
		return;
	}
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);

	if (duration == 70) 
	{
		delayMicroseconds(70000 - 45000 - duration_PowerToggle);
		return;
	}
	if (duration == 80) 
	{
		delayMicroseconds(80000 - 45000 - duration_PowerToggle);
		return;
	}
	if (duration == 90) 
	{
		delayMicroseconds(90000 - 55000 - duration_PowerToggle);
		return;
	}
	delayMicroseconds(45000 - duration_PowerToggle);
	
	// start of 3rd frame
	sendIR_Guide(IRPin);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	if (duration == 100) 
	{
		delayMicroseconds(100000 - 90000 - 8400);
		return;
	}
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 1);
	sendIRPulse(IRPin, 0);	
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
	sendIRPulse(IRPin, 0);
}
