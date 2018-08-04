/* ContrastEnhancer.cxx
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


#include "ContrastEnhancer.hxx"


ContrastEnhancer::ContrastEnhancer(ImageProducer* producer, StatusView* sv, int low, int high) :
  ImageProducer(sv),
  source(producer),
  histogram(0),
  lo(low),
  hi(high)
{
}


void ContrastEnhancer::calcHistogram(float* histogram, ImageBlock* block) {
  unsigned char* image = block->getData();
  float          max   = 0.0;

  for (int x = 0; x < 256; x++) histogram[x] = 0.0;

  for (int x = 0; x < block->getWidth() * block->getHeight(); x++) {
    histogram[image[x]] += 1.0;
  }

  for (int x = 0; x < 256; x++) if (histogram[x] > max) max = histogram[x];
  for (int x = 0; x < 256; x++) histogram[x] /= max;
}


void ContrastEnhancer::estimateParameters(float* histogram, int* low, int* high) {
  static const int BLACK_OFFSET = 16;
  int middle;
  int max_lo_x = 0;
  int max_hi_x = 255;
  float max_lo = 0.0;
  float max_hi = 0.0;
 
  while(histogram[max_hi_x] < 0.01) max_hi_x--;
  while(histogram[max_lo_x] < 0.01) max_lo_x++;

  middle = (max_hi_x + max_lo_x) / 2;

  for (int x = 0; x < middle; x++) if (histogram[x] > max_lo) {
    max_lo   = histogram[x];
    max_lo_x = x;
  }

  for (int x = middle; x < 256; x++) if (histogram[x] > max_hi) {
    max_hi   = histogram[x];
    max_hi_x = x;
  }

  *low  =  max_lo_x + BLACK_OFFSET;
  *high = (max_lo_x + max_hi_x) / 2;
}


void ContrastEnhancer::createLookupTable(unsigned char* lut, int low, int high) {
  for (int x = 0;    x < low;  x++) { lut[x] = 0; }
  for (int x = low;  x < high; x++) { lut[x] = (x-low)*255/(high-low); }
  for (int x = high; x < 256;  x++) { lut[x] = 255; }
}


ImageBlock* ContrastEnhancer::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = source->produceBlock();
  if (block == 0) return 0;

  if (histogram == 0) {
    histogram = new float[256];
    calcHistogram(histogram, block);
    if ((lo == -1) || (hi == -1)) estimateParameters(histogram, &lo, &hi);
    createLookupTable(lut, lo, hi);
  }

  unsigned char* image = block->getData();
  for (int i = 0;  i < block->getWidth() * block->getHeight(); i++) {
    image[i] = lut[image[i]];
  }

  return block;
}


ContrastEnhancer::~ContrastEnhancer() {
  if (histogram != 0) delete[] histogram;
}
