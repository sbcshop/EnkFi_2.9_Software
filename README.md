# EnkFi_2.9_Software

<img src= "https://cdn.shopify.com/s/files/1/1217/2104/files/Main_Banner_3.png?v=1729493200" />

The EnkFi series, **powered** by the **ESP32 S3 WROOM**, offers versatile **e-paper display** options in **[2.9”](https://shop.sb-components.co.uk/products/enkfi-epaper-display-board-based-on-esp32-s3-wroom-1?variant=41443707846739), [4.2”](https://shop.sb-components.co.uk/products/enkfi-epaper-display-board-based-on-esp32-s3-wroom-1?variant=41443707879507), [5.83”](https://shop.sb-components.co.uk/products/enkfi-epaper-display-board-based-on-esp32-s3-wroom-1?variant=41443707912275), and [7.5”](https://shop.sb-components.co.uk/products/enkfi-epaper-display-board-based-on-esp32-s3-wroom-1?variant=41443707945043)** sizes, catering to diverse application needs. These displays feature a wide viewing angle of over 170 degrees and utilize bistable technology, ensuring no power consumption when the screen is not refreshed, making them highly energy-efficient. The black-white-red color scheme protects the eyes by eliminating blue light and radiation. With built-in WiFi and BLE capabilities, the EnkFi series is ideal for IoT-based projects, providing robust connectivity.

This GitHub provides getting started instructions to use EnkFi 4.2" E-Paper.

### Features
- EnkFi series powered by **ESP32 S3 WROOM-1**
- **2.9”, 4.2”, 5.83” and 7.5”** E-paper display options available to cater specific applications needs.
- Wide viewing angle **(>170 degrees)** and bistable technology.
- No power usage while the screen is not refreshed, making it energy-efficient.
- **Black-White-Red** color scheme for eye protection - no blue light and no radiation.
- Inbuilt **WiFi/BLE capabilities** makes it perfect choice for IoT based projects
- Both **Serial UART** and **Native(D+/D-)** as Type C interface with Reset and Boot buttons available for programming purpose
- Status LED to indicate board power and battery charging 
- Battery connector and charge management facility for portable use case.
- GPIOs breakout as standard JST-SH-1MM-8 pin connector for additional peripheral interfacing 
- Onboard SDcard connector best for data logging based projects.
- **DS3231 RTC** module to keep track and use for real time capturing prototype.
- There is a buzzer which can be used to add audio alerts for your project.
- Four Programmable buttons for adding extra control features

## Specification:
- **Microcontroller**  : ESP32-S3 series of SoCs having Xtensa® dual-core 32-bit LX7 microprocessor
- **Connectivity** : 2.4 GHz Wi-Fi (802.11 b/g/n) and Bluetooth® 5 (LE)
- **Memory** : Flash up to 16 MB, PSRAM up to 8 MB
- **Board Supply Voltage** : 5V 
- **Operating Pin Voltage** : 3.3V 
- **GPIOs connector** : JST-SH-1MM-8 pin
- **RTC module**: DS3231 
- **Coin Cell Holder** : Supports CR1220 3V Battery
- **Battery Connector** : Supports 3.7V Lithium ion battery  
- **Battery Charge Management** : MCP73831
- **RGB LED** : WS2812B
- **Working Temperature** : 0°C ~ +40°C 
- **E-Paper Display** =>
  - **E-paper Size** : 4.2"
  - **Resolution** : 400X300
  - **Color** : Red, Black and White
  - **Interface** : SPI
  - **Driver** : UC8276


## Getting Started with EnkFi 4.2" Board
### Pinout
<img src= "https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/images/EnkFi_7.5_pinout.png" />

| | | |
|--|--|--|
|(1) Power LED          | (7) Buzzer                      | (13) Boot Button |
|(2) Charging Status    | (8) GPIOs Breakout              | (14) Reset Button |
|(3) Battery Connector  | (9) TF card slot                | (15) Programmable Buttons |
|(4) E-paper connector  | (10) Native USB (USB_D+/USB_D-) | (16) Programmable Buttons  |
|(5) DS3231 RTC         | (11) Serial USB                 | (17) 7.5” E-Paper Display |
|(6) Coin Cell Holder   | (12) ESP32 S3 WROOM-1           | |
  
### Interfacing Details
  
- **_E-Paper Display interfacing with ESP32_**

  | ESP32 | Display | Function |
  |---|---|---|
  | IO12 | DISPLAY_SCK_PIN  | Clock pin of SPI interface for E-Paper Display |
  | IO11 | DISPLAY_MOSI_PIN | MOSI (Master OUT Slave IN) pin of SPI interface|
  | IO13 | DISPLAY_DC_PIN   | Data/Command (MISO) pin of SPI interface |
  | IO10 | DISPLAY_CS_PIN   | Chip Select pin of SPI interface|
  | IO21 | DISPLAY_BUSY_PIN | Busy Status pin of E-Paper |
  | IO14 | DISPLAY_RST_PIN  | Display Reset pin |

 - **_SDcard Interface_**

   | ESP32 | SDCard | Function |
   |---|---|---|
   | IO42 | CARD_CLK | Clock pin of SPI interface for Display|
   | IO2  | CARD_MOSI | MOSI (Master OUT Slave IN) pin of SPI interface|
   | IO41 | CARD_MISO  | MISO (Master IN Slave OUT) pin of SPI interface|
   | IO1  | CARD_CS  | Chip Select pin of SPI interface|

- **_Buzzer and Buttons Interfacing_**

  | ESP32 | Hardware | Function |
  |---|---|---|
  |IO15 | Buzzer |Buzzer Positive Pin |  
  |IO0 | BOOT |Boot button |
  |IO4 | BT1 | Programmable Button |
  |IO5 | BT2 | Programmable Button |
  |IO6 | BT3 | Programmable Button |
  |IO7 | BT3 | Programmable Button |

- **_DS3231 RTC interfacing with ESP32 using I2C_** 

  | ESP32 | DS3231 | Description | 
  |---|---|---|
  | IO38 | SDA | I2C Serial Data Pin |
  | IO39 | SCL | I2C Serial Clock Pin |

- **_GPIOs Breakout_**

  | ESP32 | Type* | Multi-Function (_**Bold-Italic**_ default Function) |
  |---|---|---|    
  |3V3  | P     | Positive Supply, 3.3V |
  |5V   | P     | Positive Supply, 5V |
  |GND  | P     | Supply Ground |
  |TX   | I/O/T | RTC_GPIO17,**_GPIO17_**,U1TXD,ADC2_CH6 |
  |RX   | I/O/T | RTC_GPIO18,**_GPIO18_**,U1RXD,ADC2_CH7,CLK_OUT3 |
  |IO8  | I/O/T | RTC_GPIO8, _**GPIO8**_ , TOUCH8, ADC1_CH7, SUBSPICS1  |  
  |IO9  | I/O/T | RTC_GPIO9,**_GPIO9_**,TOUCH9,ADC1_CH8,FSPIHD,SUBSPIHD |
  |IO16 | I/O/T | RTC_GPIO16,**_GPIO16_**,U0CTS,ADC2_CH5,XTAL_32K_N |
  |IO40 | I/O/T | MTDO,**_GPIO40_**,CLK_OUT2 |

  *I-INPUT, O-OUTPUT, P-POWER & T-HIGH IMPEDENCE
  
### 1. Configure and Setup Development Environment
   - Download Arduino IDE from [official site](https://www.arduino.cc/en/software) and install into your system. We have use Arduino IDE 1.8.19
   - Once installation done will add ESP32 S3 board support into IDE, for this first you need to add below link into preference:
     
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
     
     Select File > Preference, and add link as show in below image,
      <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/preference_board.gif" />
      
   - Now will install ESP32 based boards as shown in below image,

     <img src= "https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/install_ESP32boards.gif" />
   
   - You have two options to program onboard ESP32 of EnkFi board, **_UART USB_** OR **_Native USB_**.

     <img src="https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/images/usb_options.png" width="372" height="214">   
   
   - When using Native ESP USB, you will have to press BOOT button once and then connect Type C. For UART USB no need, directly connect USB.
   
   - Once done (for UART USB), keeping default settings select the ESP32S3 Dev Module with suitable COM PORT (may be different in your case) as shown below,

     <img src="https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/select_esp32_with_comport.gif">

     You can view assigned COM port through Device Manager,

     <img src="https://github.com/sbcshop/2x2_Display_ESP32_Software/blob/main/images/UART_com_port.jpg" width="582" height="421">
     
   - When using USB native you will get COM PORT as shown in below image, and while uploading you can enable CDC Mode to visualize data on serial com port.
     
     <img src="https://github.com/sbcshop/2x2_Display_ESP32_Software/blob/main/images/Native_USB_device_com_port.jpg" width="410" height="93">
     
     <img src="https://github.com/sbcshop/2x2_Display_ESP32_Software/blob/main/images/Native_USB_Arduino_com.jpg" width="" height="">
     
        
### 2. Installing Libraries
   - When compiling sample codes there are some dependency on external libraries sometime which you can add as shown here.
   - RTC library for testing DS3231, select Sketch > Include Library > Manage Libraries. Install RTClib version 2.1.4,

     <img src= "https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/images/Lib_install.png" />

     <img src= "https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/images/RTClib_install_1.png" />

   - Similarly you can add more libraries if needed, make sure to install correct version. 


### 3. Testing First Code
   - At this step you are all set to test codes, for easy getting started we have provided various demo [example codes](https://github.com/sbcshop/EnkFi_7.5_Software/tree/main/examples) in github which you can download and try. 
   - Open one example code in Arduino and make sure you have selected correct board with suitable com port, click on upload button to transfer code on EnkFi board.
     <img src="https://github.com/sbcshop/3.2_Touchsy_ESP-32_Resistive_Software/blob/main/images/upload_process.gif">
   - Checkout other examples below and build your own custom program codes using those references.

### Example Codes
   - [Example 1](https://github.com/sbcshop/EnkFi_2.9_Software/tree/main/examples/Demo_Buzzer_Button) : Onboard Button and Buzzer Testing 
   - [Example 2](https://github.com/sbcshop/EnkFi_2.9_Software/tree/main/examples/Demo_GraphicsDisplay) : Create Graphics like line, circle, etc. on EnkFi E-Paper display
   - [Example 3](https://github.com/sbcshop/EnkFi_2.9_Software/tree/main/examples/Demo_ImageDisplay) : Image display demo, follow instructions [here](https://github.com/sbcshop/EnkFi_7.5_Software/blob/main/documents/Image%20to%20Arduino%20Byte%20Conversion.pdf) to create your own array image to use with code.   
   - and [Many more...](https://github.com/sbcshop/EnkFi_2.9_Software/tree/main/examples)

   Now you are ready to try out your own codes, **_Happy Coding!_**

## Resources
  * [Schematic](https://github.com/sbcshop/EnkFi_2.9_Hardware/blob/main/Design%20Data/EnkFi%207.5%20Sch.PDF)
  * [Hardware Files](https://github.com/sbcshop/EnkFi_2.9_Hardware)
  * [Step File](https://github.com/sbcshop/EnkFi_2.9_Hardware/blob/main/Mechanical%20Data/Enkfi_7in5.STEP)
  * [Getting Started with ESP32 in Arduino](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
  * [ESP32 S3 Hardware Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/hw-reference/index.html)
  * [ESP32 S3 Datasheet](https://github.com/sbcshop/3.2_Touchsy_ESP-32_Capacitive_Software/blob/main/documents/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf)
  * [Arduino IDE 1 overview](https://docs.arduino.cc/software/ide-v1/tutorials/Environment)
       

## Related Products  
  
  * [EnkPi Series E-Paper - 2.9", 4.2", 5.83" & 7.5"](https://shop.sb-components.co.uk/products/enkpi?_pos=1&_psq=enkpi&_ss=e&_v=1.0)

    ![EnkPi Series](https://shop.sb-components.co.uk/cdn/shop/products/3_aa603116-9e0f-4af5-a42e-14273f8fc865.png?v=1683625804&width=150)

  * [2.7" E-paper HAT for Raspberry Pi](https://shop.sb-components.co.uk/products/2-7inch-e-paper-hat-board)

    ![2.7" E-paper Hat](https://shop.sb-components.co.uk/cdn/shop/products/2.7-inche-paperHATforRaspberryPi.png?v=1670333988&width=150)

  * [Universal E-Paper HAT for Pico](https://shop.sb-components.co.uk/products/universal-e-paper-hat-for-pico)

    ![Universal E-Paper HAT for Pico](https://shop.sb-components.co.uk/cdn/shop/products/5cd1c711f78614141d3566a656c3e04b_original.png?v=1672636474&width=150)

  * [1.54" E-Paper Breakout](https://shop.sb-components.co.uk/products/1-54-inch-e-paper-breakout)

    ![1.54" E-Paper Breakout](https://shop.sb-components.co.uk/cdn/shop/products/01_9f49bed0-cb5e-4a14-bd4c-3f79ab7a9b88.png?v=1677068419&width=150)
  
  * [E-Paper Display](https://shop.sb-components.co.uk/products/e-paper-display)

    ![E-Paper Display](https://shop.sb-components.co.uk/cdn/shop/files/01_d513b796-f1b2-4929-ae1f-4fc4282b42d2.jpg?v=1683613062&width=150)


## Product License

This is ***open source*** product. Kindly check LICENSE.md file for more information.

Please contact support@sb-components.co.uk for technical support.
<p align="center">
  <img width="360" height="100" src="https://cdn.shopify.com/s/files/1/1217/2104/files/Logo_sb_component_3.png?v=1666086771&width=300">
</p>
