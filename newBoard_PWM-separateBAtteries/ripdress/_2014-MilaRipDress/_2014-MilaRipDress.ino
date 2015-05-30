const int speeds[2][2] = {{255, 255}, {255, 255}}; // [50..255]

// set times for each movement (in milliseconds)
// [initDelta, upTime, breakTime, downTime]
const int times[2][2][4] = {
	{
		{  1450,  550, 1,  1},  // 4 (Right shoulder) RS
		{  1700,  750, 1, 1} // 2 (left shoulder) LS
	},
	{
		{  1,  1000, 1, 3},  // 3 (Hem) Hem
		{  3,  2100, 1, 3} // 1 (buttons) B
	}
};

////////////////////////////////////


// control pins
// [[m0_ctl0, m0_ctl1], [m1_ctl0, m1_ctl1]]
// [[[f0_0, f0_1], [b0_0, b0_1]], [f1_0, f1_1], [b1_0, b1_1]]]
const int motorCtls[2][2][2] = {{{2, 3}, {4, 5}}, {{7, 6}, {8, 9}}};
//const int motorCtls[2][2][2] = {{{4, 5}, {7, 6}}, {{2, 3}, {8, 9}}};
const int motorEnbl = 10;
int enableCounter = 20;

// [front, back]
const int reeds[2] = {12, 11};
boolean buttonPressAllowed = true;

// for each motor, there is a fixed sequence of movement events defined:
// [stop    , up    , stop    , down    ]
const int directions[4] = {
	-1, // stop
	0,  //   up
	-1, // stop
	1   // down
};


// [[f0, b0], [f1, b1]]
int timeCounters[2][2] = {0};

// initialize index pointer to be negative (i.e. stopped)
int timeIdx[2][2] = {{-1, -1}, {-1, -1}};

void setup() {
	//Serial.begin(9600);      // open the //Serial port at 9600 bps:    

	// set reed pins as input
	for (int i = 0; i < 2; i++) {
		pinMode(reeds[i], INPUT);
	}
	


///////////////////////////////////////////////
	pinMode(motorEnbl, OUTPUT);
	digitalWrite(motorEnbl, HIGH);
///////////////////////////////////////////////
	
	// set ctl pins as output and initialise with values
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				pinMode(motorCtls[k][j][i], OUTPUT);
				// motor stopped
				digitalWrite(motorCtls[k][j][i], LOW);
			}
		}
	}
	
	
}

// where == [0 (front)/ 1 (back)]
// which == [0        / 1       ]
// dirct == [-1 (stop)/ 0    (up)/ 1 (down)]
void runMotor(int where, int which, int dirct){

	// second pin is pwm
	if (dirct == 0){
	    digitalWrite(motorCtls[which][where][0],  LOW);
	    analogWrite (motorCtls[which][where][1], speeds[which][where]);
	} else {
		if (dirct == 1){
		    digitalWrite(motorCtls[which][where][0],  HIGH);
		    analogWrite (motorCtls[which][where][1],  speeds[which][where]);
		} else { // stop
		    digitalWrite(motorCtls[which][where][0],  LOW);
		    analogWrite (motorCtls[which][where][1],  0);
		}
	}
}

void step(int where, int which){
	int idx;
	idx = timeIdx[which][where];
	
	// only do something if idx is enabled
	if ( idx >= 0){

		if(timeCounters[which][where] < times[which][where][idx]){
			// do appropriate movement
			
			runMotor(where, which, directions[idx]);
			timeCounters[which][where]++;
		} else {
			// increase movement pointer and set counter to zero
			timeIdx[which][where]++;
			timeCounters[which][where] = 0;
			
			if (timeIdx[which][where] > 4){
                                enableCounter = 10;
                                buttonPressAllowed = true; 

				timeIdx[which][where] = -1;
				// stop the motor
				runMotor(where, which, -1);
			}
		}
		
	}
}


void loop() {

	if (enableCounter > 0){
		enableCounter--;
	        digitalWrite(motorEnbl, HIGH);

		for (int i =0; i < 2; i++){
			digitalRead(reeds[i]);
		}

	} else {
		digitalWrite(13, HIGH); // status LED


                if (buttonPressAllowed){ 
		  for (int i =0; i < 2; i++){
		  	//Serial.println(digitalRead(reeds[0]));
		  	if (digitalRead(reeds[i]) == HIGH) {
                                  buttonPressAllowed = false; 
		  		// set sequence index to first step 
		  		timeIdx[0][i] = 0; // motor 0
		  		timeIdx[1][i] = 0; // motor 1
		  		//Serial.print("switch! ");
		  		//Serial.println(i);
		    }
		  }
                }
                
		for (int i = 0; i<2; i++) {
			for (int j = 0; j<2; j++) {
				step(i, j);
			}
		}

	}
	
	
	delay(10);
}
