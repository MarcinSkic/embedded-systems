#include <targets/AT91SAM7X256.h>

void example2(){
  PMC_PCER = 1<<3 | 1<<14;

  PIOB_PER = 1<<11;
  PIOB_OER = 1<<11;
  PIOB_OWER = 1<<11;

  TC0_CCR = 1<<1;
  TC0_SR;
  TC0_CMR = 0b100 | 1<<14;  //To nie ta sama 14 co powyzej
  TC0_RC = 62500;
  TC0_CCR = 1<<0 | 1<<2;

  while(1){
    if(TC0_SR & 1<<4){
      PIOB_ODSR ^= 1<<11;
    }
  }
}

//To rozwiazanie nie jest jedynym mozliwym, mozna wybrac inne kombinacje liczb niz
//ktore daja wymnozone 15 000 000 ((999 999 + 1) * 15). Ogranicza nas pojemnosc 
//CPIV ktora wynosi 1 048 575 i PICNT ktora wynosi 4095
void ex2(){
  PMC_PCER = 1<<3;

  PIT_PIVR;
  PIT_MR = 999999 | 1<<24;

  while(1){
    if(PIT_PIIR >= (15 << 20)){
      PIT_PIVR;
      //Do things
    }
  }
}

void ex5(){
  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20;
  PIOB_OWER = 1<<20;

  PMC_PCER |= 1<<12;
  TC0_CCR = 1<<1;
  TC0_SR;
  TC0_CMR = 0b011 | 1<<14;
  TC0_CCR = 1<<0 | 1<<2;

  while(1){
    if((TC0_SR & 1<<4) != 0){
      PIOB_ODSR ^= 1<<20;
    }
  }
}

int main(void){
  

  PMC_PCER = 1<<3;
  PIOB_PER = 1<<20;
  PIOB_OER = 1<<20; //DDR

  PIOB_SODR = 1<<20; //PORT

  PMC_PCER |= 1<<2;
  PIOA_PER = 1<<8;
  PIOA_ODR = 1<<8;

  if((PIOA_PDSR & 1<<8) == 0){
    //Rob rzeczy
  }
}

//------TŁUMACZENIE RÓŻNYCH PRZYKŁADÓW = MATMA + KOD---------------
/*
1s = (PRESCALER * WIELKOSC REJESTRU) / MCK (Czestotliwosc procesora)
//PIT
PRESCALER = 16
WIELKOSC REJESTRU = (CPIV = 1 048 575) * (PICNT = 4095)
MCK = tresc zadania

MCK = 8MHz, 10s
PIT_PIVR;
PIT_MR = 1 000 000 | 1<<24;

if(PIT_PIIR >= 5<<20){
  PIT_PIVR;
}


//TIMER
PRESCALER = 2/8/32/128/1024
WIELKOSC REJESTRU = 65535
MCK = tresc zadania

//Example 2
MCK = 64MHz
//Task
1s MCK = 8MHz
PR = 128
RC = 62500

1s = (PR * POJ)/8 000 000
8 000 000 = PR * POJ

PMC_PCER = 1<<12
TC0_CCR = 1<<1;
TC0_SR;
TC0_CMR = 0b011 | 1<<14;
TC0_RC = 62500;
TC0_CCR |= 1<<0 | 1<<2;

if(TC0_SR & 1<<4){
    //rob rzeczy
}

MCK 48MHz 5s PRSC = 16

CPIV * PCINT = ?

(16 * SIZE) / 48 000 000Hz = SIZE / 3 000 000
SIZE = 15 000 000

16 * 1 000 000 * PCINT / 48 000 000 = PCINT/3
PCINT = 15

CPIV = 1 000 000
PCINT = 15

//KOD 
PIT_PIVR;
PIT_MR = 999999 | 1<<24;

if(PIT_PIIR & PIT_PIIR_PICNT_MASK == 15<<PIT_PIIR_PICNT_BIT){
  PIT_PIVR;
}


0.1s = PRESCALER * WIELKOSC REJESTRU / MCK

10Hz = MCK/ PRESCALER * WIELKOSC REJESTRU

10Hz = 48 000 000 / PRSC * RC
PRSC = 128
RC = 37500
*/