#include <Arduino.h>
#include <Wire.h>

void initTimer0();
void initTimer1();
void display_centibeats(uint8_t centibeats);

volatile uint8_t centiBeatsCounted = 0;
volatile uint8_t miliseconds = 0;
const int PCF8574_address = 0x21;
uint8_t prevCentiBeat = 0;


ISR(TIMER1_COMPA_vect)
{
    centiBeatsCounted++;
    // display_centibeats(centiBeatsCounted);
}

void display_centibeats(uint8_t centibeats){
  Wire.beginTransmission(PCF8574_address);
  // Wire.write(centibeats);
  Serial.println(centibeats);
  switch (centibeats)
  {
  case 0:
    Wire.write(0xC0);
    break;
  case 1:
    Wire.write(0xF9);
    break;
  case 2:
    Wire.write(0xA4);
    break; 
  case 3:
    Wire.write(0xB0);
    break;
  case 4:
    Wire.write(0x99);
    break;
  case 5:
    Wire.write(0x92); // 5 getest
    break;
  case 6:
    Wire.write(0x82);
    break;
  case 7:
    Wire.write(0xF8);
    break;
  case 8:
    Wire.write(0x80);
    break;
  case 9:
    Wire.write(0x90);
    break;
  case 10:
    Wire.write(0b10001000);
    break;
  case 11:
    Wire.write(0b10000011);
    break;
  case 12:
    Wire.write(0b11000110);
    break;
  case 13:
    Wire.write(0b10100001);
    break;
  case 14:
    Wire.write(0b10000110);
    break;
  case 15:
    Wire.write(0b10001110);
    break;
    
  default:
    Wire.write(0x00);
    break;
  }
  
  Wire.endTransmission();

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
  Wire.begin();
  // Wire.beginTransmission(PCF8574_address);
  // Wire.write(0x02);
  // Wire.endTransmission();
  Serial.begin(9600);
  Serial.write("Ready");
  
  while (true){   
    if (centiBeatsCounted>15)
    {
      centiBeatsCounted = 0;
      prevCentiBeat = 0;
      display_centibeats(centiBeatsCounted);
    }
    if (centiBeatsCounted > prevCentiBeat && (centiBeatsCounted <16))
      {
      display_centibeats(centiBeatsCounted);
      prevCentiBeat = centiBeatsCounted;
      }
    
    








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
