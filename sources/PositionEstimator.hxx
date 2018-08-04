/* PositionEstimator.hxx
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


#ifndef _POSITIONESTIMATOR
#define _POSITIONESTIMATOR

#include "ImageProducer.hxx"


class PositionEstimator : public ImageProducer {
public:
  PositionEstimator(StatusView* sv) : ImageProducer(sv) {}
  virtual int* getXPositions(int i = 0) = 0;
  virtual int* getYPositions(int i = 0) = 0;
  virtual int  getWidth(int i = 0) = 0;
  virtual int  getCount(int i = 0) = 0;
  virtual ~PositionEstimator() {};
};


#endif
