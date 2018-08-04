/* AudioDataCollector.cxx
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


#include <limits>
#include "AudioDataCollector.hxx"


AudioDataCollector::AudioDataCollector(AudioProducer* producer, StatusView* sv) :
  AudioProducer(sv),
  source(producer)
{
  reset();
}


AudioBlock* AudioDataCollector::produceBlock() {
  if (status != 0) status->updateSubtask();

  AudioBlock* block = source->produceBlock();
  if (block == 0) return 0;

  if (channels == 0) channels = block->getChannels();

  float* sound = block->getData();

  for (int s = 0; s < block->getSamples() * block->getChannels(); s++) {
    float sample = sound[s];

    if      (sample < min_amplitude) min_amplitude = sample;
    else if (sample > max_amplitude) max_amplitude = sample;
  }

  count += block->getSamples();

  return block;
}


void AudioDataCollector::reset() {
  numeric_limits<float> float_info;
  max_amplitude    = -float_info.max();
  min_amplitude    =  float_info.max();
  count            = 0;
  samplingrate     = 44100;
  channels         = 0;
  bytes_per_sample = 2;
}
