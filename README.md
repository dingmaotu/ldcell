ldcell - a simple crystal cell processing utility
=================================================

Introduction
------------

This is a small utility I created mainly to expand a primitive cell to a larger
one. For example, in Gromacs, you need a simulation cell that is 4x4x4 of the
primitive cell you downloaded or prepared. Maybe you want to scale the cell
based on volume, too. To expand the cell, you need to replicate and translate
the coordinates of the primitive cell, and regroup same atoms together, so you
can use them with a simple topology file.

There are many file formats that describe the atomic positions of a system
(take a look at the VMD new molecule dialog!), and of course ldcell can not
process all of them. To make things simple, I adapted the VASP POSCAR format to
a simpler one, as described in the next section.

Cell file format
----------------

It is free format (spaces are ignored on the SAME line, no strict numeric
format is need), and rather simple:

1. On first line is a scaling factor. If it is a positive number, the cell
   vectors will be scalled that much; if negative, it specifies the volume of
   the primitive cell to which the primitive cell will be scaled.

2. The following three lines specify the primitive cell vectors. ldcell do not
   consider units, as long as they are consistent in the file.

3. The next line specifies numbers of each kind of atoms. ldcell do not care
   about the names of these kinds of atoms.

4. All the remaining lines specify atom coordinates of the system, in the order
   specified in the previous item. All coordinates are in units of cell
   vectors, that is, fractions of primitive cell vectors.

Here is an example (mgo.cell) of a basic MgO Periclase cell:

    1.0
    0.4217  0  0
    0  0.4217  0
    0  0  0.4217
    4  4
    0.0        0.0        0.0   
    0.5        0.0        0.5   
    0.5        0.5        0.0   
    0.0        0.5        0.5   
    0.0        0.0        0.5   
    0.5        0.0        0.0   
    0.5        0.5        0.5   
    0.0        0.5        0.0   

Usage
-----

To see the help message of ldcell, run `ldcell -h`.

Examples:

1. To calculate the volume of the cell

    ldcell -v [inputfile]

2. To expand the cell to 2x2x4, with cartesian coordinates output 
    
    ldcell -x 2 -y 2 -z 4 -r -t c [inputfile] -o [outputfile]

   The inputfile and outputfile are both optional. If omitted, stdin
   and stdout would be used.

3. Generate a Gromacs config file using utils scripts:

   expdcell mgo.cell 2 2 4 -0.1 | \
   cell2xdat "MgO Periclase 2x2x4 10% smaller" Mg O | \
   xdat2gro > conf.gro

   You need to give cell2xdat a title and names of each kind of atoms.

Build and Install
-----------------

In the project root, type

    make

and you will find the binary in `bin`.

To build a debug version, type

    make dev

To use the scripts in utils folder, you need to set a environment
variable LDCELL pointing to the root of ldcell distribution. For example, in
bash you need to add this to your `.bash_profile` (or `.profile`)

    export LDCELL=/path/to/ldcell

`xdat2gro` will use this to find its awk libraries.

Changes
-------

### 2013-3-7

1. Removed gengro.awk, which is too specific for my problems. Added expdcell,
   cell2xdat, xdat2gro. These scripts are fairly general and maybe useful to
   you.

2. `ldcell -v` returns a number (volume of cell) instead of a message, making
   it easy to use it in a script, such as `cellvol=$(ldcell -v $cellfile)`.

3. Added macro.h, a set of general C macros that vectorize C array operations,
   like Fortran. It makes writing numeric C code easier.

Acknowledgement
---------------

I used to learn C by myself, but I do not know the common idioms and practices
of real C projects. Then I found the book [Learn C The Hard
Way](http://c.learncodethehardway.org/book/), and I learned a lot, such as
project organization, tests, makefiles, etc. Here thanks Zed.
