/* PGMWriter.cxx
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


#include <limits>
#include <iostream>
#include <math.h>
#include "PGMWriter.hxx"


PGMWriter::PGMWriter(ImageProducer* producer, StatusView* sv, const char* filename, int max_gray_val) :
  ImageProducer(sv),
  source(producer),
  pgm_out(filename, ios::out),
  width(0), height(0),
  max_gray(max_gray_val)
{
  // reserve header space
  max_chars = 1 + (int)log10(numeric_limits<long>::max());
  pgm_out.seekp(2*max_chars + const_header_space);
}


ImageBlock* PGMWriter::produceBlock() {
  if (status != 0) status->updateSubtask();

  ImageBlock* block = source->produceBlock();
  if (block == 0) return 0;

  if (width == 0) width = block->getWidth();
  height += block->getHeight();

  pgm_out.write(block->getData(), block->getWidth() * block->getHeight());

  return block;
}


PGMWriter::~PGMWriter() { 
  pgm_out.seekp(0);

  pgm_out << "P5" << endl;

  pgm_out.width(max_chars);
  pgm_out << width << ' ';

  pgm_out.width(max_chars);
  pgm_out << height << endl;

  pgm_out.width(3);
  pgm_out << max_gray << endl;
}
