/* ImageBlock.hxx
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


#ifndef _IMAGEBLOCK
#define _IMAGEBLOCK


class ImageBlock {

public:
  ImageBlock(int width, int height);
  ~ImageBlock();

  unsigned char* getData() { return m_data; }

  int  getWidth() { return m_width; }
  void setWidth(int width) { m_width = width; }

  int getHeight() { return m_height; }
  void setHeight(int height) { m_height = height; }

  void copy(ImageBlock* block);

private:
  unsigned char* m_data;
  int            m_width;
  int            m_height;
};


#endif
