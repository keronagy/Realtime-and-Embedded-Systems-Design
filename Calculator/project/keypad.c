#include "tm4c123gh.h"

#include "delay.h"

void KEYPAD_init (void){
	SYSCTL_RCGCGPIO_R |=0x00000010;  //Enable PortE clock 
	delayMs(40);
	GPIO_PORTE_DIR_R =0x00;          //make PortE registers as input resgisters
	GPIO_PORTE_DEN_R |=0x0000000F;   // Digital Enable of first 4 bits of port E
	GPIO_PORTE_AMSEL_R = 0x00;       // Disabeling Analog Function of port E
	GPIO_PORTE_AFSEL_R = 0x00;       // Disabeling Alternative Function of port E
	GPIO_PORTE_PUR_R |= 0x0F;
}

unsigned char keypad_kbhit(void)
{
   int col;

    /* check to see any key pressed */
    KL_input = 0;               /* enable all rows */
    col =(Keypad_output <<4) & 0xF0;      /* read all columns */
    if (col == 0xF0) 
        return 0;   /* no key pressed */
    else
        return 1;   /* a key is pressed */
}

unsigned char keypad_getkey(void)
{
    const unsigned char keymap[4][4] = {
        { '1', '2', '3', '+'},
        { '4', '5', '6', '-'},
        { '7', '8', '9', '*'},
        { '.', '0', '=', '/'},
    };
    
    int row, col;

    /* check to see any key pressed first */
   KL_input = 0;               /* enable all rows */
    col = (Keypad_output <<4) & 0xF0;      /* read all columns */
    if (col == 0xF0) return 0;          /* no key pressed */

    /* If a key is pressed, it gets here to find out which key. */
    /* Although it is written as an infinite loop, it will take one of the breaks or return in one pass.*/
    while (1)
    {
        row = 0;
        KL_input = 0x0E;        /* enable row 0 */
        delayUs(2);                     /* wait for signal to settle */
        col = (Keypad_output <<4) & 0xF0;
        if (col != 0xF0) break;
        
        row = 1;
        KL_input = 0x0D;        /* enable row 1 */
        delayUs(2);                     /* wait for signal to settle */
        col = (Keypad_output <<4) & 0xF0;
        if (col != 0xF0) break;
        
        row = 2;
        KL_input = 0x0B;        /* enable row 2 */
        delayUs(2);                     /* wait for signal to settle */
        col = (Keypad_output <<4) & 0xF0;
        if (col != 0xF0) break;
        
        row = 3;
        KL_input = 0x07;        /* enable row 3 */
        delayUs(2);                     /* wait for signal to settle */
        col = (Keypad_output <<4) & 0xF0;
        if (col != 0xF0) break;
        
        return 0;   /* if no key is pressed */
    }
    
    /* gets here when one of the rows has key pressed */
    if (col == 0xE0) return keymap[row][0]; /* key in column 0 */
    if (col == 0xD0) return keymap[row][1]; /* key in column 1 */
    if (col == 0xB0) return keymap[row][2]; /* key in column 2 */
    if (col == 0x70) return keymap[row][3]; /* key in column 3 */ 
    return 0;   /* just to be safe */
}
