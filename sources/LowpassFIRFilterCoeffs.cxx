/* LowpassFIRFilterCoeffs.cxx
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


#include "LowpassFIRFilterCoeffs.hxx"
#include <math.h>


LowpassFIRFilterCoeffs::LowpassFIRFilterCoeffs(double cutoff_f, double samplingrate, double ripple, double transitionWidth) {
  
  static const int B = 2;

  // Kaiser's formula [Steiglitz, p.253]
  n = (int)((-20.0*log10(ripple) - 13.0) / (14.36 * transitionWidth) + 2);
  double limit = cutoff_f / samplingrate;

  if (limit < transitionWidth)     limit = transitionWidth;
  if (limit > 0.5-transitionWidth) limit = 0.5-transitionWidth;

  double bands[2*B]  = {0.0, limit-transitionWidth/2, limit+transitionWidth/2, 0.5};
  double response[B] = {1, 0};
  double weights[B]  = {1, 1};

  coeff = new double[n];

  ParksMcClellan::remez(coeff, n, B, bands, response, weights, BANDPASS);
}


LowpassFIRFilterCoeffs::~LowpassFIRFilterCoeffs() {
  if (coeff != 0) delete[] coeff;
}
