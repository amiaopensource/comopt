/* PGMWriter.hxx
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


#include <fstream>
#include "ImageProducer.hxx"


class PGMWriter : public ImageProducer {

public:
  PGMWriter(ImageProducer* producer, StatusView* sv, const char* filename,int max_gray_val = 255);

  ImageBlock* produceBlock();

  ~PGMWriter();

private:
  ImageProducer*  source;
  fstream        pgm_out;

  long           width;
  long           height;
  int            max_gray;

  int            max_chars;

  // 'P5', one space, three eol, max_gray (3 digits max)
  static const int const_header_space = 9; 
};
