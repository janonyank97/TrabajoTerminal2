#ifndef _DHT_
#define _DHT_

#include <mega2560.h>
#include <delay.h>
// Puertos de conexion
#define DDR_DHT DDRE
#define PORT_DHT PORTE
#define PIN_DHT	PINE.5
#define PIN_S 5



// Buffer para guardar datos de sensor
unsigned char bits[5];
// Codigos de retorno
enum dht_status {
    DHT_OK = 0, //!< Read operation on DHT11 succesfull
    DHT_TIMEOUT_ERROR, //!< Timeout error occured on DHT11 comunication
	DHT_CHKSUM_ERROR, //!< Checksum verification error
};



// Funciones
void dht_init()
{
 DDR_DHT |= (1<<PIN_S);  // Pin sensor como salida
 PORT_DHT |= (1<<PIN_S); // PIN OUTPUT HIGH   
}

void dht_start()
{
 DDR_DHT |= (1<<PIN_S);  // Pin sensor como salida
 PORT_DHT &= ~(1<<PIN_S);// PIN OUTPUT LOW   
 
 delay_ms(18);         // Mantenerlo LOW por 18us 
 
 PORT_DHT |= (1<<PIN_S); // PIN OUTPUT HIGH 
 DDR_DHT &= ~(1<<PIN_S); // Pin sensor como entrada
 delay_us(40) ;        // Esperar 40us para respuesta del sensor
}

static enum dht_status dht_read()
{
    unsigned char i,j = 0;
//    unsigned char aindex = 0;
//    unsigned char bcount = 7; 
    unsigned char result = 0;
    unsigned char contador;    // When Overflow ERROR
    
    // Limpiar bits de datos
    for (i=0;i<5;i++) bits[i] = 0;
    
    // ATMEGA envia senal de inicio
    dht_start(); 
    
     
/*		  18ms	   40us	    80us	80us
	¯¯¯¯|_______|¯¯¯¯¯¯¯¯|_______|¯¯¯¯¯¯¯|....datos....
	
	---------PUC---------|----------dht22-------------|      */
    
    //Esperar por respuesta de DTH max 80us LOW
     contador = 1;
     while(!PIN_DHT){
        if(!contador++)
        goto timeout;
     }   
     
     //Esperar por respuesta de DTH max 80us HIGH
     contador = 1;
     while(PIN_DHT){
        if(!contador++)
        goto timeout;
     } 
         
/*		    50ms   26-28us	  50us	     70us
	¯¯¯¯|__________|¯¯¯¯¯|__________|¯¯¯¯¯¯¯¯¯¯¯¯|....datos....
	        LOW     bit 0                bit 1
	----  DHT sensor DATA      */           
    
     //Comienza la recepcion de datos 40 bits    
     
	for (j=0; j<5; j++){
	result = 0;
		for (i=0; i<8; i++)
		{
			while (!PIN_DHT);
				delay_us(30);

			if (PIN_DHT)
				result |= (1<<(7-i));
					
			while(PIN_DHT);
		}
		bits[j] = result;
	} 
 
        return DHT_OK;
        
timeout:    
  return DHT_TIMEOUT_ERROR; 
    
}

enum dht_status dht_float22(float *temp, float *hum)
{
 // Funcion de lectura   
 
 enum dht_status s = dht_read();
 if(s == DHT_OK) { 
   // return DHT_TIMEOUT_ERROR;   // Devolvera TIMEOUT ERROR
 
 // Comprobacion CHECKSUM
 if(((bits[0]+bits[1]+bits[2]+bits[3])&0xFF) == bits[4]){
     // Guarda resultado
   
    unsigned short buf_Humd =  ((bits[0] << 8) | bits[1]);
    unsigned short buf_Temp = ( ((bits[2] << 8) | bits[3])& 0x7FFF);   
  
     
     if (bits[2] & 0x80) {
    *temp = -0.1*(float)buf_Temp;
    } else {
    *temp = 0.1*(float)buf_Temp;
	}
    *hum = 0.1*(float)(buf_Humd);
      
    return DHT_OK;  
   
 } 
 else return DHT_CHKSUM_ERROR;
     
    
}
}


#endif 