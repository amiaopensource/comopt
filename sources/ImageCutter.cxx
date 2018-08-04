/* ImageCutter.cxx
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


#include "ImageCutter.hxx"
#include <string.h>


ImageCutter::ImageCutter(ImageProducer* producer, StatusView* sv, PositionEstimator* estimator, int padL, int padR, unsigned char padVal) :
  ImageProducer(sv),
  m_source(producer),
  m_estimator(estimator),
  m_block(0),
  m_pad_l(padL),
  m_pad_r(padR),
  m_pad_val(padVal)
{
}


ImageBlock* ImageCutter::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* imb = m_source->produceBlock();
  if (imb == 0) return 0;

  int* x_pos = m_estimator->getXPositions();
  int* y_pos = m_estimator->getYPositions();
  int  count = m_estimator->getCount();
  int  w     = m_estimator->getWidth();

  int in_w = imb->getWidth();
  int in_h = imb->getHeight();

  if (m_block == 0) {
    m_block = new ImageBlock(w + m_pad_l + m_pad_r, in_h);
    memset(m_block->getData(), m_pad_val, m_block->getWidth() * m_block->getHeight());
  }

  int out_w = m_block->getWidth();
  m_block->setHeight(in_h);

  unsigned char* image_in  = imb->getData();
  unsigned char* image_out = m_block->getData();

  for (int i = 0; i < count-1; i++) {

    if (x_pos[i] == x_pos[i+1]) {
      unsigned char* in  = image_in + x_pos[i] + y_pos[i] * in_w;
      unsigned char* out = image_out + m_pad_l + y_pos[i] * out_w;
      for (int y = y_pos[i]; y < y_pos[i+1]; y++) {
        //*(in+w) = *(in+w+1) = 255;
        memcpy(out, in, w);
        in  += in_w;
        out += out_w;
      }
    } else {
      int x0 = x_pos[i];
      int x1 = x_pos[i+1];
      int y0 = y_pos[i];
      int y1 = y_pos[i+1];

      int dx = x1 - x0;
      int dy = y1 - y0;

      for (int y = y0; y < y1; y++) {
        int Iq, Ip;
        int pos = x0 + (y-y0) * dx / dy;

        unsigned char* in  = image_in + pos + y*in_w;
        unsigned char* out = image_out + m_pad_l + y*out_w;

        int a  = dy*(x0-pos) + dx*(y-y0);

        if (dx < 0) {
          Iq = -a * *(in-1) / dy;
          //*(in-1+w) = -a * 255 / dy;
          a += dy;
          //*(in+w) = *(in+w+1) = 255;
          //*(in+w) = a * 255/dy;
        } else {
          Iq = (dy-a) * *in / dy;
          //*(in+w) = *(in+w+1) = 255;
          //*(in+w) = (dy-a) * 255 / dy;
          ++in;
          //*(in+w) = a * 255 / dy;
        }

        for (int i = 0; i < w; i++) {
          Ip = a * *in / dy;
          *out = Iq+Ip;
          Iq = *in - Ip;
          ++in;
          ++out;
        }
      }
    }
  }

  //return imb;
  return m_block;
}


ImageCutter::~ImageCutter() {
  if (m_block != 0) delete m_block;
}
