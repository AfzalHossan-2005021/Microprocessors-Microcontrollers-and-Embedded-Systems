/*
 * Experiment_02.c
 *
 * Created: 1/27/2024 9:30:21 PM
 * Author : Afzal Hossan
 */ 

#define  F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>



#define row_ddr DDRB
#define col_ddr DDRA
#define row_data PORTB
#define col_data PORTA

char rows[] = {1,2,4,8,16,32,64,128}; 

char ALPHA[][8] ={
	{0x3c,0x7e,0x66,0x66,0x7e,0x66,0x66,0x66},	//A
	{0x7c,0x62,0x62,0x7c,0x7c,0x62,0x62,0x7c},	//B
	{0x3e,0x7e,0x60,0x60,0x60,0x60,0x7e,0x3e},	//C
	{0x00,0xc0,0xa0,0xa0,0xa0,0xc0,0x00,0x00},	//D
	{0x7e,0x7e,0x60,0x7c,0x7c,0x60,0x7e,0x7e},	//E
	{0x7e,0x7e,0x60,0x7c,0x7c,0x60,0x60,0x60},	//F
	{0x3e,0x7e,0x60,0x6e,0x6e,0x66,0x7e,0x3e},	//G
	{0x66,0x66,0x66,0x7e,0x7e,0x66,0x66,0x66},	//H
	{0x7e,0x7e,0x18,0x18,0x18,0x18,0x7e,0x7e},	//I
	{0x7e,0x7e,0x0c,0x0c,0x0c,0x6c,0x6c,0x3c},	//J
	{0x62,0x66,0x6c,0x78,0x78,0x6c,0x66,0x62},	//K
	{0x60,0x60,0x60,0x60,0x60,0x60,0x7e,0x7e},	//L
	{0x63,0x77,0x7f,0x6b,0x63,0x63,0x63,0x63},	//M
	{0x63,0x73,0x7b,0x7b,0x6f,0x67,0x67,0x63},	//N
	{0x3c,0x7e,0x66,0x66,0x66,0x66,0x7e,0x3c},	//O
	{0x7c,0x7e,0x62,0x7e,0x7c,0x60,0x60,0x60},	//P
	{0x3c,0x42,0x42,0x42,0x42,0x4a,0x46,0x3f},	//Q
	{0x7c,0x7e,0x62,0x7e,0x7c,0x66,0x66,0x66},	//R
	{0x3e,0x7e,0x60,0x7c,0x7e,0x06,0x7e,0x7c},	//S
	{0x7e,0x7e,0x18,0x18,0x18,0x18,0x18,0x18},	//T
	{0x66,0x66,0x66,0x66,0x66,0x66,0x7e,0x3c},	//U
	{0x66,0x66,0x66,0x66,0x66,0x26,0x3c,0x18},	//V
	{0x63,0x63,0x63,0x6b,0x6b,0x7f,0x77,0x22},	//W
	{0x66,0x66,0x3c,0x18,0x18,0x3c,0x66,0x66},	//X
	{0xc3,0x66,0x3c,0x18,0x18,0x18,0x18,0x18},	//Y
	{0x7e,0x7e,0x06,0x0c,0x18,0x30,0x7e,0x7e},	//Z
	{0x3c,0x7e,0x66,0x66,0x66,0x66,0x7e,0x3c},	//0
	{0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x7e},	//1
	{0x7c,0x7e,0x06,0x1c,0x38,0x60,0x7e,0x7e},	//2
	{0x7c,0x7e,0x06,0x3c,0x3c,0x06,0x7e,0x7c},	//3
	{0x66,0x66,0x66,0x7e,0x3e,0x06,0x06,0x06},	//4
	{0x7e,0x7e,0x60,0x7c,0x7e,0x02,0x7e,0x7c},	//5
	{0x3e,0x7e,0x60,0x7c,0x7e,0x62,0x7e,0x3c},	//6
	{0x7e,0x7e,0x06,0x06,0x06,0x06,0x06,0x06},	//7
	{0x3c,0x7e,0x66,0x3c,0x3c,0x66,0x7e,0x3c},	//8
	{0x3c,0x7e,0x66,0x7e,0x3e,0x06,0x7e,0x7c},	//9
};

