/* Instruction printing code for the LILY2
   Copyright (C) 2014 DSP Group, Institute of Microelectronics, Tsinghua University.
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
#include "safe-ctype.h"
#include "libiberty.h"
#include "opcode/lily2.h"

#define EXTEND29(x) ((x) & (unsigned long) 0x10000000 ? ((x) | (unsigned long) 0xf0000000) : ((x)))

/* Storage for error information. */
static char error[100];

const struct lily2_functional_unit lily2_dis_functional_units[] =
{
    {"[xa]", 0x0, {"x", 0}, NULL},
    {"[xa]", 0x1, {"x", 0}, NULL},
    {"[xm]", 0x2, {"x", 0}, NULL},
    {"[xd]", 0x3, {"x", 0}, NULL},
    {"[ya]", 0x4, {"y", 1}, NULL},
    {"[ya]", 0x5, {"y", 1}, NULL},
    {"[ym]", 0x6, {"y", 1}, NULL},
    {"[yd]", 0x7, {"y", 1}, NULL},
};
const size_t lily2_num_dis_functional_units = ARRAY_SIZE (lily2_dis_functional_units);

const struct lily2_condition lily2_dis_conditions[] =
{
    {"      ", 0x0},
    {"{ cf0}", 0x1},
    {"{ cf1}", 0x2},
    {"{ cf2}", 0x3},
    {"{!cf0}", 0x4},
    {"{!cf1}", 0x5},
    {"{!cf2}", 0x6},
};
const size_t lily2_num_dis_conditions = ARRAY_SIZE (lily2_dis_conditions);

const struct lily2_register lily2_dis_registers[] =
{
    {"a0" , 0 }, {"a1" , 1 }, {"a2" , 2 }, {"a3" , 3 },
    {"a4" , 4 }, {"a5" , 5 }, {"a6" , 6 }, {"a7" , 7 },
    {"a8" , 8 }, {"a9" , 9 }, {"a10", 10}, {"a11", 11},
    {"a12", 12}, {"a13", 13}, {"a14", 14}, {"a15", 15},
    {"a16", 16}, {"a17", 17}, {"a18", 18}, {"a19", 19},
    {"a20", 20}, {"a21", 21}, {"a22", 22}, {"a23", 23},
    {"c0" , 24}, {"c1" , 25}, {"c2" , 26}, {"c3" , 27},
    {"c4" , 28}, {"c5" , 29}, {"c6" , 30}, {"c7" , 31},
    {"b0" , 32}, {"b1" , 33}, {"b2" , 34}, {"b3" , 35},
    {"b4" , 36}, {"b5" , 37}, {"b6" , 38}, {"b7" , 39},
    {"b8" , 40}, {"b9" , 41}, {"b10", 42}, {"b11", 43},
    {"b12", 44}, {"b13", 45}, {"b14", 46}, {"b15", 47},
    {"b16", 48}, {"b17", 49}, {"b18", 50}, {"b19", 51},
    {"b20", 52}, {"b21", 53}, {"b22", 54}, {"b23", 55},
    {"c0" , 56}, {"c1" , 57}, {"c2" , 58}, {"c3" , 59},
    {"c4" , 60}, {"c5" , 61}, {"c6" , 62}, {"c7" , 63},
};
const size_t lily2_num_dis_registers = ARRAY_SIZE (lily2_dis_registers);

const struct lily2_register lily2_dis_register_pairs[] =
{
    {"a1:a0"  , 0 }, {"a3:a2"  , 2 },
    {"a5:a4"  , 4 }, {"a7:a6"  , 6 },
    {"a9:a8"  , 8 }, {"a11:a10", 10},
    {"a13:a12", 12}, {"a15:a14", 14},
    {"a17:a16", 16}, {"a19:a18", 18},
    {"a21:a20", 20}, {"a23:a22", 22},
    {"c1:c0"  , 24}, {"c3:c2"  , 26},
    {"c5:c4"  , 28}, {"c7:c5"  , 30},
    {"b1:b0"  , 32}, {"b3:b2"  , 34},
    {"b5:b4"  , 36}, {"b7:b6"  , 38},
    {"b9:b8"  , 40}, {"b11:b10", 42},
    {"b13:b12", 44}, {"b15:b14", 46},
    {"b17:b16", 48}, {"b19:b18", 50},
    {"b21:b20", 52}, {"b23:b22", 54},
    {"c1:c0"  , 56}, {"c3:c2"  , 58},
    {"c5:c4"  , 60}, {"c7:c5"  , 62},
};
const size_t lily2_num_dis_register_pairs = ARRAY_SIZE (lily2_dis_register_pairs);

