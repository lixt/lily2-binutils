/* LILY2-specific support for 32-bit ELF
   Copyright(C) Free Software Foundation, Inc.
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

#include "sysdep.h"
#include "bfd.h"
#include "libbfd.h"
#include "elf-bfd.h"
#include "elf/lily2.h"
#include "libiberty.h"

/* Try to minimize the amount of space occupied by relocation tables
   on the ROM (not that the ROM won't be swamped by other ELF overhead).  */
#define USE_REL	1

/* Set the right machine number for an lily2 ELF file.  */

static bfd_boolean
lily2_elf_object_p (bfd *abfd)
{
  (void) bfd_default_set_arch_mach (abfd, bfd_arch_lily2, 0);
  return TRUE;
}

/* The final processing done just before writing out an lily2 ELF object file.
   This gets the lily2 architecture right based on the machine number.  */

static void
lily2_elf_final_write_processing (bfd *abfd,
				 bfd_boolean linker ATTRIBUTE_UNUSED)
{
  elf_elfheader (abfd)->e_flags &=~ EF_LILY2_MACH;
}

static bfd_reloc_status_type
lily2_elf_32_reloc (bfd *abfd,
		   arelent *reloc_entry,
		   asymbol *symbol,
		   void * data,
		   asection *input_section,
		   bfd *output_bfd,
		   char **error_message ATTRIBUTE_UNUSED)
{
  if (output_bfd != NULL)
    {
      unsigned long insn;
      bfd_size_type addr = reloc_entry->address;

      reloc_entry->address += input_section->output_offset;

      insn = bfd_get_32 (abfd, (bfd_byte *) data + addr);
      insn += symbol->section->output_section->vma;
      insn += symbol->section->output_offset;
      insn += symbol->value;
      bfd_put_32 (abfd, insn, (bfd_byte *) data + addr);

      return bfd_reloc_ok;
    }

  return bfd_reloc_continue;
}

static bfd_reloc_status_type
lily2_elf_16_reloc (bfd *abfd,
		   arelent *reloc_entry,
		   asymbol *symbol,
		   void * data,
		   asection *input_section,
		   bfd *output_bfd,
		   char **error_message ATTRIBUTE_UNUSED)
{
  if (output_bfd != NULL)
    {
      unsigned short insn;
      bfd_size_type addr = reloc_entry->address;

      reloc_entry->address += input_section->output_offset;

      insn = bfd_get_16 (abfd, (bfd_byte *) data + addr);
      insn += symbol->section->output_section->vma;
      insn += symbol->section->output_offset;
      insn += symbol->value;
      bfd_put_16 (abfd, insn, (bfd_byte *) data + addr);

      return bfd_reloc_ok;
    }

  return bfd_reloc_continue;
}

static bfd_reloc_status_type
lily2_elf_8_reloc (bfd *abfd ATTRIBUTE_UNUSED,
		  arelent *reloc_entry,
		  asymbol *symbol,
		  void * data,
		  asection *input_section,
		  bfd *output_bfd,
		  char **error_message ATTRIBUTE_UNUSED)
{
  if (output_bfd != NULL)
    {
      unsigned char insn;
      bfd_size_type addr = reloc_entry->address;

      reloc_entry->address += input_section->output_offset;

      insn = bfd_get_8 (abfd, (bfd_byte *) data + addr);
      insn += symbol->section->output_section->vma;
      insn += symbol->section->output_offset;
      insn += symbol->value;
      bfd_put_8 (abfd, insn, (bfd_byte *) data + addr);

      return bfd_reloc_ok;
    }

  return bfd_reloc_continue;
}

/* Do a R_lily2_CONSTH relocation.  This has to be done in combination
   with a R_lily2_CONST reloc, because there is a carry from the LO16 to
   the HI16.  Here we just save the information we need; we do the
   actual relocation when we see the LO16.  lily2 ELF requires that the
   LO16 immediately follow the HI16.  As a GNU extension, we permit an
   arbitrary number of HI16 relocs to be associated with a single LO16
   reloc.  This extension permits gcc to output the HI and LO relocs
   itself. This code is copied from the elf32-mips.c.  */

