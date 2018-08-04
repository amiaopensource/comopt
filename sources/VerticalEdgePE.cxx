/* VerticalEdgePE.cxx
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


#include "VerticalEdgePE.hxx"


VerticalEdgePE::VerticalEdgePE(ImageProducer* producer, StatusView* sv, int hint, int width, int offs, int edgePreference, int subblockSize, int window) :
  PositionEstimator(sv),
  source(producer),
  w(width),
  x_off(offs),
  edge_pref(edgePreference),
  win(window),
  subblock_size(subblockSize)
{
  column_sum    = new int[win];
  nxt_block     = source->produceBlock();
  cur_block     = new ImageBlock(nxt_block->getWidth(), nxt_block->getHeight());
  int positions = nxt_block->getHeight() / subblock_size + 2;
  cur_x_pos     = new int[positions];
  nxt_x_pos     = new int[positions];
  cur_y_pos     = new int[positions];
  nxt_y_pos     = new int[positions];
  norm_x_pos    = new int[positions];
  calcNextPositions(nxt_block, hint);
}


int VerticalEdgePE::averageEdgePosition(ImageBlock* ib, int hint, int y0, int y1) {
  unsigned char* image  = ib->getData();
  int            w      = ib->getWidth();
  int            x_offs = hint - win/2;

  for (int x = 0; x < win; x++) column_sum[x] = 0;

  for (int y = y0; y < y1; y++) {
    int pos = x_offs + y*w;
    for (int x = 0; x < win; x++) column_sum[x] += image[pos+x];
  }

  for (int x = 0; x < win; x++) column_sum[x] /= (y1-y0);

  int max   = 0;
  int max_x = win/2;

  for (int x = 1; x < win-1; x++) {
    int gradient = (column_sum[x+1] - column_sum[x-1]) * edge_pref;
    if (gradient > max) {
      max   = gradient;
      max_x = x;
    }
  }

  return x_offs + max_x;
}


void VerticalEdgePE::calcNextPositions(ImageBlock* ib, int hint) {
  int block_height = ib->getHeight();
  int subblocks    = block_height / subblock_size;
  nxt_count        = subblocks + 2;

  //cout << "hint " << hint << endl;

  nxt_x_pos[0] = hint;
  nxt_y_pos[0] = 0;

  if (subblocks > 0) {
    for (int i = 0;  i < subblocks;  i++) {
      int y0 = i * block_height / subblocks;
      int y1 = (i+1) * block_height / subblocks;
      nxt_x_pos[i+1] = averageEdgePosition(ib, nxt_x_pos[i], y0, y1);
      nxt_y_pos[i+1] = (y0 + y1) / 2 ;
    }
  }

  nxt_x_pos[nxt_count-1] = nxt_x_pos[subblocks]; // sentinel
  nxt_y_pos[nxt_count-1] = block_height;
}  


ImageBlock* VerticalEdgePE::produceBlock() {
  if (status != 0) status->updateSubtask();

  if (nxt_block == 0) return 0;
  cur_block->copy(nxt_block);
  nxt_block = source->produceBlock();

  cur_count = nxt_count;

  int* swap = cur_x_pos;
  cur_x_pos = nxt_x_pos;
  nxt_x_pos = swap;

  swap      = cur_y_pos;
  cur_y_pos = nxt_y_pos;
  nxt_y_pos = swap;

  if (nxt_block != 0) {
    calcNextPositions(nxt_block, cur_x_pos[cur_count-2]);
    nxt_x_pos[0] = cur_x_pos[cur_count-1] = (nxt_x_pos[1] + cur_x_pos[cur_count-2]) / 2;
  }

  // Normalize positions (position equals left edge, width is positive)
  for (int i = 0; i < cur_count; i++) norm_x_pos[i] = cur_x_pos[i] + x_off;

  if (w < 0) {
    for (int i = 0; i < cur_count; i++) norm_x_pos[i] = norm_x_pos[i] + w;
  }

  return cur_block;
}


int VerticalEdgePE::getWidth(int i) {
  if (w < 0) return -w;
  return w;
}


VerticalEdgePE::~VerticalEdgePE() {
  if (column_sum != 0) delete[] column_sum;
  if (cur_x_pos  != 0) delete[] cur_x_pos;
  if (nxt_x_pos  != 0) delete[] nxt_x_pos;
  if (cur_y_pos  != 0) delete[] cur_y_pos;
  if (nxt_y_pos  != 0) delete[] nxt_y_pos;
  if (norm_x_pos != 0) delete[] norm_x_pos;
  if (cur_block  != 0) delete   cur_block;
}
