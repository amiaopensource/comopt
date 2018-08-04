/* AudioProducer.cxx
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


#include "AudioProducer.hxx"

AudioProducer::AudioProducer(StatusView* status_view) :
  status(status_view)
{
  if (status != 0) status->addSubtask();
}
