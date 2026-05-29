// MPU docs: https://d17t6iyxenbwp1.cloudfront.net/s3fs-public/2026-04/rm-mpu-6000a-00-mpu-6000-register-map.pdf?VersionId=jEYAYvYOKeDIa.5IJgZPIlWgNTRCs.9m
// Many camera and SD card functions courtesy of: https://www.youtube.com/watch?v=qNzlytUdB_Q&pp=ygUUZXNwMzIgc2Vuc2Ugd29ya3Nob3A%3D

#include <Arduino.h>
#include <esp_camera.h>   // Espressif's custom dirver for handling DVP and SCCB protocols to extract frames for the camera      
#include <FS.h>           // File system
#include <SD.h>           // MicroSD Card
#include <SPI.h>          // Interface for MicroSD card
#include <I2S.h> 
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

bool camera_status = false;
bool sd_status = false;

int fileCount = 1;

void writeFile(fs::FS &fs, const char *path, uint8_t *data, size_t len) {
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("FAiled to open file for writing");
    return;
  }
  if (file.write(data, len) == len) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void photo_save(const char *filename) {
  camera_fb_t *fb = esp_camera_fb_get(); // Returns a pointer to the frame buffer of the camera after freezing the buffer
  if (!fb) {
    Serial.println("Failed to get camera frame buffer");
    return;
  }
  writeFile(SD, filename, fb->buf, fb->len);
  esp_camera_fb_return(fb); // Essential memory management step that unlocks the buffer to record new data
  Serial.println("Photo saved to file");
}

// Check source on top
void CameraParameters() {
  // Define camera parameters
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
 
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 20;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }
 
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int MPU_ADDR = 0x68;

bool was_upside_down = false;  // to used for checking whether the MPU was upside down for more than a second

// numeric to string function: https://www.youtube.com/watch?v=wTfSfhjhAU0
char tmp_str[7];

char* int16_to_string(int16_t n) {
  sprintf(tmp_str, "%6d", n);
  return tmp_str;
}

void setup() 
{
  
  Serial.begin(115200);
  while (!Serial);  // Wait until port is ready/physical connection is established

  I2S.setAllPins(-1, 42, 41, -1, -1);  // Sets pins for the SD card connection
  Wire.begin(4, 2);

  CameraParameters();
  camera_status = true;
  Serial.println("Camera OK!");

  if (!SD.begin(21)) {
    Serial.println("Failed to mount MicroSD card!");
    while (1);
  }

  sd_status = true;
  
  // A begin- and endTransmission pair must wrap every single Wire.write() commands.
  Wire.beginTransmission(MPU_ADDR); // Select the device before I send any information to it
  Wire.write(0x6B);  // Set pointer to the PWR_MGMT_1
  Wire.write(0); // Write value 0 into the register in order to wake the module up
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.endTransmission(true);

  Wire.requestFrom(MPU_ADDR, 1);
  uint8_t reg = Wire.read();
  reg &= ~(0b11 << 3); // creating a mask
  reg |= 0b00 << 3; // 00 for 2G | 01 for 4G | 10 for 8G | 11 for 16G
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x1C);
  Wire.write(reg);
  Wire.endTransmission();

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Connection to OLED unsuccessful!");
    while (1);
  }

  oled.clearDisplay();
  oled.setTextSize(1);
  oled.setTextColor(SSD1306_WHITE);

  delay(100);
}

void loop() 
{
  if (camera_status && sd_status) {
  
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);  // Set the internal read pointer of the device to register 0x3B
    Wire.endTransmission(false);

    Wire.requestFrom(MPU_ADDR, 14, true); // 14 bytes starting from the internal read pointer we just set.

    int16_t accel_x = (Wire.read() << 8) | Wire.read();
    int16_t accel_y = (Wire.read() << 8) | Wire.read();
    int16_t accel_z = (Wire.read() << 8) | Wire.read();

    int16_t reg_temp = (Wire.read() << 8) | Wire.read();
    float temp_celcius = (reg_temp / 340.0) + 36.53;

    int16_t gyro_x = (Wire.read() << 8) | Wire.read();
    int16_t gyro_y = (Wire.read() << 8) | Wire.read();
    int16_t gyro_z = (Wire.read() << 8) | Wire.read();

    oled.clearDisplay();
    oled.setCursor(0,0);
    oled.print("aX=");
    oled.print(int16_to_string(accel_x));
    oled.print("\naZ=");
    oled.print(int16_to_string(accel_z));
    oled.print("\nTemp=");
    oled.print(temp_celcius);
    oled.display();
    
    bool upside_down = accel_z < -12000;

    if (upside_down && was_upside_down) {
      Serial.println("Say cheese!");
      char imageFileName[32];
      sprintf(imageFileName, "/cheese%d.jpg", fileCount);
      photo_save(imageFileName);
      Serial.printf("Saving picture: %s\r\n", imageFileName);
      fileCount++;
      delay(5000);
    }

    was_upside_down = upside_down;

    delay(50);
  }
}