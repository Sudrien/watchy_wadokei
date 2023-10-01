#include "watchy_wadokei.h"

void WatchyWadokei::drawWatchFace() {
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);


  float h_offset = -90;

  int16_t  x_point, y_point;
  int16_t  x_point2, y_point2;

  for(int16_t i = 0; i < 12; i++) {
    x_point = 100 + 87 * sin((i * 30.0 + h_offset) * DEG2RAD);
    y_point = 100 - 87 * cos((i * 30.0 + h_offset) * DEG2RAD);
    display.drawXBitmap(x_point - 8,  y_point - 8, Hours[i], 16, 16, GxEPD_BLACK);
    }


  for(int16_t i = 0; i < 24; i++) {
    x_point = 100 + 67 * sin((i * 15.0 + h_offset) * DEG2RAD);
    y_point = 100 - 67 * cos((i * 15.0 + h_offset) * DEG2RAD);
    x_point2 = 100 + 57 * sin((i * 15.0 + h_offset) * DEG2RAD);
    y_point2 = 100 - 57 * cos((i * 15.0 + h_offset) * DEG2RAD);

    display.drawLine(x_point, y_point, x_point2, y_point2, GxEPD_BLACK);
    }

}