char rotateALPHA[][8]={ // clockWise for E
	{0x00,0xc0,0xa0,0xa0,0xa0,0xc0,0x00,0x00},
	{0x00,0x60,0x50,0x50,0x50,0x60,0x00,0x00},
	{0x00,0x30,0x28,0x28,0x28,0x30,0x00,0x00},
    {0x00,0x18,0x14,0x14,0x14,0x18,0x00,0x00},
	{0x00,0x0c,0x0a,0x0a,0x0a,0x0c,0x00,0x00},
	{0x00,0x06,0x05,0x05,0x05,0x06,0x00,0x00},
	{0x00,0x03,0x02,0x02,0x02,0x03,0x00,0x00},
	{0x00,0x01,0x01,0x01,0x01,0x01,0x00,0x00},
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}	
};

char rotate_A_ACW[][8]={
	{0x66, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x7e, 0x3c},
	{0x00, 0x7f, 0xff, 0xc8, 0xc8, 0xff, 0x7f, 0x00},
	{0x3c, 0x7e, 0x66, 0x66, 0x7e, 0x66, 0x66, 0x66},
	{0x00, 0xfe, 0xff, 0x13, 0x13, 0xff, 0xfe, 0x00}
};
char rotate_C_CW[][8]={
	{0x3e, 0x7e, 0x60, 0x60, 0x60, 0x60, 0x7e, 0x3e},
	{0x00, 0x7e, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0x00},
	{0x7c, 0x7e, 0x06, 0x06, 0x06, 0x06, 0x7e, 0x7c},
	{0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0x7e, 0x00}
};
char rotate_E_CW[][8]={
	{0x7e, 0x7e, 0x06, 0x3e, 0x3e, 0x06, 0x7e, 0x7e},
	{0x00, 0xc3, 0xdb, 0xdb, 0xdb, 0xff, 0xff, 0x00},
	{0x7e, 0x7e, 0x60, 0x7c, 0x7c, 0x60, 0x7e, 0x7e},
	{0x00, 0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xc3, 0x00}
};
char rotate_E_ACW[][8]={
	{0x7e, 0x7e, 0x06, 0x3e, 0x3e, 0x06, 0x7e, 0x7e},
	{0x00, 0xff, 0xff, 0xdb, 0xdb, 0xdb, 0xc3, 0x00},
	{0x7e, 0x7e, 0x60, 0x7c, 0x7c, 0x60, 0x7e, 0x7e},
	{0x00, 0xc3, 0xdb, 0xdb, 0xdb, 0xff, 0xff, 0x00}
};

void move_show(){
	int i = 0,j=0,k;
	while(1){
		for(k=0;k<150;k++){
			row_data = rows[i];
			col_data = ~rotateALPHA[j][i];
			_delay_ms(2);
			i+=1;
			if(i>7) i = 0;
		}
		j+=1;
		if(j>8){
			j=0;
			i=0;
		}
	}
}

void static_show(){
	int i = 0;
	while(1){
		row_data = rows[i];
		col_data = ~ALPHA[3][i];
		_delay_ms(2);
		i+=1;
		if(i>7) i = 0;
	}
}

void flush_show(){
	while(1){
		for(int i=0;i<500;i++){
			row_data = rows[i%8];
			col_data = ~ALPHA[3][i%8];
			_delay_ms(2);
		}
		
		for(int i=0;i<500;i++){
			row_data = 0x00;
			col_data = 0xff;
			_delay_ms(2);
		}
	}
}

int main(void)
{    
	row_ddr =0xff;
	col_ddr = 0xff;
	//static_show();
	//flush_show();
    move_show();
}



