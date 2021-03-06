/* Main.c file generated by New Project wizard
 *
 * Created:   Fri Aug 31 2018
 * Processor: AT89C51
 * Compiler:  Keil for 8051
 */
#include <REG51.H> /* define 8051 registers */

void initINT();
unsigned char changeTrafficL1(void);
unsigned char changeTrafficL2(void);
unsigned char changeTrafficL3(void);
unsigned char changeTrafficL4(void);
void execTLightChange(unsigned char , unsigned char );

/*Declaring the functions prototypes for the delay functions*/
void delay_1ms(void);
void delay_100us(void);
void delay_50us(void);
void delay_1us(void);
/*-----------------------------------------------*/

void time_Alloc(void);
void init_Traffic(void);
#define HEX P2


/*-----------------------------------------------
;P1.0-P1.2 are connected as OUTPUT pins for Traffic Light 1(For Lane 1)
;P1.0 is connected to the RED Light of Traffic Light
;P1.1 is connected to the AMBER/YELLOW Light of Traffic Light
;P1.2 is connected to the GREEN Light of Traffic Light
-----------------------------------------------*/
sbit A0=P1^0;
sbit A1=P1^1;
sbit A2=P1^2;

/*-----------------------------------------------
;P1.3-P1.5 are connected as OUTPUT pins for Traffic Light 2(For Lane 2)
;P1.3 is connected to the RED Light of Traffic Light
;P1.4 is connected to the AMBER/YELLOW Light of Traffic Light
;P1.5 is connected to the GREEN Light of Traffic Light
-----------------------------------------------*/
sbit B0=P1^3;
sbit B1=P1^4;
sbit B2=P1^5;

/*-----------------------------------------------
;P1.6-P3.0 are connected as OUTPUT pins for Traffic Light 3(For Lane 3)
;P1.6 is connected to the RED Light of Traffic Light
;P1.7 is connected to the AMBER/YELLOW Light of Traffic Light
;P3.0 is connected to the GREEN Light of Traffic Light
-----------------------------------------------*/
sbit C0=P1^6;
sbit C1=P1^7;
sbit C2=P3^0;

/*-----------------------------------------------
;P3.1-P3.4 are connected as OUTPUT pins for Traffic Light 4(For Lane 4)
;P3.1 is connected to the RED Light of Traffic Light
;P3.3 is connected to the AMBER/YELLOW Light of Traffic Light
;P3.4 is connected to the GREEN Light of Traffic Light
-----------------------------------------------*/
sbit D0=P3^1;
sbit D1=P3^3;
sbit D2=P3^4;

/*-----------------------------------------------
;P3.5-P0.4 are connected as INPUT pins for the infrared Sensors Rx
;P3.5 is connected to the 1st infrared sensor reciever
;P3.6 is connected to the 2nd infrared sensor reciever
;P3.7 is connected to the 3red infrared sensor reciever
;P0.4 is connected to the 1st infrared sensor reciever
-----------------------------------------------*/
sbit ir1=P3^5;
sbit ir2=P3^6;
sbit ir3=P3^7;
sbit ir4=P0^4;

/*-----------------------------------------------
;P0.0-P0.5 are connected as OUTPUT pins for Pedestrains traffic control
;P0.0 is connected to Pd1
;P0.1 is connected to Pd2
;P0.3 is connected to Pd4
;P0.5 is connected to Pd3
-----------------------------------------------*/
sbit pd1=P0^0;
sbit pd2=P0^1;
sbit pd3=P0^5;
sbit pd4=P0^3;

//These are all the Possible states of the system
typedef enum {RED, RED_AND_AMBER, GREEN, AMBER} eLight_State;

/*=============================================================================
DECLARATION OF THE INTERRUPT HANDLER (THE INTERRUPT SERVICE ROUTINE) FUNCTION
THIS WILL BE EXCUTED IN CASE OF AN ERROR IN THE SYSTEM AND IT WILL CAUZE THE 
LIGHTS TO BLINK AMBER ON-OFF CONSECTIVELY FOR A NUMBER OF TIME
=============================================================================*/
unsigned char ex0_isr_counter = 0;//Keeping Records of the No. of Interrupts
unsigned char ex0_isr_exit=0;//initialise the interrupt count;
void ex0_isr (void) interrupt 0
{	
ex0_isr_exit=20;			//Re intiailise to value 20
ex0_isr_counter++;   // Increment the count(No of interrupts)
	
	HEX=0;							//Turn of the Display time(HEX)
	pd1=0;pd2=0;pd4=0;	//Turn of all the pedestrains Lights except 1 P3
	A0=0;								//Turn of all Traffiv Lights
	A2=0;
	B0=0;
	B2=0;
	C0=0;
	C2=0;
	D0=0;
	D2=0;
	
	while(ex0_isr_exit>0){	//Exit loop ex0_isr_exit is <0 & thus exit the interrupt
		/*This turns ON and OFF the amber parts of the traffic lights, while the other lights remain ON*/
		A1=0;
		B1=0;
		C1=0;
		D1=0;
		delay_100us();
		
		A1=1;
		B1=1;
		C1=1;
		D1=1;
		ex0_isr_exit--;
	}
}

