/* RunningSum.cxx
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


#ifndef _RUNNINGSUM
#define _RUNNINGSUM


template<class C> class RunningSum {

public:
  RunningSum(int elements, C init_elem);

  void add(C addend);
  C getSum() { return m_sum; };
  void reset(C init_elem);

  ~RunningSum();

private:
  int m_nof_elems;
  int m_pos;
  C   m_sum;
  C*  m_elem;
};


template<class C> RunningSum<C>::RunningSum(int elements, C init_elem) :
  m_nof_elems(elements),
  m_elem(new C[elements])
{
  reset(init_elem);
}


template<class C> void RunningSum<C>::add(C addend) {
  if (++m_pos == m_nof_elems) m_pos = 0;
  m_sum = m_sum - m_elem[m_pos] + addend;
  m_elem[m_pos] = addend;
}


template<class C> void RunningSum<C>::reset(C init_elem) {
  for (int i = 0;  i < m_nof_elems;  i++) {
    m_elem[i] = init_elem;
  }
  m_sum = m_nof_elems * init_elem;
  m_pos = 0;
}


template<class C> RunningSum<C>::~RunningSum() {
  delete[] m_elem;
}


#endif
