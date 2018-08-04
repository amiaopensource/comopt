/* PhotoCell.hxx
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


#include "AudioProducer.hxx"
#include "ImageProducer.hxx"
#include "RunningSum.cxx"


class PhotoCell : public AudioProducer {
  
public:
  PhotoCell(ImageProducer* producer, StatusView* sv, int slit_height, int channels);

  AudioBlock* produceBlock();
  AudioBlock* getBlock() { return block; }

  ~PhotoCell();

private:
  ImageProducer*    source;
  AudioBlock*       block;

  int               tracks;

  int*              track_pos;
  RunningSum<int>** slit_intensity;
};
