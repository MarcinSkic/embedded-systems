// BIT DEFINITIO
#ifdef __cplusplus
extern "C" {
#endif


#define   BIT0        0x00000001
#define   BIT1        0x00000002
#define   BIT2        0x00000004
#define   BIT3        0x00000008
#define   BIT4        0x00000010
#define   BIT5        0x00000020
#define   BIT6        0x00000040
#define   BIT7        0x00000080
#define   BIT8        0x00000100
#define   BIT9        0x00000200
#define   BIT10       0x00000400
#define   BIT11       0x00000800
#define   BIT12       0x00001000
#define   BIT13       0x00002000
#define   BIT14       0x00004000
#define   BIT15       0x00008000
#define   BIT16       0x00010000
#define   BIT17       0x00020000
#define   BIT18       0x00040000
#define   BIT19       0x00080000
#define   BIT20       0x00100000
#define   BIT21       0x00200000
#define   BIT22       0x00400000
#define   BIT23       0x00800000
#define   BIT24       0x01000000
#define   BIT25       0x02000000
#define   BIT26       0x04000000
#define   BIT27       0x08000000
#define   BIT28       0x10000000
#define   BIT29       0x20000000
#define   BIT30       0x40000000
#define   BIT31       0x80000000

#define LCDData 1
#define LCDCommand 0


#define LOW_RES         0x01                            // 8-bit R-G-B
#define HIGH_RES        0x02                            // 16-bit R-G-B 


// 12-bit color definitions 
#define WHITE 0xFFF 
#define BLACK 0x000  
#define RED 0x00F 
#define GREEN 0x0F0 
#define BLUE 0xF00 
#define CYAN 0xFF0 
#define MAGENTA 0xF0F 
#define YELLOW 0x0FF 
#define BROWN 0x22B 
#define ORANGE 0x0AF 
#define PINK 0xA6F

#define BL_ON        1
#define BL_OFF       0

// Booleans 
#define NOFILL 0 
#define FILL 1 

#define SMALL 0 
#define MEDIUM 1 
#define LARGE 2 

  #define NOP           0x00     // nop
  #define SOFTRST       0x01     // software reset
  #define BOOSTVOFF     0x02     // booster voltage OFF
  #define BOOSTVON      0x03     // booster voltage ON
  #define TESTMODE1     0x04     // test mode
  #define DISPSTATUS    0x09     // display status
  #define SLEEPIN       0x10     // sleep in
  #define SLEEPOUT      0x11     // sleep out
  #define PARTIAL       0x12     // partial display mode
  #define NORMALMODE    0x13     // display normal mode
  #define INVERSIONOFF  0x20     // inversion OFF
  #define INVERSIONON   0x21     // inversion ON
  #define ALLPIXELOFF   0x22     // all pixel OFF
  #define ALLPIXELON    0x23     // all pixel ON
  #define CONTRAST      0x25     // write contrast
  #define DISPLAYOFF    0x28     // display OFF 
  #define DISPLAYON     0x29     // display ON
  #define COLADDRSET    0x2A     // column address set
  #define PAGEADDRSET   0x2B     // page address set
  #define MEMWRITE      0x2C     // memory write
  #define COLORSET      0x2D     // colour set
  #define READRAMDATA   0x2E     // RAM data read
  #define PARTIALAREA   0x30     // partial area
  #define VERTSCROLL    0x33     // vertical scrolling definition
  #define TESTMODE2     0x34     // test mode
  #define TESTMODE3     0x35     // test mode
  #define ACCESSCTRL    0x36     // memory access control
  #define VSCRLSADDR    0x37     // vertical scrolling start address
  #define IDLEOFF       0x38     // idle mode OFF
  #define IDLEON        0x39     // idle mode ON
  #define PIXELFORMAT   0x3A     // interface pixel format
  #define TESTMODE4     0xDE     // test mode
  #define NOP2          0xAA     // nop
  #define INITESC       0xC6     // initial escape
  #define TESTMODE5     0xDA     // test mode
  #define TESTMODE6     0xDB     // test mode
  #define TESTMODE7     0xDC     // test mode
  #define TESTMODE8     0xB2     // test mode
  #define GRAYSCALE0    0xB3     // gray scale position set 0
  #define GRAYSCALE1    0xB4     // gray scale position set 1
  #define GAMMA         0xB5     // gamma curve set
  #define DISPCTRL      0xB6     // display control
  #define TEMPGRADIENT  0xB7     // temp gradient set
  #define TESTMODE9     0xB8     // test mode
  #define REFSET        0xB9     // refresh set
  #define VOLTCTRL      0xBA     // voltage control
  #define COMMONDRV     0xBD     // common driver output select
  #define PWRCTRL       0xBE     // power control

  #define PASET         0x2B      // Page address set
  #define CASET         0x2A      // Column address set
  #define RAMWR         0x2C      // Writing to memory

//   #define DISON     0xAF      // Display on
//   #define DISOFF    0xAE      // Display off
//   #define DISNOR    0xA6      // Normal display
//   #define DISINV    0xA7      // Inverse display
//   #define COMSCN    0xBB      // Common scan direction
//   #define DISCTL    0xCA      // Display control
// //  #define DISCTL    0xBA      // Display control
//   #define SLPIN     0x95      // Sleep in
//   #define SLPOUT    0x94      // Sleep out
//   #define PASET     0x75      // Page address set
//   #define CASET     0x15      // Column address set
//   #define DATCTL    0xBC      // Data scan direction, etc.
//   #define RGBSET8   0xCE      // 256-color position set
//   #define RAMWR     0x5C      // Writing to memory
//   #define RAMRD     0x5D      // Reading from memory
//   #define PTLIN     0xA8      // Partial display in
//   #define PTLOUT    0xA9      // Partial display out
//   #define RMWIN     0xE0      // Read and modify write
//   #define RMWOUT    0xEE      // End
//   #define ASCSET    0xAA      // Area scroll set
//   #define SCSTART   0xAB      // Scroll start set
//   #define OSCON     0xD1      // Internal oscillation on
//   #define OSCOFF    0xD2      // Internal oscillation off
//   #define PWRCTR    0x20      // Power control
//   #define VOLCTR    0x81      // Electronic volume control
//   #define VOLUP     0xD6      // Increment electronic control by 1
//   #define VOLDOWN   0xD7      // Decrement electronic control by 1
//   #define TMPGRD    0x82      // Temperature gradient set
//   #define EPCTIN    0xCD      // Control EEPROM
//   #define EPCOUT    0xCC      // Cancel EEPROM control
//   #define EPMWR     0xFC      // Write into EEPROM
//   #define EPMRD     0xFD      // Read from EEPROM
//   #define EPSRRD1   0x7C      // Read register 1
//   #define EPSRRD2   0x7D      // Read register 2
//   #define NOP       0x25      // NOP instruction






void Backlight(unsigned char state); 
void WriteSpiCommand(unsigned int data); 
void WriteSpiData(unsigned int data); 
void InitLCD(void); 
void LCDWrite130x130bmp(int number); 
void LCDClearScreen(void); 
void LCDSetPixel(int x, int y, int color); 
void LCDSetLine(int x1, int y1, int x2, int y2, int color); 
void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color); 
void LCDSetCircle(int x0, int y0, int radius, int color); 
void LCDPutChar(char c, int x, int y, int size, int fcolor, int bcolor); 
void LCDPutStr(char *pString, int x, int y, int Size, int fColor, int bColor);
void SetContrast(unsigned char contrast);
 void Delaya (unsigned long a);

#ifdef __cplusplus
}
#endif