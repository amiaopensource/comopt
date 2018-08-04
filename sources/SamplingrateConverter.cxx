/* SamplingrateConverter.cxx
 *
 * This file is part of the diploma thesis:
 * "Digital Image Based Restoration of Optical Movie Soundtracks"
 * ETHZ - Swiss Federal Institute of Technology Zurich
 *
 * Copyright (c) 1999 Patrick Streule
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 3 as published
 * by the Free Software Foundation.
 */


#include "SamplingrateConverter.hxx"
#include <math.h>


SamplingrateConverter::SamplingrateConverter(AudioProducer* producer, StatusView* sv, AudioDataCollector* info, int convert_to, bool antiAlias) :
  AudioProducer(sv),
  sr_from(info->getSamplingrate()),
  sr_to(convert_to),
  out_block(0)
{
  if ((sr_from > sr_to) && (antiAlias)) {
    lowpass = new LowpassFIRFilterCoeffs(sr_to/2, sr_from);
    source  = new FIRFilter(producer, status, lowpass);
  }
  else source = producer;

  info->setSamplingrate(sr_to);
}


AudioBlock* SamplingrateConverter::produceBlock() {
  if (status != 0 ) status->updateSubtask();

  AudioBlock* in_block = source->produceBlock();
  if (in_block == 0) return 0;

  if (out_block == 0) {
    channels  = in_block->getChannels();
    out_block = new AudioBlock((int)((in_block->getSamples() / sr_from) * sr_to) + 2, channels);
    past      = new float[channels];
    x_offs    = 0.0;
  }

  float* sound_in  = in_block->getData();
  float* sound_out = out_block->getData();

  int sample = 0;
  int i      = 0;
  float x    = x_offs;


  while (x < 0) {
    for (sample = 0; sample < channels; sample++) {
      sound_out[sample] = past[sample] + (sound_in[sample]-past[sample]) * (x+1);
    }
    x = (++i / sr_to) * sr_from + x_offs;
  }


  while (x < in_block->getSamples() - 1) {
    float deltaX = (x - floor(x));

    int x_left   = (int)x * channels;
    int x_right  = x_left + channels;

    for (int next = sample+channels; sample < next; sample++) {
      sound_out[sample] = sound_in[x_left] + (sound_in[x_right]-sound_in[x_left]) * deltaX;
      ++x_left;
      ++x_right;
    }

    x = (++i / sr_to) * sr_from + x_offs;
  } 

  x_offs = x - in_block->getSamples();

  if (x_offs < 0) {
    sample = (in_block->getSamples() - 1) * channels;
    for (int channel = 0; channel < channels; channel++) {
      past[channel] = sound_in[sample+channel];
    }
  }

  out_block->setSamples(i);
  return out_block;
}


SamplingrateConverter::~SamplingrateConverter() {
  if (lowpass != 0) {   // we have created our own lowpass filter
    delete lowpass;
    delete source;
  }
  delete[] past;
}
