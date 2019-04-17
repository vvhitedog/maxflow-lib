/**
 *  This file is part of maxflow-lib.
 *
 *  maxflow-lib is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  maxflow-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with maxflow-lib.  If not, see <https://www.gnu.org/licenses/>.
 *
 * @file timer.h
 *
 * @brief A simple timer written in cpp, header
 *
 * @author Matt Gara
 *
 * @date 2019-04-16
 *
 */
#ifndef UTILTIMER_H
#define UTILTIMER_H

#include <string>

#define USE_CLOCK

#ifdef USE_CLOCK
#include <time.h>
#define BILLION 1000000000
#else
#include <sys/time.h>
#endif

namespace util {

class Timer {

private:
#ifdef USE_CLOCK
  struct timespec m_start, m_stop;
#else
  struct timeval m_start, m_stop;
#endif
  double m_accum;

public:
  Timer();

  void tic();
  void toc();

  double elapsed_seconds() const;
};

#define TIMER_PRINT(name) timer_print(#name, (name))

void timer_print(const std::string &name, Timer &timer);
}

#endif // UTILTIMER_H
