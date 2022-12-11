#include <targets/AT91SAM7.h>

void delayMs(int ms){
  volatile int a,b;
  for(a=0;a<=ms;a++){
    for(b=0;b<=3000;b++){
      __asm__("NOP");
    }
  }
}

void delay(int ms){
  volatile int a,b;
  for(a=0;a<=ms;a++){
    for(b=0;b<=100;b++){
      __asm__("NOP");
    }
  }
}


void generateSoundTone(int upTone,int downTone, int ms){
  for(int i = 0; i < ms*20; i++){
    PIOB_SODR = 1<<19;
    delay(upTone);
    PIOB_CODR = 1<<19;
    delay(downTone);
  }
}

void weWillRockYou(){
  
  generateSoundTone(10,20,10);
  delayMs(100);
  generateSoundTone(10,20,10);
  delayMs(100);
  generateSoundTone(15,30,6);
  delayMs(300);
}

void mario(){
  generateSoundTone(8,6,10);  
  delayMs(50);
  generateSoundTone(8,6,10);  
  delayMs(100);
  generateSoundTone(8,6,10);  
  delayMs(100);
  generateSoundTone(7,6,10);  
  delayMs(50);
  generateSoundTone(8,6,10);  
  delayMs(100);
  generateSoundTone(7,8,30);  
}

int main(){
  
  PMC_PCER = 1<<3;

  PIOB_PER = 1<<19;
  PIOB_PER |= 1<<20;
  PIOB_PER |= 1<<24;
  PIOB_OER = 1<<19;
  PIOB_OER |= 1<<20;
  PIOB_ODR = 1<<24;

  //Joystick
  PMC_PCER = 1<<2;


  while(1){
    if((PIOB_PDSR & 1<<24) == 0){
      mario();
    }
  }
}