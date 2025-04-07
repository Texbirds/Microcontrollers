/*
 *
 * Created: 7-2-2025 10:41:46
 * Author : Pieter
 */

 #include <avr/io.h>
 #include <util/delay.h>
 
 void wait(int ms){
     for (int i = 0; i < ms; i++){
         _delay_ms(1); // Delay of 1 ms per iteration
     }
 }
 
 int main(void)
 {
     DDRD = 0xFF; // Data direction register D: all pins as output
 
     while (1) 
     {
         // Shift a single LED from PD0 to PD7
         for (int i = 0; i < 8; i++)
         {
             // Shift bit to the left by i positions
             PORTD = (0b00000001 << i);
 
             // Wait 500 ms to see the LED move
             wait(500);
         }
     }
 }
