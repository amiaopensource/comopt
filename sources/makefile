CXXFLAGS = -arch host -inline speed -O5
BIPPLOC  = /home/witt/alpha/src/bipp2.1/basic
LIBS     = -L${BIPPLOC} -lbippbasic -lm
INCLUDES = -I. -I${BIPPLOC}

#DEMOSRC  = debug.cxx 
#DEMOOBJ  = ${DEMOSRC:.cxx=.o}

PROCESSOBJ  = ImageBlock.o  StatusView.o  ImageProducer.o  PGMReader.o  PGMWriter.o  PerforationCounter.o \
              RunningSum.o  ConstPositionEstimator.o  VerticalEdgePE.o  ImageCutter.o  PhotoCell.o AudioBlock.o \
              AudioProducer.o  AudioDataCollector.o  RawAudioWriter.o  RawAudioReader.o  WAVWriter.o \
              FIRFilter.o  FIRFilterCoefficients.o  AcademyFilterCoeffs.o  remez.o  LowpassFIRFilterCoeffs.o \
              SamplingrateConverter.o  ContrastEnhancer.o  ImageDeskewer.o  EquivalenceTable.o \
              WhiteDustRemover.o  BlackDustRemover.o  SymmetryAxisPE.o  SymmetryEnforcer.o  MonotonyEnforcer.o

process: process.cxx ${PROCESSOBJ}
	cxx -o $@ ${CXXFLAGS} ${INCLUDES} $< ${PROCESSOBJ} ${LIBS}
ImageBlock.o: ImageBlock.cxx  ImageBlock.hxx
	cxx -c ${CXXFLAGS} $<

StatusView.o: StatusView.cxx  StatusView.hxx
	cxx -c ${CXXFLAGS} $<

ImageProducer.o: ImageProducer.cxx  ImageProducer.hxx
	cxx -c ${CXXFLAGS} $<

PGMReader.o: PGMReader.cxx  PGMReader.hxx  ImageBlock.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

PGMWriter.o: PGMWriter.cxx  PGMWriter.hxx  ImageBlock.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $< -lm

RunningSum.o: RunningSum.cxx
	cxx -c ${CXXFLAGS} $<

PositionEstimator.o: PositionEstimator.hxx  ImageBlock.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

PerforationCounter.o: PerforationCounter.cxx  PerforationCounter.hxx  RunningSum.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

ConstPositionEstimator.o: ConstPositionEstimator.cxx  ConstPositionEstimator.hxx  PositionEstimator.o
	cxx -c ${CXXFLAGS} $<

VerticalEdgePE.o: VerticalEdgePE.cxx  VerticalEdgePE.hxx  PositionEstimator.o
	cxx -c ${CXXFLAGS} $<

ImageCutter.o: ImageCutter.cxx  ImageCutter.hxx  PositionEstimator.o
	cxx -c ${CXXFLAGS} $<

ContrastEnhancer.o: ContrastEnhancer.cxx  ContrastEnhancer.hxx  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

EquivalenceTable.o: EquivalenceTable.cxx  EquivalenceTable.hxx
	cxx -c ${CXXFLAGS} $<

WhiteDustRemover.o: WhiteDustRemover.cxx  WhiteDustRemover.hxx  EquivalenceTable.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

BlackDustRemover.o: BlackDustRemover.cxx  BlackDustRemover.hxx  EquivalenceTable.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

SymmetryAxisPE.o: SymmetryAxisPE.cxx  SymmetryAxisPE.hxx  PositionEstimator.o
	cxx -c ${CXXFLAGS} $<

SymmetryEnforcer.o: SymmetryEnforcer.cxx  SymmetryEnforcer.hxx  SymmetryAxisPE.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

MonotonyEnforcer.o: MonotonyEnforcer.cxx  MonotonyEnforcer.hxx  SymmetryAxisPE.o  ImageProducer.o
	cxx -c ${CXXFLAGS} $<

ImageDeskewer.o: ImageDeskewer.cxx  ImageDeskewer.hxx  ImageProducer.o
	cxx -c ${CXXFLAGS} $< -lm

AudioBlock.o: AudioBlock.cxx  AudioBlock.hxx
	cxx -c ${CXXFLAGS} $<

AudioProducer.o: AudioProducer.cxx AudioProducer.hxx AudioBlock.o
	cxx -c ${CXXFLAGS} $<

PhotoCell.o: PhotoCell.cxx PhotoCell.hxx AudioProducer.o ImageProducer.o RunningSum.o
	cxx -c ${CXXFLAGS} $<

AudioDataCollector.o: AudioDataCollector.cxx  AudioDataCollector.hxx  AudioProducer.o
	cxx -c ${CXXFLAGS} $<

RawAudioWriter.o: RawAudioWriter.cxx  RawAudioWriter.hxx  AudioProducer.o
	cxx -c ${CXXFLAGS} $<

RawAudioReader.o: RawAudioReader.cxx  RawAudioReader.hxx  AudioProducer.o
	cxx -c ${CXXFLAGS} $<

remez.o: remez.c remez.h
	cxx -c ${CXXFLAGS} $< -lm

FIRFilterCoefficients.o: FIRFilterCoefficients.hxx  remez.o
	cxx -c ${CXXFLAGS} $<

FIRFilter.o: FIRFilter.cxx  FIRFilter.hxx  FIRFilterCoefficients.o  AudioProducer.o
	cxx -c ${CXXFLAGS} $<

AcademyFilterCoeffs.o: AcademyFilterCoeffs.cxx  AcademyFilterCoeffs.hxx FIRFilterCoefficients.o
	cxx -c ${CXXFLAGS} $<

LowpassFIRFilterCoeffs.o: LowpassFIRFilterCoeffs.cxx  LowpassFIRFilterCoeffs.hxx  FIRFilterCoefficients.o
	cxx -c ${CXXFLAGS} $< -lm

SamplingrateConverter.o: SamplingrateConverter.cxx  SamplingrateConverter.hxx  FIRFilter.o \
                         LowpassFIRFilterCoeffs.o  AudioDataCollector.o  AudioProducer.o
	cxx -c ${CXXFLAGS} $< -lm

WAVWriter.o: WAVWriter.cxx  WAVWriter.hxx  AudioProducer.o  AudioDataCollector.o
	cxx -c ${CXXFLAGS} $<

clean:
	rm -f *.o *~