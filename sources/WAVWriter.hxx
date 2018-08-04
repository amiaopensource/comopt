/* WAVWriter.hxx
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
#include "AudioProducer.hxx"
#include "AudioDataCollector.hxx"


// Endian

namespace Endian {

  typedef int   int32;
  typedef short int16;

  typedef union fb {
    unsigned char b[4];
    int32 i;
  } FOURBYTES;


  typedef union tb {
    unsigned char b[2];
    int16 i;
  } TWOBYTES;


  bool bigEndian();
  int32 swap_int32(int i);
  int16 swap_int16(int i);
}


// WAVWriter

class WAVWriter : public AudioProducer {

public:

  static const int WAVE_FORMAT_PCM = 0x0001;

  WAVWriter(AudioProducer* producer, StatusView* sv, AudioDataCollector* collector, const char* filename, int type = WAVE_FORMAT_PCM);
  AudioBlock* produceBlock();
  ~WAVWriter();

private:

  struct WAVHeader {
    Endian::int32  RIFF;               // RIFF header
    Endian::int32  fileLength;         // file length from here
    Endian::int32  WAVE;               // WAV header
    Endian::int32  fmt;                // fmt chunk header
    Endian::int32  nextBlockLength;    // length of next block
    Endian::int16  type;               // format tag (0x0001 == Microsoft PCM format)
    Endian::int16  nofChannels;        // number of channels
    Endian::int32  samplesPerSec;      // sampling rate
    Endian::int32  avgBytesPerSec;     // average number of bytes per second
    Endian::int16  blockAlign;         // block alignment
    Endian::int16  bitsPerSample;      // bits per sample
    Endian::int32  data;               // start of data section
    Endian::int32  nofDataBytes;       // number of data bytes
  };

  WAVHeader           wavheader;

  AudioProducer*      source;
  AudioDataCollector* info;

  fstream             audio_out;       // output stream

  unsigned char*      data8;           // block of 8bit data
  Endian::int16*      data16;          // block of 16bit data

  int                 bytes_per_sample;
  Endian::int32       bytes_written;

  float               min_a;           // minimum amplitude
  float               range;           // range of amplitudes
  float               type_min;        // lowest value possible
  float               type_max;        // highest value possible
};