const struct lily2_register lily2_dis_register_pair_pairs[] =
{
    {"a3:a2:a1:a0"    , 0 }, {"a7:a6:a5:a4"    , 4 },
    {"a11:a10:a9:a8"  , 8 }, {"a15:a14:a13:a12", 12},
    {"a19:a18:a17:a16", 16}, {"a23:a22:a21:a20", 20},
    {"c3:c2:c1:c0"    , 24}, {"c7:c6:c5:c4"    , 28},
    {"b3:b2:b1:b0"    , 32}, {"b7:b6:b5:b4"    , 36},
    {"b11:b10:b9:b8"  , 40}, {"b15:b14:b13:b12", 44},
    {"b19:b18:b17:b16", 48}, {"b23:b22:b21:b20", 52},
    {"c3:c2:c1:c0"    , 56}, {"c7:c6:c5:c4"    , 60},
};
const size_t lily2_num_dis_register_pair_pairs = ARRAY_SIZE (lily2_dis_register_pair_pairs);

const struct lily2_opcode lily2_dis_opcodes[] =
{
    {"name", "operand list", "FFF --- --- - - -------- ----- ----- ---", 0},
};
const size_t lily2_num_dis_opcodes = ARRAY_SIZE (lily2_dis_opcodes);

const struct lily2_addr_mod_sign lily2_dis_addr_mod_prefix_signs[] =
{
    {"++", 0x0}, {"\0", 0x1}, {"--", 0x2}, {"\0", 0x3},
};
const size_t lily2_num_dis_addr_mod_prefix_signs = ARRAY_SIZE (lily2_dis_addr_mod_prefix_signs);
const struct lily2_addr_mod_sign lily2_dis_addr_mod_suffix_signs[] =
{
    {"\0", 0x0}, {"++", 0x1}, {"\0", 0x2}, {"--", 0x3},
};
const size_t lily2_num_dis_addr_mod_suffix_signs = ARRAY_SIZE (lily2_dis_addr_mod_suffix_signs);

static struct lily2_functional_unit *
dis_functional_unit_find (unsigned long insn)
{
    struct lily2_functional_unit *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_functional_units; ++i) {
        if (lily2_dis_functional_units[i].code == insn) {
            retval = &lily2_dis_functional_units[i];
        }
    }

    return retval;
}

static struct lily2_condition *
dis_condition_find (unsigned long insn)
{
    struct lily2_condition *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_conditions; ++i) {
        if (lily2_dis_conditions[i].code == insn) {
            retval = &lily2_dis_conditions[i];
        }
    }

    return retval;
}

static struct lily2_register *
dis_register_find (unsigned long insn)
{
    struct lily2_register *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_registers; ++i) {
        if (lily2_dis_registers[i].code == insn) {
            retval = &lily2_dis_registers[i];
        }
    }

    return retval;
}

static struct lily2_register *
dis_register_pair_find (unsigned long insn)
{
    struct lily2_register *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_register_pairs; ++i) {
        if (lily2_dis_register_pairs[i].code == insn) {
            retval = &lily2_dis_register_pairs[i];
        }
    }

    return retval;
}

static struct lily2_register *
dis_register_pair_pair_find (unsigned long insn)
{
    struct lily2_register *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_register_pair_pairs; ++i) {
        if (lily2_dis_register_pair_pairs[i].code == insn) {
            retval = &lily2_dis_register_pair_pairs[i];
        }
    }

    return retval;
}

