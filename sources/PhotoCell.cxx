/* PhotoCell.cxx
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


#include "PhotoCell.hxx"


PhotoCell::PhotoCell(ImageProducer* producer, StatusView* sv, int slit_height, int channels) :
  AudioProducer(sv),
  source(producer), 
  tracks(channels),
  block(0)
{
  // track_pos holds the x-coordinates of the track delimiters
  track_pos = new int[tracks+1];

  // Allocate space for the running sums accumulating the slit intensities per track
  slit_intensity = new RunningSum<int>*[tracks];
  for (int t = 0; t < tracks; t++) slit_intensity[t] = new RunningSum<int>(slit_height, 0);
}


AudioBlock* PhotoCell::produceBlock() {
  if (status != 0) status->updateSubtask();

  // Get a new block of image data
  ImageBlock* imb = source->produceBlock();
  if (imb == 0) return 0;

  unsigned char* image = imb->getData();
  int in_w = imb->getWidth();
  int in_h = imb->getHeight();

  // Create an audio block with one sample per image line and track
  if (block == 0) {
    block = new AudioBlock(in_h, tracks);
  }

  float* sound = block->getData();

  // Adjust number of samples
  block->setSamples(in_h);

  // Determine the single track positions.
  track_pos[0] = 0;
  for (int i = 1; i <= tracks; i++) {
    track_pos[i] = (i * in_w) / tracks;
  }

  // Calculate the slit intensities.
  for (int y = 0;  y < in_h;  y++) {

    for (int track = 0;  track < tracks;  track++) {
      int intensity = 0;
      for (int x = track_pos[track]; x < track_pos[track+1]; x++) intensity += image[x + y*in_w];
      
      slit_intensity[track]->add(intensity);
      sound[track + y*tracks] = slit_intensity[track]->getSum();
    }

  }

  return block;
}


PhotoCell::~PhotoCell() {
  if (block != 0) delete block;
  if (track_pos != 0) delete[] track_pos;
  for (int t = 0; t < tracks; t++) { if (slit_intensity[t] != 0) delete slit_intensity[t]; }
  if (slit_intensity != 0) delete[] slit_intensity;
}
