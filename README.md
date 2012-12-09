ldcell - a simple crystal cell processing utility
=================================================

Introduction
------------

This is a small utility I created mainly to expand a primitive cell to a larger
one. For example, in Gromacs, you need a simulation cell that is 4x4x4 of the
primitive cell you downloaded or prepared. To expand the cell, you need to
replicate and translate the coordinates of the primitive cell, and regroup same
atoms together, so you can use them with a simple topology file.

There are many file formats that describe the atomic positions of a system
(take a look at the VMD open file dialog!), and of course ldcell can not
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

3. The next line specifies numbers of each kind of atoms.

4. All the remaining lines specify atom coordinates of the system, in the order
   specified in the previous item. All coordinates are in units of cell
   vectors, that is, fractions of primitive cell vectors.

Here is an example (the numbers are not real, do not use them!):

    1.0
    1.34 0.0 0.0
    0.0  1.20 0.0
    0.0  0.0  1.2
    16 16 48
    0.5 0.5 0.5
    ....

Usage
-----

To see the help message of ldcell, run `ldcell -h`.

Examples:

1. To calculate the volume of the cell

    ldcell -v [inputfile]

2. To expand the cell to 2x2x4, with cartesian coordinates output 
    
    ldcell -x 2 -y 2 -z 4 -r -t c [inputfile] -o [outputfile]

Build
-----

In the project root, type

    make

and you will find the binary in `bin`.

To build a debug version, type

    make dev

Acknowledgement
---------------

I used to learn C by myself, but I do not know the common idioms and practices
of real C projects. Then I found the book [Learn C The Hard
Way](http://c.learncodethehardway.org/book/), and I learned a lot, such as
project organization, tests, makefiles, etc. Here thanks Zed.
