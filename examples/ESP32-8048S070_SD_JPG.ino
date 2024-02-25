#include <Arduino_GFX_Library.h>
#include <SD.h>
#include "JpegFunc.h"

// microSD card
#define PIN_SD_CS 10
#define PIN_SD_CMD 11
#define PIN_SD_CLK 12
#define PIN_SD_D0 13

#define SCREEN_W 800
#define SCREEN_H 480

// ESP32-8048S070 7.0 inch 800x480 LCD Display
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(41,40,39,42,14,21,47,48,45,9,46,3,8,16,1,15,7,6,5,4,0,180,30,16,0,12,13,10);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(800,480,rgbpanel);

String image_list[5] = {"/image01.jpg","/image02.jpg","/image03.jpg","/image04.jpg","/image05.jpg"};  //using FAT32

void setup() {
  Serial.begin(115200);

  pinMode(PIN_SD_CS, OUTPUT);
  digitalWrite(PIN_SD_CS, LOW);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
      
  gfx->begin();
  
  if (!SD.begin(PIN_SD_CS)) {
    Serial.println("Error, SD Initialization Failed");
    return;
  }

  xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 20480, NULL, 2, NULL, ARDUINO_RUNNING_CORE);
}

void loop() {
}

void Task_TFT(void *pvParameters) {
  
  while (true) {    // A Task shall never return or exit.
    
    for (int i=0;i<5;i++) {
      jpegDraw(image_list[i].c_str(), jpegDrawCallback, true, 0, 0, gfx->width(), gfx->height());
      vTaskDelay(5000);
    }

  }
}

static int jpegDrawCallback(JPEGDRAW *pDraw) {
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx->draw16bitBeRGBBitmap(pDraw->x, pDraw->y, pDraw->pPixels, pDraw->iWidth, pDraw->iHeight);
  return 1;
}