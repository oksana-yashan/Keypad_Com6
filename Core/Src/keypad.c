
#include "DISPLAY_WH1602B_4Bit.h"
#include "usbd_cdc_if.h"
#include <string.h>




uint8_t data[33];

char str[4];
char str_tx[6];
char new_line[2] = "\r\n";

uint8_t cycle = 1;
uint8_t n_chars = 0;


//peripheral register addresses

uint32_t volatile *const pGPIODModeReg  =  (uint32_t*)(0x40020C00);
uint32_t volatile *const pInPutDataReg  =  (uint32_t*)(0x40020C00+0x10);
uint32_t volatile *const pOutPutDataReg =  (uint32_t*)(0x40020C00+0x14);
uint32_t volatile *const pClockCtrlReg  =  (uint32_t*)(0x40023800+0x30);
uint32_t volatile *const pPullupDownReg =  (uint32_t*)(0x40020C00 + 0x0C);





void delay(void)
{
	for(uint32_t i=0 ; i < 1200000 ; i++);
}



void setup_GPIO_for_keypad() {

	   //1.Enable the peripheral clock of GPIOD peripheral
		*pClockCtrlReg |= ( 1 << 3);

	  // 2.configure PD0,PD1,PD2,PD3 as output (rows)
		*pGPIODModeReg &= ~(0xFF); //clear
		*pGPIODModeReg |= 0x55;   //set

		// 3. configure PD8 , PD9, PD10, PD11 as input (columns)
	  *pGPIODModeReg &= ~(0xFF << 16);

		// 4.Enable internal pull-up resistors for PD8 PD9 PD10 PD11
	  *pPullupDownReg &= ~(0xFF << 16);
	  *pPullupDownReg |=  (0x55 << 16);
}


void copy(char* dest, char* src, int len) {
	for (int i=0; i<len; ++i) {
		*dest++ = *src++;
	}
}


void convert_hex_to_bin() {
		int count=0;
		while(n_chars)
		{
			switch(data[count])
			{
				case '0' : copy(str, "0000", 4);
					break;
				case '1' : copy(str, "0001", 4);
					break;
				case '2' : copy(str, "0010", 4);
					break;
				case '3' : copy(str, "0011", 4);
					break;
				case '4' : copy(str, "0100", 4);
					break;
				case '5' : copy(str, "0101", 4);
					break;
				case '6' : copy(str, "0110", 4);
					break;
				case '7' : copy(str, "0111", 4);
					break;
				case '8' : copy(str, "1000", 4);
					break;
				case '9' : copy(str, "1001", 4);
					break;
				case 'A' : copy(str, "1010", 4);
					break;
				case 'B' : copy(str, "1011", 4);
					break;
				case 'C' : copy(str, "1100", 4);
					break;
				case 'D' : copy(str, "1101", 4);
					break;
			}

			output_keypad_data(str);

			++count;
			--n_chars;
		}

		output_keypad_new_line();
}


void output_keypad_data(char str[4]) {

	copy(str_tx, str, 4);
	str_tx[4] = '\r';
	str_tx[5] = '\n';
	CDC_Transmit_FS((unsigned char*)str_tx, 6);
	print_display_array(str);
	HAL_Delay(50);
}

void output_keypad_new_line() {

	HAL_Delay(500);
	CDC_Transmit_FS((unsigned char*)new_line, 2);
}

