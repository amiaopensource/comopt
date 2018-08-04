/* MonotonyEnforcer.hxx
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


class MonotonyEnforcer: public ImageProducer {

public:
  MonotonyEnforcer(ImageProducer* producer, StatusView* sv, int tracks, bool positive, bool left2right, SymmetryAxisPE* symm_pe = 0);
  ImageBlock* produceBlock();
  ~MonotonyEnforcer();

private:
  void makeMonotonousDec(unsigned char* image, int img_w, int x0, int y0, int x1, int y1, bool left_to_right);
  void makeMonotonousInc(unsigned char* image, int img_w, int x0, int y0, int x1, int y1, bool left_to_right);
  void interpolate(unsigned char* image, int from, int to);
  
  ImageProducer*   m_source;
  SymmetryAxisPE*  m_symmetry_axis;
  bool             m_positive;
  bool             m_left_to_right;
  int              m_tracks;
};
