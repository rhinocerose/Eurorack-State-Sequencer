#include <stdio.h>
#include <stdlib.h>
#include "LPC804.h"
#include "spi_oled_master.h"

int dir, encoder0Pos;

void OLED_Test(char *status, char*status2)
{
    //char* status = " 2.25  2.35   4.47   5.53";
    //char* status2 = "  5.71v  ";
	OLED_Clear();
    OLED_SetText(0, 05, kOLED_Pixel_Set, status, 1);
    OLED_SetText(0, 20, kOLED_Pixel_Set, status, 1);
    OLED_SetText(0, 40, kOLED_Pixel_Set, status2, 3);
    OLED_DrawFrame();
    OLED_Refresh();

    //   char* status0 = "2.25";
    //   char* status1 = "2.35";

    //    float i;
    //    char status0[5];
    //    //status0[4] = '\0';
    //    char status1[5];
    //    //status1[4] = '\0';
    //    i=2.25;
    //    snprintf(status0, 5, "%1.2f\0", i);
    //    OLED_SetText(03, 05, kOLED_Pixel_Set, status0, 1);
    //    i=2.26;
    //    snprintf(status1, 5, "%1.2f\0", i);
    //    OLED_SetText(38, 05, kOLED_Pixel_Set, status1, 1);
    //	  OLED_Refresh();
}

void doEncoder(void)
{
	if (GPIO_GetVal(ENCODER_A) == HIGH)
	{
		if (GPIO_GetVal(ENCODER_B) == LOW && encoder0Pos > 0)
		{
			encoder0Pos = encoder0Pos - 1;
			dir = 0;
			printf("Moved left");
		}
		else
		{
			encoder0Pos = encoder0Pos + 1;
			dir = 1;
			printf("Moved right");
		}
	}
	else
	{
		if (GPIO_GetVal(ENCODER_B) == LOW )
		{
			encoder0Pos = encoder0Pos + 1;
			dir = 1;
			printf("Moved right");
		}
		else
		{
			if (encoder0Pos > 0)
			{
			encoder0Pos = encoder0Pos - 1;
			dir = 0;
			printf("Moved left");
			}
		}
	}
}

int main(void) {

	Board_Init();
    //    DAC_Init();
//	GPIO_PinConfig(12, OUTPUT_ON);
//    GPIO_PinConfig(ENCODER_A, INPUT);
//    GPIO_PinConfig(ENCODER_B, INPUT);

//	Board_IRQInit();
    char* str1 = " 2.25  2.35   4.47   5.53";
    char* str2 = "  5.71v  ";

    OLED_Test(str1, str2);
    str1 = " 4.25  2.35   4.47   5.53";
    str2 = "  8.71v  ";
    OLED_Test(str1, str2);
    str1 = " 2.25  4.35   4.47   5.53";
    str2 = "  2.71v  ";
    OLED_Test(str1, str2);


    while(1)
    {
    	doEncoder();

    }
    return 0 ;
}
