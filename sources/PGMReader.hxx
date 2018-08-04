/* PGMReader.hxx
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


// PGMReader is an implementation of the abstract class ImageProducer.
// PGMReader can handle grayscale binary PNM files (type P5). A call
// to produceBlock() reads binary image data into the ImageBlock
// structure block and returns it to the caller.

class PGMReader : public ImageProducer {

public:
  PGMReader(StatusView* sv, const char* filename, int blockLines, int skip = 0);
  ~PGMReader();

  ImageBlock* produceBlock();
  int getImageWidth()  { return m_img_w; }
  int getImageHeight() { return m_img_h; }

  class FileOpenExcpt {};
  class FileFormatExcpt {};

private:
  fstream     m_image_in;
  ImageBlock* block;

  int     m_img_w;
  int     m_img_h;
  int     m_max_gray_val;
  int     m_block_lines;

  void ignoreHeaderComment(fstream& is);
};
