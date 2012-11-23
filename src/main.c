/**
 * @author: Dan
 * @since November 23, 2012, 11:57 PM
 * @version 1.0
 *
 * DESC
 */

#include <xc.h>
#include "DanOS.h"

Bool blinkyLed(void);

void init(void) {
    InitialiseBoard(); // Prepare the board (this is basic IO stuff, and also CPU init stuff)
    timer_init(); // Prepare the systick timer
    serial_init(); // Prepare the serial port
    i2c_init(); // Prepare the I2C
    jobs_init(); // Prepare the job controller
}

int main(void) {
    init();

    serial_putString("Starting up..!");

    /*
     * Create a job description for a blinking LED
     */
    job_t blinkyLedJob = {
        .activationTime = timer_currentTime(),
        .jobFunction = blinkyLed
    };

    // Add the jobs
    jobs_add(&blinkyLedJob);

    while (1) {
        board_update(); // Run the low level operations (buttons, uart...)
        jobs_update(); // Run the jobs

        // Go into sleep mode
        board_idle();
    }

    return 0;
}

Bool blinkyLed(void) {
    static uint32_t delayTime = 0; // The time to delay before executing this thread again
    uint32_t currentTime = timer_currentTime();

    if (delayTime <= currentTime) { // If it is time to run this job...
        led1 ^= 1;

        delayTime = currentTime + 500; // Execute this job every 500mS
    }

    return False; // This job never stops
}
