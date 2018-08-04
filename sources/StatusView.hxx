/* StatusView.hxx
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


#ifndef _STATUSVIEW
#define _STATUSVIEW


class StatusView {

public:
  StatusView(const char* text, int tasks = 0);

  void updateSubtask();
  void updateTask();

  void setTasks(int tasks) { _tasks = tasks; }
  void addSubtask() { _subtasks++; }

  void message(const char* msg);
  void reset(const char* text, int tasks = 0);

private:
  void printStatus();

  int    task;
  int    subtask;
  int    _tasks;
  int    _subtasks;
  const char* _text;
};


#endif
