/*
 * ldcell.c -- main for ldcell
 *
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>

#include "macro.h"
#include "numhelpers.h"

typedef real (*PCoords)[3];

typedef struct {
    int natoms;
    int ngrps;
    int *grps;
    real scaling;
    real cell[3][3];
    PCoords coords;
} CoordData;

#define MAX_GROUPS 16
#define MAX_LINE 512

typedef enum {Direct, Carte} CoordType;

bool scan_cellfile(FILE *in, CoordData * d)
{
    assert(in != NULL && d != NULL);

    char line_buf[MAX_LINE];

    fgets(line_buf, MAX_LINE, in);
    if (sscanf(line_buf, ROP, &d->scaling) != 1)
        return false;

    DO(i,3)
        fgets(line_buf, MAX_LINE, in);
        if(sscanf(line_buf, ROP" "ROP" "ROP, &(d->cell[i][X]), &(d->cell[i][Y]), &(d->cell[i][Z])) != 3)
            return false;
    END

    d->natoms = 0;
    d->ngrps = 0;
    d->grps = (int*) malloc(sizeof(int) * MAX_GROUPS);
    fgets(line_buf, MAX_LINE, in);
    {
        char * buf = line_buf;
        int slen = strlen(line_buf);
        int cnt = 0;
        while(sscanf(buf, "%d%n", d->grps + d->ngrps, &cnt) == 1) {
            if (buf < line_buf + slen) buf += cnt;
            else break;
            d->ngrps ++;
        }
    }
    if(d->ngrps == 0)
        return false;
    else {
        DO(i, d->ngrps)
            d->natoms += d->grps[i];
        END
    }

#ifndef NDEBUG
    fprintf(stderr, "Groups: %d, Number of atoms: %d\n", d->ngrps, d->natoms);
    getchar();
#endif

    if(d->natoms <= 0) {
        return false;
    }

    d->coords = (PCoords) malloc( 3 * d->natoms * sizeof(real) );
    DO(i,d->natoms)
        if (fscanf(in, " "ROP" "ROP" "ROP" \n", &(d->coords[i][X]), &(d->coords[i][Y]), &(d->coords[i][Z])) != 3)
            return false;
    END

    return true;
}

void expand_coorddata(CoordData *d, int rx, int ry, int rz, bool regroup)
{
    assert(d != NULL);
    assert(d->coords != NULL);
    assert(rx > 0 && ry > 0 && rz > 0);
    int zdist = d->natoms;
    int ydist = zdist * rz;
    int xdist = ydist * ry;
    int natoms = xdist * rx;

    PCoords coords = (PCoords) malloc( 3 * natoms * sizeof(real));
    if(!regroup) {
        DO(x,rx)
            DO(y, ry)
                DO(z, rz)
                    int rbase = x*xdist+y*ydist+z*zdist;
                    DO(i,d->natoms)
                        coords[i+rbase][X] = d->coords[i][X] + x;
                        coords[i+rbase][Y] = d->coords[i][Y] + y;
                        coords[i+rbase][Z] = d->coords[i][Z] + z;
                    END
                END
            END
        END
    } else {
        int cnt = 0;
        int grp_base = 0;

        /* for each group */
        DO(ig, d->ngrps)
            DO(x,rx)
                DO(y, ry)
                    DO(z, rz)
                        DO(i,d->grps[ig])
                            coords[cnt][X] = d->coords[i+grp_base][X] + x;
                            coords[cnt][Y] = d->coords[i+grp_base][Y] + y;
                            coords[cnt][Z] = d->coords[i+grp_base][Z] + z;
                            cnt ++;
                        END
                    END
                END
            END
            grp_base += d->grps[ig];
        END
    }

    DO(i, natoms)
        coords[i][X] /= rx;
        coords[i][Y] /= ry;
        coords[i][Z] /= rz;
    END

    DO(i, d->ngrps)
        d->grps[i] *= rx*ry*rz;
    END

    d->natoms = natoms;
    free(d->coords);
    d->coords = coords;

    real_prod(d->cell[X], rx);
    real_prod(d->cell[Y], ry);
    real_prod(d->cell[Z], rz);
}

