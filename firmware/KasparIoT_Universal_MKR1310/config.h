#pragma once
#include <Arduino.h>
constexpr uint8_t MAX_SENSORS=8;
struct Sensor { char key[17]; float low,high; uint8_t enabled; };
struct Config { uint32_t magic, interval; char eui[17],key[33]; uint8_t port,confirmed,adr,format; Sensor sensors[MAX_SENSORS]; };
constexpr uint32_t MAGIC=0x4B4D0101;
