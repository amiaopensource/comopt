/* PGMReader.cxx
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


#include "PGMReader.hxx"


// Constructor
// Opens an input stream on a file and parses the PGM header
// Throws a FileOpenError or a FormatError exception if the
// file does not exist or is of a wrong type.
 
PGMReader::PGMReader(StatusView* sv, const char *filename, int blockLines, int skip) :
  ImageProducer(sv),
  m_image_in(filename, ios::in),
  m_block_lines(blockLines)
{
  if (!m_image_in) throw FileOpenExcpt();

  char magic;
  m_image_in >> magic;
  if (magic != 'P') throw FileFormatExcpt();

  m_image_in >> magic;
  if (magic != '5') throw FileFormatExcpt();

  ignoreHeaderComment(m_image_in);
  m_image_in >> m_img_w;

  ignoreHeaderComment(m_image_in);
  m_image_in >> m_img_h;

  ignoreHeaderComment(m_image_in);
  m_image_in >> m_max_gray_val;

  m_image_in.get(magic);

  if (!m_image_in.good()) throw FileFormatExcpt();

  block = new ImageBlock(m_img_w, m_block_lines);

  if (skip > 0) m_image_in.seekp(skip * m_img_w, ios::cur);
}



// nextBlock reads a block of image data from the input stream
// into an ImageBlock structure, thereby adjusting the
// dimensions if not enough data is available.

ImageBlock* PGMReader::produceBlock() {
  if (status != 0) status->updateSubtask();
  
  if (m_image_in.eof()) {
    m_image_in.close();
    return 0;
  }

  m_image_in.read(block->getData(), block->getWidth() * m_block_lines);
  block->setHeight(m_image_in.gcount() / block->getWidth());

  if (block->getHeight() == 0) return 0;
  
  return block;
}


// Skips PNM header comments beginning with '#'

void PGMReader::ignoreHeaderComment(fstream& is) {
  char c;
 
  is >> c;
  is.putback(c);
  
  while (c == '#') {
    is.ignore(70, '\n');
    is >> c;
    is.putback(c);
  }
}


// Destructor
// Does the clean-up

PGMReader::~PGMReader() {
  if (block != 0) delete block;
}
