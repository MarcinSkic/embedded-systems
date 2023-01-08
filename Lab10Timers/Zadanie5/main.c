#include <targets/AT91SAM7X256.h>


//FREQ = 10Hz
//MCK = 48MHz
//PRESCALER = ?
//REGSIZE = ? (max 65535)

//10Hz = 48 000 000Hz / PRESCALER * REGSIZE
//PRESCALER * REGSIZE = 4 800 000

//Szukanie najmniejszego mozliwego prescalera:
//Dziele wartosc przez najwiekszy mozliwy REGSIZE co daje
//zaokraglajac 73. Czyli PRESCALER musi byc wiekszy lub rowny
//tej wartosci, co oznacza ze najmniejszy z jakiego mozna skorzystac
//to 128 (TIMER4)

//PRESCALER  = 128
//REGSIZE = 37500
int main(void){
  //LCD_BL
  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20;
  PIOB_OWER = 1<<20;

  //TIMER
  PMC_PCER |= 1<<12;
  TC0_CCR = 1<<1;
  TC0_SR;
  TC0_CMR = 0b011 | 1<<14;
  TC0_RC = 37500;
  TC0_CCR = 1<<0 | 1<<2;

  while(1){
    if(TC0_SR & 1<<4){
      PIOB_ODSR ^= 1<<20;
    }
  }
}