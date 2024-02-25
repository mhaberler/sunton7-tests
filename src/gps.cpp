// #include <TinyGPSPlus.h>
// void displayInfo(void);

// #define GPS Serial1
// TinyGPSPlus gps;

// void sdcard_init();

// void setup() {
//     delay(3000);
//     Serial.begin(115200);
//     pinMode(17, OUTPUT);
//     pinMode(18, OUTPUT);
//     pinMode(19, OUTPUT);
//     pinMode(20, OUTPUT);

//     Serial.println(F("BasicExample.ino"));
//     Serial.println(F("Basic demonstration of TinyGPSPlus (no device needed)"));
//     Serial.print(F("Testing TinyGPSPlus library v. "));
//     Serial.println(TinyGPSPlus::libraryVersion());
//     Serial.println(F("by Mikal Hart"));
//     Serial.println();

//     sdcard_init();
// }

// void loop() {
//     // Serial1.printf("fooobarbaz");
//     digitalWrite(17, HIGH);       // sets the digital pin 13 on
//     delay(10);                  // waits for a second
//     digitalWrite(17, LOW);       // sets the digital pin 13 on
//     delay(10);
//     digitalWrite(18, HIGH);       // sets the digital pin 13 on
//     delay(10);                  // waits for a second
//     digitalWrite(18, LOW);       // sets the digital pin 13 on
//     delay(10);
//     digitalWrite(19, HIGH);       // sets the digital pin 13 on
//     delay(10);                  // waits for a second
//     digitalWrite(19, LOW);       // sets the digital pin 13 on
//     delay(10);
//     digitalWrite(20, HIGH);       // sets the digital pin 13 on
//     delay(10);                  // waits for a second
//     digitalWrite(20, LOW);       // sets the digital pin 13 on
// }

// // void loop() {
// //     int c = GPS.available();
// //     if (c > 0) {
// //         Serial.printf("%c",c);
// //         // gps.encode(c);
// //     }
// //     static uint32_t last;
// //     if (millis() - last > 1000) {
// //         //    displayInfo();
// //         GPS.printf("hello!\n");
// //         last = millis();
// //     }
// //     delay(1);
// // }

// void displayInfo() {
//     Serial.print(F("Location: "));
//     if (gps.location.isValid()) {
//         Serial.print(gps.location.lat(), 6);
//         Serial.print(F(","));
//         Serial.print(gps.location.lng(), 6);
//     } else {
//         Serial.print(F("INVALID"));
//     }

//     Serial.print(F("  Date/Time: "));
//     if (gps.date.isValid()) {
//         Serial.print(gps.date.month());
//         Serial.print(F("/"));
//         Serial.print(gps.date.day());
//         Serial.print(F("/"));
//         Serial.print(gps.date.year());
//     } else {
//         Serial.print(F("INVALID"));
//     }

//     Serial.print(F(" "));
//     if (gps.time.isValid()) {
//         if (gps.time.hour() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.hour());
//         Serial.print(F(":"));
//         if (gps.time.minute() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.minute());
//         Serial.print(F(":"));
//         if (gps.time.second() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.second());
//         Serial.print(F("."));
//         if (gps.time.centisecond() < 10) Serial.print(F("0"));
//         Serial.print(gps.time.centisecond());
//     } else {
//         Serial.print(F("INVALID"));
//     }

//     Serial.println();
// }

//**********************************************************************************************************
//*    audioI2S-- I2S audiodecoder for ESP32,                                                              *
//**********************************************************************************************************
//
// first release on 11/2018
// Version 3  , Jul.02/2020
//
//
// THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.
// FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR
// OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE
//

#include "Arduino.h"
#include "WiFiMulti.h"
#include "Audio.h"
#include "SPI.h"
#include "SD.h"
#include "FS.h"

// v1.1 hardware
#define I2S_DOUT      17
#define I2S_BCLK     0
#define I2S_LRC       18

// v1.0 hardware
// #define I2S_DOUT      17
// #define I2S_BCLK      19
// #define I2S_LRC       18

#define BUTTON_PIN    0

Audio audio;
WiFiMulti wifiMulti;
String ssid =    WIFI_SSID;
String password = WIFI_PASSWORD;

int music_id = 1818615845;

// optional
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
}
void audio_id3data(const char *info){  //id3 metadata
    Serial.print("id3data     ");Serial.println(info);
}
void audio_eof_mp3(const char *info){  //end of file
    Serial.print("eof_mp3     ");Serial.println(info);
}
void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}
void audio_showstreamtitle(const char *info){
    Serial.print("streamtitle ");Serial.println(info);
}
void audio_bitrate(const char *info){
    Serial.print("bitrate     ");Serial.println(info);
}
void audio_commercial(const char *info){  //duration in sec
    Serial.print("commercial  ");Serial.println(info);
}
void audio_icyurl(const char *info){  //homepage
    Serial.print("icyurl      ");Serial.println(info);
}
void audio_lasthost(const char *info){  //stream URL played
    Serial.print("lasthost    ");Serial.println(info);
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(ssid.c_str(), password.c_str());
    wifiMulti.run();
    if(WiFi.status() != WL_CONNECTED){
        WiFi.disconnect(true);
        wifiMulti.run();
    }
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(21); // 0...21

    pinMode(BUTTON_PIN, INPUT_PULLUP);

String music_url = "https://static.mah.priv.at/sound/Lagerhaus Reggae.mp3";
    // String music_url = "https://music.163.com/song/media/outer/url?id=" + String(music_id) + ".mp3";
    audio.connecttohost(music_url.c_str()); //  网易 1818615845
    //   https://music.163.com/song/media/outer/url?id=1960424259.mp3
    //   https://music.163.com/song/media/outer/url?id=1818615845.mp3
    //   https://music.163.com/song/media/outer/url?id=1929356672.mp3
}

void loop()
{
    audio.loop();

    if(Serial.available()){ // put streamURL in serial monitor
        audio.stopSong();
        String r=Serial.readString(); r.trim();
        if(r.length()>5) audio.connecttohost(r.c_str());
        log_i("free heap=%i", ESP.getFreeHeap());
    }

    if(digitalRead(BUTTON_PIN) == LOW)
    {
        delay(5);
        if(digitalRead(BUTTON_PIN) == LOW)
        {
            audio.stopSong();
            music_id = music_id + 1;
            String music_url = "https://music.163.com/song/media/outer/url?id=" + String(music_id) + ".mp3";
            audio.connecttohost(music_url.c_str()); //  网易 1818615845
            Serial.println(music_url);
            while(digitalRead(BUTTON_PIN) == LOW);
        }
    }
}
