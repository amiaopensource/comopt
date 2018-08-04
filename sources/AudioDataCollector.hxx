/* AudioDataCollector.hxx
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


#ifndef _AUDIODATACOLLECTOR
#define _AUDIODATACOLLECTOR

#include "AudioProducer.hxx"


class AudioDataCollector : public AudioProducer {

public:
  AudioDataCollector(AudioProducer* producer, StatusView* sv);

  AudioBlock* produceBlock();

  float  getMinAmplitude()   { return min_amplitude; }
  float  getMaxAmplitude()   { return max_amplitude; }
  long   getSamples()        { return count; }
  int    getSamplingrate()   { return samplingrate; }
  int    getChannels()       { return channels; }
  int    getBytesPerSample() { return bytes_per_sample; }

  void   setSamplingrate(int sr)    { samplingrate = sr; }
  void   setBytesPerSample(int bps) { bytes_per_sample = bps; }
  void   reset();

private:
  AudioProducer* source;

  float  min_amplitude;
  float  max_amplitude;
  long   count;
  int    samplingrate;
  int    channels;
  int    bytes_per_sample;
};


#endif
