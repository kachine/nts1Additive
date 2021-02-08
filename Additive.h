/*
 * File: Additive.h
 *
 * NTS-1 Additive oscillator
 *
 */
#include "userosc.h"
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define NUM_OVERTONES     7
#define MAX_PARAM         100
#define INIT_WAVEFORM     WAVEFORM_SIN
#define INIT_LEVEL_RATIO  1.f
#define MAX_MOD_RATIO     .8f
#define LFO_DEST_VAL_STEP 256 // = 1024(10bit resolution) / 4 (number of LFO detitination pattern)

enum {
  WAVEFORM_SIN = 0,
  WAVEFORM_SQU = 1,
  WAVEFORM_SAW = 2,
  WAVEFORM_NOISE = 3
};

enum {
  LFO_NONE = 0,
  LFO_ODD = 1,
  LFO_EVEN = 2,
  LFO_ALL = 3
};

struct _voice{
  uint8_t waveformType[NUM_OVERTONES]; // Waveform type, 0-3
  float   phaseRatio[NUM_OVERTONES];   // Phase ratio, 0<=phaseRatio<1.0
  float   levelRatio[NUM_OVERTONES];   // Level ratio, 0<=levelRatio<=1.0
  uint8_t lfoDestination;              // LFO destination
  float   agcCoefficient;              // Coefficient for automatic gain control
};
struct _voice VOICE;    // Global variable to keep voice parameters

void setAgc(void);
void OSC_INIT(uint32_t platform, uint32_t api);
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames);
void OSC_NOTEON(const user_osc_param_t * const params);
void OSC_NOTEOFF(const user_osc_param_t * const params);
void OSC_PARAM(uint16_t index, uint16_t value);
