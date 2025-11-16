#include <wiringPi.h>
#include <stdio.h>
#include <unistd.h> 


//USE FOR 3.3V TESTING 5V+ COULD KILL BOARD 

// MOTOR 1 pins 
#define M1_STEP 0   // wiringPi pin 0 = BCM17
#define M1_DIR  2   // wiringPi pin 2 = BCM27
#define M1_EN   3   // wiringPi pin 3 = BCM22

// MOTOR 2 pins
#define M2_STEP 4   // wiringPi pin 4 = BCM23
#define M2_DIR  5   // wiringPi pin 5 = BCM24
#define M2_EN   6   // wiringPi pin 6 = BCM25

// MOTOR 3 pins
#define M3_STEP 21  // wiringPi pin 21 = BCM5
#define M3_DIR  22  // wiringPi pin 22 = BCM6
#define M3_EN   23  // wiringPi pin 23 = BCM13

void move_motor(int step_pin, int dir_pin, int direction, int steps, int step_delay_us) {
    digitalWrite(dir_pin, direction); 
    for (int i = 0; i < steps; i++) {
        digitalWrite(step_pin, HIGH);
        usleep(step_delay_us);
        digitalWrite(step_pin, LOW);
        usleep(step_delay_us);
    }
}

void enable_motor(int en_pin) {
    digitalWrite(en_pin, LOW); 
}

void disable_motor(int en_pin) {
    digitalWrite(en_pin, HIGH); 
}

int main() {
    // Initialize wiringPi
    if (wiringPiSetup() == -1) {
        printf("wiringPi setup failed!\n");
        return 1;
    }

    int step_pins[] = {M1_STEP, M2_STEP, M3_STEP};
    int dir_pins[]  = {M1_DIR, M2_DIR, M3_DIR};
    int en_pins[]   = {M1_EN, M2_EN, M3_EN};

    for(int i = 0; i < 3; i++) {
        pinMode(step_pins[i], OUTPUT);
        pinMode(dir_pins[i], OUTPUT);
        pinMode(en_pins[i], OUTPUT);
        enable_motor(en_pins[i]);
    }

    // Move motors
    move_motor(M1_STEP, M1_DIR, 0, 200, 1000); // Motor1 forward
    move_motor(M2_STEP, M2_DIR, 1, 300, 1200); // Motor2 reverse
    move_motor(M3_STEP, M3_DIR, 0, 400, 800);  // Motor3 forward

    sleep(1);

    // Move motors back
    move_motor(M1_STEP, M1_DIR, 1, 200, 1000);
    move_motor(M2_STEP, M2_DIR, 0, 300, 1200);
    move_motor(M3_STEP, M3_DIR, 1, 400, 800);

    // KILL all motors
    for(int i = 0; i < 3; i++) {
        disable_motor(en_pins[i]);
    }

    return 0;
}
