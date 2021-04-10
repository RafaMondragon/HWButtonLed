/* Name: buttonled.c
 * Authors: Rafael Mondragón Jaramillo A01066397
 * Description: Each of the 3 LEDs in the KL25z board are controlled by 3 push buttons
 */

//[Task1] Leave GREEN LED toggling, and set RED LED to turn on when button is
//        pressed and turned off when button is released
//TODO[CODE][Task2] Leave GREEN LED toggling, and set RED LED and BLUE LEDs to turn on
//                  when button0 and button1 are pressed respectively
//                  and turned off when buttons 0 and 1 are released. Create "function1" and
//                  move this task there and call it from main.
//TODO[CODE][Task3] Create function2 that turns on/off LEDs RED, GREEN, BLUE when their
//                  respective buttons are pressed/released, but when ALL THREE buttons are pressed
//                  at the same time, LED shall start blinking (any color).

#include <stdbool.h>
#include "MKL25Z4.h"

//GREEN LED IS IN PORTB PIN19
#define GREEN_LED_BIT 19
#define GREEN_LED_PCR PORTB->PCR[GREEN_LED_BIT]
#define RED_LED_BIT 18
#define RED_LED_PCR PORTB->PCR[RED_LED_BIT]
#define BLUE_LED_BIT 1
#define BLUE_LED_PCR PORTD->PCR[BLUE_LED_BIT]
//Button0 is at PTA1
#define BUTTON0_BIT 1
#define BUTTON0_GPIO GPIOA
#define BUTTON0_PCR PORTA->PCR[BUTTON0_BIT]
//Button1 is at PTA2
#define BUTTON1_BIT 2
#define BUTTON1_GPIO GPIOA
#define BUTTON1_PCR PORTA->PCR[BUTTON1_BIT]
//Button2 is at PTD4
#define BUTTON2_BIT 4
#define BUTTON2_GPIO GPIOA
#define BUTTON2_PCR PORTA->PCR[BUTTON2_BIT]


void delayMs(int n);
void function0();
void function1();
void function2();


int main (void) {
  //Setup
    //SIM->SCGC5 |= 0x400; //Enable PORTB clock gate
    SIM->SCGC5 |= SIM_SCGC5_PORTB(1); //Enable PORTB clock gate
    SIM->SCGC5 |= SIM_SCGC5_PORTA(1); //Enable PORTA clock gate
    SIM->SCGC5 |= SIM_SCGC5_PORTD(1); //Enable PORTD clock gate
    
    //PORTB->PCR[19] = 0x0100;
    GREEN_LED_PCR = PORT_PCR_MUX(1); // Set GREEN_LED bit as GPIO
    RED_LED_PCR = PORT_PCR_MUX(1); // Set RED_LED bit as GPIO
    BLUE_LED_PCR = PORT_PCR_MUX(1); // Set BLUE_LED bit as GPIO

    BUTTON0_PCR = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1); // Set port pin as GPIO with pull up resistor enabled
    BUTTON1_PCR = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1); // Set port pin as GPIO with pull up resistor enabled
    BUTTON2_PCR = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1); // Set port pin as GPIO with pull up resistor enabled

    //GPIOB->PDDR |= 0x80000;          /* make PTB19 as output pin */
    GPIOB->PDDR |= (1<<GREEN_LED_BIT) | (1<<RED_LED_BIT) | (1<<BLUE_LED_BIT); //Set GREEN_LED_BIT, BLUE_LED_BIT and RED_LED_BIT pin as output
    GPIOB->PDOR |= (1<<RED_LED_BIT);//Turn RED LED off
    GPIOD->PDOR |= (1<<BLUE_LED_BIT);//Turn BLUE LED off
    BUTTON0_GPIO->PDDR &= ~(1<<BUTTON0_BIT); //Configure BUTTON0_BIT as input
    BUTTON1_GPIO->PDDR &= ~(1<<BUTTON1_BIT); //Configure BUTTON1_BIT as input
    BUTTON2_GPIO->PDDR &= ~(1<<BUTTON2_BIT); //Configure BUTTON2_BIT as input


    while (1) {
      //Loop
        //function0();
        //function1();
        function2();
    }
}

/* Delay n milliseconds
 * The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
 */
