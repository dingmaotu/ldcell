/*
 * macro.h -- convenient c macro definitions
 *
 * ldcell -- a simple crystal cell processing utility 
 * Copyright (C) 2013 Li Ding <iamliding at gmail.com>
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

#ifndef _MACRO_H
#define _MACRO_H

/*
 * Wrap the basic token-pasting and stringification operators
 */
#define CAT(x,y) CAT_I(x,y)
#define CAT_I(x,y) x ## y

#define STR(x) STR_I(x)
#define STR_I(x) # x

#define APPLY(macro, args) APPLY_I(macro, args)
#define APPLY_I(macro, args) macro args

/*
 * The following trick of getting the number of args of variadic macros
 * is from:
 * http://efesx.com/2010/07/17/variadic-macro-to-count-number-of-arguments/
 * */
#define VA_NUM(...) VA_NUM_I(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1)
#define VA_NUM_I(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N

/*
 * REM_PAREN removes the parenthesis if its argument has one.
 * This enables an argument to be passed to a macro as a whole 
 * even if it contains commas.
 * */
#define REM_PAREN(x) CAT(REM_PAREN_,HAS_PAREN(x)) (x)

#define REM_PAREN_1(x) x
#define REM_PAREN_2(x) REM_PAREN_2_I x
#define REM_PAREN_2_I(...) __VA_ARGS__

#define HAS_PAREN(x) APPLY(VA_NUM, (HAS_PAREN_TEST x))
#define HAS_PAREN_TEST(...) 1,1

/*
 * This is a convenient loop construct that simulates FORTRAN DO 
 * */
#define DO(i,N) for(int i=0; i<N; ++i) {
#define END }

#define V1(x) x[__i]
#define V2(x) x[__i][__j]
#define V3(x) x[__i][__j][__k]

#define VEC1(s,D1)                  \
    do{                             \
        DO(__i,D1)                  \
            REM_PAREN(s);           \
        END                         \
    }while(0)

#define VEC2(s,D1,D2)               \
    do{                             \
        DO(__i,D1)                  \
            DO(__j,D2)              \
                REM_PAREN(s);       \
            END                     \
        END                         \
    }while(0)

#define VEC3(s,D1,D2,D3)             \
    do{                              \
        DO(__i,D1)                   \
            DO(__j,D2)               \
                DO(__k,D2)           \
                    REM_PAREN(s);    \
                END                  \
            END                      \
        END                          \
    }while(0)


/*
 * If your compiler supports variadic macro, you can use
 * VEC as a general vectorized environment rather than VEC1 VEC2 ...
 * For example, instead of
 * >>> VEC3(V3(a) = 1.0, 4,5,6);
 * you can say
 * >>> VEC(V3(a) = 1.0, 4,5,6);
 * This is a little more convenient.
 */

#define VEC(s, ...) CAT(VEC,VA_NUM(__VA_ARGS__))(s, __VA_ARGS__)

#endif // _MACRO_H
