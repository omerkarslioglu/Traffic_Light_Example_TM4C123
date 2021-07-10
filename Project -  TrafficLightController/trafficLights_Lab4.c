#include <stdlib.h>
#include <stdint.h>

#include <tm4c123gh6pm.h>

/*
OUTPUTS
-------
Definition : PE1 , PE2 and PE3 are connected to south leds .
PE1 is connected to red .
PE2 is connected to yellow .
PE3 is connected to green .

PD1 , PD2 and PD3 are connected to west leds .
PD1 is connected to red .
PD2 is connected to yellow .
PD3 is connected to green .

PE4 and PE5 are connected to walking leds .
PE4 is connected to red .
PE5 is connected to green .

INPUTS
------
PA2 , PA3 and PA4 are connected to switches .
Circular reading value is 1 . I used pull up ressistor .
If switches are pressed , the data is read as 0 .
All of them are active low .
*/

#define southLeds              (*((volatile unsigned long *)0x40024338))  // pin specified address (South LEDS)         PE123  
#define westLeds               (*((volatile unsigned long *)0x40007338))  // pin specified address (West LEDS)          PD123
#define walkingLeds          	 (*((volatile unsigned long *)0x400243C0))  // pin specified address (Walking LEDS) PE45

#define inputs                 (*((volatile unsigned long *)0x40004370))  // PA2-PA3


void init_PortA(void);
void init_PortD(void);
void init_PortE(void);

void init_Clock(void);

void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait_N_ms(unsigned long N);


enum lampColors{
	tlRed 			=		0x02,
	tlYellow		=		0x04,
	tlGreen			=		0x08,
	wlRed				=		0x10,
	wlGreen     =   0x20
};

enum states {
  goS     		= 	0,
  waitS   		= 	1,
  goW     		= 	2,
	waitW   		= 	3,
	walk    		= 	4,
	walkFast		= 	5,
	allRed  		= 	6
}; 


struct State
{
	unsigned long walkingTrafficLights;
	unsigned long southTrafficLights;
	unsigned long westTrafficLights;
	unsigned long time;
	unsigned long next[8];
};typedef const struct State SType;


SType FSM[7] =
{
{wlRed	,tlGreen	,tlRed		,3000	,{waitS	,goS	,waitS	,waitS	,waitS	,waitS	,waitS	,waitS}},
{wlRed	,tlYellow	,tlRed		,500 	,{goW	,goS	,goW	,goW	,allRed	,allRed	,allRed	,allRed}},
{wlRed  ,tlRed		,tlGreen	,3000	,{waitW	,waitW	,goW	,waitW	,waitW	,waitW	,waitW	,waitW}},
{wlRed	,tlRed    ,tlYellow	,500 	,{allRed	,goS	,goW	,goS	,allRed	,allRed	,allRed	,goS}},
{wlGreen,tlRed		,tlRed		,3000	,{walkFast	,walkFast	,walkFast	,walkFast	,walk	,walkFast	,walkFast	,walkFast}},
{wlGreen,tlRed		,tlRed		,200	,{goS	,goS	,goW	,goS	,walk	,goS	,goW	,goW}}, // walkFast State
{wlRed	,tlRed		,tlRed		,100 	,{walk	,goS	,goW	,goW	,walk	,walk	,walk	,walk}} // allRed state
};


unsigned long state ;
unsigned int sensorvalue ;
int walkFastFlagState;
int i;    


int main(void)
{
init_Clock();
init_PortA();
init_PortD();
init_PortE();
	
SysTick_Init();
	
state = goS;
walkFastFlagState=0;
	
while(1)
{
	southLeds		=	FSM[state].southTrafficLights;
	westLeds		= FSM[state].westTrafficLights; 
	if(state==walkFast){
		for(i=0;i<=5;i++){
			walkingLeds	= (walkFastFlagState) ?	0x00 : 0x10 ; // green - red (toggle)
			walkFastFlagState = !(walkFastFlagState);
			SysTick_Wait_N_ms(FSM[state].time);
		}
	}
	else{
		walkingLeds	=	FSM[state].walkingTrafficLights;
		SysTick_Wait_N_ms(FSM[state].time);
	}
	//........ inputu oku
	sensorvalue =(inputs>>2) ^ 0x07 ;
	state=FSM[state].next[sensorvalue];
} 

return 0;
}
/*
I : input
O : output
*/
void init_PortA(void)
{
	GPIO_PORTA_LOCK_R  = 0x4C4F434B;   		// unlock GPIO Port A
  GPIO_PORTA_CR_R    = 0x3F;         		// allow changes to PA4-PA2
													
	GPIO_PORTA_DIR_R   = 0x00; 				 		//00000 <=> I IIII
	GPIO_PORTA_DEN_R   = 0x3F; 				 		//01110  PA2 , PA3 and PA4 are ACTIVETED
}
void init_PortD(void)
{
	GPIO_PORTD_LOCK_R  = 0x4C4F434B;   		// unlock GPIO Port D
  GPIO_PORTD_CR_R    = 0x0E;         		// allow changes to PD3-PD1
													
	GPIO_PORTD_DIR_R   = 0x0E; 				 		//01110 <=> I OOOI
	GPIO_PORTD_DEN_R   = 0x0E; 				 		//01110  PD1 , PD2 , PD3 ACTIVE
				
	westLeds =0x00; 											 		//specified address
}
void init_PortE(void)
{
	GPIO_PORTE_LOCK_R  = 0x4C4F434B;    	// unlock GPIO Port E
  GPIO_PORTE_CR_R    = 0x3F;          	// allow changes to PE5-PE0
													
	GPIO_PORTE_DIR_R   = 0x3F; 						//1 1111 <=> I OOOI
	GPIO_PORTE_DEN_R   = 0x3F; 						//0011111  PE1 , PE2 , PE3 , PE4 , PE5 ACTIVE
		
	southLeds=0x00; 													//specified address
	walkingLeds=0x00;
}
void init_Clock(void)
{
	SYSCTL_RCGC2_R |= 0x19; 							// to initialize clock to Port A , Port D and Port E
	while(!( SYSCTL_PRGPIO_R& 0x19)){ } 	//waiting to enable clk
}

void SysTick_Init(void)
{
	NVIC_ST_CTRL_R=0; // disable the st_ctrl register
	NVIC_ST_RELOAD_R =0x00FFFFFF; // maximum reload value
	NVIC_ST_CURRENT_R=0; // clear the current register
	NVIC_ST_CTRL_R=0x00000005; // enable the st_ctrl register
}

//(supposing)Clock source = 80 mhz (12.5 ns)
//SysTick Interval = 10 ms
//(reload+1)*CLK_PERIOD=10ms
//reload = (10 ms x 80 MHz)-1
//reload = 8x10^6-1 = 799999

void SysTick_Wait(unsigned long delay)
{
	NVIC_ST_RELOAD_R=delay;
	NVIC_ST_CURRENT_R=0; // clear the current register
	while((NVIC_ST_CTRL_R & 0x10000)==0){} // waiting until that flag is set
}

//wait for N*1ms
void SysTick_Wait_N_ms(unsigned long N)
{
	unsigned long i;
	for(i=0;i<N;i++){
		SysTick_Wait(80000-1); // 1 ms / 12.5ns = 80000 ==> i obtained 1 ms	
	}
}
// Omer Karslioglu - 17050211061