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
 * @brief A simple timer written in cpp, implementation
 *
 * @author Matt Gara
 *
 * @date 2019-04-16
 *
 */

#include "timer.h"

namespace util {

Timer::Timer() : m_accum(0) {}

double Timer::elapsed_seconds() const { return m_accum; }

void Timer::tic() {
#ifdef USE_CLOCK
  if (clock_gettime(CLOCK_REALTIME, &m_start) == -1) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
#else
  if (gettimeofday(&m_start, 0) == -1) {
    perror("gettimeofday");
    exit(EXIT_FAILURE);
  }
#endif
}

void Timer::toc() {
#ifdef USE_CLOCK
  if (clock_gettime(CLOCK_REALTIME, &m_stop) == -1) {
    perror("clock gettime");
    exit(EXIT_FAILURE);
  }
  m_accum += (m_stop.tv_sec - m_start.tv_sec) +
             (m_stop.tv_nsec - m_start.tv_nsec) / (double)(BILLION);
#else
  if (gettimeofday(&m_stop, 0) == -1) {
    perror("gettimeofday");
    exit(EXIT_FAILURE);
  }
  long long elapsed = (m_stop.tv_sec - m_start.tv_sec) * 1000000LL +
                      m_stop.tv_usec - m_start.tv_usec;
  m_accum += elapsed / (1000000.0);
#endif
}

void timer_print(const std::string &name, Timer &timer) {
  printf(" [TIMER] <%s>  %f sec\n", name.c_str(), timer.elapsed_seconds());
}
}
