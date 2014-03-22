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

# define DEBUG 0

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

  /* A standard low 16 bit relocation.  */
  HOWTO (R_LILY2_CONSTL,		/* type */
	 0,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 16,			/* bitsize */
	 FALSE,			/* pc_relative */
	 8,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
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
	 FALSE,			/* pc_relative */
	 8,			/* bitpos */
	 complain_overflow_dont, /* complain_on_overflow */
	 bfd_elf_generic_reloc,	/* special_function */
	 "R_LILY2_CONSTH",	/* name */
	 FALSE,			/* partial_inplace */
	 0xffff0000,		/* src_mask */
	 0x00ffff00,		/* dst_mask */
	 FALSE),		/* pcrel_offset */

  /* A standard branch relocation.  */
  HOWTO (R_LILY2_JUMPTARG,	/* type */
	 2,			/* rightshift */
	 2,			/* size (0 = byte, 1 = short, 2 = long) */
	 21,			/* bitsize */
	 TRUE,			/* pc_relative */
	 3,			/* bitpos */
	 complain_overflow_signed, /* complain_on_overflow */
	 bfd_elf_generic_reloc,/* special_function */
	 "R_LILY2_JUMPTARG",	/* name */
	 FALSE,			/* partial_inplace */
	 0xffffffff,		/* src_mask */
	 0x00fffff8,		/* dst_mask */
	 TRUE), 		/* pcrel_offset */
};

static unsigned long
mask (int nbits)
{
    int insn_bits = sizeof (unsigned long) * 8;
    return (nbits == insn_bits) ? (unsigned long) -1 : (1 << nbits) - 1;
}

static unsigned long
bits (unsigned long insn, int first, int last)
{
    int nbits = first - last + 1;
    return (insn >> last) & mask (nbits);
}

static unsigned long
sign_extend (unsigned long insn, int nbits, int sign)
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

static unsigned long
replace_bits (unsigned long old_insn, int first, int last, unsigned long new_insn)
{
    unsigned long bmask = mask (first - last + 1) << last;
    return ((new_insn << last) & bmask) | (old_insn & ~bmask);
}

static bfd_reloc_status_type
lily2_elf_final_link_relocate (reloc_howto_type *howto,
			                   bfd *input_bfd,
			                   asection *input_section,
			                   bfd_byte *contents,
			                   Elf_Internal_Rela *rel,
			                   bfd_vma relocation)
{
#if DEBUG
    printf ("<----- Enter function (lily2_elf_final_link_relocate).\n");
    printf ("       vma: 0x%08lx\n", input_section->output_section->vma);
    printf ("       inst (section offset to vma: 0x%08lx, inst offset to section: 0x%08lx)\n",
            input_section->output_offset, rel->r_offset);
    printf ("       label (relocation: 0x%08lx)\n", relocation);
#endif

    unsigned long insn;
    unsigned long tmp;
    unsigned int bitsize;
    unsigned int bitpos;
    unsigned int rightshift;

    bfd_reloc_status_type r = bfd_reloc_ok;

    contents += rel->r_offset;
    insn = bfd_get_32 (input_bfd, contents);

    bitsize = howto->bitsize;
    bitpos = howto->bitpos;
    rightshift = howto->rightshift;

    switch (howto->type) {

        case R_LILY2_CONSTL:
        case R_LILY2_CONSTH:
            tmp = bits (insn, bitsize + bitpos - 1, bitpos);
            tmp = sign_extend (tmp, bitsize - 1, 0) << rightshift;
            tmp += relocation;
            tmp >>= rightshift;
            insn = replace_bits (insn, bitsize + bitpos - 1, bitpos, tmp);
            break;

        case R_LILY2_JUMPTARG:
            tmp = relocation - (input_section->output_section->vma +
                    input_section->output_offset + rel->r_offset);
            tmp >>= rightshift;
            tmp = sign_extend (tmp, bitsize - 1, 0);
            insn = replace_bits (insn, bitsize + bitpos - 1, bitpos, tmp);
            break;
    }

    bfd_put_32 (input_bfd, insn, contents);

#if DEBUG
    printf ("-----> Leave function (lily2_elf_final_link_relocate).\n");
#endif

    return r;
}

static bfd_boolean
lily2_elf_relocate_section (bfd *output_bfd,
                            struct bfd_link_info *info,
                            bfd *input_bfd,
                            asection *input_section,
                            bfd_byte *contents,
                            Elf_Internal_Rela *relocs,
                            Elf_Internal_Sym *local_syms,
                            asection **local_sections)
{
    Elf_Internal_Shdr *symtab_hdr;
    struct elf_link_hash_entry **sym_hashes;
    Elf_Internal_Rela *rel;
    Elf_Internal_Rela *relend;

    symtab_hdr = &elf_tdata (input_bfd)->symtab_hdr;
    sym_hashes = elf_sym_hashes (input_bfd);
    relend = relocs + input_section->reloc_count;

    for (rel = relocs; rel < relend; ++rel) {
        reloc_howto_type *howto;
        unsigned long r_symndx;
        Elf_Internal_Sym *sym;
        asection *sec;
        struct elf_link_hash_entry *h;
        bfd_vma relocation;
        bfd_reloc_status_type r;
        const char *name = NULL;
        int r_type;

        r_type = ELF32_R_TYPE (rel->r_info);
        r_symndx = ELF32_R_SYM (rel->r_info);

        if ((unsigned int) r_type >
                (sizeof elf_lily2_howto_table / sizeof (reloc_howto_type))) {
  	        abort ();
        }

        howto = elf_lily2_howto_table + ELF32_R_TYPE (rel->r_info);
        h = NULL;
        sym = NULL;
        sec = NULL;

        if (r_symndx < symtab_hdr->sh_info) {
            sym = local_syms + r_symndx;
            sec = local_sections[r_symndx];
            relocation = _bfd_elf_rela_local_sym (output_bfd, sym, &sec, rel);

	        name = bfd_elf_string_from_elf_section
	          (input_bfd, symtab_hdr->sh_link, sym->st_name);
	        name = (name == NULL) ? bfd_section_name (input_bfd, sec) : name;

        } else {
       	    bfd_boolean unresolved_reloc, warned;

       	    RELOC_FOR_GLOBAL_SYMBOL (info, input_bfd, input_section, rel,
       	  			   r_symndx, symtab_hdr, sym_hashes,
       	  			   h, sec, relocation,
       	  			   unresolved_reloc, warned);
	    }

       r = lily2_elf_final_link_relocate (howto, input_bfd, input_section,
					contents, rel, relocation);
    }

    return TRUE;
}

/* Map BFD reloc types to lily2 ELF reloc types.  */

struct lily2_reloc_map
{
  bfd_reloc_code_real_type  bfd_reloc_val;
  unsigned char             elf_reloc_val;
};

static const struct lily2_reloc_map lily2_reloc_map[] =
{
  { BFD_RELOC_NONE, R_LILY2_NONE },
  { BFD_RELOC_LO16, R_LILY2_CONSTL },
  { BFD_RELOC_HI16, R_LILY2_CONSTH },
  { BFD_RELOC_32_GOT_PCREL, R_LILY2_JUMPTARG },
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

#define elf_backend_relocate_section lily2_elf_relocate_section

#include "elf32-target.h"
