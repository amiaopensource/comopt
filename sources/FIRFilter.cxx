/* FIRFilter.cxx
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


#include "FIRFilter.hxx"
//#include <iostream>

FIRFilter::FIRFilter(AudioProducer* producer, StatusView* sv, FIRFilterCoefficients* fir_coeffs) :
  AudioProducer(sv),
  source(producer),
  past(0)
{
  coeff = fir_coeffs->getCoefficients();
  taps  = fir_coeffs->getNumberOfCoeffs();
}


AudioBlock* FIRFilter::produceBlock() {
  if (status != 0) status->updateSubtask();

  AudioBlock* block = source->produceBlock();
  if (block == 0) return 0;

  float* sound    = block->getData();
  int    channels = block->getChannels();

  if (past == 0) {
    past = new float[channels * (taps-1)];
    for (int channel = 0;  channel < channels;  channel++) {
      int past_start = channel*(taps-1);
      for (int i = 0; i < taps-1; i++) past[past_start + i] = sound[channel];
    }
    begin = 0;
  }

  for (int sample = 0;  sample < block->getSamples();  sample += channels) {

    for (int channel = 0; channel < channels; channel++) {
      int past_start = channel*(taps-1);
      float sum      = coeff[0];

      past[past_start + begin] = sound[sample+channel];

      for (int i = 1, t = begin;  i < taps;  i++) {
        sum += coeff[i] * past[past_start + t];
        if (--t < 0) t = taps-2;
      }

      sound[sample+channel] = sum;
    }

    if (++begin == taps-1) begin = 0;
  }

  return block;
}


FIRFilter::~FIRFilter() {
  if (past != 0) delete[] past;
}
