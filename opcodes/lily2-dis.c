/* Instruction printing code for the LILY2
   Copyright (C) Free Software Foundation, Inc.
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

#ifndef DEBUG
#define DEBUG 0
#endif

#include "sysdep.h"
#include "dis-asm.h"
#include "opcode/lily2.h"
#include "safe-ctype.h"

#define EXTEND29(x) ((x) & (unsigned long) 0x10000000 ? ((x) | (unsigned long) 0xf0000000) : ((x)))

/* Now find the four bytes of INSN_CH and put them in *INSN.  */

static void
find_bytes_big (unsigned char *insn_ch, unsigned long *insn)
{
  *insn =
    ((unsigned long) insn_ch[0] << 24) +
    ((unsigned long) insn_ch[1] << 16) +
    ((unsigned long) insn_ch[2] << 8) +
    ((unsigned long) insn_ch[3]);
#if DEBUG
  printf ("find_bytes_big3: %lx\n", *insn);
#endif
}

static void
find_bytes_little (unsigned char *insn_ch, unsigned long *insn)
{
  *insn =
    ((unsigned long) insn_ch[3] << 24) +
    ((unsigned long) insn_ch[2] << 16) +
    ((unsigned long) insn_ch[1] << 8) +
    ((unsigned long) insn_ch[0]);
}

typedef void (*find_byte_func_type) (unsigned char *, unsigned long *);

static unsigned long
lily2_extract (char param_ch, char *enc_initial, unsigned long insn)
{
    return 0;
}

static int
lily2_opcode_match (unsigned long insn, char *encoding)
{
  unsigned long ones, zeros;

#if DEBUG
  printf ("lily2_opcode_match: %.8lx\n", insn);
#endif
  ones  = lily2_extract ('1', encoding, insn);
  zeros = lily2_extract ('0', encoding, insn);

#if DEBUG
  printf ("ones: %lx \n", ones);
  printf ("zeros: %lx \n", zeros);
#endif
  if ((insn & ones) != ones)
    {
#if DEBUG
      printf ("ret1\n");
#endif
      return 0;
    }

  if ((~insn & zeros) != zeros)
    {
#if DEBUG
      printf ("ret2\n");
#endif
      return 0;
    }

#if DEBUG
  printf ("ret3\n");
#endif
  return 1;
}

/* Print register to INFO->STREAM. Used only by print_insn.  */

static void
lily2_print_register (char param_ch,
		     char *encoding,
		     unsigned long insn,
		     struct disassemble_info *info)
{
  int regnum = lily2_extract (param_ch, encoding, insn);

#if DEBUG
  printf ("lily2_print_register: %c, %s, %lx\n", param_ch, encoding, insn);
#endif
  if (param_ch == 'A')
    (*info->fprintf_func) (info->stream, "r%d", regnum);
  else if (param_ch == 'B')
    (*info->fprintf_func) (info->stream, "r%d", regnum);
  else if (param_ch == 'D')
    (*info->fprintf_func) (info->stream, "r%d", regnum);
  else if (regnum < 16)
    (*info->fprintf_func) (info->stream, "r%d", regnum);
  else if (regnum < 32)
    (*info->fprintf_func) (info->stream, "r%d", regnum-16);
  else
    (*info->fprintf_func) (info->stream, "X%d", regnum);
}

/* Print immediate to INFO->STREAM. Used only by print_insn.  */

static void
lily2_print_immediate (char param_ch,
		      char *encoding,
		      unsigned long insn,
		      struct disassemble_info *info)
{
  int imm = lily2_extract(param_ch, encoding, insn);

  if (letter_signed(param_ch))
    (*info->fprintf_func) (info->stream, "0x%x", imm);
/*    (*info->fprintf_func) (info->stream, "%d", imm); */
  else
    (*info->fprintf_func) (info->stream, "0x%x", imm);
}

/* Print one instruction from MEMADDR on INFO->STREAM.
   Return the size of the instruction (always 4 on lily2).  */

static int
print_insn (bfd_vma memaddr, struct disassemble_info *info)
{
    return 0;
}

/* Disassemble a big-endian lily2 instruction.  */

int
print_insn_big_lily2 (bfd_vma memaddr, struct disassemble_info *info)
{
  info->private_data = find_bytes_big;

  return print_insn (memaddr, info);
}

/* Disassemble a little-endian lily2 instruction.  */

int
print_insn_little_lily2 (bfd_vma memaddr, struct disassemble_info *info)
{
  info->private_data = find_bytes_little;
  return print_insn (memaddr, info);
}
