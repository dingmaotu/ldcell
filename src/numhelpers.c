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

#include <math.h>
#include "numhelpers.h"

real dot_prod(const real a[3], const real b[3])
{
    real sum = 0.0;

    DO(i,3)
        sum += a[i]*b[i];
    END

    return sum;
}

void cross_prod(real a[3], real b[3], real c[3])
{
    c[X] = a[Y] * b[Z] - a[Z] * b[Y];
    c[Y] = a[Z] * b[X] - a[X] * b[Z];
    c[Z] = a[X] * b[Y] - a[Y] * b[X];
}

void real_prod(real a[3], real num)
{
    DO(i,3)
        a[i] *= num;
    END
}

real cell_volume(real cell[3][3])
{
    real cross[3];
    cross_prod(cell[X], cell[Y], cross);
    return dot_prod(cell[Z], cross);
}

void scale_cell(real c[3][3], real s)
{
    if (s < 0) 
        s = pow(-s/cell_volume(c), 1.0/3);

    DO(i,3)
        real_prod(c[i], s);
    END
}

