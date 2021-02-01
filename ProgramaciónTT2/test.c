/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 13/01/2021
Author  : 
Company : 
Comments: 


Chip type               : ATmega2560
Program type            : Application
AVR Core Clock frequency: 16.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 2048
*******************************************************/

#include <mega2560.h>
#include <stdlib.h>
#include <stdio.h>
#include <delay.h>

#include "DHT.h"
#include "keyboard.h"
#include "PLUMAS.h"

// Graphic Display functions
#include <glcd.h>

// Font used for displaying text
// on the graphic display
#include <font5x7.h>

//// interrupcion paro 
//int x = 0;
//// Pin change 16-23 interrupt service routine
//interrupt [PC_INT2] void pin_change_isr2(void)
//{
// x++;
// if(x==1){  
//    plantilla_paro();   
//    dht_init(); 
//    kb_init();  
//    Plumas_init();
//    }
// else x=0;
//} 

enum nivel_status nivel;
enum proceso_status proceso;


void main(void)
{

//Variables locales
// unsigned char contador = 0;
// unsigned char contador2 = 0;  
 char key;
 
// Variable used to store graphic display controller initialization data
    GLCDINIT_t init;
  

// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif

    // Specify the current font for displaying text
    init.font=font5x7;
    // No function is used for reading
    // image data from external memory
    init.readxmem=NULL;
    // No function is used for writing
    // image data to external memory
    init.writexmem=NULL;   
    glcd_init(&init);      
    
inicio:       
    dht_init(); 
    kb_init();  
    Plumas_init();
    plantilla_bienvenida();       
    Status_Sen();  // Corre funciones para estatuts de sensor  

  while (1)
  {   
      // Lee tecla de prueba 
      key = kb_read(); 
      delay_ms(10);
      
      if (key!=0)  {        
          // Para opcion A LAVADO
          if(key == 'A'){ 
                nivel = check_nivel(1);         // Revisa el nivel para LAVADO   
                
                // El nivel se encuentra debajo del indicado
                if(nivel == LLENAR){           
                    nivel = check_nivel(3);     // Procede a llenar la tina
                    goto Lavado;
                     }
                      
                // El NIVEL es superior al deseado. 
                if(nivel == NIVEL_ERROR) { 
                    plantilla_rev_tina(2);  

                    while(!PAUSA); // Espera a que el boton pausa/continuar sea pulsado.
                    goto Lavado;  
                    } 
                      
                // NIVEL CORRECTO 
                if(nivel == NIVEL_OK){
                        Lavado:
                        proceso = Lavado_manual();    // Comienza funcion de LAVADO
                        //Despues de ser pausado el proceso el programa continua  
                        //Ejecutar de nuevo?  
                        if(proceso == PROCESO_OK) {    
                            delay_us(5);
                            plantilla_again();   
                            while(1){
                                key = kb_read(); 
                                if(key == '1')
                                    goto Lavado;
                                if(key == '2')
                                    goto final;       
                            }
                        } 
                        
                        if(proceso == PARO_ON){ 
                        delay_us(5); goto STOP;}   

                }        
          } 
                   
          // Para Transicion  
          if(key == 'B'){
                check_nt:
                nivel = check_nivel(2);   // Revisa el nivel de agua para Transi
                
                // EL nivel est[a por debajo del nivel
                if(nivel == LLENAR){
                    nivel = check_nivel(4);
                    goto check_nt;
                }
                  
                if(nivel == NIVEL_ERROR_TI){ // El nivel en la tina inferior no es adecuado 
                    plantilla_rev_tina(1);   // Plantilla de error en tina onferior
                    while(!PAUSA); // Espera a que el boton pausa/continuar sea pulsado.
                    goto Transicion;  
                }
                
                if(nivel == NIVEL_OK){
                        Transicion:
                        proceso = Transicion_manual();    // Comienza funcion de LAVADO
                        //Despues de ser pausado el proceso el programa continua   
                        //Ejecutar de nuevo? 
                        if(proceso == PARO_ON){ 
                            delay_us(5);
                            goto STOP;
                            }
                        if(proceso == PROCESO_OK) {    
                            delay_us(5);
                            plantilla_again();   
                            while(1){
                            key = kb_read(); 
                            if(key == '1')
                                goto Transicion;
                            if(key == '2')
                                goto final;       
                            }
                          } 
                          
  
                }  
          }
                                    
          // Para Secado   
          if(key == 'C'){  
                Secado:
                proceso = Secado_manual();  // Comienza la funcion de secado
                //Despues de ser pausado el proceso el programa continua  
                //Ejecutar de nuevo?
                if(proceso == PROCESO_OK) {    
                    delay_us(5);
                    plantilla_again();   
                    while(1){
                    key = kb_read(); 
                    if(key == '1')
                        goto Secado;
                    if(key == '2')
                        goto final;       
                    }
                  } 
                  
                if(proceso == PARO_ON){ 
                   delay_us(5); goto STOP;}   
          }
                   
          // Para Desinfeccion. 
          if(key == 'D'){            
                UV:
                proceso = Desinfeccion_UVm();
                //Despues de ser pausado el proceso el programa continua
                //Ejecutar de nuevo?
                if(proceso == PROCESO_OK) {    
                    delay_us(5);
                    plantilla_again();   
                    while(1){
                    key = kb_read(); 
                    if(key == '1')
                        goto UV;
                    if(key == '2')
                        goto final;       
                    }
                  } 
                  
                if(proceso == PARO_ON){ 
                   delay_us(5); goto STOP;}           
        }  
        
        }
      plantilla_menu();
      continue;
      
      final: 
      plantilla_final(); 
  }
   
  STOP:
  plantilla_paro();
  goto inicio;
}


