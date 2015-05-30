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
                notMoving     = true; 
				timeIdx[which][where] = -1;

				// stop motor
				runMotor(where, which, -1);
			}
		}
		
	}
}
