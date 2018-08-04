/* ImageCutter.hxx
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


class ImageCutter : public ImageProducer {

public:
  ImageCutter(ImageProducer* producer, StatusView* sv, PositionEstimator* estimator, int padL = 0, int padR = 0, unsigned char padVal = 0);

  ImageBlock* produceBlock();

  ~ImageCutter();

private:
  void cut(unsigned char* image, int img_w, int w, int x0, int x1, int y0, int y1);

  ImageProducer*     m_source;
  PositionEstimator* m_estimator;
  ImageBlock*        m_block;

  int                m_pad_l;
  int                m_pad_r;
  unsigned char      m_pad_val;
};
