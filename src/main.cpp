#include <Arduino.h>

void initTimer0();
void initTimer1();

volatile uint8_t centiBeatsCounted = 0;

ISR(TIMER1_COMPA_vect)
{
    centiBeatsCounted++;
    Serial.write(centiBeatsCounted);
}

void display_centibeats(uint8_t centibeats){


}




int main(void){
  initTimer0();
  initTimer1();

  sei();
  Serial.begin(9600);
  Serial.write("Ready");
  
  while (true){

  }
  return 0;
}

void initTimer0(){ // setup Timer 0 voor delay van 15 miliseconden
  TCCR0A = (1<<WGM01); // zet CTC mode aan voor timer interupt
  TCCR0B |= (1<<CS02)|(1<<CS00); // zet prescaler 1/1024

  //bepaalt wanneer de interupt moet afgaan? welke van de twee?
  // OCR0A = (); 
  // TCNT0 = ();


}

void initTimer1()
// timer1 mode 4, CTC, om bij 13500 timer ticks interupt te gooien en dan te resetten naar 0;
// OCR1A 13500. \n
// OCR1AH = 00110100, OCR1AL = 10111100.
// TOV1 moet high zijn om interrupt flag te enablen dus: \n
// TIFR1 = 00000010;
// WGM13 0, WGM12(CTC1) 1, WGM11(PWM11) 0, WGM10(PWM10) 0
// prescaler 1024 --> CS12 1, CS11 0, CS10 1;
// Set OCIE1A voor output compare A Match interupt enable door OCF1A in te stellen
// TCCR1A moest alles op 0 staan, wat al default is.
{
  TCCR1B |= (1<<WGM12)|(1<<CS12)|(0<<CS11)|(1<<CS10);
  OCR1A = 13500;
  TIMSK1 |= (1<<OCIE1A);
  TIFR1 |= (1<<OCF1A);
}
// INTERUPT AANZETTEN