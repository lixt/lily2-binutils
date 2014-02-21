/* Table of opcodes for the LILY2 ISA.
   Copyright(C) 2014 DSP Group, Institute of Microelectronics, Tsinghua University
   All rights reserved.

   Contributed by Xiaotian Li <lixiaotian07@gmail.com>.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#include "sysdep.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "safe-ctype.h"
#include "ansidecl.h"
#include "libiberty.h"
#include "opcode/lily2.h"


/* Returns number of nonzero bits.  */

static int
num_ones (unsigned long value)
{
  int c = 0;

  while (value)
    {
      if (value & 1)
	c++;
      value >>= 1;
    }
  return c;
}

const struct lily2_letter lily2_letters[] = {
    {'A', 5 , NUM_UNSIGNED},
    {'B', 5 , NUM_UNSIGNED},
    {'D', 5 , NUM_UNSIGNED},
    {'X', 5 , NUM_UNSIGNED},
    {'C', 2 , NUM_UNSIGNED},
    {'H', 2 , NUM_UNSIGNED},
    {'I', 8 , NUM_SIGNED  },
    {'J', 8 , NUM_UNSIGNED},
    {'K', 5 , NUM_SIGNED  },
    {'L', 5 , NUM_UNSIGNED},
    {'M', 16, NUM_SIGNED  },
    {'N', 22, NUM_SIGNED  },
    {'S', 2 , NUM_UNSIGNED},
    {'E', 1 , NUM_UNSIGNED},
    {'F', 2 , NUM_UNSIGNED},
    {'Z', 3 , NUM_UNSIGNED},
};
const size_t lily2_num_letters = ARRAY_SIZE (lily2_letters);

