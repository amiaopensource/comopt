# Digital Image Based Restoration of Optical Movie Soundtracks

**This software is part of the diploma thesis [Digital Image Based Restoration of Optical Movie Soundtracks](report.pdf) by Patrick Streule, presented in 1999 at the ETHZ, the Swiss Federal Institute of Technology in Zurich, Switzerland.**

We are trying since 2018–03–09 to make the BIPP classes available here as well. Any help would be most appreciated! The full reference is

**Armin Wittmann: _BIPP Basel Image Processing Package_, Department for Applied Image Sciences, Institute of Physical Chemistry, University of Basel, 1996**

## ☛ **This is a work in progress!** ☚

### Contributions are very welcome!

Please:

- keep the [issue tracker](http://github.com/amiaopensource/comopt/issues) limited to bug reports and feature requests;
- use the [pull requests](http://github.com/amiaopensource/comopt/pulls) to submit patches to both the code and its documentation.

Thank you!

## Installation

Copy the files from the `sources` directory into the directory you wish to use. You must have the BIPP classes installed. Edit the second line from top in `makefile` (i.e. BIPPLOC) to reflect the path where the BIPP headers can be found. Run the following command

```
make process
```

If you want to remove all object and backup files, type

```
make clean
```

The single classes forming the restoration pipeline do not depend on BIPP. They can be used as stand-alone objects.

## Usage

After compilation, you can invoke process with the following options. An explanation is also available on the command line by typing

```
process -help
```

Required arguments are:

- `-in`
- `-out`
- one of `-fix`, `-wbe` or `-bwe`
- one of `-perf` or `-lps`.

The other arguments are optional and have to be chosen according to the restoration needs.
