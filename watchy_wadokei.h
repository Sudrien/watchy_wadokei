#ifndef WATCHY_WADOKEI_H
#define WATCHY_WADOKEI_H

#include <Watchy.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <sunset.h>

// 丑寅卯辰巳午 未申酉戌亥子
// 四五六七八九

// convert -crop 24x24 hours.xbm h%02d.c
// cat h*.c > hours.h
// convert -crop 16x16 numbers.xbm h%02d.c
//  cat h*.c > numbers.h


#include "hours.h"
#include "numbers.h"

#define DEG2RAD 0.0174532925
#define PI 3.14159265358979323846

class WatchyWadokei : public Watchy{
    using Watchy::Watchy;
    public:

        WatchyWadokei(const watchySettings& s) : Watchy(s) {}
        void drawWatchFace();
        void drawBackground(float_t midnightAngle);
        void drawHourHand(float_t hourAngle);
        void drawTime(char * centerText);
        void ringChime();
    private:

        SunSet sun;
        int16_t minutesSunrise;
        int16_t minutesSunset;
        int16_t minutesDaytime;
        int16_t minutesNighttime;
        int16_t minutesBeforeSunset;
  };
#endif
