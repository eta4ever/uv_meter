#include <TimerOne.h>

// D7 SCLK 
// D8 RCLK 
// D9 DATA

uint16_t rawADC; // для данных АЦП

#define SCLK 7
#define RCLK 8
#define DATA 9
#define SENSINPUT A0
//#define DEBUG_LED 17

const uint8_t digit_pos[4] = 
{ 0b00000001,
  0b00000010,
  0b00000100,
  0b00001000
}; 

const uint8_t digit_val[10] = 
{ 0b11000000,
  0b11111001,
  0b10100100,
  0b10110000,
  0b10011001,
  0b10010010,
  0b10000010,
  0b11111000,
  0b10000000,
  0b10011000
}; 

void push_byte(uint8_t byte2push){
// задвинуть 8 бит в сдвиговый регистр

  digitalWrite(SCLK, LOW);
  digitalWrite(DATA, LOW);

  for (int8_t bit_num=7; bit_num>=0; bit_num--){ //SIGNED!!!!!!!!!!!!!
    digitalWrite(SCLK, LOW);
    digitalWrite(DATA, (byte2push >> bit_num) & 0b1);
    digitalWrite(SCLK, HIGH);
  }
}

void led_digit(uint8_t num, uint8_t val){
// зажечь в заданной позиции заданное число

  digitalWrite(RCLK, LOW);

  push_byte(digit_val[val]);
  push_byte(digit_pos[num]);

  digitalWrite(RCLK, HIGH);
  
}

void led_out(uint16_t val){
  // отобразить четырехзначное число
  
  uint8_t vals[4];
  vals[0] = val % 10;
  vals[1] = (val % 100) / 10;
  vals[2] = (val % 1000) / 100;
  vals[3] = val / 1000;

  for (uint8_t dig_num = 0; dig_num <4; dig_num++){
    led_digit(dig_num, vals[dig_num]);
  }
  
}

void ADC_read(void){
  // получить данные с АЦП
  rawADC = analogRead(SENSINPUT);
}

void setup() {
  
  pinMode(SCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  pinMode(DATA, OUTPUT);
  //pinMode(DEBUG_LED, OUTPUT);

  Timer1.initialize(500000); // период 0.5 с
  Timer1.attachInterrupt(ADC_read);

}


void loop() {

led_out(rawADC);

}
