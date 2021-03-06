
#ifndef _KEYBOARD_INCLUDED_
#define _KEYBOARD_INCLUDED_

#include <delay.h>
#include <mega2560.h>


#define KB_DELAY 5
#define KB_DELAY2 20

//PORT Configuration

#define KB_C1_PORT PORTH
#define KB_C2_PORT PORTD
#define KB_C3_PORT PORTD
#define KB_C4_PORT PORTA
#define KB_F1_PORT PORTA
#define KB_F2_PORT PORTA
#define KB_F3_PORT PORTA
#define KB_F4_PORT PORTC

#define KB_C1_DDR  DDRH
#define KB_C2_DDR  DDRD
#define KB_C3_DDR  DDRD
#define KB_C4_DDR  DDRA
#define KB_F1_DDR  DDRA
#define KB_F2_DDR  DDRA
#define KB_F3_DDR  DDRA
#define KB_F4_DDR  DDRC

#define KB_F1_PINPORT PINA.3
#define KB_F2_PINPORT PINA.5
#define KB_F3_PINPORT PINA.7
#define KB_F4_PINPORT PINC.6

#define KB_C1_PIN 0   //
#define KB_C2_PIN 2   //
#define KB_C3_PIN 0   //
#define KB_C4_PIN 1   //
#define KB_F1_PIN 3   //  PA1
#define KB_F2_PIN 5   //  PD0
#define KB_F3_PIN 7   //  PD2
#define KB_F4_PIN 6   //  PH0


// Function Prototypes
void kb_init(void);
char kb_read(void);
void activate_column(unsigned char columna);
void reset_columns(void);

//

void kb_init(){
//Columna PINS son SALIDA
	KB_C1_DDR |= (1<<KB_C1_PIN);
	KB_C2_DDR |= (1<<KB_C2_PIN);
	KB_C3_DDR |= (1<<KB_C3_PIN);
	KB_C4_DDR |= (1<<KB_C4_PIN);
//FILA PINS son ENTRADA
	KB_F1_DDR &= ~(1<<KB_F1_PIN);
	KB_F2_DDR &= ~(1<<KB_F2_PIN);
	KB_F3_DDR &= ~(1<<KB_F3_PIN);
	KB_F4_DDR &= ~(1<<KB_F4_PIN);
//Habilitar pull ups en FILA pins
	KB_F1_PORT |= (1<<KB_F1_PIN);
	KB_F2_PORT |= (1<<KB_F2_PIN);
	KB_F3_PORT |= (1<<KB_F3_PIN);
	KB_F4_PORT |= (1<<KB_F4_PIN);  
// SET HIGH en Columnas
	KB_C1_PORT |= (1<<KB_C1_PIN);
	KB_C2_PORT |= (1<<KB_C2_PIN);
	KB_C3_PORT |= (1<<KB_C3_PIN);
	KB_C4_PORT |= (1<<KB_C4_PIN);
	
}

