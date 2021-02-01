#ifndef _PLUMAS_INCLUDED_
#define _PLUMAS_INCLUDED_


#include <mega2560.h>
#include <stdio.h>
#include <stdlib.h>
#include <delay.h>
#include <glcd.h>

#include "DHT.h"
void dht22_task();
void plantilla_menu();
void plantilla_inicio();
void plantilla_pausa();
void plantilla_uv(int i);
void plantilla_paro();

unsigned char contador = 0;

// I/O PORTS para Aplicacion plumas
// Tiempos de proceso
#define RT 15 // Retencion de RELE HIGH 
enum dht_status s;

// Botones de tablero
#define DDR_BOT DDRF 
#define PORT_BOT PORTF

#define BOT1 PINF.1
#define BOT2 PINF.3
#define BOT3 PINF.5

#define PIN_BOT1 1 
#define PIN_BOT2 3
#define PIN_BOT3 5

#define INICIO BOT1
#define PAUSA  BOT2
#define PARO   BOT3

// Sensores de nivel
#define DDR_NIV1 DDRF
#define DDR_NIV2 DDRK  // Nivel 2 y 3 en PORTK
#define DDR_NIV3 DDRK

#define PORT_NIV1 PORTF
#define PORT_NIV2 PORTK
#define PORT_NIV3 PORTK

#define NIV1 PINF
#define NIV2 PINK
#define NIV3 PINK

#define PIN_NIV1 7 
#define PIN_NIV2 1
#define PIN_NIV3 3

enum nivel_status {

 NIVEL_OK = 0 ,     // El nivel es correcto
 NIVEL_ERROR,    // Nivel de la tina no coincide con el proceso
 NIVEL_ERROR_TI, // Nivel bajo en tina inferior
 LLENAR,         // Indica que el nivel esta DEBAJO DEL DESEADO

};

enum proceso_status{
 PROCESO_OK = 0,   //   PROCESO EJECUTADO CORRECTAMENTE
 PARO_ON = 1,       //   BOTON DE PARO PRESIONADO
 
};

// Limit Switch
#define DDR_LS DDRK
#define PORT_LS PORTK

#define LS PINK
#define PIN_LS 5

//Salidas a Tablero
#define M_PIN_1_DDR DDRC
#define M_PIN_2_DDR DDRC
#define M_PIN_3_DDR DDRC
#define M_PIN_4_DDR DDRD
#define M_PIN_5_DDR DDRG
#define M_PIN_6_DDR DDRL
#define M_PIN_7_DDR DDRL
#define M_PIN_8_DDR DDRL

#define M_PIN_1_PORT PORTC
#define M_PIN_2_PORT PORTC
#define M_PIN_3_PORT PORTC
#define M_PIN_4_PORT PORTD
#define M_PIN_5_PORT PORTG
#define M_PIN_6_PORT PORTL
#define M_PIN_7_PORT PORTL
#define M_PIN_8_PORT PORTL
  
#define M_PIN_1 5
#define M_PIN_2 3
#define M_PIN_3 1
#define M_PIN_4 7
#define M_PIN_5 1
#define M_PIN_6 7
#define M_PIN_7 5
#define M_PIN_8 3  
 
#define PORT_Sop   M_PIN_1_PORT
#define PORT_Bom   M_PIN_2_PORT
#define PORT_Asp   M_PIN_3_PORT
#define PORT_Stop  M_PIN_4_PORT
#define PORT_PSop  M_PIN_5_PORT
#define PORT_PBom  M_PIN_6_PORT
#define PORT_PAsp  M_PIN_7_PORT
#define PORT_Start M_PIN_8_PORT



#define PIN_Sop   M_PIN_1
#define PIN_Bom   M_PIN_2
#define PIN_Asp   M_PIN_3
#define PIN_Stop  M_PIN_4   //
#define PIN_PSop  M_PIN_5
#define PIN_PBom  M_PIN_6
#define PIN_PAsp  M_PIN_7  //
#define PIN_Start M_PIN_8



// Electrovalvula 
#define DDR_ELE DDRK
#define PORT_ELE PORTK
#define PIN_ELE 4

// Modulo LED UV
#define DDR_UV DDRK
#define PORT_UV PORTK
#define PIN_UV 6

//Funcion de inicializacion

