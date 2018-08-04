/* ConstPositionEstimator.cxx
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


#include "ConstPositionEstimator.hxx"


ConstPositionEstimator::ConstPositionEstimator(ImageProducer* producer, StatusView* sv, int position, int width) :
  PositionEstimator(sv),
  source(producer),
  w(width)
{
  if (w < 0) {
    w = -w;
    position -= w;
  }
  x_pos[0] = x_pos[1] = position;
  y_pos[0] = 0;
}


ImageBlock* ConstPositionEstimator::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = source->produceBlock();
  if (block == 0) return 0;

  y_pos[1] = block->getHeight();
  return block;
}
