#include "watchy_wadokei.h"

void WatchyWadokei::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);
  
  sun.setPosition(settings.lat.toFloat(), settings.lon.toFloat(), settings.gmtOffset / 3600);

  //from minutes past midnight
  sun.setCurrentDate(tmYearToCalendar(currentTime.Year), currentTime.Month, currentTime.Day);
  sun.setTZOffset(settings.gmtOffset / 3600);
  minutesSunrise = sun.calcSunrise();
  minutesSunset = sun.calcSunset();
  minutesDaytime = (minutesSunset - minutesSunrise);
  minutesNighttime = (1440 - minutesDaytime);


  minutesBeforeSunset = minutesSunset -  (currentTime.Hour * 60 + currentTime.Minute); // i.e.zero degrees

  //char angle_buffer[64];
  float_t hourAngle;
  //Serial.print("minutesBeforeSunset ");
  //Serial.print(minutesBeforeSunset);
  //Serial.print(" ");
  //Serial.print("minutesDaytime ");
  //Serial.print(minutesDaytime);
  //Serial.print(" ");
  //Serial.print("minutesNighttime ");
  //Serial.println(minutesNighttime);

  if(minutesBeforeSunset > minutesDaytime) {
    hourAngle = -180.0 - 180.0 * (minutesBeforeSunset - minutesDaytime) / minutesNighttime;
    //Serial.print("before sunrise ");
    }
  else if(minutesBeforeSunset > 0 ) {
    hourAngle = -180.0 * minutesBeforeSunset / minutesDaytime;
    //Serial.print("daytime ");
    }
  else {
    hourAngle = -180.0 * minutesBeforeSunset / minutesNighttime;
    //Serial.print("after sunset ");
    }

  drawBackground(90.0);
  drawHourHand(hourAngle); 
  

  char time_buffer[8];

  sprintf(time_buffer, "%02d:%02d", currentTime.Hour, currentTime.Minute);

  drawTime(time_buffer);


 
  //Serial.print("hourAngle ");
 // Serial.println(hourAngle);
  
  
  ringChime();
  }



