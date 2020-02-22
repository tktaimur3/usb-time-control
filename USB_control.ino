#include <U8g2lib.h>;

//Initialise the OLED screen
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2 (U8G2_R0);

//Initialise all pins
int minutes_pin = 0;
int hours_pin = 1;

int count_pin = 13;
int out_pin = 9;

void setup() {

  //Set all pins as input/output
  pinMode(count_pin, INPUT);
  pinMode(out_pin, OUTPUT);
  digitalWrite(out_pin, LOW);

  //Getting ready to start printing to screen 
  u8g2.begin();

  //Welcome message with font and cursor setup
  u8g2.setFont(u8g2_font_logisoso26_tr);
  u8g2.clearBuffer();
  u8g2.setCursor(0, 32);
  u8g2.print("Welcome");
  u8g2.sendBuffer();
  delay(1000);
}

//Function to format time with or without zeroes depedning on values
String format_time (unsigned long hours, unsigned long minutes ) {
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
String format_time_seconds (unsigned long hours, unsigned long minutes, unsigned long seconds ) {
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

void loop() {

  //Clear screen/buffer every loop
  u8g2.clearBuffer();
  
  //Set new font
  u8g2.setFont(u8g2_font_logisoso32_tr);

  //Set the cursor appropriately to display text
  u8g2.setCursor(0, 32);

  //Every loop, get potentiometer values and map them linearly to a certain range
  unsigned int minutes_pin_value = analogRead(minutes_pin);
  unsigned int hours_pin_value = analogRead(hours_pin);
  unsigned long minutes = ((59*minutes_pin_value)/1023);
  unsigned long hours = ((23*hours_pin_value)/1023);
  unsigned long seconds = 60*(minutes + hours*60);
  String mes = format_time( hours, minutes );

  //Check for button push
  int count_pin_read = digitalRead(count_pin);

  //If not currently counting, show the time that will be counting from
  if (!count_pin_read) {
    
    u8g2.print(mes);
    u8g2.sendBuffer();
    digitalWrite(out_pin, LOW);
    
  } else { 
    //Otherwise, count down

    //Set the font to be smaller
    u8g2.setFont(u8g2_font_logisoso26_tr);

    //Set the timer to the current seconds value of the time picked by the user
    unsigned long timer = seconds;

    //Set formatted time with hours, seconds, and minutes
    String time_remaining = format_time_seconds(timer/3600, timer/60, timer%60); 

    //Loop through time, delaying each loop one second to count per second
    //This loop essentially clears the screen, sets a cursor, displays the time, counts down then repeats
    while ( timer >= 0 ) {
      u8g2.clearBuffer();
      u8g2.setCursor(0, 32);
      time_remaining = format_time_seconds(timer/3600, (timer/60)%60, timer%60);
      u8g2.print(time_remaining);
      u8g2.sendBuffer();
      digitalWrite(out_pin, HIGH);
      timer--;
      delay (1000);
    }

    //After counting, the counter says it's finished to go back to displaying regular time
    count_pin_read = 0;    
  }

  //Delay a little bit before looping for the screen's sake
  delay(50);
}