void Plumas_init()
{    
//    // Interrupcion paro PCINT17 interrupt: On
//    PCMSK2 |= (1<<PCINT17);
//    PCICR |=(1<<PCIE2);
//    #asm("sei") 
    
    // Inicializa botones -- Pines como entrada - NO pullup
    DDR_BOT = (0<<PIN_BOT1)| (0<<PIN_BOT2)| (0<<PIN_BOT3);  
    PORT_BOT = (0<<PIN_BOT1)| (0<<PIN_BOT2)| (0<<PIN_BOT3);

    // Inicializa Sensores de Niv - Pines como entrada - NO pullup
    DDR_NIV1  &= ~(1<<PIN_NIV1);
    DDR_NIV2  &= ~(1<<PIN_NIV2);
    DDR_NIV3  &= ~(1<<PIN_NIV3);

    PORT_NIV1  &= ~(1<<PIN_NIV1);
    PORT_NIV2  &= ~(1<<PIN_NIV2);
    PORT_NIV3  &= ~(1<<PIN_NIV3);

    // Inicializa Limit Switch - Pines como entrada - NO pullup
    DDR_LS  &= ~(1<<PIN_LS);
    PORT_LS &= ~(1<<PIN_LS);

    // Inicializa puertos de modulo Reles - Pines como salida - LOW
    M_PIN_1_DDR |= (1<<M_PIN_1); M_PIN_1_PORT |= (1<<M_PIN_1);
    M_PIN_2_DDR |= (1<<M_PIN_2); M_PIN_2_PORT |= (1<<M_PIN_2);
    M_PIN_3_DDR |= (1<<M_PIN_3); M_PIN_3_PORT |= (1<<M_PIN_3);
    M_PIN_4_DDR |= (1<<M_PIN_4); M_PIN_4_PORT |= (1<<M_PIN_4);
    M_PIN_5_DDR |= (1<<M_PIN_5); M_PIN_5_PORT |= (1<<M_PIN_5);
    M_PIN_6_DDR |= (1<<M_PIN_6); M_PIN_6_PORT |= (1<<M_PIN_6);
    M_PIN_7_DDR |= (1<<M_PIN_7); M_PIN_7_PORT |= (1<<M_PIN_7);
    M_PIN_8_DDR |= (1<<M_PIN_8); M_PIN_8_PORT |= (1<<M_PIN_8);

    // Inicializa Electrovalcula - Pines como salida - LOW
    DDR_ELE |= (1<<PIN_ELE);
    PORT_ELE |= (1<<PIN_ELE);

    // Inicializa LED UV - Pines como salida - LOW
    DDR_UV |= (1<<PIN_UV);
    PORT_UV &= ~(1<<PIN_UV);

} 
  
void Status_Sen()
{    
    glcd_clear();
    glcd_rectrel(0,0,128,64); 
    glcd_outtextxy(3, 8,"DHT   :");
    glcd_outtextxy(3,19,"NIVEL :");
    glcd_outtextxy(3,46,"PUERTA:");

       // Estatus sensor de temperatura
       s = dht_read();   
       if ( s == DHT_OK )
            glcd_outtextxy(50,8,"CONECTADO");
         
       else glcd_outtextxy(50,8,"ERROR    ");
       
       if(NIV1 &(1<<PIN_NIV1))
            glcd_outtextxy(50,19,"NIV 1 - OK ");
       else glcd_outtextxy(50,19,"NIV 1 - OUT");  
       
       if(NIV2 &(1<<PIN_NIV2))
            glcd_outtextxy(50,27,"NIV 2 - OK ");
       else glcd_outtextxy(50,27,"NIV 2 - OUT");
       
       if(NIV3 &(1<<PIN_NIV3))
            glcd_outtextxy(50,35,"NIV 3 - OK ");
       else glcd_outtextxy(50,35,"NIV 3 - OUT");
       
       if(LS & (1<<PIN_LS))
            glcd_outtextxy(50,46,"CERRADA");
       else glcd_outtextxy(50,46,"ABIERTA");  
       
    delay_ms(5000);
    glcd_clear();
} 
    
static enum nivel_status check_nivel(int NIVEL) 
{
   glcd_clear();

        // Nivel = 1 --> Lavado
        // Nivel = 2 --> Transicion   
        // Nivel = 3 --> Llena tina hasta nivel de lavado
        // Nivel = 4 --> LLena tina hasta nivel de transicion

