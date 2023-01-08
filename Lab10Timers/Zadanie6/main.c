#include <targets/AT91SAM7X256.h>

void timer_counter0_conf(int PRESCALER2, int PRESCALER1, int PRESCALER0, int doCompare){
  //TIMER
  PMC_PCER |= 1<<12;
  TC0_CCR = 1<<1;
  TC0_SR;
  TC0_CMR = PRESCALER2<<2 | PRESCALER1<<1 | PRESCALER0 << 0 | doCompare<<14;
  TC0_RC = 37500;
  TC0_CCR = 1<<0 | 1<<2;
}

int main(void){
  //LCD_BL
  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20;
  PIOB_OWER = 1<<20;

  timer_counter0_conf(0,1,1,1);

  while(1){
    if(TC0_SR & 1<<4){
      PIOB_ODSR ^= 1<<20;
    }
  }
}