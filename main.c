/*
 * File:   mainXC16.c
 * Author: miwyborn
 *
 * Created on August 15, 2018, 4:47 PM
 */

#define FOSC 32000000L
#define FCY 16000000UL
#define delay_ms __delay_ms 
#include "xc.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/interrupt_manager.h"
#include "mcc_generated_files/traps.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/mcc.h"
#include "newxc16_header.h"
#include <libpic30.h>
#include "clock.h"
#include <math.h>
#include <p24FJ1024GB610.h>

//#include <StdTypes.h>

bool IFlag = 0;

int main(void)
{
    uint16_t n = 0;
  uint8_t w, h, r,i; 
  uint16_t c = 0x2222;  
   
  uint8_t  x;
  
  uint8_t  sub_x;
  uint8_t  y;
  uint8_t  sub_y;
//  int t;
    SYSTEM_Initialize();
    TMR1_Stop;
    IFlag = 0;
    chipinit();
    
    while(1)
    {
        
        while(0)
        {
        writeByte( 0x0000,  0xfa);
       // UART1_Read();
        Nop();
        
        n = readByte(0x0000);
      //  UART1_Read();
        } 
        drawImage();
        
        delay_ms(5000);
        
        
        
        /*
        
        clearScreen(0x0000);
         for(n = 1 ;n <96 ; n += 6)
           {
               drawCircle( (_GRAMWIDTH +1) /2, (_GRAMHEIGH + 1) /2, n, 0xffff);
               drawCircle( (_GRAMWIDTH +1) /2, (_GRAMHEIGH+1) /2, n+1, 0xffff);
              // drawCircle(( _GRAMWIDTH - 1) /2, (_GRAMHEIGH- 1) /2, n+2, 0xffff);
           }
   
         delay_ms(500);
         
     
         clearScreen(0x0000);
         
         for (n = 0; n < 10; n++)
         {
            w = rand() % 0x7f;
            h = rand() % 0xff;
            r = rand() % 0x7f / 4;
            c = rand() % 0xffff;
           // i=1;
            for(x = 0 ; x < 21 ; x += 4)
             {
             //   i *= 2;
                  drawFillCircle(w, h + x, r + x / 2, c );
                  delay_ms(50);
             }
             
         }
         
        clearScreen(0);
         
/*      for(n=0;n<15;n++)
        {
            c = rand() & 0xffff;
            x  = rand() & 0x7f;
            y = rand() & 0xff;
            
            sub_x  = rand() & 0x7f /2;
            sub_y = rand() & 0xff /2 ;
            
            drawFillRect(x,y,sub_x,sub_y,c);
        }
            
            
            
        
        //drawFillRect (15, 60, 90, 30, 0x7bef); //top left corner x, top left corner y, distance x, distance y, color
        //drawFillRect (20, 55, 90, 30, 0xf800);
        
         delay_ms(1000);
         
     /*    c = 0xf800;
         
         for (n=0;n<32;n++)
         {
             clearScreen(c);
             c -= 0x0800;
         }
         delay_ms(100);
          
         c = 0x07e0;
         for (n=0;n<32;n++)
         {
             clearScreen(c);
             c -= 0x0040;
         }
          delay_ms(100);
         
         c = 0x001f;
         for (n=0;n<32;n++)
         {
             clearScreen(c);
             c -= 0x0001;
         }
          delay_ms(100);
          
         
         clearScreen(0x0);
     
	 for(i=0;i<7;i++)
	 {
			c = rand() % 0xfff;
           for(n = 0; n < _GRAMWIDTH ; n += 8)
              {
				  
                   drawLine( n, 0, _GRAMWIDTH - n , _GRAMHEIGH , c);
                   drawLine( n+1, 0, _GRAMWIDTH - n -1 , _GRAMHEIGH , c);
                   
               }
    
           for(n = _GRAMHEIGH; n >0 ; n -= 8)
              {
                   drawLine( 0, n, _GRAMWIDTH, _GRAMHEIGH - n, c);
                   drawLine( 0, n - 1, _GRAMWIDTH, _GRAMHEIGH - n - 1, c);
				}
	 }        //  delay_ms(1000);
      
 
            
            /********************
            /* DRAW RED DECENDING
            /********************
            c = 0xf800;          
            i = 0;
            for(y=0;y<160;y++)
              {
                  HLine(0, y, _GRAMWIDTH, c);
                  if( i++ == 4 )
                {
                    c -= 0x800;
                    i = 0;
                }
            }
            delay_ms(1000);
            
             /********************* /
            /* DRAW GREEN DECENDING
            /********************* /
            c = 0x07E0;          
            i = 0;
            for(y=0;y<160;y++)
              {
                  HLine(0, y, _GRAMWIDTH, c);
                if( i++ == 2 )
                {
                    c -= 0x0020;
                    i = 0;
                }
            }
            delay_ms(1000);
            
            /********************* /
            /* DRAW BLUE DECENDING* /
            /********************* /
            c = 0x001F;          
            i = 0;
            for(y=0;y<160;y++)
              {
                  HLine(0, y, _GRAMWIDTH, c);
                if( i++ == 4 )
                {
                    c -= 0x0001;
                    i = 0;
                }
            }
            delay_ms(1000);
            
            

            
            for(n = 0; n<10;n += 4)
            {
                HLine(n, n, n+5, 0x1f );
            }
            
                        
                        

     //RGBtoU16();
     
    
     /////////  draw checker board
            for(x=0;x<=20;x++)
            {
                for(y=0;y<=20;y++)
                 {
                     for(sub_x=0;sub_x<=7;sub_x++)
                      for(sub_y=0;sub_y<=7;sub_y++)
                     if(((x&0x01)&&!(y&0x01)) || (!(x&0x01)&&(y&0x01)))
                    drawPixel((x<<3)+sub_x,(y<<3)+sub_y, 0x0000);
                  else
                    drawPixel((x<<3)+sub_x,(y<<3)+sub_y, 0xFFff);
                 }
            }

           delay_ms(250);
           
           */
          // drawClock();
          // delay_ms(2000);
    }
     
    
    /*
    n =  0x80;
    //n = SPI1_Exchange8bit( n );
       while(1)
    {
         if(n == 0x0)
         {
             n = 0x80;
       }
    i = SPI1D_Exchange8bit( n );
       n = n >> 1;
    __delay_ms(50);
    }
        return 0;
        */

}