void WatchyWadokei::drawBackground(float_t midnightAngle){
  display.fillTriangle(
    100,
    100,
    100 + 200 * cos((midnightAngle) * DEG2RAD),
    100 + 200 * sin((midnightAngle) * DEG2RAD),
    100 + 200 * cos((midnightAngle + 90) * DEG2RAD),
    100 + 200 * sin((midnightAngle + 90) * DEG2RAD),
    GxEPD_BLACK
    );

  display.fillTriangle(
    100,
    100,
    100 + 200 * cos((midnightAngle) * DEG2RAD),
    100 + 200 * sin((midnightAngle) * DEG2RAD),
    100 + 200 * cos((midnightAngle - 90) * DEG2RAD),
    100 + 200 * sin((midnightAngle - 90) * DEG2RAD),
    GxEPD_BLACK
    );

  // daytime dividers
  for(int16_t i = 4; i <= 9; i++) {
    display.drawLine(
      100,
      100,
      100 + 200 * cos((midnightAngle + i * 30 - 15) * DEG2RAD),
      100 + 200 * sin((midnightAngle + i * 30 - 15) * DEG2RAD),
      GxEPD_BLACK
      );
    }

  // nighttime dividers
  for(int16_t i = 10; i <= 15; i++) {
    display.drawLine(
      100,
      100,
      100 + 200 * cos((midnightAngle + i * 30 - 15) * DEG2RAD),
      100 + 200 * sin((midnightAngle + i * 30 - 15) * DEG2RAD),
      GxEPD_WHITE
      );
    }

  //background sunset
  display.fillCircle(
    100 + 87 * cos((midnightAngle - 90) * DEG2RAD),
    100 + 87 * sin((midnightAngle - 90) * DEG2RAD),
    17,
    GxEPD_BLACK
    );
  display.fillCircle(
    100 + 62 * cos((midnightAngle - 90) * DEG2RAD),
    100 + 62 * sin((midnightAngle - 90) * DEG2RAD),
    13,
    GxEPD_BLACK
    );

  //background sunrise
  display.fillCircle(
    100 + 87 * cos((midnightAngle - 90 - 180) * DEG2RAD),
    100 + 87 * sin((midnightAngle - 90 - 180) * DEG2RAD),
    17,
    GxEPD_WHITE
    );
  display.fillCircle(
    100 + 62 * cos((midnightAngle - 90 - 180) * DEG2RAD),
    100 + 62 * sin((midnightAngle - 90 - 180) * DEG2RAD),
    13,
    GxEPD_WHITE
    );


  // daytime animals 
  for(int16_t i = 0; i < 6; i++) {
    display.drawXBitmap(
      100 + 87 * cos((midnightAngle + 90 + i * 30) * DEG2RAD) - 12,
      100 + 87 * sin((midnightAngle + 90 + i * 30) * DEG2RAD) - 12,
      Hours[i],
      24,
      24,
      GxEPD_BLACK
      );
    }

  // nightime animals 
  for(int16_t i = 6; i < 12; i++) {
    display.drawXBitmap(
      100 + 87 * cos((midnightAngle + 90 + i * 30) * DEG2RAD) - 12,
      100 + 87 * sin((midnightAngle + 90 + i * 30) * DEG2RAD) - 12,
      Hours[i],
      24,
      24,
      GxEPD_WHITE
      );
    }

  // daytime numbers 
  for(int16_t i = 0; i < 6; i++) {
    display.drawXBitmap(
      100 + 62 * cos((midnightAngle + 90 + i * 30) * DEG2RAD) - 8,
      100 + 62 * sin((midnightAngle + 90 + i * 30) * DEG2RAD) - 8,
      Numbers[i],
      16,
      16,
      GxEPD_BLACK
      );
    }

  // nightime numbers 
  for(int16_t i = 0; i < 6; i++) {
    display.drawXBitmap(
      100 + 62 * cos((midnightAngle - 90 + i * 30) * DEG2RAD) - 8,
      100 + 62 * sin((midnightAngle - 90 + i * 30) * DEG2RAD) - 8,
      Numbers[i],
      16,
      16,
      GxEPD_WHITE
      );
    }
  }

