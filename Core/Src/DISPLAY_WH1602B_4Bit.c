#include "DISPLAY_WH1602B_4Bit.h"
//#include <stdio.h>
#include <string.h>

static char empty_field[] = "                                ";
void Display_Init()
{
//	delay_milis(40);
	HAL_Delay(DISPLAY_DELAY_40);
	Display_Write_Ins(0x03); 	//as per documentation set 4-bit mode
	HAL_Delay(DISPLAY_DELAY_40);

	Display_Write_Ins(0x03); 	//as per documentation set 4-bit mode
	HAL_Delay(DISPLAY_DELAY_40);

	Display_Write_Ins(0x03); 	//as per documentation set 4-bit mode
	HAL_Delay(DISPLAY_DELAY_40);


	Display_Write_Ins(0x02); 	//as per documentation set 4-bit mode
	HAL_Delay(DISPLAY_DELAY_40);


//	Display_Write_Ins(0x28);
	Display_Write_Ins(TWO_LINE_MODE);
	HAL_Delay(DISPLAY_DELAY);

//	Display_Write_Ins(0x0F);
	Display_Write_Ins(DISPLAY_ON | CURSOR_OFF | CURSOR_BLINK_OFF);
	HAL_Delay(DISPLAY_DELAY);

//	Display_Write_Ins(0x01);
	Display_Write_Ins(DISPLAY_CLEAR);
	HAL_Delay(DISPLAY_DELAY);

//	Display_Write_Ins(0x06);
	Display_Write_Ins(INCREMENT_MODE | ENTIRE_SHIFT_OFF);
	HAL_Delay(DISPLAY_DELAY);

}






void Display_Write_Ins(char instruction)
{
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_RS, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_RW, GPIO_PIN_RESET);


	instruction & DISPLAY_BIT_7_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_6_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_5_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_4_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_RESET);

	Display_RW_pusle();
	HAL_Delay(DISPLAY_DELAY);

	instruction & DISPLAY_BIT_3_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_2_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_1_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_RESET);
	instruction & DISPLAY_BIT_0_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_RESET);

	Display_RW_pusle();
	HAL_Delay(DISPLAY_DELAY);
}

void Display_Write_Data(char data)
{
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_RW, GPIO_PIN_RESET);

	data & DISPLAY_BIT_7_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_RESET);
	data & DISPLAY_BIT_6_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_RESET);
	data & DISPLAY_BIT_5_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_RESET);
	data & DISPLAY_BIT_4_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_RESET);

	Display_RW_pusle();
	HAL_Delay(DISPLAY_DELAY);

	data & DISPLAY_BIT_3_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB7, GPIO_PIN_RESET);
	data & DISPLAY_BIT_2_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB6, GPIO_PIN_RESET);
	data & DISPLAY_BIT_1_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB5, GPIO_PIN_RESET);
	data & DISPLAY_BIT_0_MASK ? HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_SET) : HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_DB4, GPIO_PIN_RESET);

	Display_RW_pusle();
	HAL_Delay(DISPLAY_DELAY);
}

void Display_Write_Data_Array(char *data, uint8_t length)
{
//	if(length > DISPLAY_MAX_CHARACKTERS_COUNT)
//	{
//		error_signal(LED_PORT, RED_LED);
//	}
	while(length--)
	{
		Display_Write_Data(*data++);
	}
	HAL_Delay(DISPLAY_DELAY);
}

void Display_init_GPIO(void)
{
	HAL_Delay(400);
	setup_clock_for_GPIO(DISPLAY_PORT, ENABLE);
	GPIO_InitTypeDef DISPLAY_GPIO;
	DISPLAY_GPIO.Mode = GPIO_MODE_OUTPUT_PP;
	DISPLAY_GPIO.Speed = GPIO_SPEED_FREQ_HIGH;
	DISPLAY_GPIO.Pull = GPIO_NOPULL;

	DISPLAY_GPIO.Pin = DISPLAY_RS |  DISPLAY_RW | DISPLAY_ENA | DISPLAY_DB7 | DISPLAY_DB6 | DISPLAY_DB5 | DISPLAY_DB4;
	HAL_GPIO_Init(DISPLAY_PORT, &DISPLAY_GPIO);
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_GPIO.Pin, GPIO_PIN_SET);
}

