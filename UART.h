/*
 *BIBLIOTECA PARA UTILIZAÇÃO DA UART
 * Modificada por: PROF. RODRIGO RECH & Augusto "ReDKiiL" Ferreira
 * DATA: 07/21
 *CONFIGURAÇÃO: BAUD RATE   -   9600
 *              NBITS       -   8
 *              STOP BITS   -   1 
 */

#include <avr/io.h> 	    //definições do componente especificado
#include <avr/interrupt.h>

void UART_config() 
{
	//Baud Rate de 9600bps para um cristal de 16MHz (Datasheet)
    UBRR0 = 103;    
   
    //Habilita a interrupção de recepção e os pinos TX e RX
    UCSR0B =  (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0) ;
	
	//Configura a UART com 8 bits de dados
    UCSR0C =  (1<<UCSZ01) | (1<<UCSZ00);  
}

void UART_sendChar(unsigned char ch)
{
   UDR0 = ch;
   //Aguarda o buffer ser desocupado
   while (! (UCSR0A & (1<<UDRE0)) );
}

void UART_sendString(unsigned char *s)
{
   unsigned int i=0;
   while (s[i] != '\0') 
   {
       UART_sendChar(s[i]);
	   i++;
   };
}

void  UART_sendHex(unsigned char ch)
{
    unsigned char i,temp;
     
    for (i=0; i<2; i++)
    {
        temp = (ch & 0xF0)>>4;
        if ( temp <= 9)
            UART_sendChar( '0' + temp);
        else
            UART_sendChar(  'A' + temp - 10);
        ch = ch << 4;    
     }   
}
void UART_receiveString(unsigned char *buffering)
{
	if((UCSR0A & (1<<RXC0)))
	{
		unsigned short idx = 0;
		unsigned char item = 'A';
		while(item != '\n'){
			while(!(UCSR0A & (1<<RXC0)));
			item = UDR0;
			buffering[idx] = item;
			idx++;
		}
		buffering[idx] = '\0';
	}else{
		buffering[0] = '\0';
	}
}