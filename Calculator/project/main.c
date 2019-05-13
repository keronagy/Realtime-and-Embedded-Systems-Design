//LCD Project 

//Include Section 
#include "tm4c123gh6pm.h"
#include "tm4c123gh.h"
#include "LCDHeader.h"
#include "delay.h"
#include "keypad.h"
#include "math.h"
//Code section
void SystemInit(){};

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

#define LCDSIZE 16

	
enum state { number, operation };
int cursorPos;
int opEntered;
int dotAdded;
enum state currentState;
char operator_;
char previousOperand [LCDSIZE];
char currentOperand [LCDSIZE];
char c;
int i, ret;
char currentExp[LCDSIZE];
int sw1,sw2;

//output led
#define RED       0x02
#define BLUE      0x04
#define GREEN     0x08


int parseInt(char *s, int start, int end)
{
	int i;
	int result = 0;
	for (i = start; i < end; i++)
	{
		result *= 10;
		result += s[i] -'0';

	}
	return result;
}

int findDecimalIndex(char *s)
{
	int i = 0;
	while (s[i] != '.' && s[i] )
		i++;	
	return i;
}
float parsefloat(char *s)
{
	int decimalIndex = findDecimalIndex(currentOperand);
	int length = strlen(s);
	int integerPart = parseInt(s, 0, decimalIndex);
	int decimalPart = parseInt(s, decimalIndex + 1, length);

	return (float)integerPart + ((float)(decimalPart)* pow(10, -(length - 1 - decimalIndex)));


}

void PortF_Output(uint32_t data){ // write PF3-PF1 outputs
  GPIO_PORTF_DATA_R = data;      
}

	void clearStrings(int all)
	{
	for(i=0; i<LCDSIZE; i++)
		{
		previousOperand[i] = '\0';
		currentOperand[i] = '\0';
		if(all) currentExp[i] = '\0';
		opEntered=0;
		}
	}


	int calc(char * operand1, char * operand2, char operator_)
{
	float a,b,result;
	 a = parsefloat(operand1);
		printf("dummy"); //atoi doesn't work until you put dummy print no idea why atof doesn't work at all
	 b = parsefloat(operand2);
		printf("dummy");

	switch (operator_)
	{
	case '+':
		result = a + b;
		break;
	case '-':
		result = a - b;
		break;
	case '*':
		result = a * b;
		break;
	case '/':
		result = a / b;
		break;
	}
	return result;
}

int lengthOf(char* str)
{
	int length = 0;
	int j=0;
	for(j=0; j<LCDSIZE ;j++)
	{
		length++;
	if(str[j] == '\0')
		break;
	}
	return length;
}

int isOperator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}
int isNumber(char c)
{
	return (c >= '0' && c <= '9');
}

void append(char* s, char c)
{
	strcat(s, &c);
}

char getKey()
{
	char c = keypad_getkey();
	return c;
}

int display(char c)
{
  if(lengthOf(currentExp) > LCDSIZE || (c == '\0'))
		return 0; 
	
	LCD_data(c);
	
	return 1;
}



void clear()
{
		opEntered = 0;
		dotAdded = 0;
	operator_ = '\0';
		currentState = operation;
		cursorPos = 0;
		clearStrings(1);
		LCD_command(0x01);
	delayMs(40);
		LCD_command(0x80);
		delayMs(40);
		LCD_command(0x01);
	delayMs(40);
		LCD_command(0x80);
	
}

int displayString(char *s)
{
	
	int len = strlen(s);
	int i;
	delayMs(100);
	LCD_command(1);
	
	for (i = 0; i <= lengthOf(currentExp); i++)
	{
		delayMs(100);
		if(!display(s[i]))
		{
			return 0;
		}
		
	}
		return 1;
}

void operatorEntered(char c)
{
	*currentOperand = '\0';
	dotAdded = 0;
	operator_ = c;

	display(operator_);
	delayMs(100);
	currentState = operation;
	opEntered = 1;
}




int addchar(char* str , char c)
{
	//returns the index in which the char is added
	int length=-1;
	
		for(i=0; i<LCDSIZE; i++)
		{
			length++;
			if(str[i] == '\0')
			{
				str[i] = c;
				break;
			}
		}
		return length;
}


int calculator(char c)
{


		if (isOperator(c))
		{
			if (currentState == number)
			{
				if (opEntered)
				{
					float result = calc(previousOperand, currentOperand, operator_);
					
//					char res[20];
					
					if(c == '=')
						clearStrings(1);
					else clearStrings(0);
					
					sprintf(currentExp, "%f", result); // copy the result to currentexp and return 1;
					
					return 1;
					//displayString(res); 
					//strcpy(previousOperand,res);

					operatorEntered(c);
				}
				else
				{
					if (c != '=')
					{
						strcpy(previousOperand,currentOperand);
						operatorEntered(c);
					}
					else
					{
						return -1;
					}
				}
			}
			else
			{
				return -1;
			}
		}
	

	 else if (isNumber(c) || (c == '.' && currentState == number && !dotAdded))
		{
			if(c == '.')
				dotAdded = 1;
			append(currentOperand, c);
			currentState = number;
		}
		return 0;
	}


	void readswitches()
	{
		//something is wrong in initialization for switches btw
			sw1 = GPIO_PORTF_DATA_R & 0x10;
			sw2 = GPIO_PORTF_DATA_R & 0x01;
			//sw3
	}


	
	
int main(){
sw1=0; sw2=0;
cursorPos=0;
	//previousOperand [20]=;
 //currentOperand [20];
		PortF_PortB_init();
		delayMs(100);
	  LCD_init();
		KEYPAD_init();
		

		clear();
	
    while(1)
    {
			
			
			ret =0;
			while(!keypad_kbhit())
			{		
				readswitches();

				if(!sw1)
				{
					cursorPos--;
					cursorLeft();
				}
				if(!sw2)
				{
					cursorPos++;
					cursorRight();
				}
				
				if(!sw1 && !sw2)
				{
					
					clear();					
					continue;
					
				}
				delayMs(500000); 
			}
			c = getKey();
			

				
			cursorPos = addchar(currentExp, c) + 1; //addchar returns the index in which the char is added
				if(c=='=')
				{
					
				for(i=0; i<=lengthOf(currentExp);i++)
					ret = calculator(currentExp[i]);
				}
			switch(ret)
			{
				case -1:
					//red led function here
				break;
				
				case 0:		
				//blue led function here
				display(c);
				break;
				
				case 1: 
					//green led function here
					displayString(currentExp);
				
				delayMs(500000);
				while(!keypad_kbhit() || c == '=')
						c = getKey();
						

				if(isOperator(c))
						{
					displayString("ANS");
					operatorEntered(c);
					opEntered=0;
					cursorPos = addchar(currentExp,c)+1;
						}
					
						else	clear();
						
						
				break;
			}
				
				
				
        delayMs(500000);
       
       // Delay_ms(500);
    }	
}

