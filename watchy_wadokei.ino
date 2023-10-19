// Wadokei for Watchy by Sudrien
// Copyright 2023 Sudrien _+watchy@sudrien.net
// Released under free MIT License : https://github.com/sudrien/watchy_wadokei/blob/main/LICENSE

#include "watchy_wadokei.h"
#include "settings.h"

WatchyWadokei watchy(settings);

void setup(){
  //Serial.begin(115200);
  //Serial.println("gimme text");
  watchy.init();
}

void loop(){}
