/* AudioBlock.hxx
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


#ifndef _AUDIOBLOCK
#define _AUDIOBLOCK


class AudioBlock {

public:
  AudioBlock(int samples, int channels);
  ~AudioBlock();

  float* getData() { return m_data; }

  int getSamples() { return m_samples; }
  void setSamples(int samples) { m_samples = samples; }

  int getChannels() { return m_channels; }
  void setChannels(int channels) { m_channels = channels; }

private: 
  float* m_data;
  int    m_samples;
  int    m_channels;
};


#endif
