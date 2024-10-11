#include <Arduino.h>
#include <Wire.h>

void initTimer0();
void initTimer1();


enum bstate {pressed, released}; //maakt twee states aan die de knop kan hebben
enum bstate button_state(void); // geeft aan dat de functie er nog aan komt na de main
enum bstate lastState = released;  //houdt de laatste status bij

volatile uint8_t centiBeatsCounted = 0;
volatile uint8_t miliseconds = 0;
const int PCF8574_address = 0x21;
uint8_t prevCentiBeat = 0;
const uint16_t totalTimer1Ticks = 13500;


ISR(TIMER1_COMPA_vect)
{
    centiBeatsCounted++;
    // display_centibeats(centiBeatsCounted);
    // PORTB |= (1<<PB5);
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
    Wire.write(0x88);
    break;
  case 11:
    Wire.write(0x83);
    break;
  case 12:
    Wire.write(0xC6);
    break;
  case 13:
    Wire.write(0xA1);
    break;
  case 14:
    Wire.write(0x86);
    break;
  case 15:
    Wire.write(0x8E);
    break;
    
  default:
    Wire.write(0x00);
    break;
  }
  
  Wire.endTransmission();

}

ISR(INT0_vect) {
    // Check of de knop is ingedrukt of losgelaten
    if (!(PIND & (1 << PD2))) {  // Knop ingedrukt (logic 0)
        lastState = pressed;
    } else {  // Knop losgelaten (logic 1)
        lastState = released;
    }

    // Zet INT0 tijdelijk uit om ruis te voorkomen
    EIMSK &= ~(1 << INT0);

    // Start Timer0 voor debounce delay
    TCCR0B |= (1 << CS02) | (1 << CS00);  // Start Timer0 met prescaler 1024
}


ISR(TIMER0_COMPA_vect) {
    // Stop Timer0
    TCCR0B &= ~((1 << CS02) | (1 << CS00));  // Stop de timer
    TCNT0 = 0;  // Reset de timer teller
    
    // Zet INT0 weer aan voor nieuwe knopdetectie
    EIFR |= (1 << INTF0);  // Reset INT0 interrupt vlag
    EIMSK |= (1 << INT0);  // Schakel INT0 weer in
}

int main(void){
  DDRD &= ~(1 << PD2);  // Zet PD2 als input
  PORTD |= (1 << PD2);  // Zet de interne pull-up weerstand aan

  // Zet INT0 op zowel falling als rising edge
  EICRA |= (1 << ISC00);  // Zet interrupt bij elke edge
  EIMSK |= (1 << INT0);   // Zet INT0 interrupt aan

  // Initialiseer Timers
  initTimer0();
  initTimer1();

  sei();
  Wire.begin();
  Serial.begin(9600);
  Serial.write("Ready");
  
  while (true){   
    if (lastState == pressed) {
    //   PORTB |= (1 << PB5);  // led aan als knop is ingedrukt
        Serial.write("last state pressed");
      if (centiBeatsCounted>15) {
      centiBeatsCounted = 0;
      prevCentiBeat = 0;
      display_centibeats(centiBeatsCounted);
      }
      if (centiBeatsCounted > prevCentiBeat && (centiBeatsCounted <16)) {
      display_centibeats(centiBeatsCounted);
      prevCentiBeat = centiBeatsCounted;
      }
    } else if (lastState == released) {
      PORTB &= ~(1 << PB5);  // led uit als knop is losgelaten
      }
  }
  return 0;
}

void initTimer0(void){ // setup Timer 0 voor delay van 15 miliseconden
  TCCR0A = (1<<WGM01); // zet CTC mode aan voor timer interupt
  OCR0A = 233; // reset timer wanneer opgegeven waarde berreikt is (15 ms)
  TCNT0 = 0; // reset timer counter
  TIMSK0 |= (1<<OCIE0A); // Enabled matching van de TCCR0A

  }

void initTimer1()
// timer1 mode 4, CTC, om bij 13500 timer ticks interupt te gooien en dan te resetten naar 0;
// OCR1A 13500. 
// OCR1AH = 00110100, OCR1AL = 10111100.
// WGM13 0, WGM12(CTC1) 1, WGM11(PWM11) 0, WGM10(PWM10) 0
// prescaler 1024 --> CS12 1, CS11 0, CS10 1;
// Set OCIE1A voor output compare A Match interupt enable
// TCCR1A moest alles op 0 staan, wat al default is.
{
  TCCR1B |= (1<<WGM12)|(1<<CS12)|(0<<CS11)|(1<<CS10);
  OCR1A = totalTimer1Ticks;
  TIMSK1 |= (1<<OCIE1A);
  // TIFR1 |= (1<<OCF1A); geen flag nodig omdat er al een interupt wordt gegooid 
}
