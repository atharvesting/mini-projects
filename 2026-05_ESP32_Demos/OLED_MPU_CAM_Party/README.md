# ESP32_Demo: OLED MPU CAM Party!

This project integrates three components - an OLED screen, MPU 6050 sensor, and the ESP32 Sense Camera OV3360 into a simple but fun demo.

The idea is simple: show MPU readings on the OLED screen, and when the MPU is turned upside down, take a picture and save it to the SD card.

## What I learnt:

- Communication with the MPU over the I2C bus requires individual transmissions. This means that each information transaction must independent.
- Reading from and Writing to registers require specific workflows:
    - Writing:
        1. *Begin transmission and select the device*
        2. *Select the register*
        3. *Write to register*
        4. *End transmission*

        ```ino
        // Example from project
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x6B);  // Set pointer to the PWR_MGMT_1
        Wire.write(0); // Write value 0 into the register
        Wire.endTransmission(true);
        ```
    - Reading:
        1. *Begin transmission and select the device*
        2. *Select the register*
        3. *End transmission without releasing I2C bus (repeated start)*
        4. *Request register*
        5. *Read desired number of bytes starting from address*

        ```ino
        // Example from project
        Wire.beginTransmission(MPU_ADDR);
        Wire.write(0x1C);
        Wire.endTransmission(true);
        Wire.requestFrom(MPU_ADDR, 1);
        uint8_t reg = Wire.read();
        ```
- In order to manipulate specific bits inside bytes in the register map, bit masking is used to isolate the target bits, 
clear them and then update them accordingly.