char kb_read(void){
//reset_columns();
	char tecla = 0;	
//Revisar primer columna
activate_column(1);       
    if(!KB_F1_PINPORT){
       delay_ms(KB_DELAY);
        tecla='D';      
       while(!KB_F1_PINPORT);  
       return tecla; 
       }
        
    if(!KB_F2_PINPORT){
       delay_ms(KB_DELAY);
        tecla='#';  
       while(!KB_F2_PINPORT);  
       return tecla; 
       
       } 
       
    if(!KB_F3_PINPORT){
       delay_ms(KB_DELAY);
        tecla='0';      
       while(!KB_F3_PINPORT);
       return tecla; 
       } 
       
    if(!KB_F4_PINPORT){             
       delay_ms(KB_DELAY);
        tecla='*';        
       while(!KB_F4_PINPORT);
       return tecla; 
       }

//Revisa segunda columna       
activate_column(2); 
    if(!KB_F1_PINPORT){
       delay_ms(KB_DELAY);
        tecla='C';         
       while(!KB_F1_PINPORT);  
       return tecla; 
       }
        
    if(!KB_F2_PINPORT){
       delay_ms(KB_DELAY);
        tecla='9';  
       while(!KB_F2_PINPORT);  
       return tecla; 
       
       } 
       
    if(!KB_F3_PINPORT){
       delay_ms(KB_DELAY);
        tecla='8';      
       while(!KB_F3_PINPORT);
       return tecla; 
       } 
       
    if(!KB_F4_PINPORT){
       delay_ms(KB_DELAY);
        tecla='7';        
       while(!KB_F4_PINPORT); 
       return tecla; 
       } 
       
// Revisa tercer columna       
activate_column(3);     
    if(!KB_F1_PINPORT){
       delay_ms(KB_DELAY);
        tecla='B';  
       while(!KB_F1_PINPORT);  
       return tecla; 
       }
        
    if(!KB_F2_PINPORT){
       delay_ms(KB_DELAY);
        tecla='6';  
       while(!KB_F2_PINPORT);  
       return tecla; 
       
       } 
       
    if(!KB_F3_PINPORT){
       delay_ms(KB_DELAY);
        tecla='5';      
       while(!KB_F3_PINPORT);
       return tecla; 
       } 
       
    if(!KB_F4_PINPORT){
       delay_ms(KB_DELAY);
        tecla='4';        
       while(!KB_F4_PINPORT); 
       return tecla; 
       }
//Revisa cuarta columna     
    activate_column(4);
    if(!KB_F1_PINPORT){
       delay_ms(KB_DELAY);
        tecla='A';      
       while(!KB_F1_PINPORT);  
       return tecla; 
       }
        
    if(!KB_F2_PINPORT){
       delay_ms(KB_DELAY);
        tecla='3';  
       while(!KB_F2_PINPORT);  
       return tecla; 
       
       } 
       
    if(!KB_F3_PINPORT){
       delay_ms(KB_DELAY);
        tecla='2';      
       while(!KB_F3_PINPORT);
       return tecla; 
       } 
       
    if(!KB_F4_PINPORT){
       delay_ms(KB_DELAY);
        tecla='1';        
       while(!KB_F4_PINPORT); 
       return tecla; 
       }   
    reset_columns();
    delay_us(10);
}

void activate_column(unsigned char columna){
	if(columna == 1)
	{
	    KB_C1_PORT &= ~(1<< KB_C1_PIN);
		KB_C2_PORT |= (1<< KB_C2_PIN);
		KB_C3_PORT |= (1<< KB_C3_PIN);
		KB_C4_PORT |= (1<< KB_C4_PIN);	
		delay_ms(KB_DELAY2);
	}
    if(columna == 2)
	{
		KB_C1_PORT |= (1<< KB_C1_PIN);
		KB_C2_PORT &= ~(1<< KB_C2_PIN);
		KB_C3_PORT |= (1<< KB_C3_PIN);
	    KB_C4_PORT |= (1<< KB_C4_PIN);
		delay_ms(KB_DELAY2);	
	}
	if(columna == 3)
	{
		KB_C1_PORT |= (1<< KB_C1_PIN);
		KB_C2_PORT |= (1<< KB_C2_PIN);
		KB_C3_PORT &= ~(1<< KB_C3_PIN);  
        KB_C4_PORT |= (1<< KB_C4_PIN);
        delay_ms(KB_DELAY2);	
	}
	if(columna == 4)
	{
		KB_C1_PORT |= (1<< KB_C1_PIN);
		KB_C2_PORT |= (1<< KB_C2_PIN);
		KB_C3_PORT |= (1<< KB_C3_PIN);
        KB_C4_PORT &= ~(1<< KB_C4_PIN);
		delay_ms(KB_DELAY2);
	}
}

void reset_columns(){
	KB_C1_PORT |= (1<< KB_C1_PIN);
	KB_C2_PORT |= (1<< KB_C2_PIN);	
	KB_C3_PORT |= (1<< KB_C3_PIN);
	KB_C4_PORT |= (1<< KB_C4_PIN);		
}




#endif 