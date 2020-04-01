#include <U8g2lib.h>;

#define MASK(x) ((unsigned char) 1<<x)

//Initialise the OLED screen
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2 (U8G2_R0);

//Initialise all pins as unsigned char
unsigned char minutes_pin = 0;  //PC0/ADC0 analog pin 0
unsigned char hours_pin = 1;    //PC1/ADC1 analog pin 1

unsigned char count_pin = 13;   //PB5
unsigned char out_pin = 9;      //PB1

void setup() {

  //Set pins as input/output - 0 is input and 1 is output
  DDRB = ~MASK(5); //0b11011111

  //Set PB1 (digital pin 9) to LOW
  PORTB = 0;

  //Set up ADMUX register
  //0b01000000 - is what the ADMUX is set to, it sets ref voltage to Vcc = 5V
  ADMUX = 1<<REFS0;

  //0b10000111 - the ADCSRA register is set to this
  //It sets prescalar to 128 and enables the ADC
  ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  
  //Getting ready to start printing to screen
  u8g2.begin();

  //Welcome message with font and cursor setup
  u8g2.setFont(u8g2_font_logisoso26_tr);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 32);
  u8g2.print("Welcome");
  u8g2.sendBuffer();

  Serial.begin(9600);
  delay(1000);
}

//Function to format time with or without zeroes depedning on values
String format_time ( uint8_t hours, uint8_t minutes ) {
  String h = String( hours );
  String m = ":" + String( minutes );

  if ( hours < 10 ) {
    h = "0" + String( hours );
  }

  if ( minutes < 10 ) {
    m = ":0" + String( minutes );
  }

  return h + m;
}

//Function to format time but includes seconds
String format_time_seconds ( uint32_t hours, uint32_t minutes, uint32_t seconds ) {
  String h = String( hours );
  String m = ":" + String( minutes );
  String s = ":" + String( seconds );

  if ( hours < 10 ) {
    h = "0" + String( hours );
  }

  if ( minutes < 10 ) {
    m = ":0" + String( minutes );
  }

  if ( seconds < 10 ) {
    s = ":0" + String( seconds );
  }

  return h + m + s;
}

uint16_t a_read ( unsigned char channel ) {
  //Safety feature since only 8 channels
  if (channel > 7)
    return 0;

  //Select which ADC port through ADMUX register
  ADMUX = (1<<REFS0) | channel;

  //Enable conversion through enabling ADSC (start conversion) bit 
  ADCSRA |= MASK(6);

  //ADIF (interrupt) bit will become 1 when conversion finishes
  //So do nothing while this is a zero
  while ( !( ADCSRA & MASK(4) ) ) {
    
  }

  return ADC;
}

void loop() {

  //Clear screen/buffer every loop
  u8g2.clearBuffer();

  //Set new font
  u8g2.setFont(u8g2_font_logisoso32_tr);

  //Set the cursor appropriately to display text
  u8g2.setCursor(0, 32);

  //Every loop, get potentiometer values and map them linearly to a certain range
  uint16_t minutes_pin_value = a_read(minutes_pin);
  uint16_t hours_pin_value = a_read(hours_pin);
  uint16_t minutes = ((59 * minutes_pin_value) / 1023);
  uint16_t hours = ((23 * hours_pin_value) / 1023);
  uint32_t seconds = 60 * ((uint32_t)minutes + (uint32_t)hours * 60);
  String mes = format_time( hours, minutes );

  //Check for button push checking PINB register
  unsigned char count_pin_read = PINB & MASK(5);

  //If not currently counting, show the time that will be counting from
  if (!count_pin_read) {

    u8g2.print(mes);
    u8g2.sendBuffer();
    PORTB = 0;
    
  } else {
    //Otherwise, count down

    //Set the font to be smaller
    u8g2.setFont(u8g2_font_logisoso26_tr);

    //Set the timer to the current seconds value of the time picked by the user
    uint32_t timer = seconds;

    //Set formatted time with hours, seconds, and minutes
    String time_remaining = format_time_seconds(timer / 3600, timer / 60, timer % 60);

    //Loop through time, delaying each loop one second to count per second
    //This loop essentially clears the screen, sets a cursor, displays the time, counts down then repeats
    while ( timer != 0 ) {
      u8g2.clearBuffer();
      u8g2.setCursor(0, 32);
      time_remaining = format_time_seconds(timer / 3600, (timer / 60) % 60, timer % 60);
      u8g2.print(time_remaining);
      u8g2.sendBuffer();
      PORTB |= MASK(1);
      timer--;
      delay (1000);
    }

    //After counting, the counter says it's finished to go back to displaying regular time
    count_pin_read = 0;
  }

  //Delay a little bit before looping for the screen's sake
  delay(50);
}