    switch (NIVEL){
        
    // Se solicita revisar nivel para LAVADO
    case 1:
        if(NIV2 &(1<<PIN_NIV2))   // Si el nivel se encuentra en nivel 2, este sobrepasa el nivel para lavado
             return NIVEL_ERROR;
        if(!(NIV1 &(1<<PIN_NIV1))) // Si nivel esta por debajo del nivel de lavado, pedir que se llene
             return LLENAR;
        else return NIVEL_OK; 
        break;

    // Se solicita revisar nivel paraTRANSICION
    case 2:
        
        if( (NIV2 &(1<<PIN_NIV2)) && (NIV3 &(1<<PIN_NIV3)) ) // Sensor nivel superior y tina inferior activados
             return NIVEL_OK;
        else{
        if(!(NIV2 &(1<<PIN_NIV2)))
             return LLENAR;
        if(!(NIV3 &(1<<PIN_NIV3))) // Tina inferior debajo del nivel deseado
             return NIVEL_ERROR_TI;
           }              
        break;    

    // Se requiere llenar la tina hasta el nivel 1 
    case 3:      
     glcd_rectrel(0,0,128,64);  
     glcd_outtextxy(25,25,"LLENANDO TINA"); 
     
        PORT_ELE &= ~(1<<PIN_ELE);      // Activa Electrovalvula
            delay_ms(100);
        while(!(NIV1 &(1<<PIN_NIV1)));  // Retiene mientras no se llegue al nivel deseado.
            delay_ms(100);
        PORT_ELE |= (1<<PIN_ELE);      // Desactiva Electrovalvula

        glcd_clear();   // limpia pantalla de menu  
        glcd_rectrel(0,0,128,64);  
        glcd_outtextxy(25,25,"TINA - LAVADO"); 
        delay_ms(1000);
          
        return NIVEL_OK;
        break;   
        
    // Se requiere llenar la tina hasta el nivel 2    
    case 4:
     glcd_rectrel(0,0,128,64);  
     glcd_outtextxy(25,25,"LLENANDO TINA"); 
                 
        PORT_ELE &= ~(1<<PIN_ELE);     //Activa Electrovalvula
            delay_ms(100);
        while(!(NIV2 &(1<<PIN_NIV2))); // Retiene mientras no se llegue al nivel deseado.
            delay_ms(100);
        PORT_ELE |= (1<<PIN_ELE);               
        
     glcd_clear();   // limpia pantalla de menu  
     glcd_rectrel(0,0,128,64);  
     glcd_outtextxy(13,25,"TINA - TRANSICION"); 
     delay_ms(1000);    


        return NIVEL_OK;
        break;     
    }   
}

    
    
static enum proceso_status Lavado_manual() // Activo en BAJO
{   
    plantilla_inicio();
    while(!INICIO); // Espera a que Bot inicio sea presionado   
     
     glcd_clear();   // limpia pantalla de menu  
     glcd_rectrel(0,0,128,64);   
     glcd_outtextxy(39,20,">LAVADO<");     
     glcd_outtextxy(33,30,"EN PROCESO");     
     
     PORT_Asp &= ~(1<<PIN_Asp);          // Activa Aspiradora      
     delay_ms(RT);                       // Retiene encendido
     PORT_Asp |= (1<<PIN_Asp);           // Desactiva Relevador 
    
    while( !PARO && !PAUSA);  // Espera a que el proceso sea detenido    
     PORT_PAsp &= ~(1<<PIN_PAsp);        //Activa Paro de Aspiradora 
     delay_ms(RT);                      //Retiene por 2 seg encendido
     PORT_PAsp |= (1<<PIN_PAsp);         //Desactiva Relevador 
    
    if(PARO){return PARO_ON;}
    else{ 
        plantilla_pausa(); 
        return PROCESO_OK;}     
    }         

static enum proceso_status Transicion_manual()
{  
    if(!(LS & (1<<PIN_LS)))
    plantilla_uv(1);
    while(!(LS & (1<<PIN_LS))); // Revisa si la puerta esta abierta
    
    plantilla_inicio();
    while(!INICIO); // Espera a que Bot inicio sea presionado   
    glcd_clear();   // limpia pantalla de menu  
    glcd_rectrel(0,0,128,64);   
    glcd_outtextxy(28,20,">TRANSICION<");     
    glcd_outtextxy(35,30,"EN PROCESO");     
    
