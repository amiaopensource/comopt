/* SymmetryEnforcer.hxx
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
#include "SymmetryAxisPE.hxx"


class SymmetryEnforcer : public ImageProducer {
public:
  SymmetryEnforcer(ImageProducer* producer, StatusView* sv, SymmetryAxisPE* symm_pe, bool positive, int diff_allowed);
  ImageBlock* produceBlock();
  ~SymmetryEnforcer();

private:
  void enforceSymmetryNegative(unsigned char *image, int img_w, int axis, int x0, int y0, int x1, int y1);
  void enforceSymmetryPositive(unsigned char *image, int img_w, int axis, int x0, int y0, int x1, int y1);

  ImageProducer*  m_source;
  SymmetryAxisPE* m_symmetry_axis;
  bool            m_positive;
  int             m_diff_allowed;
};
