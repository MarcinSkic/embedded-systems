#include <targets\AT91SAM7.h>
#include "pcf8833u8_lcd.h"
#include "fonts.h"
#include "math.h"

#define LCD_RESET_LOW     PIOA_CODR   = BIT2
#define LCD_RESET_HIGH    PIOA_SODR   = BIT2

void Delaya (unsigned long a) { while (--a!=0); }

void Delay_ (unsigned long a) {

  volatile unsigned long d;
  d=a;
  while (--d!=0);
}

unsigned int i,j;


void WriteSpiCommand(unsigned int data){

  data = (data & ~0x0100);

  // Wait for the transfer to complete
  while((SPI0_SR & SPI0_SR_TXEMPTY) == 0);

  SPI0_TDR = data;
}

void WriteSpiData(unsigned int data){

  data = (data | 0x0100);

  // Wait for the transfer to complete
  while((SPI0_SR & SPI0_SR_TXEMPTY) == 0);

  SPI0_TDR = data;
}


void Backlight(unsigned char state) {

  if(state == BL_ON)
    PIOB_SODR   = BIT20;    // Set PB20 to HIGH
  else
    PIOB_CODR   = BIT20;    // Set PB20 to LOW

}

void SetContrast(unsigned char contrast) {

//#ifdef GE12
WriteSpiCommand(CONTRAST);
WriteSpiData(0x20+contrast);    // contrast

//   WriteSpiCommand(VOLCTR);
//   WriteSpiData(32+contrast); // contrast
//   WriteSpiData(3); // contrast

}

void SendLcd(int tryb, int data)   // TEGO NIE MA W LCD.c
{
if (tryb==LCDCommand) 
  WriteSpiCommand(data);
else WriteSpiData(data);
}


