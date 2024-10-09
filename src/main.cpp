#include <Arduino.h>


void display_centibeats(uint8_t centibeats){



}




int main(void){
  sei();

  while (true){










  }
  return 0;
}

void initTimer0(){
  TCCR0B |= (1<<CS02)|(1<<CS00); //set prescaler 1/1024
  TCCR0A = ();
  
  CLKPR = ();
  TCNT0 = ();
  OCR0A = ();
  OCR0B = ();
  GTCCR = ();


}

void initTimer1()
{
  
}