static struct lily2_addr_mod_sign *
dis_addr_mod_prefix_sign_find (unsigned long insn)
{
    struct lily2_addr_mod_sign *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_addr_mod_prefix_signs; ++i) {
        if (lily2_dis_addr_mod_prefix_signs[i].code == insn) {
            retval = &lily2_dis_addr_mod_prefix_signs[i];
        }
    }

    return retval;
}

static struct lily2_addr_mod_sign *
dis_addr_mod_suffix_sign_find (unsigned long insn)
{
    struct lily2_addr_mod_sign *retval = NULL;

    int i;
    for (i = 0; i != lily2_num_dis_addr_mod_suffix_signs; ++i) {
        if (lily2_dis_addr_mod_suffix_signs[i].code == insn) {
            retval = &lily2_dis_addr_mod_suffix_signs[i];
        }
    }

    return retval;
}

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
  //printf ("find_bytes_big3: %lx\n", *insn);
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
lily2_extract (char param_ch, char *encoding, unsigned long insn)
{
    unsigned long extract_insn = 0;
    int extract_bit = 31;

    for (; *encoding; ++encoding) {
        /* Extracts bits of INSN. */
        if (*encoding == param_ch) {
            extract_insn <<= 1;
            extract_insn += bits (insn, extract_bit, extract_bit);
        }
        /* Counting down the extracting bit. */
        if (*encoding != ' ') {
            --extract_bit;
        }
    }

    return extract_insn;
}

static int
lily2_opcode_match (unsigned long insn, char *encoding)
{
    int bit = 31;
    for (; *encoding; ++encoding) {
        /* `0' or `1'. */
        if (*encoding == '0') {
            if (bits (insn, bit, bit) != 0) {
                return 0;
            }
        } else if (*encoding == '1') {
            if (bits (insn, bit, bit) != 1) {
                return 0;
            }
        }

        if (*encoding != ' ') {
            --bit;
        }
    }

    /* Matches the opcodes. */
    return 1;
}

/* Prints the functional unit.
   On success, returns 1. On failure, returns 0. */
