/* process.cxx
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


#include <iostream>
#include <new>
#include <stdio.h>
#include <string.h>
#include "BIPPBase.H"
#include "PGMReader.hxx"
#include "PGMWriter.hxx"
#include "PerforationCounter.hxx"
#include "ConstPositionEstimator.hxx"
#include "VerticalEdgePE.hxx"
#include "ImageCutter.hxx"
#include "PhotoCell.hxx"
#include "AudioDataCollector.hxx"
#include "RawAudioWriter.hxx"
#include "RawAudioReader.hxx"
#include "WAVWriter.hxx"
#include "FIRFilter.hxx"
#include "AcademyFilterCoeffs.hxx"
#include "SamplingrateConverter.hxx"
#include "ContrastEnhancer.hxx"
#include "ImageDeskewer.hxx"
#include "WhiteDustRemover.hxx"
#include "BlackDustRemover.hxx"
#include "SymmetryAxisPE.hxx"
#include "SymmetryEnforcer.hxx"
#include "MonotonyEnforcer.hxx"


class Processor : public BIPPHelpClass {

public:
  Processor(int argc, char** argv);
  ~Processor();

protected:
  int preExec() { return 1;}
  int doit();

private:
  bool allRequiredArguments();

  StringOption*  in;
  StringOption*  out;
  IntOption*     bs;
  IntOption*     fix;
  IntOption*     bwe;
  IntOption*     wbe;
  FlagOption*    academy;
  IntOption*     srate;
  IntOption*     ch;
  IntOption*     slit;
  FlagOption*    quiet;
  IntOption*     perf;
  IntOption*     lps;
  IntOption*     thr;
  IntOption*     azi;
  StringOption*  test;
  FlagOption*    negative;
  FlagOption*    wdust;
  FlagOption*    bdust;
  StringOption*  monotony;
  StringOption*  symm;
  IntOption*     skip;

  static const String help_string;
  static const int BLOCK_SIZE         = 1000;
  static const int SUBBLOCK_SIZE      = 300;
  static const int CHANNELS           = 1;
  static const int SLIT_HEIGHT        = 1;
  static const int SYMM_DIFF_ALLOWED  = 20;
  static const int WINDOW_WIDTH       = 24;
};


const String Processor::help_string = 
"\nusage: process  -in -out (-fix | -bwe | -wbe) (-perf | -lps) [-ch] [-negative]\n"
"                 [-thr] [-wdust] [-bdust] [-symm] [-monotony][-azi]\n"
"                 [-srate] [-slit] [-bs] [-academy] [-test] [-quiet]\n"
"________________________________________________________________________________\n"
"  -in       <pgmfile>      PGM (type P5) file to be processed\n"
"________________________________________________________________________________\n"
"  -out      <wavfile>      Name of the output WAV file\n"
"________________________________________________________________________________\n"
"  -fix      <x> <w>        Fixed sound track location, <w> pixels wide,\n"
"                           located <x> pixels from the left image border.\n"
"________________________________________________________________________________\n"
"  -bwe      <x> <w> <off>  Adaptive sound track location, <w> pixels wide.\n"
"  -wbe      [<win>] [<bw>] Use -wbe for white-black transitions and -bwe\n"
"                           for black-white transitions. <x> is an initial\n"
"                           guess for the edge location. The sound track does\n"
"                           not have to start at <x>: you can specify an\n"
"                           offset <off> which is added to <x>, forming the\n"
"                           left or the right track limit. Use a negative\n"
"                           <w> if <x>+<off> denotes the right track limit.\n"
"                           The optional <win> determines the window width\n"
"                           searched for the maximum gradient. [default=24]\n"
"                           By specifying the subblock size (in lines) <bs>,\n"
"                           you can optionally influence  how quickly the\n"
"                           sound track position may change [default = 300].\n"
"________________________________________________________________________________\n"
"  -perf     <x> <w> <h>    Location of the perforation. <w> and <h> determine\n"
"                           the window dimension used for the detection of a\n"
"                           perforation hole. <h> should approximately equal 3/4\n"
"                           of a perforation hole's height.\n"
"________________________________________________________________________________\n"
"  -lps      <lines>        If you already know how many image lines correspond\n"
"                           to one second running time, you can specify it\n"
"                           instead of having the perforation holes counted.\n"
"________________________________________________________________________________\n"
"  -ch       <channels>     Number of channels/tracks [default = 1 (mono)].\n"
"________________________________________________________________________________\n"
"  -negative                Use this option for negatives. A positive is assumed\n"
"                           by default. For symmetric variable area soundtracks\n"
"                           positive means a transparent track on a black base.\n"
"                           Positive unilateral soundtracks have a white to black\n"
"                           transition from left to right.\n"
"________________________________________________________________________________\n"
"  -thr      <lo> <hi>      Thresholds for image noise suppression. You can\n"
"                           override the automatic threshold estimate by\n"
"                           specifying <lo> and <hi>. Pixel values below <lo>\n"
"                           will be set to black, values above <hi> will be\n"
"                           set to white. The automatic threshold estimator works\n"
"                           only for variable area soundtracks.\n"
"________________________________________________________________________________\n"
"  -wdust                   Remove white dust\n"
"________________________________________________________________________________\n"
"  -bdust                   Remove black dust\n"
"________________________________________________________________________________\n"
"  -symm     ['enforce']    Use this option for symmetric variable area tracks\n"
"                           A unilateral track is assumed by default\n"
"                           The optional 'enforce' leads to better dust removal.\n"
"________________________________________________________________________________\n"
"  -monotony 'l2r' | 'r2l'  This option leads to better dust removal by enforcing\n"
"                           the monotony property of variable area soundtracks.\n"
"________________________________________________________________________________\n"
"  -azi      <dy> <dx>      Corrects an azimuth deviation, specified as dy/dx \n"
"                           (tangens of the angle).\n"
"________________________________________________________________________________\n"
"  -srate    <Hz>           Samplingrate in Hz for <wavfile> [default = 44100].\n"
"________________________________________________________________________________\n"
"  -slit     <height>       Scanning slit height in pixels [default = 1].\n"
"________________________________________________________________________________\n"
"  -bs       <size>         Block size in lines [default = 1000].\n"
"________________________________________________________________________________\n"
"  -academy                 Use the Academy filter (8kHz low-pass).\n"
"________________________________________________________________________________\n"
"  -test     <pgmfile>      Write the processed image into <pgmfile>.\n"
"________________________________________________________________________________\n"
"  -skip     <lines>        Ignore the first <lines> lines of the image.\n"
"________________________________________________________________________________\n"
"  -quiet                   Suppress status messages\n"
"\n"
;


Processor::Processor(int argc, char** argv) :
  BIPPHelpClass(argc, argv, help_string)
{
  in       = new StringOption("in");
  out      = new StringOption("out");
  bs       = new IntOption("bs");
  fix      = new IntOption("fix", 2);
  wbe      = new IntOption("wbe", 5);
  bwe      = new IntOption("bwe", 5);
  academy  = new FlagOption("academy");
  srate    = new IntOption("srate");
  ch       = new IntOption("ch");
  slit     = new IntOption("slit");
  quiet    = new FlagOption("quiet");
  perf     = new IntOption("perf", 3);
  lps      = new IntOption("lps");
  thr      = new IntOption("thr", 2);
  azi      = new IntOption("azi", 2);
  test     = new StringOption("test");
  negative = new FlagOption("negative");
  wdust    = new FlagOption("wdust");
  bdust    = new FlagOption("bdust");
  monotony = new StringOption("monotony");
  symm     = new StringOption("symm");
  skip     = new IntOption("skip");
}


bool Processor::allRequiredArguments() {
  bool error = false;

  if (!in->set())  { cerr << "use -in <pgmfile>" << endl; error = true; }
  if (!out->set()) { cerr << "use -out <wavfile>" << endl; error = true; }

  if (!fix->set() && !bwe->set() && !wbe->set()) { cerr << "use either -fix or -bwe or -wbe" << endl; error = true; }
  if (fix->set() && !fix->isValid(1)) { cerr << "-fix takes two arguments" << endl; error = true; }
  if (bwe->set() && !bwe->isValid(4)) { cerr << "-bwe takes five arguments" << endl; error = true; }
  if (wbe->set() && !wbe->isValid(4)) { cerr << "-wbe takes five arguments" << endl; error = true; }

  if (!perf->set() && !lps->set()) { cerr << "use either -perf or -lps " << endl; error = true; }
  if (perf->set() && !perf->isValid(2)) { cerr << "-perf requires three arguments" << endl; error = true; }

  if (azi->set() && !azi->isValid(1)) { cerr << "-azi requires two arguments" << endl; error = true; }

  if (monotony->set() && (0 == strcmp(*(monotony->value()), ""))) {
    cerr << "-monotony requires an argument: 'l2r' or 'r2l'" << endl; error = true;
  }

  return !error;
}


int Processor::doit() {

  // Check if all required arguments are present
  if (!allRequiredArguments()) {
    _help->outHelpTextSTD();
    return 0;
  }

  // Create the processing pipeline
  try {

    // Set up the parameters for the image processing pipeline
    char        message[256];
    const char* temporary = *(out->value()) + ".tmp";

    int  out_samplingrate  = 0;
    int  in_samplingrate   = 0;
    int  block_size        = BLOCK_SIZE;
    int  channels          = CHANNELS;
    int  slit_h            = SLIT_HEIGHT;
    int  subblock_size     = SUBBLOCK_SIZE;
    int  window_width      = WINDOW_WIDTH;
    bool positive          = true;
    bool symmetric         = false;
    bool left_to_right     = true;
    int  skip_lines        = 0;

    if (ch->set() && ch->isValid())     channels         = ch->value();
    if (slit->set() && slit->isValid()) slit_h           = slit->value();
    if (bs->set() && bs->isValid())     block_size       = bs->value();
    if (srate->set())                   out_samplingrate = srate->value();
    if (negative->set())                positive         = false;
    if (symm->set())                    symmetric        = true;
    if (monotony->set() && (0 == strcmp(*(monotony->value()), "r2l"))) left_to_right = false;
    if (skip->isValid())                skip_lines       = skip->value();

    // Declare the modules
    ImageProducer*         image_source;
    AudioProducer*         audio_source;
    PositionEstimator*     PE;

    StatusView*            status        = 0;
    PGMReader*             pgm_reader    = 0;
    PerforationCounter*    perf_counter  = 0;
    ImageCutter*           image_cutter  = 0;
    PGMWriter*             pgm_writer    = 0;
    ContrastEnhancer*      contrast      = 0;
    ImageDeskewer*         deskewer      = 0;
    WhiteDustRemover*      w_dust_remove = 0;
    BlackDustRemover*      b_dust_remove = 0;
    SymmetryAxisPE*        symm_axis     = 0;
    SymmetryEnforcer*      enf_symmetry  = 0;
    MonotonyEnforcer*      enf_monotony  = 0;
    PhotoCell*             photo_cell    = 0;
    AudioDataCollector*    audio_info    = 0;
    RawAudioWriter*        raw_writer    = 0;
    RawAudioReader*        raw_reader    = 0;
    AcademyFilterCoeffs*   academyCoeffs = 0;
    FIRFilter*             fir_filter    = 0;
    SamplingrateConverter* new_srate     = 0;
    WAVWriter*             wav_writer    = 0;


    // Let the modules display their status in a status bar if this option has
    // not been explicitly disabled.
    if (!quiet->set()) status = new StatusView("[Run 1] Image block");


    // PROCESS IMAGE DATA

    // Create a PGM Reader for the file specified with -in
    pgm_reader = new PGMReader(status, *(in->value()), block_size, skip_lines);
    image_source     = pgm_reader;

    // Create a PerforationCounter if the 'lines per second' value was not explicitly
    // specified using the -lps option
    if (!lps->set()) {
      int perf_x = perf->value(0);
      int perf_w = perf->value(1);
      int perf_h = perf->value(2);
      perf_counter = new PerforationCounter(image_source, status, perf_x, perf_w, perf_h);
      image_source = perf_counter;
    }

    // Depending on the -fix, -bwe or -wbe settings, create a suitable PositionEstimator
    // for a fixed or variable soundtrack position estimator

    if (fix->set()) {
      PE = new ConstPositionEstimator(image_source, status, fix->value(0), fix->value(1));
    }
    else if (bwe->set()) {
      if (bwe->isValid(3)) window_width  = bwe->value(3);
      if (bwe->isValid(4)) subblock_size = bwe->value(4);
      PE = new VerticalEdgePE(image_source, status, bwe->value(0), bwe->value(1), bwe->value(2), 
               VerticalEdgePE::PREFER_POSITIVE_EDGE, subblock_size, window_width);
    }
    else {
      if (wbe->isValid(3)) window_width  = wbe->value(3);
      if (wbe->isValid(4)) subblock_size = wbe->value(4);
      PE = new VerticalEdgePE(image_source, status, wbe->value(0), wbe->value(1), wbe->value(2), 
               VerticalEdgePE::PREFER_NEGATIVE_EDGE, subblock_size, window_width);
    }

    // Create an ImageCutter using the previously created PositionEstimator to create
    // a new ImageBlock only containing the optical soundtrack
    image_cutter = new ImageCutter(PE, status, PE);
    image_source = image_cutter;

    // If the -thr option was chosen, create a ContrastEnhancer using either the
    // specified thresholds or letting the ContrastEnhancer do the guesswork
    if (thr->set()) {
      if (thr->isValid(1)) contrast = new ContrastEnhancer(image_source, status, thr->value(0), thr->value(1));
      else                 contrast = new ContrastEnhancer(image_source, status);
      image_source = contrast;
    }

    // If the user opted for white dust removal, create an instance of WhiteDustRemover
    if (wdust->set()) {
      w_dust_remove = new WhiteDustRemover(image_source, status, channels, symmetric, positive);
      image_source  = w_dust_remove;
    }

    // The same for black dust removal
    if (bdust->set()) {
      b_dust_remove = new BlackDustRemover(image_source, status, channels, symmetric, positive);
      image_source = b_dust_remove;
    }

    // If a symmetric soundtrack is being processed, and symmetry enforcement was chosen,
    // create an instance of SymmetryEnforcer
    if ((symmetric) && (0 == strcmp(*(symm->value()), "enforce"))) {
      symm_axis    = new SymmetryAxisPE(image_source, status, channels, positive, SUBBLOCK_SIZE);
      enf_symmetry = new SymmetryEnforcer(symm_axis, status, symm_axis, positive, SYMM_DIFF_ALLOWED);
      image_source = enf_symmetry;
    }

    // If monotony enforcement was chosen by the -monotony option, create an instance of
    // MonotonyEnforcer. If the track is symmetric, and no symmetry axis position
    // estimator exists already, create one.
    if (monotony->set()) {
      if (symmetric && (symm_axis == 0)) 
	symm_axis  = new SymmetryAxisPE(image_source, status, channels, positive, SUBBLOCK_SIZE);
      enf_monotony = new MonotonyEnforcer(image_source, status, channels, positive, left_to_right, symm_axis);
      image_source = enf_monotony;
    }

    // If the -azi option was chosen, create an ImageDeskewer to compensate for 
    // a specific azimuth deviation
    if (azi->set()) {
      deskewer     = new ImageDeskewer(image_source, status, azi->value(0), azi->value(1));
      image_source = deskewer;
    }

    // Write the processed image to the file specified with -test before passing it
    // on to the PhotoCell
    if (test->set()) {
      pgm_writer   = new PGMWriter(image_source, status, *(test->value()));
      image_source = pgm_writer;
    }

    // Create a PhotoCell using the specified channel and slit height settings,
    // or the defaults if they were omitted
    photo_cell   = new PhotoCell(image_source, status, slit_h, channels);

    // Create an AudioDataCollector which will contain all data specific to the
    // audio data
    audio_info   = new AudioDataCollector(photo_cell, status);

    // Create a RawWriter used for buffering the audio data until all parameters
    // such as the samplingrate and minimum/maximum value are known for further processing
    raw_writer   = new RawAudioWriter(audio_info, status, temporary);


    // DRIVE THE IMAGE PIPELINE
    if (status != 0) status->setTasks(pgm_reader->getImageHeight() / block_size + 1);

    while (raw_writer->produceBlock() != 0) {
      if (status != 0) status->updateTask();
    }

    if (status != 0) status->message("[Run 1 completed]");


    // PROCESS THE AUDIO DATA

    // Set up the parameters for the audio modules
    if (lps->set()) 
      in_samplingrate = lps->value();
    else 
      in_samplingrate = audio_info->getSamples() * 96 / perf_counter->getCount();

    audio_info->setSamplingrate(in_samplingrate);

    // Print out some statistics about the processed soundtrack
    if (status != 0) {
      long s = audio_info->getSamples();
      sprintf(message, "min amplitude: %.2f", audio_info->getMinAmplitude());
      status->message(message);
      sprintf(message, "max amplitude: %.2f", audio_info->getMaxAmplitude());
      status->message(message);
      sprintf(message, "      samples: %d", s);
      status->message(message);
      sprintf(message, "     channels: %d", audio_info->getChannels());
      status->message(message);
      sprintf(message, " bytes/sample: %d", audio_info->getBytesPerSample());
      status->message(message);
      sprintf(message, "   audio time: %d:%d.%d", (s/in_samplingrate)/60, (s/in_samplingrate)%60, (s % in_samplingrate)*100/in_samplingrate);
      status->message(message);
      sprintf(message, " samplingrate: %d Hz", in_samplingrate);
      status->message(message);

      status->reset("[Run 2] Audio block", audio_info->getSamples() / in_samplingrate + 1);
    }

    // Create a RawAudioReader for reading the buffered audio data again
    raw_reader   = new RawAudioReader(status, temporary, in_samplingrate, audio_info->getChannels());
    audio_source = raw_reader;

    // Create an Academy filter if the -academy option was chosen
    // With the Academy filter, there is no need for lowpass filtering the audio data 
    // before converting the samplingrate
    if (academy->set()) {
      academyCoeffs  = new AcademyFilterCoeffs(in_samplingrate);
      fir_filter     = new FIRFilter(audio_source, status, academyCoeffs);
      audio_source   = fir_filter;
      if (out_samplingrate > 0) {
        new_srate    = new SamplingrateConverter(audio_source, status, audio_info, out_samplingrate, false);
        audio_source = new_srate;
      }
    }
    else if (out_samplingrate > 0) {
      new_srate    = new SamplingrateConverter(raw_reader, status, audio_info, out_samplingrate);
      audio_source = new_srate;
    }

    // Create a WAVWriter which takes the processed audio data and writes it to the file
    // specified with -out in PCM Format
    wav_writer = new WAVWriter(audio_source, status, audio_info, *(out->value()));


    // DRIVE THE AUDIO PIPELINE
    while(wav_writer->produceBlock() != 0) {
      if (status != 0) status->updateTask();
    }

    // Remove the temporary file created by the RawAudioWriter
    remove(temporary);

    if (status != 0) status->message("[Run 2 completed]");

    // Clean up
    if (wav_writer    != 0) delete wav_writer;
    if (fir_filter    != 0) delete fir_filter;
    if (academy       != 0) delete academy;
    if (raw_reader    != 0) delete raw_reader;
    if (raw_writer    != 0) delete raw_writer;
    if (audio_info    != 0) delete audio_info;
    if (photo_cell    != 0) delete photo_cell;
    if (pgm_writer    != 0) delete pgm_writer;
    if (deskewer      != 0) delete deskewer;
    if (enf_monotony  != 0) delete enf_monotony;
    if (enf_symmetry  != 0) delete enf_symmetry;
    if (symm_axis     != 0) delete symm_axis;
    if (w_dust_remove != 0) delete w_dust_remove;
    if (b_dust_remove != 0) delete b_dust_remove;
    if (contrast      != 0) delete contrast;
    if (image_cutter  != 0) delete image_cutter;
    if (PE            != 0) delete PE;
    if (perf_counter  != 0) delete perf_counter;
    if (pgm_reader    != 0) delete pgm_reader;
    if (status        != 0) delete status;
  }

  catch (PGMReader::FileOpenExcpt& e) {
    cerr << "Error: " << *(in->value()) << " could not be opened" << endl;
  }

  catch (PGMReader::FileFormatExcpt& e) {
    cerr << "Error: " << *(in->value()) << " has wrong format. Raw PGM (P5) expected." << endl;
  }

  catch (bad_alloc) {
    cerr << "Error: Memory allocation failed" << endl;
  }

  return 1;
}


Processor::~Processor() {
  delete in;
  delete out;
  delete bs;
}


int main(int argc, char* argv[]) {
  Processor* processor = new Processor(argc, argv);
  processor->execute();
  delete processor;
}