void LCDSettings(void) {
  LCD_RESET_LOW;
  Delay_(10000);
  LCD_RESET_HIGH;
  Delay_(10000);
  
  // Sleep out (command0x11)
  WriteSpiCommand(SLEEPOUT);
  
  // Inversion on(command 0x20)
  WriteSpiCommand(INVERSIONOFF);    // seems to be required for this controller
  
  // Color Interface Pixel Format(command 0x3A)
  WriteSpiCommand(PIXELFORMAT);
  WriteSpiData(0x03);     // 0x03 =12bits-per-pixel
  
  // Write contrast  (command 0x25)
  WriteSpiCommand(CONTRAST);
  WriteSpiData(0x40);     // contrast 0x38
  Delay_(10000);
  
  // DisplayOn  (command 0x29)
  WriteSpiCommand(DISPLAYON);

  // 6. Set Normal mode (my)
  WriteSpiCommand(NORMALMODE);

  // 7. Inversion off
  WriteSpiCommand(INVERSIONOFF); // OFF?

  // 8. Column address set
  WriteSpiCommand(COLADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 9. Page address set
  WriteSpiCommand(PAGEADDRSET);
  WriteSpiData(0);
  WriteSpiData(131);

  // 10. Memory access controler
  WriteSpiCommand(ACCESSCTRL);
  WriteSpiData(0x08);

  ///// Display setting 1 end  /////


  ///// Power supply  ///////

  // 1. Power control
  WriteSpiCommand(PWRCTRL);
  WriteSpiData(4);     // Internal resistance, V1OUT -> high power mode, oscilator devision rate

  // 2. Sleep out
  WriteSpiCommand(SLEEPOUT);


  // 3. Voltage control - voltage control and write contrast define LCD electronic volume
  WriteSpiCommand(VOLTCTRL);
  //WriteSpiData(0x7f);    //  full voltage control
  //WriteSpiData(0x03);    //  must be "1"

  Delaya(2000);

  // 5. Temperature gradient
  WriteSpiCommand(TEMPGRADIENT);
  for(i=0; i<14; i++) {
	  WriteSpiData(0);
  }

  // 6. Booster voltage ON
  WriteSpiCommand(BOOSTVON);


  // Finally - Display On
  WriteSpiCommand(DISPLAYON);

}

void InitLCD(void) {

  // Pin for backlight
  PIOB_SODR   = BIT20;    // Set PB20 to HIGH
  PIOB_OER    = BIT20;    // Configure PB20 as output
  // Reset pin
  PIOA_SODR   = BIT2;     // Set PA2 to HIGH
  PIOA_OER    = BIT2;     // Configure PA2 as output
  // Init SPI0
  //set functionality to pins:
  //port0.12 -> NPCS0
  //port0.16 -> MISO
  //port0.17 -> MOSI
  //port0.18 -> SPCK
  PIOA_PDR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  PIOA_ASR = BIT12 | BIT16 | BIT17 | BIT18 | BIT13;
  PIOA_BSR = 0;
  //enable the clock of SPI
  PMC_PCER = PMC_PCER_SPI0;
  // Fixed mode
  SPI0_CR      = 0x80;               //SPI Enable, Sowtware reset
  SPI0_CR      = 0x01;               //SPI Enable
  SPI0_MR      = 0x100E0011;            //Master mode, fixed select, disable decoder, FDIV=1 (MCK), PCS=1110
  SPI0_CSR0  = 0x01010C11;           //9bit, CPOL=1, ClockPhase=0, SCLK = 48Mhz/32*12 = 125kHz
  SPI0_CSR1  = 0x01010502;
Delay_(10000);
LCDSettings();
}


void LCDWrite130x130bmp(unsigned char bmp) {
	  // Display OFF
	  WriteSpiCommand(DISPLAYOFF);

	  // WRITE MEMORY
	  WriteSpiCommand(MEMWRITE);


	  for(j=0; j<sizeof(bmp); j++) {
		  WriteSpiData(bmp[j]);
	  }


	  // Display On
	  WriteSpiCommand(DISPLAYON);
}


// ***************************************************************************** 
// LCDClearScreen.c 
// 
// Clears the LCD screen to single color (BLACK) 
// 
// Inputs: none 
// 
// Author: James P Lynch August 30, 2007 
// ***************************************************************************** 
 void LCDClearScreen(void) { 
  
  long i; // loop counter 
  
  // Row address set (command 0x2B) 
  WriteSpiCommand(PASET); //PASET NA PAGEADDRSET
  WriteSpiData(0); 
  WriteSpiData(131); 
  
  // Column address set (command 0x2A) 
  WriteSpiCommand(CASET); //CASET na COLADDRSET
  WriteSpiData(0); 
  WriteSpiData(131); 
  
  // set the display memory to BLACK 
  WriteSpiCommand(RAMWR); //RAMWR na MEMWRITE
  for(i = 0; i < ((132 * 132) / 2); i++) { 
  WriteSpiData((WHITE >> 4) & 0xFF); 
  WriteSpiData(((WHITE & 0xF) << 4) | ((WHITE >> 8) & 0xF)); 
  WriteSpiData(WHITE & 0xFF); 
  } 
 }
 
// 
// ************************************************************************************* 
// LCDSetPixel.c 
// 
// Lights a single pixel in the specified color at the specified x and y addresses 
// 
// Inputs: x = row address (0 .. 131) 
// y = column address (0 .. 131) 
// color = 12-bit color value rrrrggggbbbb 
// rrrr = 1111 full red 
// : 
// 0000 red is off 
// 
// gggg = 1111 full green 
// : 
// 0000 green is off 
// 
// bbbb = 1111 full blue 
// : 
// 0000 blue is off 
// 
// Returns: nothing 
// 
// Note: see lcd.h for some sample color settings 
// 
// Author: James P Lynch August 30, 2007 
// *************************************************************************************
 void LCDSetPixel(int x, int y, int color) { 
  
  // Row address set (command 0x2B) 
  WriteSpiCommand(PASET); 
  WriteSpiData(x); 
  WriteSpiData(x); 
  
  // Column address set (command 0x2A) 
  WriteSpiCommand(CASET); 
  WriteSpiData(y); 
  WriteSpiData(y); 
  
  // Now illuminate the pixel (2nd pixel will be ignored) 
  WriteSpiCommand(RAMWR); 
  WriteSpiData((color >> 4) & 0xFF); 
  WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF)); 
  WriteSpiData(color & 0xFF); 
 }