     PORT_Bom &= ~(1<<PIN_Bom);          //Activa Bomba    
     delay_ms(RT);                      //Retiene por 2 seg encendido
     PORT_Bom |= (1<<PIN_Bom);         //Desactiva Relevador 
     
    while(!PARO && (LS & (1<<PIN_LS)) && !PAUSA);  // Espera a que el proceso sea detenido    
     PORT_PBom &= ~(1<<PIN_PBom);        //Activa Paro de Bomba   
     delay_ms(RT);                      //Retiene por 2 seg encendido
     PORT_PBom |= (1<<PIN_PBom);         //Desactiva Relevador ;    

    if(PARO){
        return PARO_ON;
        }
    else{ 
        plantilla_pausa(); 
        return PROCESO_OK;
    }
 }      
    
static enum proceso_status Secado_manual()
{  
    if(!(LS & (1<<PIN_LS)))
    plantilla_uv(1);
    
    while(!(LS & (1<<PIN_LS))); // Revisa si la puerta esta abierta
    plantilla_inicio();
    
    while(!INICIO); // Espera a que Bot inicio sea presionado   
    glcd_clear();   // limpia pantalla de menu  
    glcd_rectrel(0,0,128,64);   
    glcd_outtextxy(42,11,">SECADO<");     
    glcd_outtextxy(36,21,"EN PROCESO"); 
    glcd_barrel(0,38,128,25);
    glcd_setcolor(0);
    glcd_setbkcolor(1);      
     
     PORT_Sop &= ~(1<<PIN_Sop);          //Activa Soplador     
     delay_ms(RT);                      //Retiene por 2 seg encendido
     PORT_Sop |= (1<<PIN_Sop);         //Desactiva Relevador 
     
        while(!PARO && (LS & (1<<PIN_LS)) && !PAUSA){ 
            contador++;
            if(contador >= 20){   
                contador = 0; 
                dht22_task();
            }
            else delay_ms(100);    
        }
          
    // Espera a que el proceso sea detenido    
     PORT_PSop &= ~(1<<PIN_PSop);        //Activa Paro de Soplador 
     delay_ms(RT);                      //Retiene por 2 seg encendido
     PORT_PSop |= (1<<PIN_PSop);        //Desactiva Relevador ;    
     glcd_setcolor(1);
     glcd_setbkcolor(0); 
     if(PARO){ return PARO_ON;}
      else{
        glcd_setcolor(1);
        glcd_setbkcolor(0);
        plantilla_pausa();
        return PROCESO_OK;
        }
    }

static enum proceso_status Desinfeccion_UVm()
{
    if(!(LS & (1<<PIN_LS)))
    plantilla_uv(1);
    
    while(!(LS & (1<<PIN_LS))); // Revisa si la puerta esta abierta
    // Si puerta cerrada, continua el proceso;
    plantilla_inicio();
    while(!INICIO); // Espera a que Bot inicio sea presionado   
    
    plantilla_uv(2);
    
    while(!PARO && (LS & (1<<PIN_LS)) && !PAUSA){     // solo mientras la puerta permanezca cerrada
    PORT_UV |= (1<<PIN_UV);
    } 
    PORT_UV &= ~(1<<PIN_UV);
    if(PARO){ return PARO_ON;}
    else{ 
        plantilla_pausa(); 
        return PROCESO_OK;
    }
}

// Modificar el lugar donde se mostrara en la LCD 
void dht22_task()
{
    char buffer[10];
    char buf1[10];
    
	float temperatura;
	float humedad;  

    enum dht_status result;  
    
 

        result = dht_float22(&temperatura,&humedad);    
        if(result == DHT_OK){  
            ftoa(temperatura,2,buf1);
            sprintf(buffer,"    TEMP: %s C  ",buf1);
            glcd_outtextxy(3,42,buffer); 
            
            ftoa(humedad,2,buf1);    
            sprintf(buffer,"    Humd: %s %%  ",buf1);
            glcd_outtextxy(3,52,buffer); 
        } 
//        else if (result == DHT_TIMEOUT_ERROR ) {
//            glcd_clear();   
//            glcd_outtextxy(0,8,"DHT TIME ERROR "); 
 //       } 
        else {     
            glcd_outtextxy(3,42," -- DHT -- ERROR --");
            glcd_outtextxy(3,52,"                   ");     
             }
        
       
}


// Plantillas LCD 

