 #if defined(__GAMMASET1)
		const uint8_t pGammaSet[15]= {0x36,0x29,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x12,0x0A,0x11,0x0B,0x06};
		const uint8_t nGammaSet[15]= {0x09,0x16,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x34,0x39};
	#elif defined(__GAMMASET2)
		const uint8_t pGammaSet[15]= {0x3F,0x21,0x12,0x22,0x1C,0x15,0x42,0xB7,0x2F,0x13,0x02,0x0A,0x01,0x00,0x00};
		const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#elif defined(__GAMMASET3)
		const uint8_t pGammaSet[15]= {0x3F,0x26,0x23,0x30,0x28,0x10,0x55,0xB7,0x40,0x19,0x10,0x1E,0x02,0x01,0x00};
		const uint8_t nGammaSet[15]= {0x09,0x18,0x2D,0x0D,0x13,0x15,0x40,0x48,0x53,0x0C,0x1D,0x25,0x2E,0x24,0x29};
	#else
		const uint8_t pGammaSet[15]= {0x3F,0x25,0x1C,0x1E,0x20,0x12,0x2A,0x90,0x24,0x11,0x00,0x00,0x00,0x00,0x00};
		const uint8_t nGammaSet[15]= {0x20,0x20,0x20,0x20,0x05,0x15,0x00,0xA7,0x3D,0x18,0x25,0x2A,0x2B,0x2B,0x3A};
	#endif

#define _TFTWIDTH  		128//the REAL W resolution of the TFT
#define _TFTHEIGHT 		160//the REAL H resolution of the TFT
#define _GRAMWIDTH      128
#define _GRAMHEIGH      160  //160
#define _GRAMSIZE		_GRAMWIDTH * _GRAMHEIGH//*see note 1
#define __COLORSPC		0// 1:GBR - 0:RGB
#define __GAMMASET3		//uncomment for another gamma
#define __OFFSET		0//*see note 2
#define	BLACK   		0x0000
        
#define fillScreen clearScreen
        
#define WREN 0X6
#define WR 0x2
#define RD 0x3
#define RDSTAT 0x5
#define WRDIS 0x04

#define ACK  0x06 
#define NAK  0x15 
#define SOH  0x01
#define EOT  0x04
#define ETB  0x17


#define CSLow LATDbits.LATD12 = 0
#define CSHigh LATDbits.LATD12 = 1        
        

#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include "TFT_ILI9163C_registers.h"
#include "mcc_generated_files/spi1.h"
//#include "mcc_generated_files/spi1.c"

void chipinit();
void colorSpace(uint8_t ); 
void clearScreen(uint16_t); 
void setRotation(uint8_t ); 
void setAddrWindow(uint16_t, uint16_t , uint16_t, uint16_t);
void swap(int16_t *xp, int16_t *yp);
void VLine(int16_t, int16_t , int16_t, uint16_t); //__attribute__((always_inline)) 
void HLine(int16_t, int16_t, int16_t, uint16_t);
void drawFastVLine(int16_t, int16_t, int16_t, uint16_t);
void drawFastHLine(int16_t, int16_t, int16_t, uint16_t);
bool boundaryCheck(int16_t  ,int16_t  );
void drawPixel(int16_t, int16_t, uint16_t);
void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color);
uint16_t RGBtoU16(uint16_t r, uint16_t g, uint16_t b);
void drawFillCircle(uint16_t xCenter, uint16_t yCenter, uint16_t radius, uint16_t color);
void resetChip();
uint8_t readByte(uint32_t );
void writeByte(uint32_t, uint8_t);
uint16_t getStoreImage(void);


uint8_t _Mactrl_Data;
uint8_t rotation = 0;
uint8_t _width;
uint8_t _height;
uint8_t	_colorspaceData = 0x00;
uint16_t timeout;

extern bool IFlag;

