// trigger all motors at the same time
// comment out if original behaviour intended.
#define ALLMOTORS



const int speeds[2][2] = {{225, 255}, {225, 255}}; // [50..255]

// set times for each movement (in milliseconds)
// [initDelta, upTime, breakTime, downTime]
const int times[2][2][4] = 
{
	{
		{  10,   550, 0, 0},  // back  0 (3.alin)
		{   0,  1000, 0, 0} // front 1 (2.alin) 
	}, {
		{  0, 250, 0, 0},  // back  1 (4.alin)
		{  1000, 400, 0, 0} // front 0 (alin) 
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
const int switches[2] = {12, 11};

// for each motor, there is a fixed sequence of movement events defined:
// [stop    , up    , stop    , down    ]
const int directions[4] = {
	-1, // stop
	0,  //   up
	-1, // stop
	1   // down
};

// is anything moving?
boolean notMoving = true;

// [[f0, b0], [f1, b1]]
int timeCounters[2][2] = {0};

// initialize index pointer to be negative (i.e. stopped)
int timeIdx[2][2] = {{-1, -1}, {-1, -1}};

void setup() {

	// debug
	//Serial.begin(9600);

	pinMode     (motorEnbl, OUTPUT);
	digitalWrite(motorEnbl,   HIGH);

	for (int i = 0; i < 2; i++) {
		pinMode(switches[i], INPUT);
	}

	// ctl pins are output and initialised to stop the motors
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


void loop() {
	if (enableCounter > 0) {
		enableCounter--;
		digitalWrite(motorEnbl, HIGH);

		for (int i =0; i < 2; i++){
			digitalRead(switches[i]);
		}

	} else {
		digitalWrite(13, HIGH); // status LED

		// if not in a motion already
		if (notMoving){ 

			// for all switches that are pressed, 
			// set sequence index of both its associated motors
			// to 'start' and disallow button press activation
			for (int i =0; i < 2; i++){

			  	if (digitalRead(switches[i]) == HIGH) {
			  		notMoving = false; // we're moving now

			  		#ifdef ALLMOTORS
		  			// every switch triggers all motors
		  			timeIdx[0][0] = 0; // motor 0, sw 0
		  			timeIdx[1][0] = 0; // motor 1, sw 0
		  			timeIdx[0][1] = 0; // motor 0, sw 1
		  			timeIdx[1][1] = 0; // motor 1, sw 1
		  			#else
			  		// original behaviour, 
			  		// two motors attached to one switch
		  			timeIdx[0][i] = 0; // motor 0, sw i
		  			timeIdx[1][i] = 0; // motor 1, sw i
		  			#endif
		  		}
		  	}
		}

		for (int i = 0; i<2; i++) {
			for (int j = 0; j<2; j++) {
				step(i, j);
			}
		}


		#ifdef ALLMOTORS
		int inactiveCounter = 0;
		for (int i = 0; i<2; i++) {
			for (int j = 0; j<2; j++) {
				if (timeIdx[i][j] > 4) {
					inactiveCounter++
				}
			}
		}
		if (inactiveCounter == 4) {
			notMoving = true;
		}
		#endif
	}
	
	
	delay(10);
}