void print_coorddata(FILE *out, const CoordData *d, CoordType out_type)
{
#ifndef NDEBUG
    assert(out != NULL);
    assert(d != NULL);
    assert(d->coords != NULL);
#endif

    PCoords coords = d->coords;
    bool need_free = false;

    if(out_type == Carte) {
        /* Calculate real coords from relative (Direct) ones */
        PCoords tmp_coords = (PCoords) malloc(3 * d->natoms * sizeof(real));
        DO(i,d->natoms)
            tmp_coords[i][X] = dot_prod(d->cell[X], d->coords[i]);
            tmp_coords[i][Y] = dot_prod(d->cell[Y], d->coords[i]);
            tmp_coords[i][Z] = dot_prod(d->cell[Z], d->coords[i]);
        END
        coords = tmp_coords;
        need_free = true;
    }

    fprintf(out, ROP"\n", d->scaling);
    DO(i,3)
        fprintf(out, ROP" "ROP" "ROP"\n", d->cell[i][X], d->cell[i][Y], d->cell[i][Z]);
    END

    if (d->ngrps > 1) {
        DO(i, d->ngrps)
            if (i == 0)  /* careful NOT i=0 !!!*/
                fprintf(out, "%d", d->grps[i]);
            else
                fprintf(out, " %d", d->grps[i]);
        END
        fprintf(out, "\n");
    } else {
        fprintf(out, "%d\n", d->natoms);
    }

    DO(i,d->natoms)
        fprintf(out, ROP" "ROP" "ROP"\n", coords[i][X], coords[i][Y], coords[i][Z]);
    END

    if (need_free)
        free(coords);
}

void do_usage()
{
    puts("Usage: ldcell [options] [inputfile]");
    puts("options:");
    puts("\t     -x n: repeat n in X direction, default is 1");
    puts("\t     -y n: repeat n in Y direction, default is 1");
    puts("\t     -z n: repeat n in Z direction, default is 1");
    puts("\t-o output: output file, default is stdout");
    puts("\t  -t type: output type, one of the following:");
    puts("\t           direct: default, use relative coordinates based on proportions of the cell");
    puts("\t           carte: expand coordinates to real cartesian");
    puts("\t       -r: regroup to make atoms of same species are neighbers in output positions");
    puts("\t       -v: calculate cell volume only, to stdout, and quit");
    puts("\t       -h: output this message");
    puts("\tinputfile: input coordinates in cellfile format");
    puts("\t           if not provided, stdin will be used");
}

int main(int argc, char *argv[])
{
    CoordData cdata;

    int repeat[3] = {1,1,1};
    CoordType out_type = Direct;

    bool regroup = false;
    bool show_vol = false;

    FILE *in = stdin; 
    FILE *out = stdout;

    const char * optstr = "o:x:y:z:t:hrv";
    int c;
    while ((c = getopt(argc, argv, optstr)) != -1){
        switch(c)
        {
            case 'h':
                do_usage();
                exit(0);
            case 'r':
                regroup = true;
                break;
            case 'v':
                show_vol = true;
                break;
            case 'o':
                if ( (out = fopen(optarg, "w")) == NULL)
                    out = stdout;
                break;
            case 'x':
                if ( (repeat[X] = atoi(optarg)) <= 0)
                    repeat[X] = 1;
                break;
            case 'y':
                if ( (repeat[Y] = atoi(optarg)) <= 0)
                    repeat[Y] = 1;
                break;
            case 'z':
                if ( (repeat[Z] = atoi(optarg)) <= 0)
                    repeat[Z] = 1;
                break;
            case 't':
                if ( optarg[0] == 'c' ) out_type = Carte;
                else out_type = Direct;
                break;
        }
    }

    if (optind < argc)
        if((in = fopen(argv[optind], "r")) == NULL)
            in = stdin;


    if (!scan_cellfile(in, &cdata)){
        fprintf(stderr, "Bad input! Check your cell file format!\n");
        exit(1);
    }

    if(show_vol){ 
        fprintf(stderr, "Cell volume is "ROP".\n", cell_volume(cdata.cell));
        exit(0);
    }

    scale_cell(cdata.cell, cdata.scaling);
    cdata.scaling = 1.0;
    expand_coorddata(&cdata, repeat[X], repeat[Y], repeat[Z], regroup);
    print_coorddata(out, &cdata, out_type);

    return 0;
}