void plantilla_bienvenida(){
    
    glcd_rectrel(0,0,128,64);
    glcd_outtextxy(3,18," PROTOTIPO DE LAVADO");
    glcd_outtextxy(3,28,"  Y  DESINFECCION   ");
    glcd_outtextxy(3,38,"      DE PLUMAS     ");    
    
    delay_ms(2000);
    glcd_clear();
    glcd_rectrel(0,0,128,64);
    glcd_outtextxy(4,16,">PREPARANDO SISTEMA<") ;   
    glcd_outtextxy(4,32,"  -- BIENVENIDO --  ") ; 
    delay_ms(2500);  
    glcd_clear();
}

void plantilla_menu(){
     // Menu de pruebas 
      glcd_rectrel(0,0,128,64);
      glcd_outtextxy(3, 3,"    MENU PRICIPAL");
      glcd_outtextxy(3,15,"A.Lavado");
      glcd_outtextxy(3,24,"B.Transicion") ; 
      glcd_outtextxy(3,33,"C.Secado") ;
      glcd_outtextxy(3,42,"D.Desinfecion UV") ;
}

void plantilla_pausa(){
     glcd_clear();   // limpia pantalla de menu  
     glcd_rectrel(0,0,128,64);   
     glcd_outtextxy(15,25,"PROCESO en PAUSA"); 
     delay_ms(1000);
}

void plantilla_inicio(){
    glcd_clear();   // limpia pantalla de menu  
    glcd_rectrel(0,0,128,64);  
    glcd_outtextxy(23,23,"PULSE >INICIO<");
    glcd_outtextxy(26,33,"PARA COMENZAR"); 
}

void plantilla_again(){
 glcd_clear();
 glcd_rectrel(0,0,128,64);  
 glcd_outtextxy(10,23,"Ejecutar de nuevo?");
 glcd_outtextxy(30,33,"1.SI   2.NO");
 }   
 
void plantilla_uv(int i)
{
     glcd_clear();
     glcd_rectrel(0,0,128,64);
     //Puerta abierta  
     if(i == 1){  
     glcd_outtextxy(20,10,"CERRAR   PUERTA");
     }
     if (i == 2){
     glcd_outtextxy(20,10,"MANTENER PUERTA");
     glcd_outtextxy(40,25,"CERRADA");    
     //glcd_floodfill(0,39,1);
     glcd_barrel(0,38,128,25);
     glcd_setcolor(0);
     glcd_setbkcolor(1);
     glcd_outtextxy(10,47,"---- UV -- ON ----"); 
     glcd_setcolor(1);
     glcd_setbkcolor(0);
     }
}

void plantilla_rev_tina(int i){
       glcd_clear(); 
       glcd_rectrel(0,0,128,64); 
       glcd_barrel(0,0,128,25);
       glcd_setcolor(0);
       glcd_setbkcolor(1);
        if(i == 1){ // el  nivel en la tina inferior es menor
            glcd_outtextxy(15,4,"REVISAR NIVEL EN");  
            glcd_outtextxy(20,14,"TINA INFERIOR");
        }
        if(i == 2){   // Nivel en 2 cuando deberia ser para lavado
            glcd_outtextxy(4,8," EL NIVEL SOBREPASA!");
        }
       glcd_setcolor(1);
       glcd_setbkcolor(0);
       glcd_outtextxy(13,30,"PULSE >CONTINUAR<");
       glcd_outtextxy(7,40,"CUANDO EL NIVEL SEA");
       glcd_outtextxy(32,50,"EL ADECUADO");
}

void plantilla_paro(){
    glcd_clear();
    glcd_rectrel(0,0,128,64); 
    glcd_barrel(0,14,128,36);
    glcd_setcolor(0);
    glcd_setbkcolor(1);  
    glcd_outtextxy(18,20,"PARO DE SISTEMA");   
    glcd_outtextxy(13,37,"PULSE >CONTINUAR<"); 
    while(!PAUSA);
    glcd_setcolor(1);
    glcd_setbkcolor(0);  
    delay_ms(500);
    glcd_clear();
}
void plantilla_final(){
      glcd_clear(); 
      glcd_rectrel(0,0,128,64); 
      glcd_barrel(0,14,128,36);
      glcd_setcolor(0);
      glcd_setbkcolor(1);
      glcd_outtextxy(13,28,"PROCESO TERMINADO");
      delay_ms(1000);
      glcd_setcolor(1);
      glcd_setbkcolor(0);
      glcd_clear(); 

}

#endif 