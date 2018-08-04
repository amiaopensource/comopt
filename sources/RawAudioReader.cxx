/* RawAudioReader.cxx
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


#include "RawAudioReader.hxx"
#include <iostream>


RawAudioReader::RawAudioReader(StatusView* sv, const char* filename, int samples, int channels) :
  AudioProducer(sv),
  raw_in(filename, ios::in)
{
  block = new AudioBlock(samples, channels);
  raw_in.seekp(0);
}


AudioBlock* RawAudioReader::produceBlock() {
  if (status != 0) status->updateSubtask();

  if (raw_in.eof()) {
    raw_in.close();
    return 0;
  }

  raw_in.read((unsigned char *)block->getData(), block->getSamples() * block->getChannels() * sizeof(float));
  block->setSamples(raw_in.gcount() / (block->getChannels() * sizeof(float)));

  return block;
}


RawAudioReader::~RawAudioReader() {
  if (block != 0) delete block;
}