/*=============================================================================
The main C function.  Program execution starts
here after stack initialization.
=============================================================================*/
void main(){
	ir1=ir2=ir3=ir4=0;			//Clear the Infrared Sensor Rx inputs bits
	pd1=0;pd2=0;pd4=0;pd3=0; //Clear the Pedestrain traffic output bits
	
	init_Traffic();
	
/*-----------------------------------------------
Wait forever/Loop Forever.
Note that an embedded program never exits (because
there is no operating system to return to).  It
must loop and execute forever.
-----------------------------------------------*/
	while(1){
		if(ir1==1){
				ir1=changeTrafficL1();
		}
		else if(ir2==1){
				ir2=changeTrafficL2();
		}
		else if(ir3==1){
				ir3=changeTrafficL3();
		}
		else if(ir4==1){
				ir4=changeTrafficL4();
		}
		
	};
}
/*-----------------------------------------------
Function thats responsible for initialising the 
System and configuring the variables
-----------------------------------------------*/
void init_Traffic(void){

/*-----------------------------------------------
Configure INT0 (external interrupt 0) to generate
an interrupt on the falling-edge of /INT0 (P3.2).
Enable the EX0 interrupt and then enable the
global interrupt flag.
-----------------------------------------------*/
IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
EX0 = 1;   // Enable EX0 Interrupt
EA = 1;    // Enable Global Interrupt Flag

	changeTrafficL1();
}
unsigned char changeTrafficL1(void){
	unsigned char x;
	
	delay_1ms();
	execTLightChange('A',1);
	
	delay_1ms();
	execTLightChange('A',2);
	
	pd1=0;pd2=0;pd4=1;
	time_Alloc();
	for(x=0;x<2;x++){
		if(ir1==1){
			time_Alloc();
		}else
			break;
	}
	return 0;
}
unsigned char changeTrafficL2(void){
	unsigned char x;
	
	delay_1ms();
	execTLightChange('C',1);
	
	delay_1ms();
	execTLightChange('C',2);
	
	pd1=1;pd2=1;pd4=0;
	time_Alloc();
	for(x=0;x<2;x++){
		if(ir2==1){
			time_Alloc();
		}else
			break;
	}
	return 0;
}
unsigned char changeTrafficL3(void){
	unsigned char x;
	
	delay_1ms();
	execTLightChange('B',1);
	
	delay_1ms();
	execTLightChange('B',2);
	
	pd1=0;pd2=0;pd4=1;
	time_Alloc();
	for(x=0;x<2;x++){
		if(ir3==1){
			time_Alloc();
		}else
			break;
	}
	return 0;
}

