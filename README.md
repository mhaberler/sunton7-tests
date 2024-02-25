## The Sunton ESP32-8048S070 Board Reverse Engineered

Documentation of this board is abysmal. The best source I coud find is [here](http://specs.buho.ch/MCU/ESP32-8048S070/) ([mirror](https://static.mah.priv.at/public/ESP32-8048S070/)).

Some information can be found in the [Makerfabs Wiki](https://wiki.makerfabs.com/Sunton_ESP32_S3_5_7_inch_IPS_with_Touch.html).

A [nice example](https://github.com/HarryVienna/Makerfabs-Sunton-ESP32-S3-7-Display-with-LovyanGFX-and-LVGL) is here, [blogpost is here](https://www.haraldkreuzer.net/en/news/getting-started-sunton-esp32-s3-7-inch-display-lovyangfx-and-lvgl).

## Summary:

There are 2 I2C buses, 1 SPI bus and 1 I2S bus:

* SDA1=IO19 SCL1=IO20  - Touch controller, also on P3
* SDA0=IO17 SCL=IO18 - external connectors P4, P5
* MISO=IO13 MOSI=IO11 SCLK=12 - TF card and external connector P2
* DIN=IO1

The role of IO19 on P2 is unclear - seems to be SDA of the touch controller (?) - edit: confirmed with logic analyzer, so yes

AFAICT there is no freeGPIO pin beyond these buses.

By just using I2C1 (touch controller) one could disable I2C0 and use IO17 and IO18 as GPIO pins.

This means the SPI connector cannot be used since a second chip select pin is not available.

## P1 - next to USB

The RX and TX pins are the 3.3V versions of the console serial pins and also connect ot the CH340C serial/USB converter pins TXD and RXD - so still the console port as TTL signal, not a separate UART.

![](assets/p1_near_USB.png)

## P2 - label: SPI

HC-1.25-4PWT 1.25mm SMD connector

IO19 is labeled USB-PW R-DM; since USB/JTAG is not used, this pin seems to be free for SPI device select or other purposes

| Pin  | Signal<br />name | Used for | Code<br />example | source                                                                                                             | Purpose   |
| ---- | ---------------- | -------- | ----------------- | ------------------------------------------------------------------------------------------------------------------ | --------- |
| IO13 | MCU_MISO         | SD_D0    | SDcard            | [ESP32-8048S070-MCU-V1.1.png](https://static.mah.priv.at/public/ESP32-8048S070/Schematic/ESP32-8048S070-MCU-V1.1.png) | SPI MISO  |
| IO12 | TF_CLK           | SD_CLK   | SDcard            |                                                                                                                    | SPI CLOCK |
| IO11 | MCU_MOSI         | SD_CMD   | SDcard            |                                                                                                                    | SPI MOSI  |
| IO19 | USB-PW R-DM      | Select?  | SDA1 (?)          |                                                                                                                    |           |

![img](assets/p2-spi.png)                                                                                                  | no idea   |

## P3 - label: USB/UART

HC-1.25-4PWT 1.25mm SMD connector

| Pin  | Signal<br />name | used for                                          |  |  |
| ---- | ---------------- | ------------------------------------------------- | - | - |
| IO20 | USB?             | SCL1 of Touch controller                         |  |  |
| IO19 | n/a              | SDA1 of Touch controller, same pin as P2/IO19 (?) |  |  |
| IO18 | UART1            | SCL0 (?)                                          |  |  |
| IO17 | n/a              | SDA0  (?)                                        |  |  |

![img](assets/p3.png)

## P4 - unlabeled, 1.25mm

HC-1.25-4PWT 1.25mm SMD connector

| Pin  | Signal<br />name | used for |  |  |
| ---- | ---------------- | -------- | - | - |
| IO18 |                  | SCL0     |  |  |
| IO17 |                  | SDA0     |  |  |
| 3.3V |                  |          |  |  |
| GND  |                  |          |  |  |

![img](assets/p4.png)

## P5 - unlabeled, Stemma - same as P4

Stemma connector, 1mm SH-1.0-4PWB, standard pin usage for I2C, same pins and ordering as P4

![](assets/p5_stemma.png)

![img](assets/stemma.webp)

## FPC1 - Touch Controller

9 pins total, 4 of which are GND

| number | Label | usage                 | IO pin |
| ------ | ----- | --------------------- | ------ |
| 6      | VDD   | 3.3v                  |        |
| 5      | INT   | TP_INT                | n/c?   |
| 4      | REST  | Touch ctrl reset line | IO38   |
| 3      | SCL   | SCL1                  | IO20   |
| 2      | SDA   | SDA1                  | IO19   |
| 1      | GND   |                       |        |

![](assets/fcp1-touch-connector.png)

![](assets/touch-gt911.png)Seems the touch controller IRQ pin is not connected, so polling required

## TF Card Connections

Seems the Card Detect switch (pin 9, CD) is not connected, although it goes into a through-hole?

![](assets/TF_card.png)

I2S Sound pins

The I2S amplifier is a [MAX98357](https://cdn-shop.adafruit.com/product-files/3006/MAX98357A-MAX98357B.pdf) and needs 3 IO pins as per diagram:

| signal<br />bane | pin  | purpose                | remark                                     |
| ---------------- | ---- | ---------------------- | ------------------------------------------ |
| DIN              | IO17 | I2S                    |                                            |
| BCLK             | IO0  | I2S Bit clock line    | also BOOT switch<br />IO0 has a 10k pullup |
| LRCLK            | IO18 | I2S Serial data input |                                            |

These are connected to the CPU pins via 0-Ohm resistors R26, R27, R28.

One could disable the speaker and remove R26 and R27 to free 2 GPIO pins

GPIO0/IO0 is special-purpose (BOOT) and better be left alone

It seems they patched up the hardware:


```
Description of changes from V1.0 to V1.1:
1. The IO corresponding to the original V1.0 of BCLK that controls audio is IO19, and V1.1 changes the IO corresponding to BCLK to IO0.
2. V1.1 solves the problem that some computers fail to burn.
```

## Backlight control via PWM

is GPIO2.

## Status of examples

SD card IO works.

I2C0 works if sound is not activated.

Sound never worked for me.

I do not think connecing an SPI device will work due to a lack of chip select pins.

## Verdict

This is broken hardware, foregt about it,
