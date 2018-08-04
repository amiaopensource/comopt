/* ImageBlock.cxx
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


#include <new>
#include "ImageBlock.hxx"
#include <string.h>


ImageBlock::ImageBlock(int width, int height) :
  m_width(width),
  m_height(height)
{
  m_data = new unsigned char[m_width * m_height];
}


void ImageBlock::copy(ImageBlock* block) {
  m_width  = block->getWidth();
  m_height = block->getHeight();
  memcpy(m_data, block->getData(), m_width * m_height);
}


ImageBlock::~ImageBlock() {
  delete[] m_data;
}
