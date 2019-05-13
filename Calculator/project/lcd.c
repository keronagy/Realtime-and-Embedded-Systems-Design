
/* p3_2.c: Initialize and display "Hello" on the LCD sing 4-bit data mode. All interface uses Port B */

/* This program strictly follows HD44780 datasheet for timing. You may want to adjust the amount of delay for your LCD controller. */


#define RS 1    /* BIT0 mask */ 
#define RW 2    /* BIT1 mask */ 
#define EN 4    /* BIT2 mask */


void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
#include "tm4c123gh.h"
#include "delay.h"
//Function to delay t microseconds




void LCD_init(void)
{
		
		SYSCTL_RCGCGPIO_R |=0x00000008;            //Enable PortC clock 
	delayMs(40);
	GPIO_PORTD_DIR_R |= 0x00000007;            //make first 3 bits as output
	GPIO_PORTD_DEN_R |= 0x00000007;            // enable first 3 bits of port c 
	GPIO_PORTD_AMSEL_R = 0x0;                 // Disabeling Analog Function of port C
	GPIO_PORTD_AFSEL_R = 0x0;                 // Disabeling Alternative Function of port B

    delayMs(20);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(50);
    LCD_nibble_write(0x30, 0);
    delayUs(1000);
    LCD_nibble_write(0x30, 0);
    delayUs(400);


    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);          /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
		LCD_command(1);       /* clear display */
		LCD_command(0x80);    /* lcd cursor location */	
		delayMs(20);
		LCD_command(1);
		LCD_command(0x80);
		LCD_command(2);
}

void LCD_nibble_write(unsigned char data, unsigned char control)
{
  data &= 0xF0;       /* clear lower nibble for control */
	control &= 0x0F;    /* clear upper nibble for data */ 
	LCD_Control = control;
	KL_input = data>>4;
	LCD_Control = control | EN;    
  delayUs(0);
	LCD_Control = 0;
  KL_input = 0;
}

void LCD_command(unsigned char command)
{
    LCD_nibble_write(command & 0xF0, 0);   /* upper nibble first */
    LCD_nibble_write(command << 4, 0);     /* then lower nibble */
    
    if (command < 4)
        delayMs(2);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayUs(40);        /* all others 40 us */
}

void LCD_data(unsigned char data)
{
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */
    
    delayUs(40);
}




void cursorLeft()
{
	LCD_command(0x10);
}


void cursorRight()
{
	LCD_command(0x14);
}
/* This function is called by the startup assembly code to perform system specific initialization tasks. */
