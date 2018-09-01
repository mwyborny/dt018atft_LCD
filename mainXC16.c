/*
 * File:   mainXC16.c
 * Author: miwyborn
 *
 * Created on August 15, 2018, 4:47 PM
 */

#define FOSC 8000000L
#define FCY 8000000UL
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

int main(void)
{
    uint16_t n = 0;
  uint8_t w, h, r ; 
  uint16_t c = 0x2222;  
   
  uint8_t  x;
  
  uint8_t  sub_x;
  uint8_t  y;
  uint8_t  sub_y;
//  int t;
    SYSTEM_Initialize();
    
    chipinit();
    
    while(1)
    {
         for(n = 0 ;n <64 ; n += 3)
           {
               drawCircle(( _GRAMWIDTH - 1) /2, (_GRAMHEIGH- 1) /2, n, 0xffff);
           }
   
        // delay_ms(750);
         
         //clearScreen(0x0000);
         
         for (n =0; n<20; n++)
         {
            w = rand() % _GRAMWIDTH ;
            h = rand() % _GRAMHEIGH ;
            r = rand() % _GRAMWIDTH / 8;
            c = rand() % 0xffff;
            
            for(x = 0 ; x < 40 ;x += 4)
             {
                  drawFillCircle(w, h + x, r + (x / 2), c );
             }
             
         }
         
         
      //   clearScreen(0xffff);
     
           for(n = 0; n < _GRAMWIDTH ; n += 4)
              {
                   drawLine( n, 0, _GRAMWIDTH - n, _GRAMHEIGH - 1, 0Xf800);
               }
    
           for(n = 0; n < _GRAMHEIGH ; n += 4)
              {
                   drawLine( 0, n, _GRAMWIDTH, _GRAMHEIGH - n, 0Xf800);
            }
                
             delay_ms(1000);
      
            fillScreen(0xF800 );
            delay_ms(1000);

            fillScreen(0x07e0  );
            delay_ms(1000);
     
      
            fillScreen( 0x001f );
            delay_ms(1000);

/*
            fillScreen( 0x7E0 );
            delay_ms(1000);

            fillScreen( 0x1F );
            delay_ms(1000);

            fillScreen( 0x79E0 );
            delay_ms(1000);

            fillScreen( 0xF81F );
            delay_ms(1000);
*/
            
            for(n = 0; n<10;n += 4)
            {
                HLine(n, n, n, 0x1f );
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

           delay_ms(1000);
           
           
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