struct lily2_consth
{
  struct lily2_consth *next;
  bfd_byte *addr;
  bfd_vma addend;
};

/* FIXME: This should not be a static variable.  */

static struct lily2_consth *lily2_consth_list;

static bfd_reloc_status_type
lily2_elf_const_reloc (bfd *abfd ATTRIBUTE_UNUSED,
		       arelent *reloc_entry,
		       asymbol *symbol,
		       void * data,
		       asection *input_section,
		       bfd *output_bfd,
		       char **error_message ATTRIBUTE_UNUSED)
{
    unsigned long insn, tmp;
    unsigned long src_mask, dst_mask;
    unsigned int bitpos, rightshift;
    bfd_size_type addr = reloc_entry->address;

    reloc_entry->address += input_section->output_offset;
    src_mask = reloc_entry->howto->src_mask;
    dst_mask = reloc_entry->howto->dst_mask;
    bitpos = reloc_entry->howto->bitpos;
    rightshift = reloc_entry->howto->rightshift;

    insn = bfd_get_32 (abfd, (bfd_byte *) data + addr);
    tmp = ((insn & dst_mask) >> bitpos) << rightshift;
    tmp += symbol->section->output_section->vma;
    tmp += symbol->section->output_offset;
    tmp += symbol->value;
    insn = (insn & ~dst_mask) | (((tmp & src_mask) >> rightshift) << bitpos);
    bfd_put_32 (abfd, insn, (bfd_byte *) data + addr);

    return bfd_reloc_ok;
}

static bfd_reloc_status_type
lily2_elf_consth_reloc (bfd *abfd ATTRIBUTE_UNUSED,
		       arelent *reloc_entry,
		       asymbol *symbol,
		       void * data,
		       asection *input_section,
		       bfd *output_bfd,
		       char **error_message ATTRIBUTE_UNUSED)
{
    return lily2_elf_const_reloc (abfd,
                                  reloc_entry,
                                  symbol,
                                  data,
                                  input_section,
                                  output_bfd,
                                  error_message);
}

/* Do a R_LILY2_CONST relocation.  This is a straightforward 16 bit
   inplace relocation; this function exists in order to do the
   R_LILY2_CONSTH relocation described above.  */

static bfd_reloc_status_type
lily2_elf_constl_reloc (bfd *abfd,
		      arelent *reloc_entry,
		      asymbol *symbol,
		      void * data,
		      asection *input_section,
		      bfd *output_bfd,
		      char **error_message)
{
    return lily2_elf_const_reloc (abfd,
                                  reloc_entry,
                                  symbol,
                                  data,
                                  input_section,
                                  output_bfd,
                                  error_message);
}

static bfd_reloc_status_type
lily2_elf_jumptarg_reloc (bfd *abfd,
			 arelent *reloc_entry,
			 asymbol *symbol ATTRIBUTE_UNUSED,
			 void * data,
			 asection *input_section,
			 bfd *output_bfd,
			 char **error_message ATTRIBUTE_UNUSED)
{
  if (output_bfd != NULL)
    {
      unsigned long insn, tmp;
      bfd_size_type addr = reloc_entry->address;

      reloc_entry->address += input_section->output_offset;

      insn = bfd_get_32 (abfd, (bfd_byte *) data + addr);
      tmp = insn | 0xfc000000;
      tmp -= (input_section->output_offset >> 2);
      insn = (insn & 0xfc000000) | (tmp & 0x03ffffff);
      bfd_put_32 (abfd, insn, (bfd_byte *) data + addr);

      return bfd_reloc_ok;
    }

  return bfd_reloc_continue;
}

