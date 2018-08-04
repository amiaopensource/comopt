/* FIRFilter.hxx
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


#ifndef _FIRFILTER
#define _FIRFILTER

#include "AudioProducer.hxx"
#include "FIRFilterCoefficients.hxx"


class FIRFilter : public AudioProducer {

public:
  FIRFilter(AudioProducer* producer, StatusView* sv, FIRFilterCoefficients* fir_coeff);
  AudioBlock* produceBlock();
  ~FIRFilter();

private:
  AudioProducer* source;

  double*        coeff;
  int            taps;

  float*         past;
  int            begin;
};


#endif
