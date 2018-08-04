/* LowpassFIRFilterCoeffs.hxx
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


#include "FIRFilterCoefficients.hxx"


class LowpassFIRFilterCoeffs : public FIRFilterCoefficients {

public:
  LowpassFIRFilterCoeffs(double cutoff_f, double samplingrate, double ripple = 0.001, double transitionWidth = 0.05);

  double* getCoefficients()   { return coeff; }
  int     getNumberOfCoeffs() { return n; }

  ~LowpassFIRFilterCoeffs();

private:
  double* coeff;
  int     n;
};
