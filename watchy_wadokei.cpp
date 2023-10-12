#include "watchy_wadokei.h"

void WatchyWadokei::drawWatchFace() {
  sun.setPosition(settings.lat.toFloat(), settings.lon.toFloat(), settings.gmtOffset / 3600);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);


  int16_t  x_point, y_point, x_point2, y_point2;

  // get sunrise/sunset angles, from minutes past midnight. Apparently.
  sun.setCurrentDate(tmYearToCalendar(currentTime.Year), currentTime.Month, currentTime.Day);
  sun.setTZOffset(settings.gmtOffset / 3600);
  float sunrise = sun.calcSunrise();
  float sunset = sun.calcSunset();


  float this_minute = currentTime.Hour * 60 + currentTime.Minute;

  //Not solar midnight, midnight according to time zone
  float midnight_angle = 270;

  if(this_minute > sunrise && this_minute <= sunset){
    //day

    midnight_angle = (this_minute - sunrise) / (sunset - sunrise) * 180 + 180;
    }
  else if( this_minute <= sunrise ){
    //before midnight
    midnight_angle = (this_minute + 1440 - sunset) / sunset * 180 ;
    }
  else {
    //after midnight
    midnight_angle = (this_minute - sunset) / sunset * 180;
    }


  //

  display.fillTriangle(
    100,
    100,
    100 + 200 * cos((midnight_angle) * DEG2RAD),
    100 - 200 * sin((midnight_angle) * DEG2RAD),
    100 + 200 * cos((midnight_angle + 90) * DEG2RAD),
    100 - 200 * sin((midnight_angle + 90) * DEG2RAD),
    GxEPD_BLACK);

  display.fillTriangle(
    100,
    100,
    100 + 200 * cos((midnight_angle) * DEG2RAD),
    100 - 200 * sin((midnight_angle) * DEG2RAD),
    100 + 200 * cos((midnight_angle - 90) * DEG2RAD),
    100 - 200 * sin((midnight_angle - 90) * DEG2RAD),
    GxEPD_BLACK);

  // daytime dividers
  for(int16_t i = 4; i <= 9; i++) {
    display.drawLine(
      100,
      100,
      100 + 200 * cos((midnight_angle + i * 30 - 15) * DEG2RAD),
      100 - 200 * sin((midnight_angle + i * 30 - 15) * DEG2RAD),
      GxEPD_BLACK);
    }

  // nighttime dividers
  for(int16_t i = 10; i <= 15; i++) {
    display.drawLine(
      100,
      100,
      100 + 200 * cos((midnight_angle + i * 30 - 15) * DEG2RAD),
      100 - 200 * sin((midnight_angle + i * 30 - 15) * DEG2RAD),
      GxEPD_WHITE);
    }



  //background sunset
  display.fillCircle(
    100 + 87 * cos((midnight_angle - 90 - 0) * DEG2RAD),
    100 - 87 * sin((midnight_angle - 90 - 0) * DEG2RAD),
    17,
    GxEPD_BLACK
    );
  display.fillCircle(
    100 + 62 * cos((midnight_angle - 90 - 0) * DEG2RAD),
    100 - 62 * sin((midnight_angle - 90 - 0) * DEG2RAD),
    13,
    GxEPD_BLACK
    );

  //background sunrise
  display.fillCircle(
    100 + 87 * cos((midnight_angle - 90 - 0 - 180) * DEG2RAD),
    100 - 87 * sin((midnight_angle - 90 - 0 - 180) * DEG2RAD),
    17,
    GxEPD_WHITE
    );
  display.fillCircle(
    100 + 62 * cos((midnight_angle - 90 - 0 - 180) * DEG2RAD),
    100 - 62 * sin((midnight_angle - 90 - 0 - 180) * DEG2RAD),
    13,
    GxEPD_WHITE
    );


  // daytime animals 
  for(int16_t i = 0; i < 6; i++) {
    x_point = 100 + 87 * cos((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    y_point = 100 - 87 * sin((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    display.drawXBitmap(x_point - 12,  y_point - 12, Hours[i], 24, 24, GxEPD_BLACK);
    }

  // nightime animals 
  for(int16_t i = 6; i < 12; i++) {
    x_point = 100 + 87 * cos((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    y_point = 100 - 87 * sin((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    display.drawXBitmap(x_point - 12,  y_point - 12, Hours[i], 24, 24, GxEPD_WHITE);
    }

  // daytime numbers 
  for(int16_t i = 0; i < 6; i++) {
    x_point = 100 + 62 * cos((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    y_point = 100 - 62 * sin((midnight_angle - 90 - 30 - i * 30) * DEG2RAD);
    display.drawXBitmap(x_point - 8,  y_point - 8, Numbers[i], 16, 16, GxEPD_BLACK);
    }

  // nightime numbers 
  for(int16_t i = 0; i < 6; i++) {
    x_point = 100 + 62 * cos((midnight_angle + 90 - 30 - i * 30) * DEG2RAD);
    y_point = 100 - 62 * sin((midnight_angle + 90 - 30 - i * 30) * DEG2RAD);
    display.drawXBitmap(x_point - 8,  y_point - 8, Numbers[i], 16, 16, GxEPD_WHITE);
    }


  // hour hub black outline
  display.fillCircle(100, 100, 33, GxEPD_BLACK);

  // hour hand black outline
  display.fillTriangle(
    100 + 43 * sin(( 0 ) * DEG2RAD),
    100 - 43 * cos(( 0 ) * DEG2RAD),
    100 + 33 * sin(( 20 + 0 ) * DEG2RAD),
    100 - 33 * cos(( 20 + 0 ) * DEG2RAD),
    100 + 33 * sin(( -20 + 0 ) * DEG2RAD),
    100 - 33 * cos(( -20 + 0 ) * DEG2RAD),
    GxEPD_BLACK
    );


  // hour hub white outline
  display.fillCircle(100, 100, 30, GxEPD_WHITE);

  // hour hand white outline
  display.fillTriangle(
    100 + 40 * sin(( 0 ) * DEG2RAD),
    100 - 40 * cos(( 0 ) * DEG2RAD),
    100 + 30 * sin(( 20 + 0 ) * DEG2RAD),
    100 - 30 * cos(( 20 + 0 ) * DEG2RAD),
    100 + 30 * sin(( -20 + 0 ) * DEG2RAD),
    100 - 30 * cos(( -20 + 0 ) * DEG2RAD),
    GxEPD_WHITE
    );

  // hour hub
  display.fillCircle(100, 100, 27, GxEPD_BLACK);

  // hour hand
  display.fillTriangle(
    100 + 37 * sin(( 0 ) * DEG2RAD),
    100 - 37 * cos(( 0 ) * DEG2RAD),
    100 + 27 * sin(( 20 + 0 ) * DEG2RAD),
    100 - 27 * cos(( 20 + 0 ) * DEG2RAD),
    100 + 27 * sin(( -20 + 0 ) * DEG2RAD),
    100 - 27 * cos(( -20 + 0 ) * DEG2RAD),
    GxEPD_BLACK
    );

  display.setFont(&FreeSansBold12pt7b);
  int16_t  x_center, y_center;
  uint16_t w_center, h_center;
  char time_buffer[8];

  sprintf(time_buffer, "%02d:%02d",
    currentTime.Hour,
    currentTime.Minute
    );

  display.getTextBounds(time_buffer, 100, 100, &x_center, &y_center, &w_center, &h_center);
  display.setTextColor(GxEPD_WHITE);
  for(int16_t i = -3; i <= 3; i++) {
    for(int16_t j = -3; j <= 3; j++) {
      display.setCursor(99 - w_center / 2 + i, 116 - h_center / 2 + j);
      display.println(time_buffer);
    }
  }
  display.setTextColor(GxEPD_BLACK);
  display.setCursor(99 - w_center / 2, 116 - h_center / 2);
  display.println(time_buffer);

      //  if (currentTime.Minute == 0) {
          // The RTC wakes us up once per minute
pinMode(VIB_MOTOR_PIN, OUTPUT);
          analogWrite(VIB_MOTOR_PIN, 153);
          delay(75);

          analogWrite(VIB_MOTOR_PIN, 123);
          delay(75);

          analogWrite(VIB_MOTOR_PIN, 73);
          delay(175);

          analogWrite(VIB_MOTOR_PIN, 33);
          delay(275);
          analogWrite(VIB_MOTOR_PIN, 0);

      //  }

}

