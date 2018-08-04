/* ImageDeskewer.hxx
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


class ImageDeskewer : public ImageProducer {
  
public:
  ImageDeskewer(ImageProducer* producer, StatusView* sv, double dy, double dx);

  ImageBlock* produceBlock();

  ~ImageDeskewer();

private:
  ImageProducer*   source;
  ImageBlock*      block;

  unsigned char*   buffer;

  double           delta;

  int              rowspan;
  int              inc;
  int              line;
  int              ready;
};