void chipinit()
{
    
	uint8_t i;
  
    resetChip();
    
    PORTDbits.RD11 = 0;
	
		SPI1C_Exchange8bit(CMD_SWRESET);//software reset
		__delay_ms(500);
		
		SPI1C_Exchange8bit(CMD_SLPOUT);//exit sleep
		__delay_ms(120);
        
          SPI1C_Exchange8bit(CMD_DISPON);//display ON 
		__delay_ms(20);
        
        
		
		SPI1C_Exchange8bit(CMD_PIXFMT);//Set Color Format 16bit   
		SPI1D_Exchange8bit(0x05);
		__delay_ms(100);
		
		SPI1C_Exchange8bit(CMD_GAMMASET);//default gamma curve 3
		SPI1D_Exchange8bit(0x04);//0x04
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_GAMRSEL);//Enable Gamma adj    
		SPI1D_Exchange8bit(0x01); 
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_NORML);
	
		SPI1C_Exchange8bit(CMD_DFUNCTR);
		SPI1D_Exchange8bit(0b11111111);//
		SPI1D_Exchange8bit(0b00000110);//

		SPI1C_Exchange8bit(CMD_PGAMMAC);//Positive Gamma Correction Setting
		for (i=0;i<15;i++){
			SPI1D_Exchange8bit(pGammaSet[i]);
		}
		
		SPI1C_Exchange8bit(CMD_NGAMMAC);//Negative Gamma Correction Setting
		for (i=0;i<15;i++){
			SPI1D_Exchange8bit(nGammaSet[i]);
		}

		SPI1C_Exchange8bit(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
		SPI1D_Exchange8bit(0x08);//0x0C//0x08
		SPI1D_Exchange8bit(0x02);//0x14//0x08
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_DINVCTR);//display inversion 
		SPI1D_Exchange8bit(0x07);
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
		SPI1D_Exchange8bit(0x0A);//4.30 - 0x0A
		SPI1D_Exchange8bit(0x02);//0x05
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
		SPI1D_Exchange8bit(0x02);
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
		SPI1D_Exchange8bit(0x50);//0x50
		SPI1D_Exchange8bit(99);//0x5b
		__delay_ms(1);
		
		SPI1C_Exchange8bit(CMD_VCOMOFFS);
		SPI1D_Exchange8bit(0);//0x40
		__delay_ms(1);
  
		SPI1C_Exchange8bit(CMD_CLMADRS);//Set Column Address  
		SPI1D_Exchange8bit(0x00);   
        SPI1D_Exchange8bit(0x00);  //added two extra transfers to make up for slash 16's
        SPI1D_Exchange8bit(0x00);  //16
		SPI1D_Exchange8bit(_GRAMWIDTH);   //16
  __delay_ms(1);
  
		SPI1C_Exchange8bit(CMD_PGEADRS);//Set Page Address  
		SPI1D_Exchange8bit(0X00);    
        SPI1D_Exchange8bit(0X00);  //added two extra transfers to make up for slash 16's
        SPI1D_Exchange8bit(0X00);  //16
		SPI1D_Exchange8bit(_GRAMHEIGH);   //16
	__delay_ms(1);
        
		SPI1C_Exchange8bit(CMD_VSCLLDEF);
        SPI1D_Exchange8bit(0X00);  //added extra transfer
		SPI1D_Exchange8bit(__OFFSET);     //16
        SPI1D_Exchange8bit(0X00);  //added extra transfer
		SPI1D_Exchange8bit(_GRAMHEIGH - __OFFSET);        //16
        SPI1D_Exchange8bit(0X00);  //added extra transfer
		SPI1D_Exchange8bit(0);    //16
		__delay_ms(1);
        
		colorSpace(_colorspaceData);
		
		setRotation(2);
		
        SPI1C_Exchange8bit(CMD_DISPON);//display ON 
		__delay_ms(20);
		SPI1C_Exchange8bit(CMD_RAMWR);//Memory Write
        __delay_ms(100);
	
    	clearScreen(BLACK);
}
    
    
void colorSpace(uint8_t cspace) 
{
	if (cspace < 1)
    {
		_Mactrl_Data &= ~(1 << 3);  //bitClear(_Mactrl_Data,3);//  
	}
    else {
		_Mactrl_Data |= 1 << 3;  //bitSet(_Mactrl_Data,3);//_
	}
}

