/* PerforationCounter.cxx
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


#include "PerforationCounter.hxx"


PerforationCounter::PerforationCounter(ImageProducer* producer, StatusView* sv, int x_offset, int width, int height) :
  ImageProducer(sv),
  m_source(producer),
  m_moving_average(height, 0),
  m_x_off(x_offset),
  m_width(width),
  m_height(height),
  m_transparent_area(0),
  m_perforation_count(0),
  m_was_opaque(true)
{
}


ImageBlock* PerforationCounter::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = m_source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            img_w = block->getWidth();
  int            img_h = block->getHeight();

  if (m_transparent_area == 0) {

    int start = m_x_off;
    int min   = m_width * m_height * 255;
    int max   = 0;

    for (int l = 0; l < m_height; l++) {
      int intensity = 0;
      for(int x = start;  x < start + m_width;  x++) intensity += image[x];
      m_moving_average.add(intensity);
      start += img_w;
    }

    while (start < img_w * img_h) {
      int intensity = 0;

      int sum = m_moving_average.getSum();
      if      (sum > max) max = sum;
      else if (sum < min) min = sum;

      for(int x = start; x < start + m_width; x++) intensity += image[x];
      m_moving_average.add(intensity);

      start += img_w;
    }

    m_transparent_area = max - (max-min)/10;

    m_moving_average.reset(0);
  }

  int start = m_x_off;

  while (start < block->getWidth() * block->getHeight()) {
    int intensity = 0;

    for(int x = start; x < start + m_width; x++) intensity += image[x];
    m_moving_average.add(intensity);
   
    if (m_moving_average.getSum() >= m_transparent_area) {
      if (m_was_opaque) {
	++m_perforation_count;
	m_was_opaque = false;
      }
    } 
    else m_was_opaque = true;

    start += block->getWidth();
  }

  return block;
}
