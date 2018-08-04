/* EquivalenceTable.cxx
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


#include "EquivalenceTable.hxx"


EquivalenceTable::EquivalenceTable(int size) :
  table(size) 
{
  reset();
}

void EquivalenceTable::addEquivalence(int label, int equivalent) {
  int minor, major;
  int table_size = table.size();

  if (label > equivalent) { major = label; minor = equivalent; }
  else { major = equivalent; minor = label; }

  if (major > table_size) {
    int new_size = (major > 2*table_size) ? 2*major : 2*table_size;
    table.resize(new_size);
    reset(table_size);
  }

  table[major] = minor;
}


void EquivalenceTable::normalize() {
  for (int i = 0; i < table.size(); i++) {
    int label = i;
    while (table[label] != label) label = table[label];
    table[i] = table[label];
  }
}


void EquivalenceTable::reset(int start) {
  for (int i = start; i < table.size(); i++) table[i] = i;
}


EquivalenceTable::~EquivalenceTable() {
  // clean up here
}