void clearScreen(uint16_t color) 
{
	int px;
    int n;
    
	setAddrWindow(0x00,0x00,_GRAMWIDTH - 1,_GRAMHEIGH - 1);//go home
    
	for(n=0; n < _GRAMHEIGH * _GRAMWIDTH; n++)
    {
     
        SPI1D_ExchangeColor(color); //16
           
    }

}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

		SPI1C_Exchange8bit(CMD_CLMADRS); // Column
		if (rotation == 0 || rotation > 1)
        {   SPI1D_Exchange8bit(0X00);  //added extra transfer
			SPI1D_Exchange8bit(x0); //16
            SPI1D_Exchange8bit(0X00);  //added extra transfer
			SPI1D_Exchange8bit(x1);////16
        
        }
                        else {

                        SPI1D_Exchange8bit(0X00);  //added extra transfer
                        SPI1D_Exchange8bit(x0 + __OFFSET);//16
                        SPI1D_Exchange8bit(0X00);  //added extra transfer
                        SPI1D_Exchange8bit(x1 + __OFFSET);//16
		}

		SPI1C_Exchange8bit(CMD_PGEADRS); // Page
		if (rotation == 0){
			SPI1D_Exchange8bit(0X00);//16
			SPI1D_Exchange8bit( y0 );//16
            SPI1D_Exchange8bit(0X00);//16
			SPI1D_Exchange8bit( y1 );//16
		}
                        else {
                           SPI1D_Exchange8bit(0X00);//16
                            SPI1D_Exchange8bit( y0  + __OFFSET);//16
                            SPI1D_Exchange8bit(0X00);//16
                             SPI1D_Exchange8bit( y1 + __OFFSET );//16
		}
		SPI1C_Exchange8bit(CMD_RAMWR); //Into RAM
        
}


void setRotation(uint8_t m) 
{
	rotation = m % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		_Mactrl_Data = 0b00001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 1:
		_Mactrl_Data = 0b01101000;
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		break;
	case 2:
		_Mactrl_Data = 0b11001000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	case 3:
		_Mactrl_Data = 0b10101000;
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		break;
	}
	//colorSpace(_colorspaceData);


  //  uint8_t i = 0xc8;
		SPI1C_Exchange8bit(CMD_MADCTL);
		//SPI1D_Exchange8bit(0x00);
        SPI1D_Exchange8bit(_Mactrl_Data);
        //SPI1D_Exchange8bit(_Mactrl_Data << 8);
    //    SPI1D_Exchange8bit(0x00);
     //   SPI1D_Exchange8bit(i);
        //SPI1C_Exchange8bit(CMD_RAMWR); //Into RAM

}

void drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if (boundaryCheck(x,y)) return;
	//setAddrWindow(x,y,x+1,y+1);
    setAddrWindow(x,y,x,y);
	 
	SPI1D_ExchangeColor(color);

}



void  drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((x+w) - 1) >= _width)  w = _width-x;
	setAddrWindow(x,y,(x+w)-1,y);
	while (w-- > 0)
    {
		
			SPI1D_ExchangeColor(color);
		 
	}
 
}

bool  boundaryCheck(int16_t x,int16_t y)
{
	if ((x >= _width) || (y >= _height)) return true;
	return false;
}

void  drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height-y;
	setAddrWindow(x,y,x,(y+h)-1);
	while (h-- > 0)
    {
		 
			SPI1D_ExchangeColor(color);
		 
	}
	
}

void HLine(int16_t x, int16_t y, int16_t w, uint16_t color) //__attribute__((always_inline))
{
			setAddrWindow(x, y, x+w-1, y);
            
			do {
                SPI1D_ExchangeColor(color); 
              }
            while (--w > 0);
            
        SPI1C_Exchange8bit(CMD_RAMWR); //Into RAM
}


void VLine(int16_t x, int16_t y, int16_t h, uint16_t color) //__attribute__((always_inline)) 
{
			setAddrWindow(x, y, x, y+h-1);
            
			do {
                SPI1D_ExchangeColor(color); 
              }
            while (--h > 0);
}

void  drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	if (y0 == y1) {
		if (x1 > x0) {
			drawFastHLine(x0, y0, x1 - x0 + 1, color);
		} else if (x1 < x0) {
			drawFastHLine(x1, y0, x0 - x1 + 1, color);
		} else {
			drawPixel(x0, y0, color);
		}
		return;
	} else if (x0 == x1) {
		if (y1 > y0) {
			drawFastVLine(x0, y0, y1 - y0 + 1, color);
		} else {
			drawFastVLine(x0, y1, y0 - y1 + 1, color);
		}
		return;
	}

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep)
    {
		swap(&x0, &y0);
		swap(&x1, &y1);
	}
	if (x0 > x1)
    {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	
	
	int16_t xbegin = x0;
	if (steep) {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					VLine(y0, xbegin, len + 1, color);
				} else {
					drawPixel(y0, x0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) {
			VLine(y0, xbegin, x0 - xbegin, color);
		}

	} else {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					HLine(xbegin, y0, len + 1, color);
				} else {
					drawPixel(x0, y0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) {
			HLine(xbegin, y0, x0 - xbegin, color);
		}
	}
	//writecommand_last(CMD_NOP);   //what is this?
	//SPI.endTransaction();
}