// 
// ************************************************************************************************* 
// LCDSetLine.c 
// 
// Draws a line in the specified color from (x0,y0) to (x1,y1) 
// 
// Inputs: x = row address (0 .. 131) 
// y = column address (0 .. 131) 
// color = 12-bit color value rrrrggggbbbb 
// rrrr = 1111 full red 
// : 
// 0000 red is off 
// 
// gggg = 1111 full green 
// : 
// 0000 green is off 
// 
// bbbb = 1111 full blue 
// : 
// 0000 blue is off 
// 
// Returns: nothing 
// 
// Note: good write-up on this algorithm in Wikipedia (search for Bresenham's line algorithm) 
// see lcd.h for some sample color settings 
// 
// Authors: Dr. Leonard McMillan, Associate Professor UNC 
// Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962) 
// 
// Note: taken verbatim from Professor McMillan's presentation: 
// http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html 
// 
// ************************************************************************************************
 void LCDSetLine(int x0, int y0, int x1, int y1, int color) { 
  
  int dy = y1 - y0; 
  int dx = x1 - x0; 
  int stepx, stepy; 
   
  
  if (dy < 0) { dy = -dy; stepy = -1; } else { stepy = 1; } 
  if (dx < 0) { dx = -dx; stepx = -1; } else { stepx = 1; } 
  dy <<= 1; // dy is now 2*dy 
  dx <<= 1; // dx is now 2*dx 
  
  LCDSetPixel(x0, y0, color); 
  if (dx > dy) { 
  int fraction = dy - (dx >> 1); // same as 2*dy - dx 
  while (x0 != x1) { 
  if (fraction >= 0) { 
  y0 += stepy; 
  fraction -= dx; // same as fraction -= 2*dx 
  } 
  x0 += stepx; 
  fraction += dy; // same as fraction -= 2*dy 
  LCDSetPixel(x0, y0, color); 
  } 
  } else { 
  int fraction = dx - (dy >> 1); 
  while (y0 != y1) { 
  if (fraction >= 0) { 
  x0 += stepx; 
  fraction -= dy; 
  } 
  y0 += stepy; 
  fraction += dx; 
  LCDSetPixel(x0, y0, color); 
  } 
  } 
 }
