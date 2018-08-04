/* ImageDeskewer.cxx
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


#include <math.h>
#include "ImageDeskewer.hxx"


ImageDeskewer::ImageDeskewer(ImageProducer* producer, StatusView* sv, double dy, double dx) :
  ImageProducer(sv),
  source(producer),
  delta(dy/dx),
  block(0)
{
  // Are the lines slanted forwards or backwards? --> inc
  if (delta < 0.0) inc = -1;
  else inc = 1;

  // ready indicates a completely processed line
  ready = 0;
}


ImageBlock* ImageDeskewer::produceBlock() {
  if (status != 0) status->updateSubtask();

  // Get a new block of image data
  block = source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            in_w  = block->getWidth();
  int            in_h  = block->getHeight();

  // Initialization according to the before unknown image block width
  if (buffer == 0) {

    // Calculate how many scanlines are affected by the azimuth deviation  --> rowspan
    double y  = delta * in_w;
    rowspan   = (int)y + 2;
    if (y - (int)y == 0.0) --rowspan;

    // Different line number start depending on line slant
    line = (delta < 0.0) ? rowspan-1 : 0;

    // Buffer to cope with block limits
    buffer =  new unsigned char[in_w * rowspan];
  }

  // See documentation for a more detailed description.
  for (int y = 0; y < in_h; y++) {

    double A = 0.5 * delta;
    int    l = line;

    for (int x = 0; x < in_w; x++) {
      unsigned char pixel_intensity = image[x + y*in_w];
      unsigned char contribution    = (unsigned char)abs(A * pixel_intensity);

      buffer[x + ((l+inc) % rowspan) * in_w] += contribution;
      buffer[x + (l % rowspan)       * in_w] += pixel_intensity - contribution;

      A += delta;
      if (fabs(A) > fabs(inc)) {
        A -= inc;
        l += inc;
      }

      image[x + y*in_w] = buffer[x + (ready % rowspan) * in_w];
      buffer[x + (ready % rowspan) * in_w] = 0;
    }

    ++line;
    ++ready;
  }

  return block;
}


ImageDeskewer::~ImageDeskewer() {
  if (buffer != 0) delete[] buffer;
}
