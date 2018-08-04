/* EquivalenceTable.hxx
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


#ifndef _EQUIVALENCETABLE
#define _EQUIVALENCETABLE

#include <vector>


class EquivalenceTable {

public:
  EquivalenceTable(int size);

  void addEquivalence(int label, int equivalent);
  void normalize();
  void reset(int start = 0);
  int  getEquivalent(int i) { return table[i]; }

  ~EquivalenceTable();

private:
  vector<int> table;
};


#endif