// ***************************************************************************************** 
// LCDSetRect.c 
// 
// Draws a rectangle in the specified color from (x1,y1) to (x2,y2) 
// Rectangle can be filled with a color if desired 
// 
// Inputs: x = row address (0 .. 131) 
// y = column address (0 .. 131) 
// fill = 0=no fill, 1-fill entire rectangle 
// color = 12-bit color value for lines rrrrggggbbbb 
// rrrr = 1111 full red 
// : 
// 0000 red is off 
// 
// gggg = 1111 full green 
// : 
// 0000 green is off 
// 
// bbbb = 1111 full blue 
// : 
// 0000 blue is off 
// 
// Returns: nothing 
// 
// Notes: 
// 
// The best way to fill a rectangle is to take advantage of the "wrap-around" featute 
// built into the Epson S1D15G00 controller. By defining a drawing box, the memory can 
// be simply filled by successive memory writes until all pixels have been illuminated. 
// 
// 1. Given the coordinates of two opposing corners (x0, y0) (x1, y1) 
// calculate the minimums and maximums of the coordinates 
// 
// xmin = (x0 <= x1) ? x0 : x1; 
// xmax = (x0 > x1) ? x0 : x1; 
// ymin = (y0 <= y1) ? y0 : y1; 
// ymax = (y0 > y1) ? y0 : y1; 
// 
// 2. Now set up the drawing box to be the desired rectangle 
// 
// WriteSpiCommand(PASET); // set the row boundaries 
// WriteSpiData(xmin); 
// WriteSpiData(xmax); 
// WriteSpiCommand(CASET); // set the column boundaries 
// WriteSpiData(ymin); 
// WriteSpiData(ymax); 
// 
 void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color) { 
  int xmin, xmax, ymin, ymax; 
  int i; 
  
  // check if the rectangle is to be filled 
  if (fill == FILL) { 
  
  // best way to create a filled rectangle is to define a drawing box 
  // and loop two pixels at a time 
  
  // calculate the min and max for x and y directions 
  xmin = (x0 <= x1) ? x0 : x1; 
  xmax = (x0 > x1) ? x0 : x1; 
  ymin = (y0 <= y1) ? y0 : y1; 
  ymax = (y0 > y1) ? y0 : y1; 
  
  // specify the controller drawing box according to those limits 
  // Row address set (command 0x2B) 
  WriteSpiCommand(PASET); 
  WriteSpiData(xmin); 
  WriteSpiData(xmax); 
  
  // Column address set (command 0x2A) 
  WriteSpiCommand(CASET); 
  WriteSpiData(ymin); 
  WriteSpiData(ymax); 
  
  // WRITE MEMORY 
  WriteSpiCommand(RAMWR); 
  
  // loop on total number of pixels / 2 
  for (i = 0; i < ((((xmax - xmin + 1) * (ymax - ymin + 1)) / 2) + 130); i++) { 
  
  // use the color value to output three data bytes covering two pixels 
  WriteSpiData((color >> 4) & 0xFF); 
  WriteSpiData(((color & 0xF) << 4) | ((color >> 8) & 0xF)); 
  WriteSpiData(color & 0xFF); 
  } 
  
  } else { 
  
  // best way to draw un unfilled rectangle is to draw four lines 
  LCDSetLine(x0, y0, x1, y0, color); 
  LCDSetLine(x0, y1, x1, y1, color); 
  LCDSetLine(x0, y0, x0, y1, color); 
  LCDSetLine(x1, y0, x1, y1, color); 
  } 
 } 
//  
//  
//  // ************************************************************************************* 
// LCDSetCircle.c 
// 
// Draws a line in the specified color at center (x0,y0) with radius 
// 
// Inputs: x0 = row address (0 .. 131) 
// y0 = column address (0 .. 131) 
// radius = radius in pixels 
// color = 12-bit color value rrrrggggbbbb 
// 
// Returns: nothing 
// 
// Author: Jack Bresenham IBM, Winthrop University (Father of this algorithm, 1962) 
// 
// Note: taken verbatim Wikipedia article on Bresenham's line algorithm 
// http://www.wikipedia.org 
// 
// ************************************************************************************* 
//  
 void LCDSetCircle(int x0, int y0, int radius, int color) { 
  int f = 1 - radius; 
  int ddF_x = 0; 
  int ddF_y = -2 * radius; 
  int x = 0; 
  int y = radius; 
  
  LCDSetPixel(x0, y0 + radius, color); 
  LCDSetPixel(x0, y0 - radius, color); 
  LCDSetPixel(x0 + radius, y0, color); 
  LCDSetPixel(x0 - radius, y0, color); 
  
  while(x < y) { 
  if(f >= 0) { 
  y--; 
  ddF_y += 2; 
  f += ddF_y; 
  } 
  x++; 
  ddF_x += 2; 
  f += ddF_x + 1; 
  LCDSetPixel(x0 + x, y0 + y, color); 
  LCDSetPixel(x0 - x, y0 + y, color); 
  LCDSetPixel(x0 + x, y0 - y, color); 
  LCDSetPixel(x0 - x, y0 - y, color); 
  LCDSetPixel(x0 + y, y0 + x, color); 
  LCDSetPixel(x0 - y, y0 + x, color); 
  LCDSetPixel(x0 + y, y0 - x, color); 
  LCDSetPixel(x0 - y, y0 - x, color); 
  } 
 } 