unsigned char changeTrafficL4(void){
	unsigned char x;
	
	delay_1ms();
	execTLightChange('D',1);
	
	delay_1ms();
	execTLightChange('D',2);
	
	pd1=1;pd2=1;pd4=0;
	time_Alloc();
	for(x=0;x<2;x++){
		if(ir4==1){
			time_Alloc();
		}else
			break;
	}
	return 0;
}
void execTLightChange(unsigned char TrafficLight, unsigned char Light_State ){
	
	
	if(TrafficLight=='A' || TrafficLight=='B'){
		switch(Light_State)
		{
			case RED:
				A0=1;
				A1=0;
				A2=0;
				B0=1;
				B1=0;
				B2=0;
			
				C0=0;
				C1=0;
				C2=1;
				D0=0;
				D1=0;
				D2=1;
				break;
			
			case RED_AND_AMBER:
				A0=1;
				A1=1;
				A2=0;
				B0=1;
				B1=1;
				B2=0;
			
				C0=0;
				C1=1;
				C2=0;
				D0=0;
				D1=1;
				D2=0;
			
			case GREEN:
				A0=0;
				A1=0;
				A2=1;
				B0=0;
				B1=0;
				B2=1;
			
				C0=1;
				C1=0;
				C2=0;
				D0=1;
				D1=0;
				D2=0;
			
				break;
			
			case AMBER:
				A0=0;
				A1=1;
				A2=0;
				B0=0;
				B1=1;
				B2=0;
				
				C0=0;
				C1=1;
				C2=0;
				D0=0;
				D1=1;
				D2=0;
				break;
			
			default:
					A0=0;
					A1=0;
					A2=0;
					C0=0;
					C1=0;
					C2=0;
				while(1){
					A1=1;
					C1=1;
					A1=0;
					C1=0;
			}
		}
	}
	else if(TrafficLight=='C' || TrafficLight=='D' ){
		switch(Light_State)
		{
			case RED:
				D0=1;
				D1=0;
				D2=0;
				C0=1;
				C1=0;
				C2=0;
			
				A0=0;
				A1=0;
				A2=1;
				B0=0;
				B1=0;
				B2=1;
				break;
			
			case RED_AND_AMBER:
				D0=1;
				D1=1;
				D2=0;
				C0=1;
				C1=1;
				C2=0;
			
				A0=0;
				A1=1;
				A2=0;
				B0=0;
				B1=1;
				B2=0;
			
			case GREEN:
				D0=0;
				D1=0;
				D2=1;
				C0=0;
				C1=0;
				C2=1;
			
				A0=1;
				A1=0;
				A2=0;
			
				B0=1;
				B1=0;
				B2=0;
				break;
			
			case AMBER:
				D0=0;
				D1=1;
				D2=0;
				C0=0;
				C1=1;
				C2=0;
				
				A0=0;
				A1=1;
				A2=0;
				B0=0;
				B1=1;
				B2=0;
				break;
			
			default:
					D0=0;
					D1=0;
					A2=0;
					C0=0;
					C1=0;
					C2=0;
				while(1){
					D1=1;
					C1=1;
					D1=0;
					C1=0;
			}
		}
	}
}
void time_Alloc(void){
	unsigned char x;
	for(x=0;x<49;x++){
		HEX=x;
		delay_100us();
	}
}

 /*=============================================================================
	CIRCUIT DELAYS, FOR GENERATING DELAYS IN THE CIRCUIT, EACH FUNCTION DELAYS,
	ARE 1mS, 100uS, 50uS & 1uS RESPECTIVELY. MAX DELAY ACHIAVABLE FOR 8051 OPERATING,
	AT TMOD 1 16BIT IS 65mS AND MIN DELAY IS 1us
=============================================================================*/
void delay_1ms(void){
	
/*Timer 0, mode 1(16-bit mode no Auto-reload) Selected*/
	TMOD&=0x0F; //CLEAR all Timer 0(T0) bits (T1 Unchanged)
	TMOD|=0x01;	//SET required bits (i.e. 1111, en mode 1)
	
	/* Values for 1mS Delay*/
	TH0=0xFC;	//the high byte
	TL0=0x66; //the low byte
	
	TF0=0;	//initialise by clearing timer 0 flag
	TR0=1;	//start the timer 0
	
	while(TF0==0);	//Loop until timer 0 overflows/rolls over(TF0==1)
	TR0=0;	//stop timer 0
	TF0=0;	//clear timer 0 flag
}
void delay_100us(void){
	
	/*Timer 0, mode 1(16-bit mode no Auto-reload) Selected*/
	TMOD&=0x0F; //CLEAR all Timer 0(T0) bits (T1 Unchanged)
	TMOD|=0x01;	//SET required bits (i.e. 1111, en mode 1)
	
	/* Values for 100uS Delay*/
	TH0=0xFF;	//the high byte
	TL0=0xA4; //the low byte
	
	TF0=0;	//initialise by clearing timer 0 flag
	TR0=1;	//start the timer 0
	
	while(TF0==0);	//Loop until timer 0 overflows/rolls over(TF0==1)
	TR0=0;	//stop timer 0
	TF0=0;	//clear timer 0 flag
}
void delay_50us(void){
	
	/*Timer 0, mode 1(16-bit mode no Auto-reload) Selected*/
	TMOD&=0x0F; //CLEAR all Timer 0(T0) bits (T1 Unchanged)
	TMOD|=0x01;	//SET required bits (i.e. 1111, en mode 1)
	
	/* Values for 50uS Delay*/
	TH0=0xFF;	//the high byte
	TL0=0xD2; //the low byte
	
	TF0=0;	//initialise by clearing timer 0 flag
	TR0=1;	//start the timer 0
	
	while(TF0==0);	//Loop until timer 0 overflows/rolls over(TF0==1)
	TR0=0;	//stop timer 0
	TF0=0;	//clear timer 0 flag
}
void delay_1us(void){
	
	/*Timer 0, mode 1(16-bit mode no Auto-reload) Selected*/
	TMOD&=0x0F; //CLEAR all Timer 0(T0) bits (T1 Unchanged)
	TMOD|=0x01;	//SET required bits (i.e. 1111, en mode 1)
	
	/* Values for 1uS Delay*/
	TH0=0xFF;	//the high byte
	TL0=0xFF; //the low byte
	
	TF0=0;	//initialise by clearing timer 0 flag
	TR0=1;	//start the timer 0
	
	while(TF0==0);	//Loop until timer 0 overflows/rolls over(TF0==1)
	TR0=0;	//stop timer 0
	TF0=0;	//clear timer 0 flag
}
 /*=============================================================================
=============================================================================*/