void delayMs(int n) {
    int i;
    int j;
    for(i = 0 ; i < n; i++)
        for (j = 0; j < 7000; j++) {}
}
void function0() {
	GPIOB->PDOR &= ~(1<<GREEN_LED_BIT);     /* turn on green LED */
	delayMs(250);
	GPIOB->PDOR |= (1<<GREEN_LED_BIT);      /* turn off green LED */
	delayMs(250);

	if((BUTTON0_GPIO->PDIR & (1<<BUTTON0_BIT)) == (1<<BUTTON0_BIT)){ //BUTTON0 NOT PRESSSED
	   GPIOB->PDOR |= (1<<RED_LED_BIT); //Turn RED LED off
	}else{ //BUTTON0 IS PRESSED
	   GPIOB->PDOR &= ~(1<<RED_LED_BIT); //Turn RED LED on
	}
}
void function1() {
	GPIOB->PDOR &= ~(1<<GREEN_LED_BIT);     /* turn on green LED */
	delayMs(250);
	GPIOB->PDOR |= (1<<GREEN_LED_BIT);      /* turn off green LED */
	delayMs(250);

	if((BUTTON0_GPIO->PDIR & (1<<BUTTON0_BIT)) == (1<<BUTTON0_BIT)){ //BUTTON0 NOT PRESSSED
	   GPIOB->PDOR |= (1<<RED_LED_BIT); //Turn RED LED off
	}else{ //BUTTON0 IS PRESSED
	   GPIOB->PDOR &= ~(1<<RED_LED_BIT); //Turn RED LED on
	}

	if((BUTTON1_GPIO->PDIR & (1<<BUTTON1_BIT)) == (1<<BUTTON1_BIT)){ //BUTTON1 NOT PRESSSED
	   GPIOD->PDOR |= (1<<BLUE_LED_BIT); //Turn BLUE LED off
    }else{ //BUTTON1 IS PRESSED
	   GPIOD->PDOR &= ~(1<<BLUE_LED_BIT); //Turn BLUE LED on
	}
}

void function2() {
	bool red=false;
	bool green=false;
	bool blue=false;
   if((BUTTON0_GPIO->PDIR & (1<<BUTTON0_BIT)) == (1<<BUTTON0_BIT)){ //BUTTON0 NOT PRESSSED
	   GPIOB->PDOR |= (1<<RED_LED_BIT); //Turn RED LED off
      red=false;
	}else{ //BUTTON0 IS PRESSED
	   GPIOB->PDOR &= ~(1<<RED_LED_BIT); //Turn RED LED on
      red=true;
	}

   if((BUTTON1_GPIO->PDIR & (1<<BUTTON1_BIT)) == (1<<BUTTON1_BIT)){ //BUTTON1 NOT PRESSSED
	   GPIOB->PDOR |= (1<<GREEN_LED_BIT); //Turn GREEN LED off
      green=false;
	}else{ //BUTTON1 IS PRESSED
	   GPIOB->PDOR &= ~(1<<GREEN_LED_BIT); //Turn GREEN LED on
      green=true;
	}

	if((BUTTON2_GPIO->PDIR & (1<<BUTTON2_BIT)) == (1<<BUTTON2_BIT)){ //BUTTON2 NOT PRESSSED
	   GPIOD->PDOR |= (1<<BLUE_LED_BIT); //Turn BLUE LED off
      blue=false;
    }else{ //BUTTON2 IS PRESSED
	   GPIOD->PDOR &= ~(1<<BLUE_LED_BIT); //Turn BLUE LED on
      blue=true;
   }

   if ((red==true) & (green==true) & (blue==true)){
      GPIOB->PDOR &= ~(1<<RED_LED_BIT); //Turn RED LED on
      GPIOB->PDOR &= ~(1<<GREEN_LED_BIT); //Turn GREEN LED on
      GPIOD->PDOR &= ~(1<<BLUE_LED_BIT); //Turn BLUE LED on
      delayMs(150);
      GPIOB->PDOR |= (1<<RED_LED_BIT); //Turn RED LED off
      GPIOB->PDOR |= (1<<GREEN_LED_BIT); //Turn GREEN LED off
      GPIOD->PDOR |= (1<<BLUE_LED_BIT); //Turn BLUE LED off
      delayMs(150);
   }
}
