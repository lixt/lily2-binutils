/* LILY2 ELF support for BFD.
   Copyright (C) Free Software Foundation, Inc.
   Contributed by Xiaotian Li <lixiaotian07@gmail.com>

   This file is part of BFD, the Binary File Descriptor library.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */

#ifndef _ELF_LILY2_H
#define _ELF_LILY2_H

#include "elf/reloc-macros.h"

/* Relocations.  */
START_RELOC_NUMBERS (elf_lily2_reloc_type)
  RELOC_NUMBER (R_LILY2_NONE, 0)
  RELOC_NUMBER (R_LILY2_CONSTL, 1)
  RELOC_NUMBER (R_LILY2_CONSTH, 2)
  RELOC_NUMBER (R_LILY2_JUMPTARG, 3)
END_RELOC_NUMBERS (R_LILY2_max)

/* Four bit OR32 machine type field.  */
#define EF_LILY2_MACH             0x0000000f

/* Various CPU types.  */
#define E_LILY2_MACH_BASE         0x00000000
#define E_LILY2_MACH_UNUSED1      0x00000001
#define E_LILY2_MACH_UNUSED2      0x00000002
#define E_LILY2_MACH_UNUSED4      0x00000003

/* Processor specific section headers, sh_type field */
#define SHT_ORDERED		SHT_HIPROC	/* Link editor is to sort the \
						   entries in this section \
						   based on the address \
						   specified in the associated \
						   symbol table entry.  */

#endif /* _ELF_LILY2_H */
