/* ImageProducer.hxx
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


#ifndef _IMAGEPRODUCER
#define _IMAGEPRODUCER

#include "ImageBlock.hxx"
#include "StatusView.hxx"


class ImageProducer {

public:
  ImageProducer(StatusView* status_view);
  virtual ImageBlock* produceBlock() = 0;
  virtual ~ImageProducer() {}

protected:
  StatusView* status;
};


#endif
