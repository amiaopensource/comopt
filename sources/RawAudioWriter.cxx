/* RawAudioWriter.cxx
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


#include "RawAudioWriter.hxx"


RawAudioWriter::RawAudioWriter(AudioProducer* producer, StatusView* sv, const char* filename) :
  AudioProducer(sv),
  source(producer),
  raw_out(filename, ios::out)
{
}


AudioBlock* RawAudioWriter::produceBlock() {
  if (status != 0) status->updateSubtask();

  AudioBlock* block = source->produceBlock();
  if (block == 0) {
    raw_out.close();
    return 0;
  }

  raw_out.write((unsigned char *)block->getData(), block->getSamples() * block->getChannels() * sizeof(float));

  return block;
}
