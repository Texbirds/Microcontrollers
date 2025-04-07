/*
 * Alarm System with FSM
 *
 * This program implements an alarm system with three states:
 * - UIT:       Alarm is off (LED1)
 * - AAN:       Alarm is on (LED2)
 * - WAARSCHUWING: Alarm in warning mode (LED3)
 *
 * Controls:
 * - PD0: SET (turn alarm ON)
 * - PD1: RESET (turn alarm OFF)
 * - PD2: WAARSCHUWING2 (trigger warning mode)
 */

 #define F_CPU 10000000UL  // Define CPU frequency for delay calculations

 #include <avr/io.h>
 #include <util/delay.h>
 #include <stdio.h>
 
 // Enum for possible FSM states
 typedef enum {
     UIT,         // Alarm off
     AAN,         // Alarm on
     WAARSCHUWING // Alarm in warning state
 } State;
 
 // Enum for possible FSM events (triggered by buttons)
 typedef enum {
     SET,         // Turn alarm ON
     RESET,       // Turn alarm OFF
     WAARSCHUWING2 // Trigger warning mode
 } Event;
 
 // FSM structure to hold the current state
 typedef struct {
     State currentState;
 } FSM;
 
 void initFSM(FSM *fsm) {
     fsm->currentState = UIT; // Start the FSM in the "UIT" state
 }
 
 // Update LEDs based on the current state
 void updateLEDs(State state) {
     PORTC = 0x00; // Turn off all LEDs initially
 
     switch (state) {
         case UIT:
             PORTC = (1 << PC0);  // Turn on LED1 (PC0) for UIT
             break;
         case AAN:
             PORTC = (1 << PC1);  // Turn on LED2 (PC1) for AAN
             break;
         case WAARSCHUWING:
             PORTC = (1 << PC2);  // Turn on LED3 (PC2) for WAARSCHUWING
             break;
     }
 }
 
 void updateFSM(FSM *fsm, Event event) {
     switch (fsm->currentState) {
         case UIT:
             if (event == SET) {
                 fsm->currentState = AAN; // Switch to AAN
                 printf("Alarm is AAN\n");
             } else if (event == WAARSCHUWING2) {
                 fsm->currentState = WAARSCHUWING; // Switch to WAARSCHUWING
                 printf("Alarm in WAARSCHUWING\n");
             }
             break;
 
         case AAN:
             if (event == RESET) {
                 fsm->currentState = UIT; // Switch to UIT
                 printf("Alarm is UIT\n");
             } else if (event == WAARSCHUWING2) {
                 fsm->currentState = WAARSCHUWING; // Switch to WAARSCHUWING
                 printf("Alarm in WAARSCHUWING\n");
             }
             break;
 
         case WAARSCHUWING:
             if (event == RESET) {
                 fsm->currentState = UIT; // Switch to UIT
                 printf("Alarm is UIT\n");
             } else if (event == SET) {
                 fsm->currentState = AAN; // Switch to AAN
                 printf("Alarm is AAN\n");
             }
             break;
     }
 }
 
 // Read buttons and return the corresponding event
 Event readButtons() {
     if (PIND & (1 << PD0)) {
         return SET; // Button PD0 pressed: SET
     } else if (PIND & (1 << PD1)) {
         return RESET; // Button PD1 pressed: RESET
     } else if (PIND & (1 << PD2)) {
         return WAARSCHUWING2; // Button PD2 pressed: WAARSCHUWING
     }
     return -1; // No button pressed
 }
 
 int main(void) {
     FSM alarmSystem;
     initFSM(&alarmSystem); // Initialize the FSM
 
     // Configure LEDs on PORTC (PC0, PC1, PC2) as outputs
     DDRC = 0x07; // Lower three bits as outputs
     DDRD = 0x00; // All PORTD pins as inputs
     PORTD = 0xFF; // Enable internal pull-up resistors on PORTD
 
     // FSM loop
     while (1) {
         // Read button input and get the corresponding event
         Event event = readButtons();
 
         // If an event is detected, update the FSM and LEDs
         if (event != -1) {
             updateFSM(&alarmSystem, event);
             updateLEDs(alarmSystem.currentState);
         }
 
         _delay_ms(100); // Simple debounce delay
     }
 
     return 0;
 }