void WatchyWadokei::drawHourHand(float_t hourAngle) {
  // hour hub black outline
  display.fillCircle(100, 100, 33, GxEPD_BLACK);

  // hour hand black outline
  display.fillTriangle(
    100 + 43 * cos(( hourAngle ) * DEG2RAD),
    100 + 43 * sin(( hourAngle ) * DEG2RAD),
    100 + 33 * cos(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 33 * sin(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 33 * cos(( hourAngle - 20 + 0 ) * DEG2RAD),
    100 + 33 * sin(( hourAngle - 20 + 0 ) * DEG2RAD),
    GxEPD_BLACK
    );

  // hour hub white outline
  display.fillCircle(100, 100, 30, GxEPD_WHITE);

  // hour hand white outline
  display.fillTriangle(
    100 + 40 * cos(( hourAngle ) * DEG2RAD),
    100 + 40 * sin(( hourAngle ) * DEG2RAD),
    100 + 30 * cos(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 30 * sin(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 30 * cos(( hourAngle - 20 + 0 ) * DEG2RAD),
    100 + 30 * sin(( hourAngle - 20 + 0 ) * DEG2RAD),
    GxEPD_WHITE
    );

  // hour hub
  display.fillCircle(100, 100, 27, GxEPD_BLACK);

  // hour hand
  display.fillTriangle(
    100 + 37 * cos(( hourAngle ) * DEG2RAD),
    100 + 37 * sin(( hourAngle ) * DEG2RAD),
    100 + 27 * cos(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 27 * sin(( hourAngle + 20 + 0 ) * DEG2RAD),
    100 + 27 * cos(( hourAngle - 20 + 0 ) * DEG2RAD),
    100 + 27 * sin(( hourAngle - 20 + 0 ) * DEG2RAD),
    GxEPD_BLACK
    );
  }


void WatchyWadokei::drawTime(char * centerText) {
  display.setFont(&FreeSansBold12pt7b);

  int16_t  x_center, y_center;
  uint16_t w_center, h_center;


  display.getTextBounds(centerText, 100, 100, &x_center, &y_center, &w_center, &h_center);
  display.setTextColor(GxEPD_WHITE);
  for(int16_t i = -3; i <= 3; i++) {
    for(int16_t j = -3; j <= 3; j++) {
      display.setCursor(99 - w_center / 2 + i, 116 - h_center / 2 + j);
      display.println(centerText);
      }
    }

  display.setTextColor(GxEPD_BLACK);
  display.setCursor(99 - w_center / 2, 116 - h_center / 2);
  display.println(centerText);
  }

void WatchyWadokei::ringChime(){
  int16_t chimes[36] = {
    //daytime
    floor(minutesDaytime / 12.0), 6,
    floor(minutesDaytime / 12.0 *  3), 7,
    floor(minutesDaytime / 12.0 *  5), 8,
    floor(minutesDaytime / 12.0 *  7), 9,
    floor(minutesDaytime / 12.0 *  9), 4,
    floor(minutesDaytime / 12.0 * 11), 5,
    // counterclockwise nighttime
    minutesDaytime + floor(minutesNighttime / 12.0 *  1), 6,
    minutesDaytime + floor(minutesNighttime / 12.0 *  3), 7,
    minutesDaytime + floor(minutesNighttime / 12.0 *  5), 8,
    minutesDaytime + floor(minutesNighttime / 12.0 *  7), 9,
    minutesDaytime + floor(minutesNighttime / 12.0 *  9), 4,
    minutesDaytime + floor(minutesNighttime / 12.0 * 11), 5,
    // clockwise nighttime
    floor(minutesNighttime / 12.0 *  -1), 5,
    floor(minutesNighttime / 12.0 *  -3), 4,
    floor(minutesNighttime / 12.0 *  -5), 9,
    floor(minutesNighttime / 12.0 *  -7), 8,
    floor(minutesNighttime / 12.0 *  -9), 7,
    floor(minutesNighttime / 12.0 * -11), 6
    };

  for(int16_t j = 0; j < 32; j += 2) {
    //Serial.print(j);
    //Serial.print( " minutes ");
    //Serial.print(chimes[j]);
    //Serial.print( " chimes ");
    //Serial.print(chimes[j+1]);

    //Serial.print( " in ");
    //Serial.print( minutesBeforeSunset - chimes[j]);
    //Serial.println( " minutes ");
    

    if(minutesBeforeSunset - chimes[j] == 0) {
      for(int16_t i = 5; i > 0; i--) {
        pinMode(VIB_MOTOR_PIN, OUTPUT);
        analogWrite(VIB_MOTOR_PIN, 164);
        delay(500);

        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 124);
        delay(200);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 124);
        delay(180);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 124);
        delay(160);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 124);
        delay(140);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 104);
        delay(120);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 84);
        delay(100);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 64);
        delay(80);
        analogWrite(VIB_MOTOR_PIN, 12);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 32);
        delay(10);
        analogWrite(VIB_MOTOR_PIN, 44);
        delay(60);
        analogWrite(VIB_MOTOR_PIN, 12);
      }
      // final fall off
      delay(10);
      analogWrite(VIB_MOTOR_PIN, 16);
      delay(10);
      analogWrite(VIB_MOTOR_PIN, 24);
      delay(60);
      analogWrite(VIB_MOTOR_PIN, 6);
      delay(10);
      analogWrite(VIB_MOTOR_PIN, 10);
      delay(10);
      analogWrite(VIB_MOTOR_PIN, 16);
      delay(60);
      analogWrite(VIB_MOTOR_PIN, 2);
      delay(10);
      analogWrite(VIB_MOTOR_PIN, 0);
      }
    }
  }