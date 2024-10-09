#include <Arduino.h>


void display_centibeats(uint8_t centibeats){



}




int main(void){

  while (true){










  }
  return 0;
}

void initTimer0()
{

}

void initTimer1()
// timer1 mode 4, CTC, om bij 13500 timer ticks interupt te gooien en dan te resetten naar 0;
// OCR1A 13500. \n
// OCR1AH = 00110100, OCR1AL = 10111100.
// TOV1 moet high zijn om interrupt flag te enablen dus: \n
// TIFR1 = 00000010;
// WGM13 0, WGM12(CTC1) 1, WGM11(PWM11) 0, WGM10(PWM10) 0
// prescaler 1024 --> CS12 1, CS11 0, CS10 1;
// Set OCIE1A voor output compare A Match interupt enable

{
  TCCR1A |= 00001101;
}