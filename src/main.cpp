#include <Arduino.h>
#include <Wire.h>

void initTimer0();
void initTimer1();

volatile uint8_t centiBeatsCounted = 0;
volatile uint8_t miliseconds = 0;

ISR(TIMER1_COMPA_vect)
{
    centiBeatsCounted++;
    PORTB ^= (1<<PB5);
}

void display_centibeats(uint8_t centibeats){



}

ISR(TIMER0_COMPA_vect){ // functie word uitgevoerd wanneer timer0 interrupt afgaat
  miliseconds++; // testcode. Mag Weg
  PORTB |= (1<<PB5); // testcode. mag weg
}




int main(void){
  initTimer0();
  initTimer1();
  DDRB |= (1<<PB5);

  sei();
  Serial.begin(9600);
  Serial.write("Ready");
  
  while (true){









  }
  return 0;
}

void initTimer0(void){ // setup Timer 0 voor delay van 15 miliseconden
  TCCR0A = (1<<WGM01); // zet CTC mode aan voor timer interupt
  TCCR0B |= (1<<CS02)|(1<<CS00); // zet prescaler op 1024
  OCR0A = 233; // reset timer wanneer opgegeven waarde berreikt is ipv overflow (255)
  TCNT0 = 0; // Zet timer register op 0
  TIMSK0 = (1<<OCIE0A); // Enabled matching van de TCCR0A
  }

void initTimer1()
// timer1 mode 4, CTC, om bij 13500 timer ticks interupt te gooien en dan te resetten naar 0;
// OCR1A 13500. \n
// OCR1AH = 00110100, OCR1AL = 10111100.
// WGM13 0, WGM12(CTC1) 1, WGM11(PWM11) 0, WGM10(PWM10) 0
// prescaler 1024 --> CS12 1, CS11 0, CS10 1;
// Set OCIE1A voor output compare A Match interupt enable
// TCCR1A moest alles op 0 staan, wat al default is.
{
  TCCR1B |= (1<<WGM12)|(1<<CS12)|(0<<CS11)|(1<<CS10);
  OCR1A = 13500;
  TIMSK1 |= (1<<OCIE1A);
  // TIFR1 |= (1<<OCF1A); geen flag nodig omdat er al een interupt wordt gegooid 
}
