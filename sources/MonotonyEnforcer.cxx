/* MonotonyEnforcer.cxx
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


#include "MonotonyEnforcer.hxx"


MonotonyEnforcer::MonotonyEnforcer(ImageProducer* producer, StatusView* sv, int tracks, bool positive, bool left2right, SymmetryAxisPE* symm_pe) :
  ImageProducer(sv),
  m_source(producer),
  m_tracks(tracks),
  m_positive(positive),
  m_left_to_right(left2right),
  m_symmetry_axis(symm_pe)
{
}


void MonotonyEnforcer::interpolate(unsigned char *image, int from, int to) {
  if (image[from] == image[to]) {
    unsigned char val = image[from];
    for (int i = from+1; i < to; i++) image[i] = val;
  }
  
  else {
    float m   = (float)(image[to] - image[from]) / (float)(to - from);
    float inc = 0.0;

    for (int i = from+1; i < to; i++) {
      inc += m;
      image[i] = image[from] + (unsigned char)(inc+0.5);
    }
  }
}


void MonotonyEnforcer::makeMonotonousDec(unsigned char* image, int img_w, int x0, int y0, int x1, int y1, bool left_to_right) {
  int           pos, from, to, end;
  unsigned char val;

  if (left_to_right) {
    for (int i = y0; i < y1; i++) {
      pos = x0 + i*img_w;
      end = x1 + i*img_w;

      while (pos < end) {
        val = image[pos++];
        while ((pos < end) && (image[pos] <= val)) val = image[pos++];
        from = pos - 1;
        while ((pos < end) && (image[pos] >= val)) ++pos;
        to = pos;
        if (to == end) image[--to] = image[from];
        interpolate(image, from, to);
      }
    }
  }

  else {
   for (int i = y0;  i < y1; i++) {
      end = x0 + i*img_w - 1;
      pos = x1 + i*img_w - 1;

      while (pos > end) {
        val = image[pos--];
        while ((pos > end) && (image[pos] >= val)) val = image[pos--];
        to = pos + 1;
        while ((pos > end) && (image[pos] <= val)) --pos;
        from = pos;
        if (from == end) image[++from] = image[to];
        interpolate(image, from, to);
      }
    }
  }
}


void MonotonyEnforcer::makeMonotonousInc(unsigned char* image, int img_w, int x0, int y0, int x1, int y1, bool left_to_right) {
  int           pos, from, to, end;
  unsigned char val;

  if (left_to_right) {
    for (int i = y0;  i < y1; i++) {
      pos = x0 + i*img_w;
      end = x1 + i*img_w;

      while (pos < end) {
        val = image[pos++];
        while ((pos < end) && (image[pos] >= val)) val = image[pos++];
        from = pos - 1;
        while ((pos < end) && (image[pos] <= val)) ++pos;
        to = pos;
        if (to == end) image[--to] = image[from]; 
        interpolate(image, from, to);
      }
    }
  } else {
    for (int i = y0; i < y1; i++) {
      end = x0 + i*img_w - 1;
      pos = x1 + i*img_w - 1;

      while (pos > end) {
        val = image[pos--];
        while ((pos > end) && (image[pos] <= val)) val = image[pos--];
        to = pos + 1;
        while ((pos > end) && (image[pos] >= val)) --pos;
        from = pos;
        if (from == end) image[++from] = image[to];
        interpolate(image, from, to);
      }
    }
  }
}


ImageBlock* MonotonyEnforcer::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = m_source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            img_w = block->getWidth();
  int            img_h = block->getHeight();

  for (int track = 0;  track < m_tracks;  track++) {
    int x_left  = track     * img_w / m_tracks;
    int x_right = (track+1) * img_w / m_tracks;

    if (m_symmetry_axis == 0) {
      if (m_positive) 
        makeMonotonousDec(image, img_w, x_left, 0, x_right, img_h, m_left_to_right);
      else
        makeMonotonousInc(image, img_w, x_left, 0, x_right, img_h, m_left_to_right);
    } else {
      int* x_pos = m_symmetry_axis->getXPositions(track);
      int* y_pos = m_symmetry_axis->getYPositions(track);

      for (int i = 0; i < m_symmetry_axis->getCount()-1; i++) {
        if (m_positive) {
          makeMonotonousInc(image, img_w, x_left, y_pos[i], x_pos[i], y_pos[i+1], m_left_to_right);
          makeMonotonousDec(image, img_w, x_pos[i], y_pos[i], x_right, y_pos[i+1], !m_left_to_right);
        } else {
          makeMonotonousDec(image, img_w, x_left, y_pos[i], x_pos[i], y_pos[i+1], m_left_to_right);
          makeMonotonousInc(image, img_w, x_pos[i], y_pos[i], x_right, y_pos[i+1], !m_left_to_right);
        }
      }
    }
  }

  return block;
}


MonotonyEnforcer::~MonotonyEnforcer() {
}
