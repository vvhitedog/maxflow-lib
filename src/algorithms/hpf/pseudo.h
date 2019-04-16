/**
 *  This file is part of maxflow-lib.
 *
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for educational, research, and not-for-profit purposes,
 *  without fee and without a signed licensing agreement, is hereby granted,
 *  provided that the above copyright notice, this paragraph and the following
 *  two paragraphs appear in all copies, modifications, and distributions.
 *  Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
 *  Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
 *  licensing opportunities. Created by Bala Chandran and Dorit S. Hochbaum,
 *  Department of Industrial Engineering and Operations Research, University of
 *  California, Berkeley.
 *
 *    IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
 *    SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS,
 *    ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
 *    REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED
 *    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *    PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF ANY,
 *    PROVIDED HEREUNDER IS PROVIDED "AS IS". REGENTS HAS NO OBLIGATION TO
 *    PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * @file pseudo.h
 *
 * @brief Header for HPF implementation, adapted from original HPF
 *        implementation
 *
 * @author Matt Gara
 *
 * @date 2019-04-15
 *
 */
#ifndef MAXFLOWLIB_HPF_H
#define MAXFLOWLIB_HPF_H

typedef unsigned int uint;
typedef long int lint;
typedef long long int llint;
typedef unsigned long long int ullint;

void add_arc(uint from, uint to, uint fcap, uint rcap = 0);
void set_tweights(uint id, uint source_cap, uint sink_cap);
void initializeGraph();
void allocateGraph(uint _numNodes, uint _numArcs);
ullint maxflow_from_pseudoflow();
ullint pseudoflow();
int what_segment(uint id);

#endif
