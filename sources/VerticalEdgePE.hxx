/* VerticalEdgePE.hxx
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


class VerticalEdgePE : public PositionEstimator {

public:

  static const int PREFER_NEGATIVE_EDGE = -1; // white-->black
  static const int PREFER_POSITIVE_EDGE =  1; // black-->white

  VerticalEdgePE(ImageProducer* producer, StatusView* sv, int hint, int width, int offs, int edgePreference, int subblockSize = 300, int window = 24);

  ImageBlock* produceBlock();
  int*        getXPositions(int i = 0) { return norm_x_pos; }
  int*        getYPositions(int i = 0) { return cur_y_pos; }
  int         getWidth(int i = 0);
  int         getCount(int i = 0) { return cur_count; }

  ~VerticalEdgePE();

private:
  int  averageEdgePosition(ImageBlock* ib, int hint, int y0, int y1);
  void calcNextPositions(ImageBlock* ib, int hint);

  ImageProducer* source;
  ImageBlock*    cur_block;
  ImageBlock*    nxt_block;

  int*           column_sum;
  int            win;
  int            edge_pref;

  int*           cur_x_pos;
  int*           nxt_x_pos;  
  int*           cur_y_pos;
  int*           nxt_y_pos;
  int*           norm_x_pos;
  
  int            w;
  int            x_off;
  int            cur_count;
  int            nxt_count;
  int            subblock_size;
};
