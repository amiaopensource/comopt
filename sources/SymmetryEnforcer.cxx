/* SymmetryEnforcer.cxx
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


#include "SymmetryEnforcer.hxx"
#include <stdlib.h>
#include <iostream>


SymmetryEnforcer::SymmetryEnforcer(ImageProducer* producer, StatusView* sv, SymmetryAxisPE* symm_pe, bool positive, int diff_allowed) :
  ImageProducer(sv),
  m_source(producer),
  m_symmetry_axis(symm_pe),
  m_positive(positive),
  m_diff_allowed(diff_allowed)
{
}


void SymmetryEnforcer::enforceSymmetryNegative(unsigned char* image, int img_w, int axis, int x0, int y0, int x1, int y1) {
  int n;
  if (axis-x0 < x1-axis) n = axis-x0;
  else n = x1-axis;

  axis += y0 * img_w;

  for (int line = y0; line < y1; line++) {
    for (int i = 1; i <= n; i++) {
      int l = axis-i;
      int r = axis+i;
      if (abs(image[l]-image[r]) > m_diff_allowed) {
	if (image[l] < image[r]) image[l] = image[r];
	else image[r] = image[l];
      }
    }
    axis += img_w;
  }
}


void SymmetryEnforcer::enforceSymmetryPositive(unsigned char* image, int img_w, int axis, int x0, int y0, int x1, int y1) {
  int n;
  if (axis-x0 < x1-axis) n = axis-x0; 
  else n = x1-axis;

  axis += y0 * img_w;

  for (int line = y0; line < y1; line++) {
    for (int i = 1; i <= n; i++) {
      int l = axis-i;
      int r = axis+i;
      if (abs(image[l]-image[r]) > m_diff_allowed) {
        if (image[l] > image[r]) image[l] = image[r];
        else image[r] = image[l];
      }
    }
    axis += img_w;
  }
}


ImageBlock* SymmetryEnforcer::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = m_source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            img_w = block->getWidth();
  int            img_h = block->getHeight();

  int tracks     = m_symmetry_axis->getTracks();
  int sub_blocks = m_symmetry_axis->getCount();

  for (int track = 0;  track < tracks;  track++) {
    int* axis_x_pos = m_symmetry_axis->getXPositions(track);
    int* axis_y_pos = m_symmetry_axis->getYPositions(track);

    int left  = track     * img_w / tracks;
    int right = (track+1) * img_w / tracks;

    if (m_positive)
      for (int sub = 0;  sub < sub_blocks;  sub++)
        enforceSymmetryPositive(image, img_w, axis_x_pos[sub], left, axis_y_pos[sub], right, axis_y_pos[sub+1]);
    else
      for (int sub = 0;  sub < sub_blocks;  sub++)
        enforceSymmetryNegative(image, img_w, axis_x_pos[sub], left, axis_y_pos[sub], right, axis_y_pos[sub+1]);
  }

  return block;
}


SymmetryEnforcer::~SymmetryEnforcer() {
}
