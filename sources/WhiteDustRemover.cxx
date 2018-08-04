/* WhiteDustRemover.cxx
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


#include "WhiteDustRemover.hxx"


WhiteDustRemover::WhiteDustRemover(ImageProducer* producer, StatusView* sv, int tracks, bool symmetric, bool positive) : 
  ImageProducer(sv),
  m_source(producer),
  m_equivalence_table(10000),
  m_labels(0)
{
  if (!symmetric && positive) {
    float area_w       = 0.5 / tracks;
    m_white_areas      = tracks;
    m_white_area_pos   = new float[2*m_white_areas];
    m_static_label_pos = new float[m_white_areas];

    for (int t = 0; t < tracks; t++) {
      float p = (float)t / tracks;
      m_white_area_pos[t*2]   = p;
      m_white_area_pos[t*2+1] = p+area_w;
      m_static_label_pos[t]   = p;
    }
  } else if (!symmetric && !positive) {
    float area_w       = 0.5 / tracks;
    m_white_areas      = tracks;
    m_white_area_pos   = new float[2*m_white_areas];
    m_static_label_pos = new float[m_white_areas];

    for (int t = 0; t < tracks; t++) {
      float p = (t+1.0) / tracks;
      m_white_area_pos[t*2]   = p-area_w;
      m_white_area_pos[t*2+1] = p;
      m_static_label_pos[t]   = p;
    }
  } else if (symmetric && positive) {
    float area_w       = 0.5 / tracks;
    m_white_areas      = tracks;
    m_white_area_pos   = new float[2*m_white_areas];
    m_static_label_pos = new float[m_white_areas];

    for (int t = 0; t < tracks; t++) {
      float p = (float)t / tracks;
      cout << "p=" << p << endl;
      m_white_area_pos[t*2]   = p+area_w/2;
      m_white_area_pos[t*2+1] = m_white_area_pos[t*2] + area_w;
      m_static_label_pos[t]   = p+area_w;
    }
  } else {
    float area_w       = 0.5 / tracks;
    m_white_areas      = tracks + 1;
    m_white_area_pos   = new float[2*m_white_areas];
    m_static_label_pos = new float[m_white_areas];

    m_white_area_pos[0]   = 0.0;
    m_white_area_pos[1]   = 0.25/tracks;
    m_static_label_pos[0] = 0.0;

    for (int t = 1; t < tracks; t++) {
      float p = (float)t / tracks;
      m_white_area_pos[t*2]   = p-area_w/2;
      m_white_area_pos[t*2+1] = p+area_w/2;
      m_static_label_pos[t]   = p;
    }

    m_white_area_pos[2*tracks]   = 1.0-area_w/2;
    m_white_area_pos[2*tracks+1] = 1.0;
    m_static_label_pos[tracks]   = 1.0;
  }
}


void WhiteDustRemover::initFirstLabelLine(int w) {
  for (int p = 0; p < w; p++) { m_labels[p] = BACKGROUND; }
  for (int a = 0; a < m_white_areas; a++) {
    for (int p = m_white_area_pos[2*a] * (w-1); p < m_white_area_pos[2*a+1] * (w-1); p++) {
      m_labels[p+1] = a+1;
    }
  }
}


void WhiteDustRemover::initLabels(int w, int h) {
  for (int p = 0; p < w; p++) {
    if (m_labels[p] > m_white_areas) m_labels[p] = BACKGROUND;
  }
  for (int p = w; p < w * h; p++) m_labels[p] = BACKGROUND;
  for (int p = 0; p < m_white_areas;  p++) {
    int pos = m_static_label_pos[p] * (w-1);
    for (int i = 1; i < h; i++) {
      pos += w;
      m_labels[pos] = p+1;
    }
  }
}


ImageBlock* WhiteDustRemover::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = m_source->produceBlock();
  if (block == 0) return 0;

  unsigned char* image = block->getData();
  int            w     = block->getWidth() + 1;
  int            h     = block->getHeight() + 1;

  if (m_labels == 0) {
    m_labels = new int[w*h];
    initFirstLabelLine(w);
  }

  initLabels(w, h);
  m_current_label = m_white_areas;
  m_equivalence_table.reset();

  for (int l = 1; l < h;  l++) {
    for (int p = 1; p < w;  p++) {
      if (image[(p-1)+(l-1)*(w-1)] > BLACK) {
        int pos  = p+l*w;
        int left = m_labels[pos-1];
        int top  = m_labels[pos-w];

        if ((left == BACKGROUND) && (top == BACKGROUND)) {
          m_labels[pos] = ++m_current_label;
        } else if ((left == BACKGROUND) && (top > BACKGROUND)) {
          m_labels[pos] = top;
        }nelse if ((left > BACKGROUND) && (top == BACKGROUND)) {
          m_labels[pos] = left;
        } else {
          if (left == top) {
            m_labels[pos] = left;
          } else {
            if (left < top) m_labels[pos] = left; 
            else m_labels[pos] = top;
            m_equivalence_table.addEquivalence(left, top);
          }
        }
      }
    }
  }

  m_equivalence_table.normalize();

  for (int l = 1; l < h; l++) {
    for (int p = 1; p < w; p++) {
      int eq = m_equivalence_table.getEquivalent(m_labels[p+l*w]);
      if (eq > m_white_areas) image[(p-1)+(l-1)*(w-1)] = BLACK;
    }
  }

  return block;
}


WhiteDustRemover::~WhiteDustRemover() {
  if (m_labels != 0) delete[] m_labels;
  if (m_white_area_pos != 0) delete[] m_white_area_pos;
  if (m_static_label_pos != 0) delete[] m_static_label_pos;
}
