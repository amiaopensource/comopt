/* ConstPositionEstimator.hxx
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


#include "PositionEstimator.hxx"


class ConstPositionEstimator: public PositionEstimator {

public:
  ConstPositionEstimator(ImageProducer* producer, StatusView* sv, int position, int width);

  ImageBlock* produceBlock();

  int* getXPositions(int i = 0) { return x_pos; }
  int* getYPositions(int i = 0) { return y_pos; }
  int  getWidth(int i = 0) { return w; }
  int  getCount(int i = 0) { return 2; }

private:
  ImageProducer* source;
  int            x_pos[2];
  int            y_pos[2];
  int            w;
};
