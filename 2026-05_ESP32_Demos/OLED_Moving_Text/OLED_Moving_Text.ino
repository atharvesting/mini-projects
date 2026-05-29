#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int16_t i = 0;
String my_string = "MoViNg";
int16_t string_length = 15 * my_string.length();

void setup() {
  Serial.begin(115200);
  Wire.begin(4, 2);
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }
  oled.clearDisplay();
  oled.setTextSize(3);
  oled.setTextColor(SSD1306_WHITE);
}

void loop() {
  oled.clearDisplay();
  oled.setCursor(i, 10);
  oled.print(my_string);
  oled.display();
  i = i + 1;
  if (i > SCREEN_WIDTH) i = -string_length;
}
