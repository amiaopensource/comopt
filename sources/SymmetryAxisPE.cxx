/* SymmetryAxisPE.cxx
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


#include "SymmetryAxisPE.hxx"


SymmetryAxisPE::SymmetryAxisPE(ImageProducer* producer, StatusView* sv, int tracks, bool positive, int subblock_size) :
  PositionEstimator(sv),
  m_source(producer),
  m_tracks(tracks),
  m_positive(positive),
  m_subblock_size(subblock_size),
  m_sums(0)
{  
  m_x_pos = new int*[m_tracks];
  m_y_pos = new int*[m_tracks];
}


int SymmetryAxisPE::estimateSymmetryAxis(unsigned char *image, int img_w, int x0, int y0, int x1, int y1) {
  static const int MINMAX_OFFSET = 10;
  int min = 255;
  int max = 0;
  int min_pos;
  int max_pos;
  int axis;

  // calculate offsets into image
  int left  = x0 + y0 * img_w;
  int right = x1 + y0 * img_w;

  // calculate column averages for the subblock
  for (int i = left; i < right; i++) {
    int col_sum = 0;
    int y_pos   = i;
    for (int l = y0; l < y1; l++) {
      col_sum += image[y_pos];
      y_pos   += img_w;
    }
    m_sums[i-left] = col_sum / (y1-y0);
  }

  // determine minimum and its position
  for (int i = 0; i < right-left; i++)
    if (m_sums[i] < min) { min = m_sums[i]; min_pos = i; }
  min += MINMAX_OFFSET;

  // determine maximum and its position
  for (int i = 0; i < right-left; i++)
    if (m_sums[i] > max) { max = m_sums[i]; max_pos = i; }
  max -= MINMAX_OFFSET;

  int l = 0;
  int r = 0;

  // calculate position of corresponding intensities on opposite flanks
  // distinguish positive and negative tracks
  if (m_positive) {
    for (int i = min; i <= max; i++) {
      int x = max_pos;
      while (m_sums[x] > i) x--;
      l += x;
      x = max_pos;
      while (m_sums[x] > i) x++;
      r += x;
    }
  }

  else {
    for (int i = min; i <= max; i++) {
      int x = min_pos;
      while (m_sums[x] < i) x--;
      l += x;
      x = min_pos;
      while (m_sums[x] < i) x++;
      r += x;
    }
  }

  axis = (int)((r+l)/(2.0 * (max-min+1)) + 0.5);
  //int pos = left+axis;
  //for (int p = y0; p < y1; p++, pos+=img_w) image[pos] = 0;

  return x0 + axis;
}


ImageBlock* SymmetryAxisPE::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = m_source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            img_w = block->getWidth();
  int            img_h = block->getHeight();
  
  // calculate number of subblocks
  m_subblocks = block->getHeight() / m_subblock_size;

  // lazy initialization
  if (m_sums == 0) {
    m_sums = new int[img_w / m_tracks + 1];
    for (int t = 0; t < m_tracks; t++) {
      m_x_pos[t] = new int[m_subblocks+1];
      m_y_pos[t] = new int[m_subblocks+1];
    }
  }

  // calculate symmetry axis for every subblock in each track
  for (int track = 0;  track < m_tracks;  track++) {
    int left  =  track    * img_w / m_tracks;
    int right = (track+1) * img_w / m_tracks;

    int top = 0;
    for (int sub = 0;  sub < m_subblocks-1;  sub++) {
      m_x_pos[track][sub] = estimateSymmetryAxis(image, img_w, left, top, right, top + m_subblock_size);
      m_y_pos[track][sub] = top;
      top += m_subblock_size;
    }

    m_x_pos[track][m_subblocks-1] = estimateSymmetryAxis(image, img_w, left, top, right, img_h);
    m_y_pos[track][m_subblocks-1] = top;

    m_x_pos[track][m_subblocks] = m_x_pos[track][m_subblocks-1];
    m_y_pos[track][m_subblocks] = img_h;
  }

  return block;
}


SymmetryAxisPE::~SymmetryAxisPE() {
  for (int t = 0; t < m_tracks; t++) {
    if (m_x_pos[t] != 0) delete[] m_x_pos[t];
    if (m_y_pos[t] != 0) delete[] m_y_pos[t];
  }
  delete[] m_x_pos;
  delete[] m_y_pos;

  if (m_sums != 0) delete[] m_sums;
}