static int
lily2_print_functional_unit (char param_ch,
                             char *encoding,
                             unsigned long insn,
                             struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_functional_unit).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long functional_unit_insn = lily2_extract (param_ch, encoding, insn);
    struct lily2_functional_unit *
    functional_unit = dis_functional_unit_find (functional_unit_insn);

    if (!functional_unit) {
        /* Illegal INSN of functional unit. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error, "can't disasemble functional unit (0x%x).\n", insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", functional_unit->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s ", functional_unit->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_functional_unit).\n");
    printf ("\n");
#endif

    return retval;
}

/* Prints the execution condition. */
static int
lily2_print_condition (char param_ch,
                       char *encoding,
                       unsigned long insn,
                       struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_condition).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long condition_insn = lily2_extract (param_ch, encoding, insn);
    struct lily2_condition *condition = dis_condition_find (condition_insn);

    if (!condition) {
        /* Illegal INSN of condition. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error, "can't disassemble condition (0x%x).\n", insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", condition->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s ", condition->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_condition).\n");
    printf ("\n");
#endif

    return retval;
}

/* Calculates the insn of registers. */
static unsigned long
calc_register_insn (char param_ch, char *encoding, unsigned long insn)
{
    unsigned long register_insn = lily2_extract (param_ch, encoding, insn);
    switch (param_ch) {
        case 'A':
        case 'B':
        case 'D': /* Normal. */
            register_insn += (bits (insn, 31, 31) == 0 ? 0 : 32);
            break;
        case 'X': /* Cross-Cluster. */
            register_insn += (bits (insn, 31, 31) == 0 ? 32 : 0);
            break;
        case 'C': /* Miscellaneous. */
        default :
            break;
    }

    return register_insn;
}

/* Prints the register operand.
   On success, returns 1. On failure, returns 0.*/
static int
lily2_print_register (char param_ch,
		              char *encoding,
		              unsigned long insn,
		              struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_register).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long register_insn = calc_register_insn (param_ch, encoding, insn);
    struct lily2_register *reg = dis_register_find (register_insn);

    if (!reg) {
        /* Illegal INSN of register. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error, "can't disassemble register (0x%x).\n", register_insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", reg->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s", reg->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_register).\n");
    printf ("\n");
#endif

    return retval;
}

/* Prints the register-pair operand.
   On success, returns 1. On failure, returns 0.*/
static int
lily2_print_register_pair (char param_ch,
		                   char *encoding,
		                   unsigned long insn,
		                   struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_register_pair).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long register_pair_insn = calc_register_insn (param_ch, encoding, insn);
    struct lily2_register *reg_pair = dis_register_pair_find (register_pair_insn);

    if (!reg_pair) {
        /* Illegal INSN of register-pair. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error, "can't disassemble register-pair (0x%x).\n", register_pair_insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", reg_pair->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s", reg_pair->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_register_pair).\n");
    printf ("\n");
#endif

    return retval;
}

/* Prints the register-pair-pair operand.
   On success, returns 1. On failure, returns 0.*/
static int
lily2_print_register_pair_pair (char param_ch,
		                        char *encoding,
		                        unsigned long insn,
		                        struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_register_pair_pair).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long register_pair_pair_insn = calc_register_insn (param_ch, encoding, insn);
    struct lily2_register
    *reg_pair_pair = dis_register_pair_pair_find (register_pair_pair_insn);

    if (!reg_pair_pair) {
        /* Illegal INSN of register-pair-pair. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf ("can't disassemble register-pair-pair (0x%x).\n", register_pair_pair_insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", reg_pair_pair->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s", reg_pair_pair->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_register_pair_pair).\n");
    printf ("\n");
#endif

    return retval;
}

/* Prints the immediate operand. */
static int
lily2_print_immediate (char param_ch,
		               char *encoding,
		               unsigned long insn,
		               struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_immediate).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long imm_insn = lily2_extract (param_ch, encoding, insn);
    (*info->fprintf_func) (info->stream, "0x%x", imm_insn);

#if DEBUG
    printf ("       extract str = 0x%x", imm_insn);
    printf ("-----> Leave function (lily2_print_immediate).\n");
    printf ("\n");
#endif

    return retval = 1;
}

/* Prints the address modification prefix sign.
   On success, returns 1. On failure, returns 0. */
static int
lily2_print_addr_mod_prefix_sign (char param_ch,
                                  char *encoding,
                                  unsigned long insn,
                                  struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_addr_mod_prefix_sign).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long addr_mod_prefix_sign_insn = lily2_extract (param_ch, encoding, insn);
    struct lily2_addr_mod_sign *
    addr_mod_prefix_sign = dis_addr_mod_prefix_sign_find (addr_mod_prefix_sign_insn);

    if (!addr_mod_prefix_sign) {
        /* Illegal INSN of address modification prefix sign. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error,
                 "can't disassemble address modification prefix sign (0x%x).\n",
                 insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", addr_mod_prefix_sign->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s", addr_mod_prefix_sign->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_addr_mod_prefix_sign).\n");
    printf ("\n");
#endif

    return retval;
}

/* Prints the address modification suffix sign.
   On success, returns 1. On failure, returns 0. */
static int
lily2_print_addr_mod_suffix_sign (char param_ch,
                                  char *encoding,
                                  unsigned long insn,
                                  struct disassemble_info *info)
{
#if DEBUG
    printf ("<----- Enter function (lily2_print_addr_mod_suffix_sign).\n");
    printf ("       param_ch: `%c'.\n", param_ch);
    printf ("       encoding: ``%s''.\n", encoding);
    printf ("       insn: 0x%08x.\n", insn);
#endif

    int retval;

    unsigned long addr_mod_suffix_sign_insn = lily2_extract (param_ch, encoding, insn);
    struct lily2_addr_mod_sign *
    addr_mod_suffix_sign = dis_addr_mod_suffix_sign_find (addr_mod_suffix_sign_insn);

    if (!addr_mod_suffix_sign) {
        /* Illegal INSN of address modification suffix sign. */
#if DEBUG
        printf ("       extract str: (nil).\n");
#endif
        sprintf (error,
                 "can't disassemble address modification suffix sign (0x%x).\n",
                 insn);
        retval = 0;
    } else {
#if DEBUG
        printf ("       extract str: ``%s''.\n", addr_mod_suffix_sign->name);
#endif
        retval = 1;
        (*info->fprintf_func) (info->stream, "%s", addr_mod_suffix_sign->name);
    }

#if DEBUG
    printf ("-----> Leave function (lily2_print_addr_mod_suffix_sign).\n");
    printf ("\n");
#endif

    return retval;
}

/* Print one instruction from MEMADDR on INFO->STREAM.
   Return the size of the instruction (always 4 on lily2).  */

static int
print_insn (bfd_vma memaddr, struct disassemble_info *info)
{
    int retval = 1;

    unsigned char insn_ch[4];
    unsigned long insn;
    find_byte_func_type find_byte_func = (find_byte_func_type) info->private_data;

    struct lily2_opcode *opcode = NULL;
    struct lily2_opcode *dis_opcode = lily2_dis_opcodes;

    int status = (*info->read_memory_func) (memaddr, (bfd_byte *) &insn_ch[0], 4, info);

    if (status != 0)
    {
        (*info->memory_error_func) (status, memaddr, info);
        return -1;
    }

    (*find_byte_func) (&insn_ch[0], &insn);

#if DEBUG
    printf ("********************** NEW INSTRUCTION (0x%08x) *********************\n", insn);
#endif

    while (1) {

        if (opcode == NULL) {
            opcode = lily2_opc_opcodes_a;
        } else if (opcode == lily2_opc_opcodes_a + lily2_num_opc_opcodes_a) {
            opcode = lily2_opc_opcodes_m;
        } else if (opcode == lily2_opc_opcodes_m + lily2_num_opc_opcodes_m) {
            opcode = lily2_opc_opcodes_d;
        } else if (opcode == lily2_opc_opcodes_d + lily2_num_opc_opcodes_d) {
            /* Can't find opcode, maybe in data section, just print the machine code. */
            (*info->fprintf_func) (info->stream, "0x%x", insn);
            break;
        }

        if (lily2_opcode_match (insn, opcode->encoding)) {
            /* Finds the matching opcode. */

            /* Prints the functional units. */
            lily2_print_functional_unit ('F', dis_opcode->encoding, insn, info);

            /* Prints the conditions. */
            lily2_print_condition ('Z', opcode->encoding, insn, info);

            /* Prints the instruction name. */
            (*info->fprintf_func) (info->stream, "%s ", opcode->name);

            /* Prints the operand lists. */
            char *s;
            for (s = opcode->args; s && *s != '\0' && retval; ++s) {
                switch (*s) {
                    case 'r': /* Register. */
                        retval = lily2_print_register
                            (*++s, opcode->encoding, insn, info);
                        break;

                    case 'd': /* Register-Pair. */
                        retval = lily2_print_register_pair
                            (*++s, opcode->encoding, insn, info);
                        break;

                    case 'q': /* Register-Pair-Pair. */
                        retval = lily2_print_register_pair_pair
                            (*++s, opcode->encoding, insn, info);
                        break;

                    case 'i': /* Immediate. */
                        retval = lily2_print_immediate
                            (*++s, opcode->encoding, insn, info);
                        break;

                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case ',': /* Symbols. */
                        retval = (*info->fprintf_func)
                            (info->stream, "%c", *s);
                        break;

                    case '*':
                        retval = lily2_print_addr_mod_prefix_sign
                            ('G', opcode->encoding, insn, info);
                        break;
                    case '#':
                        retval = lily2_print_addr_mod_suffix_sign
                            ('G', opcode->encoding, insn, info);
                        break;

                    default:
                        retval = 0;
                }
            }

            /* Exits. */
            break;
        }

        /* Advance iteration. */
        ++opcode;
    }

    /* On success, returns the size of instruction.
       On failure, returns -1. */
    if (retval == 0) {
        fprintf (stderr, "%s\n", error);
        return -1;
    } else {
        return 4;
    }
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
