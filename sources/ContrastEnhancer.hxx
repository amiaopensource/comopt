/* ContrastEnhancer.hxx
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


class ContrastEnhancer : public ImageProducer {

public:
  ContrastEnhancer(ImageProducer* producer, StatusView* sv, int low = -1, int high = -1);
  ImageBlock* produceBlock();
  ~ContrastEnhancer();

private:
  void calcHistogram(float* histogram, ImageBlock* block);
  void estimateParameters(float* histogram, int* low, int* high);
  void createLookupTable(unsigned char* lut, int low, int high);

  ImageProducer* source;
  float*         histogram;
  int            lo;
  int            hi;
  unsigned char  lut[256];
};