//  
 void LCDPutChar(char c, int x, int y, int size, int fColor, int bColor) { 
  
  extern const unsigned char FONT6x8[97][8]; 
  extern const unsigned char FONT8x8[97][8]; 
  extern const unsigned char FONT8x16[97][16]; 
  
  int i,j; 
  unsigned int nCols; 
  unsigned int nRows; 
  unsigned int nBytes; 
  unsigned char PixelRow; 
  unsigned char Mask; 
  unsigned int Word0; 
  unsigned int Word1; 
  unsigned char *pFont; 
  unsigned char *pChar; 
  unsigned char *FontTable[] = {(unsigned char *)FONT6x8, (unsigned char *)FONT8x8, 
  (unsigned char *)FONT8x16}; 
  
  // get pointer to the beginning of the selected font table 
  pFont = (unsigned char *)FontTable[size]; 
  
  // get the nColumns, nRows and nBytes 
  nCols = *pFont; 
  nRows = *(pFont + 1); 
  nBytes = *(pFont + 2); 
  
  // get pointer to the last byte of the desired character 
  pChar = pFont + (nBytes * (c - 0x1F)) + nBytes - 1; 
  
  // Row address set (command 0x2B) 
  WriteSpiCommand(PASET); 
  WriteSpiData(x); 
  WriteSpiData(x - nRows + 1); 
  
  // Column address set (command 0x2A) 
  WriteSpiCommand(CASET); 
  WriteSpiData(y); 
  WriteSpiData(y + nCols - 1); 
  
  // WRITE MEMORY 
  WriteSpiCommand(RAMWR); 
  
  // loop on each row, working backwards from the bottom to the top 
  for (i = nRows - 1; i >= 0; i--) { 
  
  // copy pixel row from font table and then decrement row 
  PixelRow = *pChar--; 
  
  // loop on each pixel in the row (left to right) 
  // Note: we do two pixels each loop 
  Mask = 0x80; 
  for (j = 0; j < nCols; j += 2) { 
  
  // if pixel bit set, use foreground color; else use the background color 
  // now get the pixel color for two successive pixels 
  if ((PixelRow & Mask) == 0) 
  Word0 = bColor; 
  else 
  Word0 = fColor; 
  Mask = Mask >> 1; 
  if ((PixelRow & Mask) == 0) 
  Word1 = bColor; 
  else 
  Word1 = fColor; 
  Mask = Mask >> 1; 
  
  // use this information to output three data bytes 
  WriteSpiData((Word0 >> 4) & 0xFF); 
  WriteSpiData(((Word0 & 0xF) << 4) | ((Word1 >> 8) & 0xF)); 
  WriteSpiData(Word1 & 0xFF); 
  } 
  } 
  // terminate the Write Memory command 
  WriteSpiCommand(NOP); 
 } 
//  
//  
// ************************************************************************************************* 
// LCDPutStr.c 
// 
// Draws a null-terminates character string at the specified (x,y) address, size and color 
// 
// Inputs: pString = pointer to character string to be displayed 
// x = row address (0 .. 131) 
// y = column address (0 .. 131) 
// Size = font pitch (SMALL, MEDIUM, LARGE) 
// fColor = 12-bit foreground color value rrrrggggbbbb 
// bColor = 12-bit background color value rrrrggggbbbb 
// 
// 
// Returns: nothing 
// 
// Notes: Here's an example to display "Hello World!" at address (20,20) 
// 
// LCDPutChar("Hello World!", 20, 20, LARGE, WHITE, BLACK); 
// 
// 
// Author: James P Lynch August 30, 2007 
// ************************************************************************************************* 
 void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor) { 
  
  // loop until null-terminator is seen 
  while (*pString != 0x00) { 
  
  // draw the character 
  LCDPutChar(*pString++, x, y, Size, fColor, bColor); 
  
  // advance the y position 
  if (Size == SMALL) 
  y = y + 6; 
  else if (Size == MEDIUM) 
  y = y + 8; 
  else 
  y = y + 8; 
  
  // bail out if y exceeds 131 
  if (y > 131) break; 
  } 
 } 