void swap(int16_t *xp, int16_t *yp)
{
    int16_t temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color)
  {
  uint8_t x = radius;
  uint8_t y = 0;
  int16_t radiusError = 1 - (int16_t) x;

  while (x >= y)
    {
    //11 O'Clock
    drawPixel(x0 - y, y0 + x, color);
    //1 O'Clock
    drawPixel(x0 + y, y0 + x, color);
    //10 O'Clock
    drawPixel(x0 - x, y0 + y, color);
    //2 O'Clock
    drawPixel(x0 + x, y0 + y, color);
    //8 O'Clock
    drawPixel(x0 - x, y0 - y, color);
    //4 O'Clock
    drawPixel(x0 + x, y0 - y, color);
    //7 O'Clock
    drawPixel(x0 - y, y0 - x, color);
    //5 O'Clock
    drawPixel(x0 + y, y0 - x, color);

    y++;
    if (radiusError < 0)
      radiusError += (int16_t)(2 * y + 1);
    else
      {
      x--;
      radiusError += 2 * (((int16_t) y - (int16_t) x) + 1);
      }
    }
  }

uint16_t RGBtoU16(uint16_t r, uint16_t g, uint16_t b)
{
    uint16_t n;
    n = r << 8;
    
    n = n | (g << 3);
    
    n = n | (b >> 3);
    
    return n;
    
}

void drawFillCircle(uint16_t xCenter, uint16_t yCenter, uint16_t radius, uint16_t color)
{

int f = 1 - radius;
int ddF_x = 1;
int ddF_y = -2 * radius;
uint8_t x = 0;
uint8_t y = radius;
 
	/*
	 * Fill in the center between the two halves
	 */
	drawLine(xCenter, yCenter-radius, xCenter, yCenter+radius, color);
 
	while(x < y)
	{
    // ddF_x == 2 * x + 1;
    // ddF_y == -2 * y;
    // f == x*x + y*y - radius*radius + 2*x - y + 1;
		if(f >= 0) 
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		/*
		 * Now draw vertical lines between the points on the circle rather than
		 * draw the points of the circle. This draws lines between the 
		 * perimeter points on the upper and lower quadrants of the 2 halves of the circle.
		 */

		drawLine(xCenter+x, yCenter+y, xCenter+x, yCenter-y, color);
		drawLine(xCenter-x, yCenter+y, xCenter-x, yCenter-y, color);
		drawLine(xCenter+y, yCenter+x, y+xCenter, yCenter-x, color);
		drawLine(xCenter-y, yCenter+x, xCenter-y, yCenter-x, color);
  	}
}

void resetChip()
{
    delay_ms(10);
    LATAbits.LATA7 = 0;
    delay_ms(1);
    LATAbits.LATA7 = 1;
    delay_ms(200);
    
    
}

void drawFillRect( uint8_t  x0, uint8_t  y0, uint8_t x1, uint8_t y1, uint16_t c )
{
    uint16_t size = 0;
    
     //  if( boundaryCheck( x0, y0) || boundaryCheck(x0 + x1, y0 + y1))
     //   return;
                
    		setAddrWindow(x0, y0, x0+x1, y0+y1);
            
			do {
                SPI1D_ExchangeColor(c); 
              }
            while ( --size > 0);
            
    
}

void drawImage(void)
{
    uint16_t size16, x, b;
    uint8_t data8, start8;
    uint32_t addr32 ,size32, n32;
    
    addr32 = getStoreImage();
    
   size16 = readByte(0x00000002 );
    
    
    size16 = size16  | readByte(0x03) << 8  ;
    
    start8 = readByte(0xa);
            
     setAddrWindow(0,0,_GRAMWIDTH - 1, _GRAMHEIGH - 1);

    
     for(n32 = 0 ; n32 < (size16 - start8) ; n32 +=2)
        {
         
         x = readByte(n32 + start8) ;
         x = x | ( readByte(n32 + start8 + 1) <<8 );
         
         SPI1D_ExchangeColor( x ); 
        
        }
     
    
}

