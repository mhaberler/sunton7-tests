#include <TinyGPSPlus.h>
void displayInfo(void);

#define GPS Serial1
TinyGPSPlus gps;

void setup() {
    delay(3000);
    Serial.begin(115200);
    pinMode(17, OUTPUT);
    pinMode(18, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(20, OUTPUT);

    Serial.println(F("BasicExample.ino"));
    Serial.println(F("Basic demonstration of TinyGPSPlus (no device needed)"));
    Serial.print(F("Testing TinyGPSPlus library v. "));
    Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();

}

void loop() {
    // Serial1.printf("fooobarbaz");
    digitalWrite(17, HIGH);       // sets the digital pin 13 on
    delay(10);                  // waits for a second
    digitalWrite(17, LOW);       // sets the digital pin 13 on
    delay(10);
    digitalWrite(18, HIGH);       // sets the digital pin 13 on
    delay(10);                  // waits for a second
    digitalWrite(18, LOW);       // sets the digital pin 13 on
    delay(10);
    digitalWrite(19, HIGH);       // sets the digital pin 13 on
    delay(10);                  // waits for a second
    digitalWrite(19, LOW);       // sets the digital pin 13 on
    delay(10);
    digitalWrite(20, HIGH);       // sets the digital pin 13 on
    delay(10);                  // waits for a second
    digitalWrite(20, LOW);       // sets the digital pin 13 on
}

// void loop() {
//     int c = GPS.available();
//     if (c > 0) {
//         Serial.printf("%c",c);
//         // gps.encode(c);
//     }
//     static uint32_t last;
//     if (millis() - last > 1000) {
//         //    displayInfo();
//         GPS.printf("hello!\n");
//         last = millis();
//     }
//     delay(1);
// }

void displayInfo() {
    Serial.print(F("Location: "));
    if (gps.location.isValid()) {
        Serial.print(gps.location.lat(), 6);
        Serial.print(F(","));
        Serial.print(gps.location.lng(), 6);
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F("  Date/Time: "));
    if (gps.date.isValid()) {
        Serial.print(gps.date.month());
        Serial.print(F("/"));
        Serial.print(gps.date.day());
        Serial.print(F("/"));
        Serial.print(gps.date.year());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.print(F(" "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) Serial.print(F("0"));
        Serial.print(gps.time.hour());
        Serial.print(F(":"));
        if (gps.time.minute() < 10) Serial.print(F("0"));
        Serial.print(gps.time.minute());
        Serial.print(F(":"));
        if (gps.time.second() < 10) Serial.print(F("0"));
        Serial.print(gps.time.second());
        Serial.print(F("."));
        if (gps.time.centisecond() < 10) Serial.print(F("0"));
        Serial.print(gps.time.centisecond());
    } else {
        Serial.print(F("INVALID"));
    }

    Serial.println();
}