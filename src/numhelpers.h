/*
 * ldcell -- a simple crystal cell processing utility 
 * Copyright (C) 2012 Li Ding <iamliding at gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NUMHELPERS_H
#define _NUMHELPERS_H

#define X 0
#define Y 1
#define Z 2
#define DO(i,N) for(int i = 0; i < N; ++i) {
#define END }
#define real double
#define ROP "%lf"

/* basic vector calculations */

real dot_prod(const real a[3], const real b[3]);
void cross_prod(real a[3], real b[3], real c[3]);
void real_prod(real a[3], real num);

/* cell related helper */

real cell_volume(real cell[3][3]);
void scale_cell(real c[3][3], real s);

#endif /* _NUMHELPERS_H */