const struct lily2_opcode lily2_opc_opcodes_a[] =
{
    {"add"      , "rD,rA,rB", "E 00 000 000 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.b.4"  , "rD,rA,rB", "E 00 000 000 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.2"  , "rD,rA,rB", "E 00 000 000 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.4"  , "dD,dA,dB", "E 00 000 000 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.w.2"  , "dD,dA,dB", "E 00 000 000 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.i"    , "rD,rA,iI", "E 00 000 000 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"and"      , "rD,rA,rB", "E 00 001 000 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.b.4"  , "rD,rA,rB", "E 00 001 000 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.h.2"  , "rD,rA,rB", "E 00 001 000 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.h.4"  , "dD,dA,dB", "E 00 001 000 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.w.2"  , "dD,dA,dB", "E 00 001 000 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.i"    , "rD,rA,iI", "E 00 001 000 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"tgt"      , "rD,rA,rB", "E 00 010 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tgt.i"    , "rD,rA,iI", "E 00 010 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"tgt.u"    , "rD,rA,rB", "E 00 010 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tgt.u.i"  , "rD,rA,iJ", "E 00 010 000 0 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"gto.b.4"  , "rD,rA,rB", "E 00 011 000 1 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.h.2"  , "rD,rA,rB", "E 00 011 000 1 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.h.4"  , "dD,dA,dB", "E 00 011 000 1 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.w.2"  , "dD,dA,dB", "E 00 011 000 1 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.b.4"  , "rD,rA,rB", "E 00 011 000 1 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.h.2"  , "rD,rA,rB", "E 00 011 000 1 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.h.4"  , "dD,dA,dB", "E 00 011 000 1 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.w.2"  , "dD,dA,dB", "E 00 011 000 1 - 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.u.b.4", "rD,rA,rB", "E 00 011 000 0 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.u.h.2", "rD,rA,rB", "E 00 011 000 0 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.u.h.4", "dD,dA,dB", "E 00 011 000 0 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.u.w.2", "dD,dA,dB", "E 00 011 000 0 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.u.b.4", "rD,rA,rB", "E 00 011 000 0 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.u.h.2", "rD,rA,rB", "E 00 011 000 0 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.u.h.4", "dD,dA,dB", "E 00 011 000 0 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.u.w.2", "dD,dA,dB", "E 00 011 000 0 - 111BBBBB AAAAA DDDDD ZZZ", 0},


    {"lsl"      , "rD,rA,rB", "E 00 100 000 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.b.4"  , "rD,rA,rB", "E 00 100 000 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.h.2"  , "rD,rA,rB", "E 00 100 000 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.h.4"  , "dD,dA,dB", "E 00 100 000 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.w.2"  , "dD,dA,dB", "E 00 100 000 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.i"    , "rD,rA,iL", "E 00 100 000 - 1 ---LLLLL AAAAA DDDDD ZZZ", 0},

    {"mkl"      , "rD,iM"   , "E 00 101 00M M M MMMMMMMM MMMMM DDDDD ZZZ", 0},
    {"mkh"      , "rD,iM"   , "E 00 101 01M M M MMMMMMMM MMMMM DDDDD ZZZ", 0},

    {"zip.b.4"  , "dD,rA,rB", "E 00 110 000 - 0 -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"zip.h.2"  , "dD,rA,rB", "E 00 110 000 - 0 -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"zip.h.4"  , "qD,dA,dB", "E 00 110 000 - 0 -10BBBBB AAAAA DDDDD ZZZ", 0},
    {"zip.w.2"  , "qD,dA,dB", "E 00 110 000 - 0 -11BBBBB AAAAA DDDDD ZZZ", 0},

    {"add.sp"   , "rD,rA,rB", "E 01 000 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.dp"   , "dD,dA,dB", "E 01 000 000 - - 01-BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.sp.2" , "dD,dA,dB", "E 01 000 000 - - 10-BBBBB AAAAA DDDDD ZZZ", 0},

    {"tgt.sp"   , "rD,rA,rB", "E 01 010 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},
    {"tgt.dp"   , "dD,dA,dB", "E 01 010 000 - - 01-BBBBB AAAAA DDDDD ZZZ", 0},

    {"gto.sp.2" , "dD,dA,dB", "E 01 011 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.sp.2" , "dD,dA,dB", "E 01 011 000 - - 10-BBBBB AAAAA DDDDD ZZZ", 0},
};
const size_t lily2_num_opc_opcodes_a = ARRAY_SIZE (lily2_opc_opcodes_a);

const struct lily2_opcode lily2_opc_opcodes_m[] =
{
    {"mul"      , "rD,rA,rB", "E 10 000 000 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.b.4"  , "rD,rA,rB", "E 10 000 000 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.h.2"  , "rD,rA,rB", "E 10 000 000 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.h.4"  , "dD,dA,dB", "E 10 000 000 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.w.2"  , "dD,dA,dB", "E 10 000 000 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.i"    , "rD,rA,iI", "E 10 000 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"mul.u"    , "rD,rA,rB", "E 10 000 000 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.u.b.4", "rD,rA,rB", "E 10 000 000 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.u.h.2", "rD,rA,rB", "E 10 000 000 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.u.h.4", "dD,dA,dB", "E 10 000 000 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.u.w.2", "dD,dA,dB", "E 10 000 000 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.u.i"  , "rD,rA,iJ", "E 10 000 000 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"cmu.ch"   , "rD,rA,rB", "E 10 001 000 1 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.u.ch" , "rD,rA,rB", "E 10 001 000 0 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.cw"   , "dD,dA,dB", "E 10 001 000 1 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.u.cw" , "dD,dA,dB", "E 10 001 000 0 - 011BBBBB AAAAA DDDDD ZZZ", 0},

    {"add"      , "rD,rA,rB", "E 10 010 000 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.b.4"  , "rD,rA,rB", "E 10 010 000 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.2"  , "rD,rA,rB", "E 10 010 000 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.4"  , "dD,dA,dB", "E 10 010 000 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.w.2"  , "dD,dA,dB", "E 10 010 000 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.i"    , "rD,rA,iI", "E 10 010 000 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"mul.sp"   , "rD,rA,rB", "E 10 100 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.dp"   , "dD,dA,dB", "E 10 100 000 - - 01-BBBBB AAAAA DDDDD ZZZ", 0},
    {"mul.sp.2" , "dD,dA,dB", "E 10 100 000 - - 10-BBBBB AAAAA DDDDD ZZZ", 0},

    {"cmu.csp"  , "dD,dA,dB", "E 10 101 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},

    {"add.sp"   , "rD,rA,rB", "E 10 110 000 - - 00-BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.dp"   , "dD,dA,dB", "E 10 110 000 - - 01-BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.sp.2" , "dD,dA,dB", "E 10 110 000 - - 10-BBBBB AAAAA DDDDD ZZZ", 0}
};
const size_t lily2_num_opc_opcodes_m = ARRAY_SIZE (lily2_opc_opcodes_m);

const struct lily2_opcode lily2_opc_opcodes_d[] =
{
    {"ldb"       , "rD,rB(rA)"  , "E 11 000 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.i"     , "rD,iI(rA)"  , "E 11 000 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldb.u"     , "rD,rB(rA)"  , "E 11 000 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.u.i"   , "rD,iI(rA)"  , "E 11 000 000 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldb.ds"    , "rD,rB(rA)"  , "E 11 001 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.i"  , "rD,iI(rA)"  , "E 11 001 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.u"  , "rD,rB(rA)"  , "E 11 001 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.u.i", "rD,iI(rA)"  , "E 11 001 000 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldm"       , "rD,rB(*rA#)", "E 11 010 000 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.i"     , "rD,iK(*rA#)", "E 11 010 000 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"ldm.u"     , "rD,rB(*rA#)", "E 11 010 000 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.u.i"   , "rD,iK(*rA#)", "E 11 010 000 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"ldm.ds"    , "rD,rB(*rA#)", "E 11 010 000 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.ds.i"  , "rD,iK(*rA#)", "E 11 010 000 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"ldm.ds.u"  , "rD,rB(*rA#)", "E 11 010 000 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.ds.u.i", "rD,iK(*rA#)", "E 11 010 000 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
};
const size_t lily2_num_opc_opcodes_d = ARRAY_SIZE (lily2_opc_opcodes_d);

/* Here, functional unit shall be encoded in the opcode table.
   This table is just to get the cluster and the pointer to the specified
   opcode table. */
const struct lily2_functional_unit lily2_opc_functional_units[] =
{
    {"[xa]", 0x0, {"x", 0x0}, lily2_opc_opcodes_a},
    {"[xm]", 0x0, {"x", 0x0}, lily2_opc_opcodes_m},
    {"[xd]", 0x0, {"x", 0x0}, lily2_opc_opcodes_d},
    {"[ya]", 0x0, {"y", 0x1}, lily2_opc_opcodes_a},
    {"[ym]", 0x0, {"y", 0x1}, lily2_opc_opcodes_m},
    {"[yd]", 0x0, {"y", 0x1}, lily2_opc_opcodes_d},
};
const size_t lily2_num_opc_functional_units = ARRAY_SIZE (lily2_opc_functional_units);

const struct lily2_condition lily2_opc_conditions[] =
{
    {"\0"       , 0x0},
    {"{cf0}"    , 0x1},
    {"{cf1}"    , 0x2},
    {"{cf2}"    , 0x3},
    {"{!cf0}"   , 0x4},
    {"{!cf1}"   , 0x5},
    {"{!cf2}"   , 0x6},
};
const size_t lily2_num_opc_conditions = ARRAY_SIZE (lily2_opc_conditions);

const struct lily2_register lily2_opc_registers_x[] =
{
    {"x0" , 0 }, {"x1",  1 }, {"x2" , 2 }, {"x3" , 3 },
    {"x4" , 4 }, {"x5",  5 }, {"x6" , 6 }, {"x7" , 7 },
    {"x8" , 8 }, {"x9",  9 }, {"x10", 10}, {"x11", 11},
    {"x12", 12}, {"x13", 13}, {"x14", 14}, {"x15", 15},
    {"x16", 16}, {"x17", 17}, {"x18", 18}, {"x19", 19},
    {"x20", 20}, {"x21", 21}, {"x22", 22}, {"x23", 23},
    {"x24", 24}, {"x25", 25}, {"x26", 26}, {"x27", 27},
    {"x28", 28}, {"x29", 29}, {"x30", 30}, {"x31", 31},

    {"y24", 24}, {"y25", 25}, {"y26", 26}, {"y27", 27},
    {"y28", 28}, {"y29", 29}, {"y30", 30}, {"y31", 31},


    {"g0" , 24}, {"g1" , 25}, {"g2" , 26}, {"g3" , 27},
    {"g4" , 28}, {"g5" , 29}, {"g6" , 30}, {"g7" , 31},
};
const size_t lily2_num_opc_registers_x = ARRAY_SIZE (lily2_opc_registers_x);

const struct lily2_register lily2_opc_registers_y[] =
{
    {"y0" , 0 }, {"y1",  1 }, {"y2" , 2 }, {"y3" , 3 },
    {"y4" , 4 }, {"y5",  5 }, {"y6" , 6 }, {"y7" , 7 },
    {"y8" , 8 }, {"y9",  9 }, {"y10", 10}, {"y11", 11},
    {"y12", 12}, {"y13", 13}, {"y14", 14}, {"y15", 15},
    {"y16", 16}, {"y17", 17}, {"y18", 18}, {"y19", 19},
    {"y20", 20}, {"y21", 21}, {"y22", 22}, {"y23", 23},
    {"y24", 24}, {"y25", 25}, {"y26", 26}, {"y27", 27},
    {"y28", 28}, {"y29", 29}, {"y30", 30}, {"y31", 31},

    {"x24", 24}, {"x25", 25}, {"x26", 26}, {"x27", 27},
    {"x28", 28}, {"x29", 29}, {"x30", 30}, {"x31", 31},

    {"g0" , 24}, {"g1" , 25}, {"g2" , 26}, {"g3" , 27},
    {"g4" , 28}, {"g5" , 29}, {"g6" , 30}, {"g7" , 31},
};
const size_t lily2_num_opc_registers_y = ARRAY_SIZE (lily2_opc_registers_y);

const struct lily2_register lily2_opc_register_pairs_x[] =
{
    {"x1:x0"  ,  0}, {"x3:x2"  , 2 },
    {"x5:x4"  ,  4}, {"x7:x6"  , 6 },
    {"x9:x8"  ,  8}, {"x11:x10", 10},
    {"x13:x12", 12}, {"x15:x14", 14},
    {"x17:x16", 16}, {"x19:x18", 18},
    {"x21:x20", 20}, {"x23:x22", 22},
    {"x25:x24", 24}, {"x27:x26", 26},
    {"x29:x28", 28}, {"x31:x30", 30},

    {"y25:y24", 24}, {"y27:y26", 26},
    {"y29:y28", 28}, {"y31:y30", 30},

    {"g1:g0"  , 24}, {"g3:g2"  , 26},
    {"g5:g4"  , 28}, {"g7:g6"  , 30},
};
const size_t lily2_num_opc_register_pairs_x = ARRAY_SIZE (lily2_opc_register_pairs_x);

const struct lily2_register lily2_opc_register_pairs_y[] =
{
    {"y1:y0"  ,  0}, {"y3:y2"  , 2 },
    {"y5:y4"  ,  4}, {"y7:y6"  , 6 },
    {"y9:y8"  ,  8}, {"y11:y10", 10},
    {"y13:y12", 12}, {"y15:y14", 14},
    {"y17:y16", 16}, {"y19:y18", 18},
    {"y21:y20", 20}, {"y23:y22", 22},
    {"y25:y24", 24}, {"y27:y26", 26},
    {"y29:y28", 28}, {"y31:y30", 30},

    {"x25:x24", 24}, {"x27:x26", 26},
    {"x29:x28", 28}, {"x31:x30", 30},

    {"g1:g0"  , 24}, {"g3:g2"  , 26},
    {"g5:g4"  , 28}, {"g7:g6"  , 30},
};
const size_t lily2_num_opc_register_pairs_y = ARRAY_SIZE (lily2_opc_register_pairs_y);

const struct lily2_register lily2_opc_register_pair_pairs_x[] =
{
    {"x3:x2:x1:x0"    , 0 }, {"x7:x6:x5:x4"    , 4 },
    {"x11:x10:x9:x8"  , 8 }, {"x15:x14:x13:x12", 12},
    {"x19:x18:x17:x16", 16}, {"x23:x22:x21:x20", 20},
    {"x27:x26:x25:x24", 24}, {"x31:x30:x29:x28", 28},

    {"y27:y26:y25:y24", 24}, {"y31:y30:y29:y28", 28},

    {"g3:g2:g1:g0"    , 24}, {"g7:g6:g5:g4"    , 28},
};
const size_t lily2_num_opc_register_pair_pairs_x = ARRAY_SIZE (lily2_opc_register_pair_pairs_x);

const struct lily2_register lily2_opc_register_pair_pairs_y[] =
{
    {"y3:y2:y1:y0"    , 0 }, {"y7:y6:y5:y4"    , 4 },
    {"y11:y10:y9:y8"  , 8 }, {"y15:y14:y13:y12", 12},
    {"y19:y18:y17:y16", 16}, {"y23:y22:y21:y20", 20},
    {"y27:y26:y25:y24", 24}, {"y31:y30:y29:y28", 28},

    {"x27:x26:x25:x24", 24}, {"x31:x30:x29:x28", 28},

    {"g3:g2:g1:g0"    , 24}, {"g7:g6:g5:g4"    , 28},
};
const size_t lily2_num_opc_register_pair_pairs_y = ARRAY_SIZE (lily2_opc_register_pair_pairs_y);

const struct lily2_register lily2_opc_registers_m[] =
{
    {"m0" , 0 }, {"m1",  1 }, {"m2" , 2 }, {"m3" , 3 },
    {"m4" , 4 }, {"m5",  5 }, {"m6" , 6 }, {"m7" , 7 },
    {"m8" , 8 }, {"m9",  9 }, {"m10", 10}, {"m11", 11},
    {"m12", 12}, {"m13", 13}, {"m14", 14}, {"m15", 15},
    {"m16", 16}, {"m17", 17}, {"m18", 18}, {"m19", 19},
    {"m20", 20}, {"m21", 21}, {"m22", 22}, {"m23", 23},
    {"m24", 24}, {"m25", 25}, {"m26", 26}, {"m27", 27},
    {"m28", 28}, {"m29", 29}, {"m30", 30}, {"m31", 31},
};
const size_t lily2_num_opc_registers_m = ARRAY_SIZE (lily2_opc_registers_m);

const struct lily2_register lily2_opc_registers_c[] =
{
    {"cf0", 0 }, {"cf1", 1 }, {"cf2", 2 },
};
const size_t lily2_num_opc_registers_c = ARRAY_SIZE (lily2_opc_registers_c);

const struct lily2_addr_mod_sign lily2_opc_addr_mod_signs[] =
{
    {"(+", 0x0}, {"(-", 0x1}, {"+)", 0x2}, {"-)", 0x3},
};
const size_t lily2_num_opc_addr_mod_signs = ARRAY_SIZE (lily2_opc_addr_mod_signs);

struct lily2_letter *letter_find (char param_ch)
{
    int i;
    for (i = 0; i != lily2_num_letters; ++i) {
        if (lily2_letters[i].ch == param_ch)
            return &lily2_letters[i];
    }

    return NULL;
}

unsigned long mask (int nbits)
{
    int insn_bits = sizeof (unsigned long) * 8;
    return (nbits == insn_bits) ? (unsigned long) -1 : (1 << nbits) - 1;
}

unsigned long bits (unsigned long insn, int first, int last)
{
    int nbits = first - last + 1;
    return (insn >> last) & mask (nbits);
}

unsigned long replace_bits (unsigned long old_insn, int first, int last, unsigned long new_insn)
{
    unsigned long bmask = mask (first - last + 1) << last;
    return ((new_insn << last) & bmask) | (old_insn & ~bmask);
}

unsigned long sign_extend (unsigned long insn, int nbits, int sign)
{
    if (sign) { /* Signed. */
        if (bits (insn, nbits - 1, nbits - 1)) {
            return insn | ~mask (nbits);
        } else {
            return insn & mask (nbits);
        }
    } else { /* Unsigned. */
        return insn & mask (nbits);
    }
}

int check_overflow (unsigned long insn, int nbits, int sign)
{
    if (sign) { /* Signed. */
        return !(((long) (insn | mask (nbits - 1)) == -1) ||
                ((insn & ~mask (nbits - 1)) == 0));
    } else { /* Unsigned. */
        return insn & ~mask (nbits);
    }
}
