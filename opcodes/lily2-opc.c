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
    {'M', 16, NUM_UNSIGNED},
    {'N', 22, NUM_SIGNED  },
    {'P', 10, NUM_UNSIGNED},
    {'S', 2 , NUM_UNSIGNED},
    {'E', 1 , NUM_UNSIGNED},
    {'F', 2 , NUM_UNSIGNED},
    {'Z', 3 , NUM_UNSIGNED},
};
const size_t lily2_num_letters = ARRAY_SIZE (lily2_letters);

const struct lily2_opcode lily2_opc_opcodes_a[] =
{
    /* FU (0b'00) + OPH (0b'000). */
    {"add"      , "rD,rA,rB", "E 00 000 000 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.b.4"  , "rD,rA,rB", "E 00 000 000 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.2"  , "rD,rA,rB", "E 00 000 000 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.4"  , "dD,dA,dB", "E 00 000 000 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.w.2"  , "dD,dA,dB", "E 00 000 000 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.i"    , "rD,rA,iI", "E 00 000 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"adc"      , "rD,rA,rB", "E 00 000 001 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"adc.i"    , "rD,rA,iI", "E 00 000 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sub"      , "rD,rA,rB", "E 00 000 010 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.b.4"  , "rD,rA,rB", "E 00 000 010 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.h.2"  , "rD,rA,rB", "E 00 000 010 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.h.4"  , "dD,dA,dB", "E 00 000 010 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.w.2"  , "dD,dA,dB", "E 00 000 010 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.i"    , "rD,rA,iI", "E 00 000 010 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sbc"      , "rD,rA,rB", "E 00 000 011 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"sbc.i"    , "rD,rA,iI", "E 00 000 011 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"rsb"      , "rD,rA,rB", "E 00 000 100 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.b.4"  , "rD,rA,rB", "E 00 000 100 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.h.2"  , "rD,rA,rB", "E 00 000 100 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.h.4"  , "dD,dA,dB", "E 00 000 100 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.w.2"  , "dD,dA,dB", "E 00 000 100 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.i"    , "rD,rA,iI", "E 00 000 100 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"rsc"      , "rD,rA,rB", "E 00 000 101 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsc.i"    , "rD,rA,iI", "E 00 000 101 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"abs"      , "rD,rA"   , "E 00 000 111 1 0 0--00000 AAAAA DDDDD ZZZ", 0},
    {"abs.b.4"  , "rD,rA"   , "E 00 000 111 1 0 10000000 AAAAA DDDDD ZZZ", 0},
    {"abs.h.2"  , "rD,rA"   , "E 00 000 111 1 0 10100000 AAAAA DDDDD ZZZ", 0},
    {"abs.h.4"  , "dD,dA"   , "E 00 000 111 1 0 11000000 AAAAA DDDDD ZZZ", 0},
    {"abs.w.2"  , "dD,dA"   , "E 00 000 111 1 0 11100000 AAAAA DDDDD ZZZ", 0},

    {"neg"      , "rD,rA"   , "E 00 000 111 1 0 0--00001 AAAAA DDDDD ZZZ", 0},
    {"neg.b.4"  , "rD,rA"   , "E 00 000 111 1 0 10000001 AAAAA DDDDD ZZZ", 0},
    {"neg.h.2"  , "rD,rA"   , "E 00 000 111 1 0 10100001 AAAAA DDDDD ZZZ", 0},
    {"neg.h.4"  , "dD,dA"   , "E 00 000 111 1 0 11000001 AAAAA DDDDD ZZZ", 0},
    {"neg.w.2"  , "dD,dA"   , "E 00 000 111 1 0 11100001 AAAAA DDDDD ZZZ", 0},

    {"sxb"      , "rD,rA"   , "E 00 000 111 1 0 0--00010 AAAAA DDDDD ZZZ", 0},
    {"sxh"      , "rD,rA"   , "E 00 000 111 1 0 0--00011 AAAAA DDDDD ZZZ", 0},
    {"zxb"      , "rD,rA"   , "E 00 000 111 1 0 0--00100 AAAAA DDDDD ZZZ", 0},
    {"zxh"      , "rD,rA"   , "E 00 000 111 1 0 0--00101 AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'001). */
    {"and"      , "rD,rA,rB", "E 00 001 000 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.b.4"  , "rD,rA,rB", "E 00 001 000 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.h.2"  , "rD,rA,rB", "E 00 001 000 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.h.4"  , "dD,dA,dB", "E 00 001 000 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.w.2"  , "dD,dA,dB", "E 00 001 000 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"and.i"    , "rD,rA,iI", "E 00 001 000 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"nad"      , "rD,rA,rB", "E 00 001 001 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nad.b.4"  , "rD,rA,rB", "E 00 001 001 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nad.h.2"  , "rD,rA,rB", "E 00 001 001 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nad.h.4"  , "dD,dA,dB", "E 00 001 001 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nad.w.2"  , "dD,dA,dB", "E 00 001 001 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nad.i"    , "rD,rA,iI", "E 00 001 001 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"orr"      , "rD,rA,rB", "E 00 001 010 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"orr.b.4"  , "rD,rA,rB", "E 00 001 010 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"orr.h.2"  , "rD,rA,rB", "E 00 001 010 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"orr.h.4"  , "dD,dA,dB", "E 00 001 010 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"orr.w.2"  , "dD,dA,dB", "E 00 001 010 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"orr.i"    , "rD,rA,iI", "E 00 001 010 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"nor"      , "rD,rA,rB", "E 00 001 011 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nor.b.4"  , "rD,rA,rB", "E 00 001 011 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nor.h.2"  , "rD,rA,rB", "E 00 001 011 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nor.h.4"  , "dD,dA,dB", "E 00 001 011 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nor.w.2"  , "dD,dA,dB", "E 00 001 011 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nor.i"    , "rD,rA,iI", "E 00 001 011 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"xor"      , "rD,rA,rB", "E 00 001 100 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"xor.b.4"  , "rD,rA,rB", "E 00 001 100 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"xor.h.2"  , "rD,rA,rB", "E 00 001 100 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"xor.h.4"  , "dD,dA,dB", "E 00 001 100 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"xor.w.2"  , "dD,dA,dB", "E 00 001 100 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"xor.i"    , "rD,rA,iI", "E 00 001 100 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"nxr"      , "rD,rA,rB", "E 00 001 101 0 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nxr.b.4"  , "rD,rA,rB", "E 00 001 101 0 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nxr.h.2"  , "rD,rA,rB", "E 00 001 101 0 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nxr.h.4"  , "dD,dA,dB", "E 00 001 101 0 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nxr.w.2"  , "dD,dA,dB", "E 00 001 101 0 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nxr.i"    , "rD,rA,iI", "E 00 001 101 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"not"      , "rD,rA"   , "E 00 001 111 0 0 0--00000 AAAAA DDDDD ZZZ", 0},
    {"not.b.4"  , "rD,rA"   , "E 00 001 111 0 0 10000000 AAAAA DDDDD ZZZ", 0},
    {"not.h.2"  , "rD,rA"   , "E 00 001 111 0 0 10100000 AAAAA DDDDD ZZZ", 0},
    {"not.h.4"  , "dD,dA"   , "E 00 001 111 0 0 11000000 AAAAA DDDDD ZZZ", 0},
    {"not.w.2"  , "dD,dA"   , "E 00 001 111 0 0 11100000 AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'010). */
    {"tgt"      , "rD,rA,rB", "E 00 010 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tgt.i"    , "rD,rA,iI", "E 00 010 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"tgt.u"    , "rD,rA,rB", "E 00 010 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tgt.u.i"  , "rD,rA,iJ", "E 00 010 000 0 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"tge"      , "rD,rA,rB", "E 00 010 001 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tge.i"    , "rD,rA,iI", "E 00 010 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"tge.u"    , "rD,rA,rB", "E 00 010 001 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tge.u.i"  , "rD,rA,iJ", "E 00 010 001 0 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"tlt"      , "rD,rA,rB", "E 00 010 010 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tlt.i"    , "rD,rA,iI", "E 00 010 010 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"tlt.u"    , "rD,rA,rB", "E 00 010 010 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tlt.u.i"  , "rD,rA,iJ", "E 00 010 010 0 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"tle"      , "rD,rA,rB", "E 00 010 011 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tle.i"    , "rD,rA,iI", "E 00 010 011 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"tle.u"    , "rD,rA,rB", "E 00 010 011 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tle.u.i"  , "rD,rA,iJ", "E 00 010 011 0 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"teq"      , "rD,rA,rB", "E 00 010 100 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"teq.i"    , "rD,rA,iI", "E 00 010 100 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"tne"      , "rD,rA,rB", "E 00 010 101 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"tne.i"    , "rD,rA,iI", "E 00 010 101 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'011). */
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

    {"geo.b.4"  , "rD,rA,rB", "E 00 011 001 1 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.h.2"  , "rD,rA,rB", "E 00 011 001 1 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.h.4"  , "dD,dA,dB", "E 00 011 001 1 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.w.2"  , "dD,dA,dB", "E 00 011 001 1 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.b.4"  , "rD,rA,rB", "E 00 011 001 1 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.h.2"  , "rD,rA,rB", "E 00 011 001 1 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.h.4"  , "dD,dA,dB", "E 00 011 001 1 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.w.2"  , "dD,dA,dB", "E 00 011 001 1 - 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.u.b.4", "rD,rA,rB", "E 00 011 001 0 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.u.h.2", "rD,rA,rB", "E 00 011 001 0 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.u.h.4", "dD,dA,dB", "E 00 011 001 0 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.u.w.2", "dD,dA,dB", "E 00 011 001 0 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.u.b.4", "rD,rA,rB", "E 00 011 001 0 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.u.h.2", "rD,rA,rB", "E 00 011 001 0 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.u.h.4", "dD,dA,dB", "E 00 011 001 0 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.u.w.2", "dD,dA,dB", "E 00 011 001 0 - 111BBBBB AAAAA DDDDD ZZZ", 0},

    {"lto.b.4"  , "rD,rA,rB", "E 00 011 010 1 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.h.2"  , "rD,rA,rB", "E 00 011 010 1 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.h.4"  , "dD,dA,dB", "E 00 011 010 1 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.w.2"  , "dD,dA,dB", "E 00 011 010 1 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.b.4"  , "rD,rA,rB", "E 00 011 010 1 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.h.2"  , "rD,rA,rB", "E 00 011 010 1 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.h.4"  , "dD,dA,dB", "E 00 011 010 1 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.w.2"  , "dD,dA,dB", "E 00 011 010 1 - 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.u.b.4", "rD,rA,rB", "E 00 011 010 0 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.u.h.2", "rD,rA,rB", "E 00 011 010 0 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.u.h.4", "dD,dA,dB", "E 00 011 010 0 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"lto.u.w.2", "dD,dA,dB", "E 00 011 010 0 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.u.b.4", "rD,rA,rB", "E 00 011 010 0 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.u.h.2", "rD,rA,rB", "E 00 011 010 0 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.u.h.4", "dD,dA,dB", "E 00 011 010 0 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lta.u.w.2", "dD,dA,dB", "E 00 011 010 0 - 111BBBBB AAAAA DDDDD ZZZ", 0},

    {"leo.b.4"  , "rD,rA,rB", "E 00 011 011 1 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.h.2"  , "rD,rA,rB", "E 00 011 011 1 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.h.4"  , "dD,dA,dB", "E 00 011 011 1 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.w.2"  , "dD,dA,dB", "E 00 011 011 1 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.b.4"  , "rD,rA,rB", "E 00 011 011 1 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.h.2"  , "rD,rA,rB", "E 00 011 011 1 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.h.4"  , "dD,dA,dB", "E 00 011 011 1 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.w.2"  , "dD,dA,dB", "E 00 011 011 1 - 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.u.b.4", "rD,rA,rB", "E 00 011 011 0 - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.u.h.2", "rD,rA,rB", "E 00 011 011 0 - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.u.h.4", "dD,dA,dB", "E 00 011 011 0 - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"leo.u.w.2", "dD,dA,dB", "E 00 011 011 0 - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.u.b.4", "rD,rA,rB", "E 00 011 011 0 - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.u.h.2", "rD,rA,rB", "E 00 011 011 0 - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.u.h.4", "dD,dA,dB", "E 00 011 011 0 - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lea.u.w.2", "dD,dA,dB", "E 00 011 011 0 - 111BBBBB AAAAA DDDDD ZZZ", 0},

    {"gto.b.4"  , "rD,rA,rB", "E 00 011 100 - - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.h.2"  , "rD,rA,rB", "E 00 011 100 - - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.h.4"  , "dD,dA,dB", "E 00 011 100 - - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"gto.w.2"  , "dD,dA,dB", "E 00 011 100 - - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.b.4"  , "rD,rA,rB", "E 00 011 100 - - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.h.2"  , "rD,rA,rB", "E 00 011 100 - - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.h.4"  , "dD,dA,dB", "E 00 011 100 - - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gta.w.2"  , "dD,dA,dB", "E 00 011 100 - - 111BBBBB AAAAA DDDDD ZZZ", 0},

    {"geo.b.4"  , "rD,rA,rB", "E 00 011 101 - - 000BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.h.2"  , "rD,rA,rB", "E 00 011 101 - - 001BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.h.4"  , "dD,dA,dB", "E 00 011 101 - - 010BBBBB AAAAA DDDDD ZZZ", 0},
    {"geo.w.2"  , "dD,dA,dB", "E 00 011 101 - - 011BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.b.4"  , "rD,rA,rB", "E 00 011 101 - - 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.h.2"  , "rD,rA,rB", "E 00 011 101 - - 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.h.4"  , "dD,dA,dB", "E 00 011 101 - - 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"gea.w.2"  , "dD,dA,dB", "E 00 011 101 - - 111BBBBB AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'100). */
    {"lsl"      , "rD,rA,rB", "E 00 100 000 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.b.4"  , "rD,rA,rB", "E 00 100 000 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.h.2"  , "rD,rA,rB", "E 00 100 000 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.h.4"  , "dD,dA,dB", "E 00 100 000 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.w.2"  , "dD,dA,dB", "E 00 100 000 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsl.i"    , "rD,rA,iL", "E 00 100 000 - 1 ---LLLLL AAAAA DDDDD ZZZ", 0},

    {"lsr"      , "rD,rA,rB", "E 00 100 001 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsr.b.4"  , "rD,rA,rB", "E 00 100 001 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsr.h.2"  , "rD,rA,rB", "E 00 100 001 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsr.h.4"  , "dD,dA,dB", "E 00 100 001 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsr.w.2"  , "dD,dA,dB", "E 00 100 001 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"lsr.i"    , "rD,rA,iL", "E 00 100 001 - 1 ---LLLLL AAAAA DDDDD ZZZ", 0},

    {"asr"      , "rD,rA,rB", "E 00 100 010 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"asr.b.4"  , "rD,rA,rB", "E 00 100 010 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"asr.h.2"  , "rD,rA,rB", "E 00 100 010 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"asr.h.4"  , "dD,dA,dB", "E 00 100 010 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"asr.w.2"  , "dD,dA,dB", "E 00 100 010 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"asr.i"    , "rD,rA,iL", "E 00 100 010 - 1 ---LLLLL AAAAA DDDDD ZZZ", 0},

    {"rsr"      , "rD,rA,rB", "E 00 100 011 - 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsr.b.4"  , "rD,rA,rB", "E 00 100 011 - 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsr.h.2"  , "rD,rA,rB", "E 00 100 011 - 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsr.h.4"  , "dD,dA,dB", "E 00 100 011 - 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsr.w.2"  , "dD,dA,dB", "E 00 100 011 - 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsr.i"    , "rD,rA,iL", "E 00 100 011 - 1 ---LLLLL AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'101). */
    {"mkl"      , "rD,iM"   , "E 00 101 00M M M MMMMMMMM MMMMM DDDDD ZZZ", RELOC_LO16},
    {"mkh"      , "rD,iM"   , "E 00 101 01M M M MMMMMMMM MMMMM DDDDD ZZZ", RELOC_HI16},

    {"mrw"      , "rD,rA"   , "E 00 101 111 - - ---00000 AAAAA DDDDD ZZZ", 0},
    {"srw"      , "rD,rA"   , "E 00 101 111 - - ---00001 AAAAA DDDDD ZZZ", 0},
    {"mrd"      , "dD,dA"   , "E 00 101 111 - - ---00010 AAAAA DDDDD ZZZ", 0},
    {"srd"      , "dD,dA"   , "E 00 101 111 - - ---00011 AAAAA DDDDD ZZZ", 0},
    {"maw"      , "rX,rA"   , "E 00 101 111 - - ---00100 AAAAA DDDDD ZZZ", 0},
    {"saw"      , "rX,rA"   , "E 00 101 111 - - ---00101 AAAAA DDDDD ZZZ", 0},
    {"mad"      , "dX,dA"   , "E 00 101 111 - - ---00110 AAAAA DDDDD ZZZ", 0},
    {"sad"      , "dX,dA"   , "E 00 101 111 - - ---00111 AAAAA DDDDD ZZZ", 0},
    {"mcw"      , "rD,rC"   , "E 00 101 111 - - ---01010 AAAAA DDDDD ZZZ", 0},
    {"scw"      , "rD,rC"   , "E 00 101 111 - - ---01011 AAAAA DDDDD ZZZ", 0},

    /* FU (0b'00) + OPH (0b'110). */
    {"zip.b.4"  , "dD,rA,rB", "E 00 110 000 - - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"zip.h.2"  , "dD,rA,rB", "E 00 110 000 - - -01BBBBB AAAAA DDDDD ZZZ", 0},

    {"uzp.b.4"  , "dD,rA,rB", "E 00 110 001 - - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"uzp.h.2"  , "dD,rA,rB", "E 00 110 001 - - -01BBBBB AAAAA DDDDD ZZZ", 0},

    {"uct.b.4"  , "rD,rA,iH", "E 00 110 010 - - -00---HH AAAAA DDDDD ZZZ", 0},
    {"uct.h.2"  , "rD,rA,iG", "E 00 110 010 - - -01----G AAAAA DDDDD ZZZ", 0},

    {"mct.b.4"  , "rD,rA,iH", "E 00 110 011 - - -00---HH AAAAA DDDDD ZZZ", 0},
    {"mct.h.2"  , "rD,rA,iG", "E 00 110 011 - - -01----G AAAAA DDDDD ZZZ", 0},

    {"swp.b.4"  , "rD,rA"   , "E 00 110 111 - - -0000000 AAAAA DDDDD ZZZ", 0},
    {"swp.h.2"  , "rD,rA"   , "E 00 110 111 - - -0100000 AAAAA DDDDD ZZZ", 0},

    {"rvs.b.4"  , "rD,rA"   , "E 00 110 111 - - -0011111 AAAAA DDDDD ZZZ", 0},
    {"rvs.h.2"  , "rD,rA"   , "E 00 110 111 - - -0111111 AAAAA DDDDD ZZZ", 0},

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
    /* FU (0b'10) + OPH (0b'000). */
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

    {"nmu"      , "rD,rA,rB", "E 10 000 001 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.b.4"  , "rD,rA,rB", "E 10 000 001 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.h.2"  , "rD,rA,rB", "E 10 000 001 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.h.4"  , "dD,dA,dB", "E 10 000 001 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.w.2"  , "dD,dA,dB", "E 10 000 001 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.i"    , "rD,rA,iI", "E 10 000 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"nmu.u"    , "rD,rA,rB", "E 10 000 001 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.u.b.4", "rD,rA,rB", "E 10 000 001 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.u.h.2", "rD,rA,rB", "E 10 000 001 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.u.h.4", "dD,dA,dB", "E 10 000 001 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.u.w.2", "dD,dA,dB", "E 10 000 001 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nmu.u.i"  , "rD,rA,iJ", "E 10 000 001 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"mac"      , "rD,rA,rB", "E 10 000 010 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.b.4"  , "rD,rA,rB", "E 10 000 010 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.h.2"  , "rD,rA,rB", "E 10 000 010 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.h.4"  , "dD,dA,dB", "E 10 000 010 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.w.2"  , "dD,dA,dB", "E 10 000 010 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.i"    , "rD,rA,iI", "E 10 000 010 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"mac.u"    , "rD,rA,rB", "E 10 000 010 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.u.b.4", "rD,rA,rB", "E 10 000 010 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.u.h.2", "rD,rA,rB", "E 10 000 010 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.u.h.4", "dD,dA,dB", "E 10 000 010 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.u.w.2", "dD,dA,dB", "E 10 000 010 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"mac.u.i"  , "rD,rA,iJ", "E 10 000 010 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"nma"      , "rD,rA,rB", "E 10 000 011 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.b.4"  , "rD,rA,rB", "E 10 000 011 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.h.2"  , "rD,rA,rB", "E 10 000 011 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.h.4"  , "dD,dA,dB", "E 10 000 011 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.w.2"  , "dD,dA,dB", "E 10 000 011 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.i"    , "rD,rA,iI", "E 10 000 011 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"nma.u"    , "rD,rA,rB", "E 10 000 011 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.u.b.4", "rD,rA,rB", "E 10 000 011 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.u.h.2", "rD,rA,rB", "E 10 000 011 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.u.h.4", "dD,dA,dB", "E 10 000 011 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.u.w.2", "dD,dA,dB", "E 10 000 011 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nma.u.i"  , "rD,rA,iJ", "E 10 000 011 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"msu"      , "rD,rA,rB", "E 10 000 100 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.b.4"  , "rD,rA,rB", "E 10 000 100 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.h.2"  , "rD,rA,rB", "E 10 000 100 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.h.4"  , "dD,dA,dB", "E 10 000 100 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.w.2"  , "dD,dA,dB", "E 10 000 100 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.i"    , "rD,rA,iI", "E 10 000 100 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"msu.u"    , "rD,rA,rB", "E 10 000 100 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.u.b.4", "rD,rA,rB", "E 10 000 100 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.u.h.2", "rD,rA,rB", "E 10 000 100 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.u.h.4", "dD,dA,dB", "E 10 000 100 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.u.w.2", "dD,dA,dB", "E 10 000 100 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"msu.u.i"  , "rD,rA,iJ", "E 10 000 100 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"nms"      , "rD,rA,rB", "E 10 000 101 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.b.4"  , "rD,rA,rB", "E 10 000 101 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.h.2"  , "rD,rA,rB", "E 10 000 101 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.h.4"  , "dD,dA,dB", "E 10 000 101 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.w.2"  , "dD,dA,dB", "E 10 000 101 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.i"    , "rD,rA,iI", "E 10 000 101 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"nms.u"    , "rD,rA,rB", "E 10 000 101 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.u.b.4", "rD,rA,rB", "E 10 000 101 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.u.h.2", "rD,rA,rB", "E 10 000 101 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.u.h.4", "dD,dA,dB", "E 10 000 101 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.u.w.2", "dD,dA,dB", "E 10 000 101 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"nms.u.i"  , "rD,rA,iJ", "E 10 000 101 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"div"      , "rD,rA,rB", "E 10 000 110 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.b.4"  , "rD,rA,rB", "E 10 000 110 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.h.2"  , "rD,rA,rB", "E 10 000 110 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.h.4"  , "dD,dA,dB", "E 10 000 110 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.w.2"  , "dD,dA,dB", "E 10 000 110 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.i"    , "rD,rA,iI", "E 10 000 110 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"div.u"    , "rD,rA,rB", "E 10 000 110 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.u.b.4", "rD,rA,rB", "E 10 000 110 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.u.h.2", "rD,rA,rB", "E 10 000 110 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.u.h.4", "dD,dA,dB", "E 10 000 110 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.u.w.2", "dD,dA,dB", "E 10 000 110 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"div.u.i"  , "rD,rA,iJ", "E 10 000 110 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    {"rem"      , "rD,rA,rB", "E 10 000 111 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.b.4"  , "rD,rA,rB", "E 10 000 111 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.h.2"  , "rD,rA,rB", "E 10 000 111 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.h.4"  , "dD,dA,dB", "E 10 000 111 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.w.2"  , "dD,dA,dB", "E 10 000 111 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.i"    , "rD,rA,iI", "E 10 000 111 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"rem.u"    , "rD,rA,rB", "E 10 000 111 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.u.b.4", "rD,rA,rB", "E 10 000 111 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.u.h.2", "rD,rA,rB", "E 10 000 111 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.u.h.4", "dD,dA,dB", "E 10 000 111 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.u.w.2", "dD,dA,dB", "E 10 000 111 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"rem.u.i"  , "rD,rA,iJ", "E 10 000 111 1 1 JJJJJJJJ AAAAA DDDDD ZZZ", 0},

    /* FU (0b'10) + OPH (0b'001). */
    {"cmu.ch"   , "rD,rA,rB", "E 10 001 000 1 - -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.u.ch" , "rD,rA,rB", "E 10 001 000 0 - -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.cw"   , "dD,dA,dB", "E 10 001 000 1 - -11BBBBB AAAAA DDDDD ZZZ", 0},
    {"cmu.u.cw" , "dD,dA,dB", "E 10 001 000 0 - -11BBBBB AAAAA DDDDD ZZZ", 0},

    {"dmu.b.4"  , "rD,rA,rB", "E 10 001 100 1 - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.u.b.4", "rD,rA,rB", "E 10 001 100 0 - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.h.2"  , "rD,rA,rB", "E 10 001 100 1 - -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.u.h.2", "rD,rA,rB", "E 10 001 100 0 - -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.h.4"  , "rD,dA,dB", "E 10 001 100 1 - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.u.h.4", "rD,dA,dB", "E 10 001 100 0 - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.w.2"  , "rD,dA,dB", "E 10 001 100 1 - -01BBBBB AAAAA DDDDD ZZZ", 0},
    {"dmu.u.w.2", "rD,dA,dB", "E 10 001 100 0 - -01BBBBB AAAAA DDDDD ZZZ", 0},

    /* FU (0b'10) + OPH (0b'010). */
    {"add"      , "rD,rA,rB", "E 10 010 000 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.b.4"  , "rD,rA,rB", "E 10 010 000 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.2"  , "rD,rA,rB", "E 10 010 000 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.h.4"  , "dD,dA,dB", "E 10 010 000 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.w.2"  , "dD,dA,dB", "E 10 010 000 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"add.i"    , "rD,rA,iI", "E 10 010 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"adc"      , "rD,rA,rB", "E 10 010 001 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"adc.i"    , "rD,rA,iI", "E 10 010 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sub"      , "rD,rA,rB", "E 10 010 010 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.b.4"  , "rD,rA,rB", "E 10 010 010 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.h.2"  , "rD,rA,rB", "E 10 010 010 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.h.4"  , "dD,dA,dB", "E 10 010 010 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.w.2"  , "dD,dA,dB", "E 10 010 010 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"sub.i"    , "rD,rA,iI", "E 10 010 010 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sbc"      , "rD,rA,rB", "E 10 010 011 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"sbc.i"    , "rD,rA,iI", "E 10 010 011 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"rsb"      , "rD,rA,rB", "E 10 010 100 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.b.4"  , "rD,rA,rB", "E 10 010 100 1 0 100BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.h.2"  , "rD,rA,rB", "E 10 010 100 1 0 101BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.h.4"  , "dD,dA,dB", "E 10 010 100 1 0 110BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.w.2"  , "dD,dA,dB", "E 10 010 100 1 0 111BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsb.i"    , "rD,rA,iI", "E 10 010 100 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"rsc"      , "rD,rA,rB", "E 10 010 101 1 0 0--BBBBB AAAAA DDDDD ZZZ", 0},
    {"rsc.i"    , "rD,rA,iI", "E 10 010 101 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"abs"      , "rD,rA"   , "E 10 010 111 1 0 0--00000 AAAAA DDDDD ZZZ", 0},
    {"abs.b.4"  , "rD,rA"   , "E 10 010 111 1 0 10000000 AAAAA DDDDD ZZZ", 0},
    {"abs.h.2"  , "rD,rA"   , "E 10 010 111 1 0 10100000 AAAAA DDDDD ZZZ", 0},
    {"abs.h.4"  , "dD,dA"   , "E 10 010 111 1 0 11000000 AAAAA DDDDD ZZZ", 0},
    {"abs.w.2"  , "dD,dA"   , "E 10 010 111 1 0 11100000 AAAAA DDDDD ZZZ", 0},

    {"neg"      , "rD,rA"   , "E 10 010 111 1 0 0--00001 AAAAA DDDDD ZZZ", 0},
    {"neg.b.4"  , "rD,rA"   , "E 10 010 111 1 0 10000001 AAAAA DDDDD ZZZ", 0},
    {"neg.h.2"  , "rD,rA"   , "E 10 010 111 1 0 10100001 AAAAA DDDDD ZZZ", 0},
    {"neg.h.4"  , "dD,dA"   , "E 10 010 111 1 0 11000001 AAAAA DDDDD ZZZ", 0},
    {"neg.w.2"  , "dD,dA"   , "E 10 010 111 1 0 11100001 AAAAA DDDDD ZZZ", 0},

    {"sxb"      , "rD,rA"   , "E 10 000 111 1 0 0--00010 AAAAA DDDDD ZZZ", 0},
    {"sxh"      , "rD,rA"   , "E 10 000 111 1 0 0--00011 AAAAA DDDDD ZZZ", 0},
    {"zxb"      , "rD,rA"   , "E 10 000 111 1 0 0--00100 AAAAA DDDDD ZZZ", 0},
    {"zxh"      , "rD,rA"   , "E 10 000 111 1 0 0--00101 AAAAA DDDDD ZZZ", 0},

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
    /* FU (0b'11) + OPH (0b'000). */
    {"ldb"       , "rD,rB(rA)"  , "E 11 000 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.i"     , "rD,iI(rA)"  , "E 11 000 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldb.u"     , "rD,rB(rA)"  , "E 11 000 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.u.i"   , "rD,iI(rA)"  , "E 11 000 000 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldh"       , "rD,rB(rA)"  , "E 11 000 001 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldh.i"     , "rD,iI(rA)"  , "E 11 000 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldh.u"     , "rD,rB(rA)"  , "E 11 000 001 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldh.u.i"   , "rD,iI(rA)"  , "E 11 000 001 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldw"       , "rD,rB(rA)"  , "E 11 000 010 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldw.i"     , "rD,iI(rA)"  , "E 11 000 010 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldd"       , "dD,rB(rA)"  , "E 11 000 011 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldd.i"     , "dD,iI(rA)"  , "E 11 000 011 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"stb"       , "rB(rA),rD"  , "E 11 000 100 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"stb.i"     , "iI(rA),rD"  , "E 11 000 100 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sth"       , "rB(rA),rD"  , "E 11 000 101 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"sth.i"     , "iI(rA),rD"  , "E 11 000 101 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"stw"       , "rB(rA),rD"  , "E 11 000 110 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"stw.i"     , "iI(rA),rD"  , "E 11 000 110 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"std"       , "rB(rA),dD"  , "E 11 000 111 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"std.i"     , "iI(rA),dD"  , "E 11 000 111 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    /* FU (0b'11) + OPH (0b'001). */
    {"ldb.ds"    , "rD,rB(rA)"  , "E 11 001 000 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.i"  , "rD,iI(rA)"  , "E 11 001 000 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.u"  , "rD,rB(rA)"  , "E 11 001 000 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldb.ds.u.i", "rD,iI(rA)"  , "E 11 001 000 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldh.ds"    , "rD,rB(rA)"  , "E 11 001 001 1 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldh.ds.i"  , "rD,iI(rA)"  , "E 11 001 001 1 1 IIIIIIII AAAAA DDDDD ZZZ", 0},
    {"ldh.ds.u"  , "rD,rB(rA)"  , "E 11 001 001 0 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldh.ds.u.i", "rD,iI(rA)"  , "E 11 001 001 0 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldw.ds"    , "rD,rB(rA)"  , "E 11 001 010 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldw.ds.i"  , "rD,iI(rA)"  , "E 11 001 010 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"ldd.ds"    , "dD,rB(rA)"  , "E 11 001 011 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"ldd.ds.i"  , "dD,iI(rA)"  , "E 11 001 011 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"stb.ds"    , "rB(rA),rD"  , "E 11 001 100 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"stb.ds.i"  , "iI(rA),rD"  , "E 11 001 100 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"sth.ds"    , "rB(rA),rD"  , "E 11 001 101 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"sth.ds.i"  , "iI(rA),rD"  , "E 11 001 101 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"stw.ds"    , "rB(rA),rD"  , "E 11 001 110 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"stw.ds.i"  , "iI(rA),rD"  , "E 11 001 110 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    {"std.ds"    , "rB(rA),dD"  , "E 11 001 111 - 0 ---BBBBB AAAAA DDDDD ZZZ", 0},
    {"std.ds.i"  , "iI(rA),dD"  , "E 11 001 111 - 1 IIIIIIII AAAAA DDDDD ZZZ", 0},

    /* FU (0b'11) + OPH (0b'010). */
    {"lbm"       , "rD,rB(*rA#)", "E 11 010 000 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lbm.i"     , "rD,iK(*rA#)", "E 11 010 000 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"lbm.u"     , "rD,rB(*rA#)", "E 11 010 000 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lbm.u.i"   , "rD,iK(*rA#)", "E 11 010 000 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"lhm"       , "rD,rB(*rA#)", "E 11 010 001 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lhm.i"     , "rD,iK(*rA#)", "E 11 010 001 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"lhm.u"     , "rD,rB(*rA#)", "E 11 010 001 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lhm.u.i"   , "rD,iK(*rA#)", "E 11 010 001 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"lwm"       , "rD,rB(*rA#)", "E 11 010 010 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lwm.i"     , "rD,iK(*rA#)", "E 11 010 010 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"ldm"       , "dD,rB(*rA#)", "E 11 010 011 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.i"     , "dD,iK(*rA#)", "E 11 010 011 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"sbm"       , "rB(*rA#),rD", "E 11 010 100 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"sbm.i"     , "iK(*rA#),rD", "E 11 010 100 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"shm"       , "rB(*rA#),rD", "E 11 010 101 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"shm.i"     , "iK(*rA#),rD", "E 11 010 101 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"swm"       , "rB(*rA#),rD", "E 11 010 110 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"swm.i"     , "iK(*rA#),rD", "E 11 010 110 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"sdm"       , "rB(*rA#),dD", "E 11 010 111 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"sdm.i"     , "iK(*rA#),dD", "E 11 010 111 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    /* FU (0b'11) + OPH (0b'011). */
    {"lbm.ds"    , "rD,rB(*rA#)", "E 11 011 000 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lbm.ds.i"  , "rD,iK(*rA#)", "E 11 011 000 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"lbm.ds.u"  , "rD,rB(*rA#)", "E 11 011 000 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lbm.ds.u.i", "rD,iK(*rA#)", "E 11 011 000 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"lhm.ds"    , "rD,rB(*rA#)", "E 11 011 001 1 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lhm.ds.i"  , "rD,iK(*rA#)", "E 11 011 001 1 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},
    {"lhm.ds.u"  , "rD,rB(*rA#)", "E 11 011 001 0 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lhm.ds.u.i", "rD,iK(*rA#)", "E 11 011 001 0 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"lwm.ds"    , "rD,rB(*rA#)", "E 11 011 010 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"lwm.ds.i"  , "rD,iK(*rA#)", "E 11 011 010 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"ldm.ds"    , "dD,rB(*rA#)", "E 11 011 011 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"ldm.ds.i"  , "dD,iK(*rA#)", "E 11 011 011 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"stm.ds"    , "rB(*rA#),rD", "E 11 011 100 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"stm.ds.i"  , "iK(*rA#),rD", "E 11 011 100 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"stm.ds"    , "rB(*rA#),rD", "E 11 011 101 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"stm.ds.i"  , "iK(*rA#),rD", "E 11 011 101 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"stm.ds"    , "rB(*rA#),rD", "E 11 011 110 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"stm.ds.i"  , "iK(*rA#),rD", "E 11 011 110 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    {"stm.ds"    , "rB(*rA#),dD", "E 11 011 111 - 0 -SSBBBBB AAAAA DDDDD ZZZ", 0},
    {"stm.ds.i"  , "iK(*rA#),dD", "E 11 011 111 - 1 -SSKKKKK AAAAA DDDDD ZZZ", 0},

    /* FU (0b'11) + OPH (0b'100). */
    {"bri"       , "iN"         , "E 11 100 00N N N NNNNNNNN NNNNN NNNNN ZZZ", 0},
    {"bil"       , "iN"         , "E 11 100 01N N N NNNNNNNN NNNNN NNNNN ZZZ", 0},

    {"brr"       , "rA"         , "E 11 100 100 - - -------- AAAAA ----- ZZZ", 0},

    {"ret"       , ""           , "E 11 100 101 - - ---00000 ----- ----- ZZZ", 0},
    {"irt"       , ""           , "E 11 100 101 - - ---00001 ----- ----- ZZZ", 0},

    {"brl"       , "rA"         , "E 11 100 110 - - -------- AAAAA ----- ZZZ", 0},

    {"nop"       , ""           , "E 11 100 111 - - ---00000 ----- ----- ZZZ", 0},
    {"smr"       , ""           , "E 11 100 111 - - ---00001 ----- ----- ZZZ", 0},
    {"smv"       , ""           , "E 11 100 111 - - ---00010 ----- ----- ZZZ", 0},
    {"sys"       , "iP"         , "E 11 100 111 - - ---00011 PPPPP PPPPP ZZZ", 0},

    /* FU (0b'11) + OPH (0b'101). */
    {"bri.ds"    , "iN"         , "E 11 101 00N N N NNNNNNNN NNNNN NNNNN ZZZ", 0},
    {"bil.ds"    , "iN"         , "E 11 101 01N N N NNNNNNNN NNNNN NNNNN ZZZ", 0},

    {"brr.ds"    , "rA"         , "E 11 101 100 - - -------- AAAAA ----- ZZZ", 0},

    {"ret.ds"    , ""           , "E 11 101 101 - - ---00000 ----- ----- ZZZ", 0},
    {"irt.ds"    , ""           , "E 11 101 101 - - ---00001 ----- ----- ZZZ", 0},

    {"brl.ds"    , "rA"         , "E 11 101 110 - - -------- AAAAA ----- ZZZ", 0},

    /* FU (0b'11) + OPH (0b'110). */
    {"mkl"      , "rD,iM"   , "E 11 110 00M M M MMMMMMMM MMMMM DDDDD ZZZ", RELOC_LO16},
    {"mkh"      , "rD,iM"   , "E 11 110 01M M M MMMMMMMM MMMMM DDDDD ZZZ", RELOC_HI16},

    {"mrw"      , "rD,rA"   , "E 11 110 111 - - ---00000 AAAAA DDDDD ZZZ", 0},
    {"srw"      , "rD,rA"   , "E 11 110 111 - - ---00001 AAAAA DDDDD ZZZ", 0},
    {"mrd"      , "dD,dA"   , "E 11 110 111 - - ---00010 AAAAA DDDDD ZZZ", 0},
    {"srd"      , "dD,dA"   , "E 11 110 111 - - ---00011 AAAAA DDDDD ZZZ", 0},
    {"maw"      , "rX,rA"   , "E 11 110 111 - - ---00100 AAAAA DDDDD ZZZ", 0},
    {"saw"      , "rX,rA"   , "E 11 110 111 - - ---00101 AAAAA DDDDD ZZZ", 0},
    {"mad"      , "dX,dA"   , "E 11 110 111 - - ---00110 AAAAA DDDDD ZZZ", 0},
    {"sad"      , "dX,dA"   , "E 11 110 111 - - ---00111 AAAAA DDDDD ZZZ", 0},
    {"mcw"      , "rD,rC"   , "E 11 110 111 - - ---01010 AAAAA DDDDD ZZZ", 0},
    {"scw"      , "rD,rC"   , "E 11 110 111 - - ---01011 AAAAA DDDDD ZZZ", 0},

    /* FU (0b'11) + OPH (0b'111). */
    {"zip.b.4"  , "dD,rA,rB", "E 11 111 000 - - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"zip.h.2"  , "dD,rA,rB", "E 11 111 000 - - -01BBBBB AAAAA DDDDD ZZZ", 0},

    {"uzp.b.4"  , "dD,rA,rB", "E 11 111 001 - - -00BBBBB AAAAA DDDDD ZZZ", 0},
    {"uzp.h.2"  , "dD,rA,rB", "E 11 111 001 - - -01BBBBB AAAAA DDDDD ZZZ", 0},

    {"uct.b.4"  , "rD,rA,iH", "E 11 111 010 - - -00---HH AAAAA DDDDD ZZZ", 0},
    {"uct.h.2"  , "rD,rA,iG", "E 11 111 010 - - -01----G AAAAA DDDDD ZZZ", 0},

    {"mct.b.4"  , "rD,rA,iH", "E 11 111 011 - - -00---HH AAAAA DDDDD ZZZ", 0},
    {"mct.h.2"  , "rD,rA,iG", "E 11 111 011 - - -01----G AAAAA DDDDD ZZZ", 0},

    {"swp.b.4"  , "rD,rA"   , "E 11 111 111 - - -0000000 AAAAA DDDDD ZZZ", 0},
    {"swp.h.2"  , "rD,rA"   , "E 11 111 111 - - -0100000 AAAAA DDDDD ZZZ", 0},

    {"rvs.b.4"  , "rD,rA"   , "E 11 111 111 - - -0011111 AAAAA DDDDD ZZZ", 0},
    {"rvs.h.2"  , "rD,rA"   , "E 11 111 111 - - -0111111 AAAAA DDDDD ZZZ", 0},
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
