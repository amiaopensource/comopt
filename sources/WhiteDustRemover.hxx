/* WhiteDustRemover.hxx
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
#include "EquivalenceTable.hxx"


class WhiteDustRemover : public ImageProducer {

public:
  WhiteDustRemover(ImageProducer* producer, StatusView* sv, int tracks, bool symmetric, bool positive);
  ImageBlock* produceBlock();
  ~WhiteDustRemover();

private:
  void initFirstLabelLine(int w);
  void initLabels(int w, int h);

  ImageProducer*   m_source;
  EquivalenceTable m_equivalence_table;

  int*             m_labels;
  int              m_current_label;

  float*           m_white_area_pos;
  float*           m_static_label_pos;
  int              m_white_areas;

  static const int           BACKGROUND = 0;
  static const unsigned char BLACK      = 0;
};
