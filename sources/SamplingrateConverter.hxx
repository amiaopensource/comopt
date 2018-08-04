/* SamplingrateConverter.hxx
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


#include "AudioProducer.hxx"
#include "FIRFilter.hxx"
#include "LowpassFIRFilterCoeffs.hxx"
#include "AudioDataCollector.hxx"


class SamplingrateConverter : public AudioProducer {

public:
  SamplingrateConverter(AudioProducer* producer, StatusView* sv, AudioDataCollector* info, int convert_to, bool antiAlias = true);
  AudioBlock* produceBlock();
  ~SamplingrateConverter();

private:
  AudioProducer* source;
  AudioBlock*    out_block;

  float          sr_from;
  float          sr_to;

  float          x_offs;
  float*         past;

  int            channels;

  LowpassFIRFilterCoeffs* lowpass;
};
