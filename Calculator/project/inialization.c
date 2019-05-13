#include "tm4c123gh.h"
#include "LCDHeader.h"
//This Function is used to send commands to the LCD



void PortF_PortB_init(void){
	  SYSCTL_RCGCGPIO_R |=0x00000022;               // Enable Port B&F Clock
  	GPIO_PORTF_LOCK_R = 0x4C4F434B;
	  GPIO_PORTB_DIR_R  |=0x0000000F;               // make first 4 bits of port b as outputs
	  GPIO_PORTB_DEN_R  |=0x0000000F;               // Digital Enable of first 4 bits 
	  GPIO_PORTB_DATA_R |=0x00000000;               //make the  pins equal 1 in data register
	  GPIO_PORTB_AMSEL_R = 0x00;                  // Disabeling Analog Function of port B
		GPIO_PORTB_AFSEL_R = 0x00;                 // Disabeling Alternative Function of port B
		
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog on PF
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) PCTL GPIO on PF4-0
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 in, PF3-1 out
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) disable alt funct on PF7-0
  GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital I/O on PF4-0

}



