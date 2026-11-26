#pragma once
#include "config.h"
#include <math.h>
// Δυαδικό v1: έκδοση, πλήθος, επαναλήψεις ID + signed int32 big endian /100.
inline size_t encodeBinary(const Config &c,const float *v,uint8_t *out){
 size_t n=2; out[0]=1; out[1]=0;
 for(uint8_t i=0;i<MAX_SENSORS;i++) if(c.sensors[i].enabled){
 int32_t x=(int32_t)lroundf(v[i]*100); uint32_t u=(uint32_t)x;
 out[n++]=i+1; for(int j=3;j>=0;j--) out[n++]=(uint8_t)(u>>(j*8)); out[1]++;
 } return n;
}
