/* StatusView.cxx
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
#include "StatusView.hxx"

StatusView::StatusView(const char* text, int tasks) {
  reset(text, tasks);
}

void StatusView::printStatus() {
  cout << "\r";
  cout << _text << " " << task << "/" << _tasks << " [";
  for (int i = 0; i < subtask; i++) cout << "|";
  for (int i = subtask; i < _subtasks; i++) cout << " ";
  cout << "]" << flush;
}


void StatusView::updateTask() {
  ++task;
  subtask = 0;
  printStatus();
}


void StatusView::updateSubtask() {
  ++subtask;
  printStatus();
}


void StatusView::message(const char* msg) {
  cout << "\r                                                                               \r";
  cout << msg << endl;
}


void StatusView::reset(const char* text, int tasks) {
  _text     = text;
  _tasks    = tasks;
  _subtasks = 0;
  task      = 1;
  subtask   = 0;
}
