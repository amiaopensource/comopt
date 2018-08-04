/* PerforationCounter.hxx
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


#include "ImageProducer.hxx"
#include "RunningSum.cxx"


class PerforationCounter : public ImageProducer {

public:
  PerforationCounter(ImageProducer* producer, StatusView* sv, int x_offset, int width, int height);

  ImageBlock* produceBlock();
  int         getCount() { return m_perforation_count; }

private:
  ImageProducer*     m_source;

  int                m_perforation_count;
  int                m_x_off;
  int                m_width;
  int                m_height;
  int                m_transparent_area;
  RunningSum<int>    m_moving_average;

  bool               m_was_opaque;
};
