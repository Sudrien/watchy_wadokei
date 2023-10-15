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

  //char angle_buffer[24];
  float_t hourAngle;

  if(minutesBeforeSunset > minutesDaytime) {
    hourAngle = -180.0 - 180.0 * (minutesBeforeSunset - minutesDaytime) / minutesNighttime;
    // sprintf(angle_buffer, "%s %+4d %+3.1f",
    // "before",
    //  minutesBeforeSunset,
    //  hourAngle
    //  );
    }
  if(minutesBeforeSunset > 0 ) {
    hourAngle = -180.0 * minutesBeforeSunset / minutesDaytime;
    // sprintf(angle_buffer,"%s %+4d %+3.1f",
    //  "mid",
    //  minutesBeforeSunset,
    //  hourAngle
    //  );
    }
  else {
    hourAngle = -180.0 * minutesBeforeSunset / minutesNighttime;
    // sprintf(angle_buffer, "%s %+4d %+3.1f",
    //  "after",
    //  minutesBeforeSunset,
    //  hourAngle
    //  );
    }

  drawBackground(90.0);
  drawHourHand(hourAngle); 
  

  char time_buffer[8];

  sprintf(time_buffer, "%02d:%02d", currentTime.Hour, currentTime.Minute);

  drawTime(time_buffer);
  //drawTime(angle_buffer);
  
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



  uint16_t chimes = 0;

  // counterclockwise daytime
  if(      minutesBeforeSunset == floor(minutesDaytime / 12.0)) {      chimes = 6; }
  else if( minutesBeforeSunset == floor(minutesDaytime / 12.0 *  3)) { chimes = 7; }
  else if( minutesBeforeSunset == floor(minutesDaytime / 12.0 *  5)) { chimes = 8; }
  else if( minutesBeforeSunset == floor(minutesDaytime / 12.0 *  7)) { chimes = 9; }
  else if( minutesBeforeSunset == floor(minutesDaytime / 12.0 *  9)) { chimes = 4; }
  else if( minutesBeforeSunset == floor(minutesDaytime / 12.0 * 11)) { chimes = 5; }

  // counterclockwise nighttime
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 *  1)) { chimes = 6; }
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 *  3)) { chimes = 7; }
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 *  5)) { chimes = 8; }
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 *  7)) { chimes = 9; }
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 *  9)) { chimes = 4; }
  else if( minutesBeforeSunset == minutesDaytime + floor(minutesNighttime / 12.0 * 11)) { chimes = 5; }

  // clockwise nighttime
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 *  -1)) { chimes = 5; }
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 *  -3)) { chimes = 4; }
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 *  -5)) { chimes = 9; }
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 *  -7)) { chimes = 8; }
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 *  -9)) { chimes = 7; }
  else if( minutesBeforeSunset == floor(minutesNighttime / 12.0 * -11)) { chimes = 6; }


  for(uint16_t i = chimes; i > 0; i--) {
    pinMode(VIB_MOTOR_PIN, OUTPUT);
    analogWrite(VIB_MOTOR_PIN, 255);
    delay(75);

    analogWrite(VIB_MOTOR_PIN, 123);
    delay(75);

    analogWrite(VIB_MOTOR_PIN, 73);
    delay(175);

    analogWrite(VIB_MOTOR_PIN, 33);
    delay(275);
    analogWrite(VIB_MOTOR_PIN, 0);
    delay(275);
    }
  }