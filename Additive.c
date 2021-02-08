/*
 * File: Additive.c
 *
 * NTS-1 Additive oscillator
 *
 */
#include <Additive.h>

void setAgc(void){
  float agc = 0.f;
  for(int8_t i=0; i<NUM_OVERTONES; i++){
    agc += VOICE.levelRatio[i];
  }
  VOICE.agcCoefficient = 1.f / agc; // Zero div is not occur, because levelRatio[0] is always INIT_LEVEL_RATIO and not equal 0.
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// Initialize function
//  called once on instantiation of the oscillator
void OSC_INIT(uint32_t platform, uint32_t api)
{
  // Initialize voice parameters
  for(int8_t i=0; i<NUM_OVERTONES; i++){
    VOICE.waveformType[i] = INIT_WAVEFORM;  // Unable to assign parameter, due to NTS-1 param is limited to 6 (+2), so it is fixed to INIT_WAVEFORM and only sine wave was implemented.
    VOICE.phaseRatio[i] = (float) osc_rand() / UINT_MAX; // Randomize phase ratio within 0 to 1
    VOICE.levelRatio[i] = INIT_LEVEL_RATIO; // The root tone is fixed to this value
  }
  VOICE.lfoDestination = LFO_NONE;
  setAgc();
}

// Wave Generation function
//  callbacked for each sample (or frames)
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  // Pointer to output buffer
  q31_t * __restrict y = (q31_t *) yn;

  // Last address of output buffer
  const q31_t * y_e = y + frames;

  // MIDI note# and pitch modifier of current process
  // If pitch bend message has already received, note value may be differ from actual MIDI note#
  // Pitch modifier value takes within 0 to 255, the value indicate 1/255 of semitone
  // The pitch modifier is always upperward, so downer pitch bend is processed as a combination of note# decrement and adequate upperward pitch modifier.
  uint8_t note = params->pitch >> 8;
  uint8_t mod = params->pitch & 0xFF;

  // Current LFO value within -1 to 1
  float lfo = q31_to_f32(params->shape_lfo);
  // Convert LFO value within 0 to 1;
  lfo = .5* (lfo + 1.f);

  // Working memory to store current sample value
  // Effective range is -1.0 <= sample < 1.0 to convert into Q31 format later
  float sample;

  // Process one sample by sample in frames
  while( y != y_e ) {
    sample = 0.f;
    // Generate wave sample
    for(int8_t i=0; i<NUM_OVERTONES; i++){
      float level = VOICE.levelRatio[i];
      switch(VOICE.lfoDestination){
        case LFO_NONE:
          break;
        case LFO_ODD:
          level *= i%2 ? 1.f : ((1.f - MAX_MOD_RATIO) + MAX_MOD_RATIO * lfo); // Note: i is i+1 for overtone count
          break;
        case LFO_EVEN:
          level *= i%2 ? ((1.f - MAX_MOD_RATIO) + MAX_MOD_RATIO * lfo) : 1.f; // Note: i is i+1 for overtone count
          break;
        case LFO_ALL:
          level *= ((1.f - MAX_MOD_RATIO) + MAX_MOD_RATIO * lfo);
        default:
          break;
      }
      sample += level * osc_sinf(VOICE.phaseRatio[i]);
      // Step a phase ratio
      VOICE.phaseRatio[i] += osc_w0f_for_note(note, mod) * (i + 1);
      // Keep the phase ratio within 0 <= phase < 1
      VOICE.phaseRatio[i] -= (uint32_t) VOICE.phaseRatio[i];
    }

    // Apply AGC and convert a sample into Q31 format, and write to output
    *(y++) = f32_to_q31(sample * VOICE.agcCoefficient);
  }
}

// MIDI note-on event process function
//  * This function is not hooked if active note is already exist
void OSC_NOTEON(const user_osc_param_t * const params)
{
  // Nothing to do
}

// MIDI note-off event process function
//  * This function is not hooked if active note remains
void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  // Nothing to do
}

// Parameter change event process funnction
void OSC_PARAM(uint16_t index, uint16_t value)
{
  // 0-200 for bipolar percent parameters. 0% at 100, -100% at 0.
  // 0-100 for unipolar percent and typeless parameters.
  // 10 bit resolution for shape/shift-shape.
  switch (index) {
    case k_user_osc_param_id1: // Level of Overtone 2 (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio[1] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_id2: // Level of Overtone 3 (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio[2] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_id3: // Level of Overtone 4 (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio[3] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_id4: // Level of Overtone 5 (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio[4] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_id5: // Level of Overtone 6 (0-100)
      // Choose this param by pressing OSC and tweaking TYPE knob, then input by B knob
      VOICE.levelRatio[5] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_id6: // Level of Overtone 7 (0-100)
      VOICE.levelRatio[6] = (float) value / MAX_PARAM;
      setAgc();
      break;
    case k_user_osc_param_shape: // LFO destination
      // 10bit parameter, 0 <= value <= 1023
      // Mapped to OSC mode A knob(shape) and MIDI CC#54
      // To select 4 patterns of LFO destination, value resolution is divided by 4
      VOICE.lfoDestination = (float) value / LFO_DEST_VAL_STEP + 0.5;
      break;
    case k_user_osc_param_shiftshape: // Not used in this sample
      // Similar to k_user_osc_param_shape, but mapped to OSC mode B knob(alt) and MIDI CC#55
      break;
    default:
      break;
  }
}
#pragma GCC diagnostic pop
