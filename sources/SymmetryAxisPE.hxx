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


#ifndef _SYMMETRYAXISPE
#define _SYMMETRYAXISPE

#include "PositionEstimator.hxx"


class SymmetryAxisPE: public PositionEstimator {

public:
  SymmetryAxisPE(ImageProducer* producer, StatusView* sv, int tracks, bool positive, int subblockSize);

  ImageBlock*  produceBlock();

  int* getXPositions(int i = 0) { return m_x_pos[i]; }
  int* getYPositions(int i = 0) { return m_y_pos[i]; }
  int  getWidth(int i = 0) { return 0; }
  int  getCount(int i = 0) { return m_subblocks + 1; }
  int  getTracks() { return m_tracks; }

  ~SymmetryAxisPE();

private:
  int estimateSymmetryAxis(unsigned char *image, int img_w, int x0, int y0, int x1, int y1);

  ImageProducer* m_source;

  int    m_tracks;
  int    m_subblock_size;
  int    m_subblocks;
  int**  m_x_pos;
  int**  m_y_pos;
  int*   m_sums;
  bool   m_positive;
};


#endif