void write128Byte(uint32_t addr, uint8_t *data)
{
    int n;
    CSLow;
    SPI2_Exchange8bit(WREN);  //wren bit
    CSHigh;
    
    CSLow;
    SPI2_Exchange8bit(WR);  //write command
    
    SPI2_Exchange8bit(addr >> 16);  
    
    SPI2_Exchange8bit(addr >> 8);  
    
    SPI2_Exchange8bit(addr ); //bottom byte of 16 bit addr
  
    for(n=0;n<128;n++)
    {
        SPI2_Exchange8bit(*data++);   //8 bits
    }
   
    CSHigh;
    delay_ms(10);
      
   
}
void writeByte(uint32_t addr, uint8_t data)
{
    
    CSLow;
    SPI2_Exchange8bit(WREN);  //wren bit
    CSHigh;
    
    CSLow;
    SPI2_Exchange8bit(WR);  //write command
    
    SPI2_Exchange8bit(addr >> 16);  
    
    SPI2_Exchange8bit(addr >> 8);  
    
    SPI2_Exchange8bit(addr ); //bottom byte of 16 bit addr
  
    
    SPI2_Exchange8bit(data);   //8 bits
    Nop();
   
    CSHigh;
     delay_ms(10);
      
   
}
 
uint8_t readByte(uint32_t addr)
{
    uint8_t recieve, a,b,c;
    CSLow;
    SPI2_Exchange8bit(RD);
  //  Nop();
  //  Nop();
    
    
    a = addr >> 16;
     b = addr >> 8;
      c = addr ;
    SPI2_Exchange8bit(addr >> 16);
    
    SPI2_Exchange8bit(addr >> 8);
    SPI2_Exchange8bit(addr );
  //  Nop();
   //  Nop();
     
    //  Nop();
     //  Nop();
 
    recieve = SPI2_Exchange8bit(0);
    
    // recieve = SPI2_Exchange8bit(0);
    
    CSHigh;
    // delay_ms(1);
    
    return recieve;
}
 
void clearTMR1(void)
{
    TMR1_Stop();
    TMR1 = 0;
    TMR1_Start();
    
    
    
    
}

uint16_t getStoreImage(void)
{
    //SYSTEM_Initialize();
    
    
    uint8_t n, x = 0, data ;
    uint16_t  i16, sizeofBMP;
    static uint32_t addr,  y = 0;
    uint8_t UData[133] = {55,55,55,55,55,55,44,44,33,33,33,77,77,77,77,88,88,88,88,88,88,88,66};
            
   
        writeByte( y,  0xfa);
      
        Nop();
        
        n = readByte(y);
        
        Nop();

  
  
    printf("\r\nReady to receive 16-bit color, 128x160 .bmp over xmodem  \n\r");
    printf("\r\press anykey and then send...  \n\r");
//     UART1_Read();
    
    addr = 0;
    timeout = 0;
    n = U1RXREG;
    n = U1RXREG;
    n = U1RXREG;
    n = U1RXREG;
    n = U1RXREG;
    
    IFlag = 0;
    uint8_t when[10] ;
    
    while(!( U1STAbits.URXDA == 1) )
      {
       // delay_ms(1000);
        printf("%c", 0x43);
       // delay_ms(1);
      }
    
    
  //  TMR1_Start();
    TMR1_Stop();
    while(1)
     {
        again:
         for(i16 = 0; i16 < 133; i16++)  // 133 looks like 133 is the correct size. not sure, drinking now.
             
             {
                UData[i16] = UART1_Read();
                if(UData[0] == EOT)
                {
                    printf("%c",ACK);
                    printf("%c",ACK);
                    goto done;
                }
                    
             }
     
          
         //TMR1_Stop();
          
         if(addr == 0)
         {
             sizeofBMP = UData[6] << 8 | UData[5];
           
         }
         
         if(addr >= sizeofBMP)   
         {
             printf("%c",ACK);
             printf("%c",ACK);
             
             goto done;
         }
          //writeByte(addr++, UData[3]);
          //delay_ms(5);
         write128Byte(addr, &UData[3]);
         addr += 128;
          
      //   TMR1_Counter16BitSet(0);
      //   TMR1_Start();
         printf("%c",ACK);
           
    }
    
         
 done:
// TMR1_Stop();
 
 for(n=0;n<128;n++)
 {
     printf("%x ", readByte(n) );
    // delay_ms(5);
 }
 //TMR1_Stop();
 return addr;
}
