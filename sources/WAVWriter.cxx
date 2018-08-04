/* WAVWriter.cxx
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
#include "WAVWriter.hxx"
#include <iostream>

#define _LITTLE_ENDIAN


// Implementation of namespace Endian

bool Endian::bigEndian() {
  Endian::FOURBYTES big;
  big.b[0] = 0xb0;
  big.b[1] = 0xb1;
  big.b[2] = 0xb2;
  big.b[3] = 0xb3;
  return (big.i == 0xb0b1b2b3);
}


Endian::int32 Endian::swap_int32(int i) {
  Endian::FOURBYTES big, little;
  big.i = (int32)i;
  little.b[0] = big.b[3];
  little.b[1] = big.b[2];
  little.b[2] = big.b[1]; 
  little.b[3] = big.b[0];
  return little.i;
}


Endian::int16 Endian::swap_int16(int i) {
  Endian::TWOBYTES big, little;
  big.i = (int16)i;
  little.b[0] = big.b[1];
  little.b[1] = big.b[0];
  return little.i;
}


// Implementation of class WAVWriter

WAVWriter::WAVWriter(AudioProducer* producer, StatusView* sv, AudioDataCollector* collector, const char* filename, int type) :
  AudioProducer(sv),
  audio_out(filename, ios::out),
  source(producer),
  info(collector),
  bytes_written(0),
  data8(0),
  data16(0)
{
  const int RIFF = 0x46464952;
  const int WAVE = 0x45564157;
  const int fmt  = 0x20746d66;
  const int data = 0x61746164;

  int srate        = info->getSamplingrate();
  int ch           = info->getChannels();
  bytes_per_sample = info->getBytesPerSample();

#ifndef _LITTLE_ENDIAN
  wavheader.RIFF            = Endian::swap_int32(RIFF);
  wavheader.WAVE            = Endian::swap_int32(WAVE);
  wavheader.fmt             = Endian::swap_int32(fmt );
  wavheader.data            = Endian::swap_int32(data);
  wavheader.nextBlockLength = Endian::swap_int32(0x10);
  wavheader.type            = Endian::swap_int16(type);
  wavheader.nofChannels     = Endian::swap_int16(ch);
  wavheader.samplesPerSec   = Endian::swap_int32(srate);
  wavheader.avgBytesPerSec  = Endian::swap_int32(srate * ch * bytes_per_sample);
  wavheader.blockAlign      = Endian::swap_int16(ch * bytes_per_sample);
  wavheader.bitsPerSample   = Endian::swap_int16(bytes_per_sample * 8);
#endif  

#ifdef _LITTLE_ENDIAN
  wavheader.RIFF            = (Endian::int32)RIFF;
  wavheader.WAVE            = (Endian::int32)WAVE;
  wavheader.fmt             = (Endian::int32)fmt;
  wavheader.data            = (Endian::int32)data;
  wavheader.nextBlockLength = (Endian::int32)0x10;
  wavheader.type            = (Endian::int16)type;
  wavheader.nofChannels     = (Endian::int16)ch;
  wavheader.samplesPerSec   = (Endian::int32)srate;
  wavheader.avgBytesPerSec  = (Endian::int32)(srate * ch * bytes_per_sample);
  wavheader.blockAlign      = (Endian::int16)(ch * bytes_per_sample);
  wavheader.bitsPerSample   = (Endian::int16)(bytes_per_sample * 8);
#endif

  audio_out.write((unsigned char *)&wavheader, sizeof(wavheader));

  if (bytes_per_sample == 1) { 
    type_min = numeric_limits<unsigned char>::min();
    type_max = numeric_limits<unsigned char>::max();
  }
  else {
    type_min = numeric_limits<Endian::int16>::min();
    type_max = numeric_limits<Endian::int16>::max();
  }

  min_a  = info->getMinAmplitude();
  range  = info->getMaxAmplitude() - min_a;
}


AudioBlock* WAVWriter::produceBlock() {
  static const float ATTENUATION = 0.9;

  if (status != 0) status->updateSubtask();

  AudioBlock* block = source->produceBlock();
  if (block == 0) return 0;

  float* sound   = block->getData();
  int    samples = block->getSamples() * block->getChannels();
  int    sample  = 0;

  if (bytes_per_sample == 1) {
    if (data8 == 0) data8 = new unsigned char[samples];

    while (sample < samples) {
      data8[sample] = (unsigned char)((sound[sample]-min_a) / range * type_max * ATTENUATION);
      ++sample;
    }
    audio_out.write(data8, samples);
    bytes_written += samples;
  }

  else {
    if (data16 == 0) data16 = new Endian::int16[samples];

    while (sample < samples) {
      Endian::int16 value = (Endian::int16)(((sound[sample]-min_a) / range - 0.5) * (type_max-type_min) * ATTENUATION);
#ifndef _LITTLE_ENDIAN
      data16[sample] = Endian::swap16(value);
#endif
#ifdef _LITTLE_ENDIAN
      data16[sample] = value;
#endif
      ++sample;
    }
    audio_out.write((unsigned char*)data16, samples * sizeof(Endian::int16));
    bytes_written += samples * sizeof(Endian::int16);
  }

  return block;
}


WAVWriter::~WAVWriter() {
  int file_length_wo_riff_header = bytes_written + sizeof(wavheader) - sizeof(wavheader.fileLength) - sizeof(wavheader.RIFF);

#ifndef _LITTLE_ENDIAN
  wavheader.fileLength      = Endian::swap_int32(file_length_wo_riff_header);
  wavheader.nofDataBytes    = Endian::swap_int32(bytes_written);
#endif
#ifdef _LITTLE_ENDIAN
  wavheader.fileLength      = (Endian::int32)file_length_wo_riff_header;
  wavheader.nofDataBytes    = (Endian::int32)(bytes_written);
#endif

  audio_out.seekp(0);
  audio_out.write((unsigned char *)&wavheader, sizeof(wavheader));

  if (data8 != 0)  delete[] data8;
  if (data16 != 0) delete[] data16;
}