//pulse
void Display_RW_pusle(void)
{
	HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_ENA, GPIO_PIN_SET);
    HAL_Delay(DISPLAY_DELAY);
    HAL_GPIO_WritePin(DISPLAY_PORT, DISPLAY_ENA, GPIO_PIN_RESET);
}

void Display_Write_srt(char *str, uint8_t length, uint8_t first_line_offset, uint8_t second_line_offset)
{
//	if(length > DISPLAY_MAX_CHARACKTERS_COUNT)
//	{
//		error_signal(LED_PORT, RED_LED);
//	}

	Display_Write_Ins(GO_TO_START_FIRST_LINE + first_line_offset);

	uint8_t i = 0;

	while(str[i] != '\0' && i < DISPLAY_MAX_FIRST_LINE)
	{
		Display_Write_Data(str[i++]);
	}

	if(i <= length)
	{
		Display_Write_Ins(GO_TO_START_SECOND_LINE + second_line_offset);
		while(str[i] != '\0' && i < DISPLAY_MAX_CHARACKTERS_COUNT)
		{
			Display_Write_Data(str[i++]);
		}
	}

}

void Display_clear(void)
{
	Display_Write_Ins(DISPLAY_CLEAR);
	HAL_Delay(2);
}


void Display_clear_field(uint8_t size)
{
	while(size--)
	{
		Display_Write_Data(' ');
	}
}

void setup_Display(void)
{
	Display_init_GPIO();
	Display_Init();
}

void setup_first_line() {
	Display_Write_Ins(GO_TO_START_FIRST_LINE);
}

void setup_second_line() {
	Display_Write_Ins(GO_TO_START_SECOND_LINE);
}

void print_display(char ch) {
	Display_Write_Data(ch);
}

void print_display_array(char array[4]) {
	for (int i=0; i<4; ++i) {
		Display_Write_Data(array[i]);
	}
}

void Display_example(void)
{
	Display_init_GPIO();

	Display_Init();

	Display_Write_Ins(GO_TO_START_FIRST_LINE);
	Display_Write_Data('H');
	Display_Write_Data('e');
	Display_Write_Data('l');
	Display_Write_Data('l');
	Display_Write_Data('o');
//	Display_Write_Data(',');
//	Display_Write_Data(' ');
//	Display_Write_Data('E');
//	Display_Write_Data('m');
//	Display_Write_Data('b');
//	Display_Write_Data('e');
//	Display_Write_Data('d');
//	Display_Write_Data('d');
//	Display_Write_Data('e');
//	Display_Write_Data('d');


//	HAL_Delay(1);
//
//	Display_Write_Ins(GO_TO_START_SECOND_LINE+5);
//	Display_Write_Data('N');
//	Display_Write_Data('U');
//	Display_Write_Data(' ');
//	Display_Write_Data('K');
//	Display_Write_Data('P');
//	Display_Write_Data('I');
//	Display_Write_Data(' ');
//	Display_Write_Data('F');
//	Display_Write_Data('O');
//	Display_Write_Data('R');
//	Display_Write_Data('E');
//	Display_Write_Data('V');
//	Display_Write_Data('E');
//	Display_Write_Data('R');
//	Display_Write_Data('!');
//	Display_Write_Data('!');
//	Display_Write_Data('!');
}

void Display_example2(void)
{
	Display_init_GPIO();
	Display_Init();

	char buff[31];
	uint32_t c = 1;
	char *header = "count = ";
	uint8_t header_len = strlen(header);

	Display_clear();
	Display_Write_srt(header ,header_len, 0, 0);
	while(c < 100)
	{
		Display_Write_Ins(GO_TO_START_FIRST_LINE + header_len);
		sprintf(buff, "%ld", c++);
		Display_Write_Data_Array(buff ,strlen(buff));
//		delay_milis(100);
		Display_Write_Data_Array(empty_field ,strlen(buff)); //clear field
	}
	HAL_Delay(1000);
	c--;
	while(c > 0)
	{
		Display_Write_Ins(GO_TO_START_FIRST_LINE + header_len);
		sprintf(buff, "%ld", c--);
		Display_Write_Data_Array(buff, strlen(buff));
//		delay_milis(100);
		Display_Write_Data_Array(empty_field ,strlen(buff)); //clear field
	}
}