static reloc_howto_type elf_lily2_howto_table[] =
{
  /* This reloc does nothing.  */
  HOWTO (R_LILY2_NONE,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 32,			/* bitsize */
	 FALSE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_LILY2_NONE",		/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 0,			/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard 32 bit relocation.  */
  HOWTO (R_LILY2_32,		/* type */
	 0,	                /* rightshift */
	 2,	                /* size (0 = byte, 1 = short, 2 = long) */
	 32,	                /* bitsize */
	 FALSE,	                /* pc_relative */
	 0,	                /* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 lily2_elf_32_reloc, 	/* special_function */
	 "R_LILY2_32",		/* name */
	 FALSE,	                /* partial_inplace */
	 0xffffffff,	        /* src_mask */
	 0xffffffff,   		/* dst_mask */
	 FALSE),                /* pcrel_offset */

  /* A standard 16 bit relocation.  */
  HOWTO (R_LILY2_16,		/* type */
	 0,	                /* rightshift */
	 1,	                /* size (0 = byte, 1 = short, 2 = long) */
	 16,	                /* bitsize */
	 FALSE,	                /* pc_relative */
	 0,	                /* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 lily2_elf_16_reloc, 	/* special_function */
	 "R_LILY2_16",		/* name */
	 FALSE,	                /* partial_inplace */
	 0x0000ffff,	        /* src_mask */
	 0x0000ffff,   		/* dst_mask */
	 FALSE),                /* pcrel_offset */

  /* A standard 8 bit relocation.  */
  HOWTO (R_LILY2_8,		/* type */
	 0,	                /* rightshift */
	 0,	                /* size (0 = byte, 1 = short, 2 = long) */
	 8,	                /* bitsize */
	 FALSE,	                /* pc_relative */
	 0,	                /* bitpos */
	 complain_overflow_bitfield, /* complain_on_overflow */
	 lily2_elf_8_reloc, 	/* special_function */
	 "R_LILY2_8",		/* name */
	 FALSE,	                /* partial_inplace */
	 0x000000ff,	        /* src_mask */
	 0x000000ff,   		/* dst_mask */
	 FALSE),                /* pcrel_offset */

  /* A standard low 16 bit relocation.  */
  HOWTO (R_LILY2_CONSTL,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 FALSE,			/* pc_relative */
	 8,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 lily2_elf_constl_reloc,	/* special_function */
	 "R_LILY2_CONSTL",	/* name */
	 FALSE,			/* partial_inplace */
	 0x0000ffff,		/* src_mask */
	 0x00ffff00,		/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard high 16 bit relocation.  */
  HOWTO (R_LILY2_CONSTH,		/* type */
	 16,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 TRUE,			/* pc_relative */
	 8,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 lily2_elf_consth_reloc,	/* special_function */
	 "R_LILY2_CONSTH",	/* name */
	 FALSE,			/* partial_inplace */
	 0xffff0000,		/* src_mask */
	 0x00ffff00,		/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard branch relocation.  */
  HOWTO (R_LILY2_JUMPTARG,	/* type */
	 2,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 28,			/* bitsize */
	 TRUE,			/* pc_relative */
	 0,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 lily2_elf_jumptarg_reloc,/* special_function */
	 "R_LILY2_JUMPTARG",	/* name */
	 FALSE,			/* partial_inplace */
	 0,			/* src_mask */
	 0x03ffffff,		/* dst_mask */
	 TRUE), 		/* pcrel_offset */

  /* GNU extension to record C++ vtable hierarchy.  */
  HOWTO (R_LILY2_GNU_VTINHERIT, /* type */
         0,                     /* rightshift */
         2,                     /* size (0 = byte, 1 = short, 2 = long) */
         0,                     /* bitsize */
         FALSE,                 /* pc_relative */
         0,                     /* bitpos */
         complain_overflow_dont, /* complain_on_overflow */
         NULL,                  /* special_function */
         "R_LILY2_GNU_VTINHERIT", /* name */
         FALSE,                 /* partial_inplace */
         0,                     /* src_mask */
         0,                     /* dst_mask */
         FALSE),                /* pcrel_offset */

  /* GNU extension to record C++ vtable member usage.  */
  HOWTO (R_LILY2_GNU_VTENTRY,     /* type */
         0,                     /* rightshift */
         2,                     /* size (0 = byte, 1 = short, 2 = long) */
         0,                     /* bitsize */
         FALSE,                 /* pc_relative */
         0,                     /* bitpos */
         complain_overflow_dont, /* complain_on_overflow */
         _bfd_elf_rel_vtable_reloc_fn,  /* special_function */
         "R_LILY2_GNU_VTENTRY",   /* name */
         FALSE,                 /* partial_inplace */
         0,                     /* src_mask */
         0,                     /* dst_mask */
         FALSE),                /* pcrel_offset */
};

/* Map BFD reloc types to lily2 ELF reloc types.  */

struct lily2_reloc_map
{
  bfd_reloc_code_real_type  bfd_reloc_val;
  unsigned char             elf_reloc_val;
};

static const struct lily2_reloc_map lily2_reloc_map[] =
{
  { BFD_RELOC_NONE, R_LILY2_NONE },
  { BFD_RELOC_32, R_LILY2_32 },
  { BFD_RELOC_16, R_LILY2_16 },
  { BFD_RELOC_8, R_LILY2_8 },
  { BFD_RELOC_LO16, R_LILY2_CONSTL },
  { BFD_RELOC_HI16, R_LILY2_CONSTH },
  { BFD_RELOC_32_GOT_PCREL, R_LILY2_JUMPTARG },
  { BFD_RELOC_VTABLE_INHERIT, R_LILY2_GNU_VTINHERIT },
  { BFD_RELOC_VTABLE_ENTRY, R_LILY2_GNU_VTENTRY },
};

static reloc_howto_type *
bfd_elf32_bfd_reloc_type_lookup (bfd *abfd ATTRIBUTE_UNUSED,
				 bfd_reloc_code_real_type code)
{
  unsigned int i;

  for (i = ARRAY_SIZE (lily2_reloc_map); i--;)
    if (lily2_reloc_map[i].bfd_reloc_val == code)
      return &elf_lily2_howto_table[lily2_reloc_map[i].elf_reloc_val];

  return NULL;
}

static reloc_howto_type *
bfd_elf32_bfd_reloc_name_lookup (bfd *abfd ATTRIBUTE_UNUSED,
				 const char *r_name)
{
  unsigned int i;

  for (i = 0;
       i < sizeof (elf_lily2_howto_table) / sizeof (elf_lily2_howto_table[0]);
       i++)
    if (elf_lily2_howto_table[i].name != NULL
	&& strcasecmp (elf_lily2_howto_table[i].name, r_name) == 0)
      return &elf_lily2_howto_table[i];

  return NULL;
}

/* Set the howto pointer for an lily2 ELF reloc.  */

static void
lily2_info_to_howto_rel (bfd *abfd ATTRIBUTE_UNUSED,
			arelent *cache_ptr,
			Elf_Internal_Rela *dst)
{
  unsigned int r_type;

  r_type = ELF32_R_TYPE (dst->r_info);
  BFD_ASSERT (r_type < (unsigned int) R_LILY2_max);
  cache_ptr->howto = &elf_lily2_howto_table[r_type];
}

#define TARGET_LITTLE_SYM	bfd_elf32_lily2_little_vec
#define TARGET_LITTLE_NAME	"elf32-littlelily2"
#define TARGET_BIG_SYM		bfd_elf32_lily2_big_vec
#define TARGET_BIG_NAME		"elf32-lily2"
#define ELF_ARCH		bfd_arch_lily2
#define ELF_MACHINE_CODE	EM_LILY2
#define ELF_MAXPAGESIZE		0x1000

#define elf_info_to_howto	0
#define elf_info_to_howto_rel	lily2_info_to_howto_rel
#define elf_backend_object_p	lily2_elf_object_p
#define elf_backend_final_write_processing \
				lily2_elf_final_write_processing

#include "elf32-target.h"
