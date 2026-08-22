#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define TRIG 5
#define ECHO 6

int i = 0;
// float distance = 1;

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("BOOTED");

  Wire.begin(4, 2);

  pinMode(TRIG, OUTPUT);   // 5 - Trigger is the output
  pinMode(ECHO, INPUT);    // 6 - Echo is the input

  if (!oled.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED couldn't be activated.");
    while(1);
  }
  oled.clearDisplay();
  oled.setTextSize(2);
  oled.setTextColor(SSD1306_WHITE);
  Serial.println("Hello working Hello!\n");
  
}

void loop() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  unsigned long duration = pulseIn(ECHO, HIGH, 30000);

  float speed = 343e-4;
  float distance = (duration * speed) / 2.0F;
  
  oled.clearDisplay();
  oled.setCursor(2, 10);
  oled.printf("%.1f cm", distance);
  oled.display();
  delay(200);
}