char keypad_symbols[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

void Keypad_loop() {

//		  	 if (n_chars == 15) {
//		  		 setup_second_line();
//		  	 }


			for (uint8_t i=0; i<4; ++i) {

				//make all rows HIGH
				*pOutPutDataReg |= 0x0f;

				//make Ri LOW(PD0)
				*pOutPutDataReg &= ~( 1 << i);

				 //scan the columns

				//check C1 of Ri is low or high
				if(!(*pInPutDataReg & ( 1 << 8))) {
					//key is pressed
					if (i != 3) {
						delay();
						print_display(keypad_symbols[i*4]);
						data[n_chars] = keypad_symbols[i*4];
						++n_chars;
					}
					else {
						Display_Write_Ins(GO_TO_START_SECOND_LINE);
						convert_hex_to_bin();
					}
				}

				//check C1 of Ri is low or high
				if(!(*pInPutDataReg & ( 1 << 9))) {
					//key is pressed
					delay();
					print_display(keypad_symbols[i*4+1]);
					data[n_chars] = keypad_symbols[i*4+1];
					++n_chars;
				}

				//check C1 of Ri is low or high
				if(!(*pInPutDataReg & ( 1 << 10))) {
					//key is pressed
					if (i != 3) {
						delay();
						print_display(keypad_symbols[i*4+2]);
						data[n_chars] = keypad_symbols[i*4+2];
						++n_chars;
					}
					else {
						Display_clear();
						Display_Write_Ins(GO_TO_START_FIRST_LINE);
						n_chars = 0;
					}
				}

				//check C1 of Ri is low or high
				if(!(*pInPutDataReg & ( 1 << 11))) {
					//key is pressed
					if (i != 3) {
						delay();
						print_display(keypad_symbols[i*4+3]);
						data[n_chars] = keypad_symbols[i*4+3];
						++n_chars;
					}
					else {
						Display_Write_Ins(GO_TO_START_SECOND_LINE);
						convert_hex_to_bin();
					}
				}
				delay();
			}
}





//		     //make all rows HIGH
//		    *pOutPutDataReg |= 0x0f;
//
//		    //make R1 LOW(PD0)
//		    *pOutPutDataReg &= ~( 1 << 0);
//
//		    //scan the columns
//
//		    //check C1(PD8) low or high
//		    if(!(*pInPutDataReg & ( 1 << 8))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('1');
//		    	data[n_chars] = '1';
//		    	++n_chars;
//		    }
//
//		    //check C2(PD9) low or high
//		    if(!(*pInPutDataReg & ( 1 << 9))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('2');
//		    	data[n_chars] = '2';
//		    	++n_chars;
//		    }
//
//		    //check C3(PD10) low or high
//		    if(!(*pInPutDataReg & ( 1 << 10))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('3');
//		    	data[n_chars] = '3';
//		    	++n_chars;
//		    }
//
//		    //check C4(PD11) low or high
//		    if(!(*pInPutDataReg & ( 1 << 11))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('A');
//		    	data[n_chars] = 'A';
//		    	++n_chars;
//		    }
//
//
//		    //make all rows HIGH
//		     *pOutPutDataReg |= 0x0f;
//		    //make R2 LOW(PD1)
//		    *pOutPutDataReg &= ~( 1 << 1);
//
//		    //scan the columns
//		    //check C1(PD8) low or high
//		    if(!(*pInPutDataReg & ( 1 << 8))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('4');
//		    	data[n_chars] = '4';
//		    	++n_chars;
//		    }
//
//		    //check C2(PD9) low or high
//		    if(!(*pInPutDataReg & ( 1 << 9))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('5');
//		    	data[n_chars] = '5';
//		    	++n_chars;
//		    }
//
//		    //check C3(PD10) low or high
//		    if(!(*pInPutDataReg & ( 1 << 10))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('6');
//		    	data[n_chars] = '6';
//		    	++n_chars;
//		    }
//
//		    //check C4(PD11) low or high
//		    if(!(*pInPutDataReg & ( 1 << 11))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('B');
//		    	data[n_chars] = 'B';
//		    	++n_chars;
//		    }
//
//		    //make all rows HIGH
//		     *pOutPutDataReg |= 0x0f;
//		    //make R3 LOW(PD2)
//		    *pOutPutDataReg &= ~( 1 << 2);
//
//		    //scan the columns
//		    //check C1(PD8) low or high
//		    if(!(*pInPutDataReg & ( 1 << 8))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('7');
//		    	data[n_chars] = '7';
//		    	++n_chars;
//		    }
//
//		    //check C2(PD9) low or high
//		    if(!(*pInPutDataReg & ( 1 << 9))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('8');
//		    	data[n_chars] = '8';
//		    	++n_chars;
//		    }
//
//		    //check C3(PD10) low or high
//		    if(!(*pInPutDataReg & ( 1 << 10))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('9');
//		    	data[n_chars] = '9';
//		    	++n_chars;
//		    }
//
//		    //check C4(PD11) low or high
//		    if(!(*pInPutDataReg & ( 1 << 11))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('C');
//		    	data[n_chars] = 'C';
//		    	++n_chars;
//		    }
//
//		    //make all rows HIGH
//		     *pOutPutDataReg |= 0x0f;
//		    //make R4 LOW(PD2)
//		    *pOutPutDataReg &= ~( 1 << 3);
//
//		    //scan the columns
//		    //check C1(PD8) low or high
//		    if(!(*pInPutDataReg & ( 1 << 8))){
//		    	//key is pressed
//		    	delay();
//	//	    	print_display('*');
//		    	Display_Write_Ins(GO_TO_START_SECOND_LINE);
//		    	convert_hex_to_bin();
//		    }
//
//		    //check C2(PD9) low or high
//		    if(!(*pInPutDataReg & ( 1 << 9))){
//		    	//key is pressed
//		    	delay();
//		    	print_display('0');
//		    	data[n_chars] = '0';
//		    	++n_chars;
//		    }
//
//		    //check C3(PD10) low or high
//		    if(!(*pInPutDataReg & ( 1 << 10))){
//		    	//key is pressed
//		    	delay();
//	//	    	print_display('#');
//		    	Display_Write_Ins(GO_TO_START_SECOND_LINE);
//		    	convert_hex_to_bin();
//		    }
//
//		    //check C4(PD11) low or high
//		    if(!(*pInPutDataReg & ( 1 << 11))){
//		    	//key is pressed
//		    	delay();
////		    	print_display('D');
////		    	data[n_chars] = 'D';
////		    	++n_chars;
//		    	Display_clear();
//		    	Display_Write_Ins(GO_TO_START_FIRST_LINE);
//		    	n_chars = 0;
//		    }
//		    delay();
//}


//
//int main(void)
//{
//
////  setup_clock();
////  setup_LED(LED_PORT, LED_PINS);
//  setup_GPIO_for_keypad();
//
//  //Display_example();
//  //delay();
//
//  setup_Display();
//  setup_first_line();
//
//
//}

