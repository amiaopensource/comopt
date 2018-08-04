/* AcademyFilterCoeffs.cxx
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


#include "AcademyFilterCoeffs.hxx"
#include <iostream>


AcademyFilterCoeffs::AcademyFilterCoeffs(double samplingrate) :
  n(30)
{
  static const int B = 2;

  double _2kHz = 2000 / samplingrate;
  double _8kHz = 8000 / samplingrate;

  double bands[2*B]  = {0, _2kHz, _8kHz, 0.5};
  double response[B] = {1, 0};
  double weights[B]  = {1, 1};
  
  coeff = new double[n];

  ParksMcClellan::remez(coeff, n, B, bands, response, weights, BANDPASS);
  //cout << endl;
  //for (int c = 0; c < n; c++) {
  //  cout << "coeff[" << c << "]=" << coeff[c] << endl;
  //}
}


AcademyFilterCoeffs::~AcademyFilterCoeffs() {
  if (coeff != 0) delete[] coeff;
}
