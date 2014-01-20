/* Assembly backend for the LILY2.
   Copyright (C) DSP Group, Institute of Microelectronics, Tsinghua University
   All rights reserved.

   Contributed by Xiaotian Li <lixiaotian07@gmail.com>.

   This file is part of GAS, the GNU Assembler.

   GAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   GAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GAS; see the file COPYING.  If not, write to
   the Free Software Foundation, 51 Franklin Street - Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* tc-a29k.c used as a template.  */

#include "as.h"
#include "safe-ctype.h"
#include "opcode/lily2.h"
#include "elf/lily2.h"

#define DEBUG 1

#ifndef REGISTER_PREFIX
#define REGISTER_PREFIX   '%'
#endif

/* Make it easier to clone this machine desc into another one.  */
#define machine_opcode  lily2_opcode
#define machine_opcodes lily2_opcodes
#define machine_ip      lily2_ip
#define machine_it      lily2_it

/* To make difference with hashes in dis.c. */
#define opc_hash_control hash_control
#define opc_hash_new     hash_new
#define opc_hash_insert  hash_insert
#define opc_hash_find    hash_find

struct machine_it
{
    char error[100];   /* Error information. */

    unsigned long insn;
    expressionS exp;
    int pcrel;
    int reloc_offset;
    int reloc;

    struct lily2_cluster cluster; /* Auxiliary info. */
    struct lily2_opcode *opcode;  /* Auxiliary info. */
}
the_insn;

/* Handle of OPC_OPCODES. */
static struct opc_hash_control *opc_opcodes_a_hash = NULL;
static struct opc_hash_control *opc_opcodes_m_hash = NULL;
static struct opc_hash_control *opc_opcodes_d_hash = NULL;

/* Handle of OPC_FUNCTIONAL_UNITS. */
static struct opc_hash_control *opc_functional_units_hash = NULL;

/* Handle of OPC_CONDITIONS. */
static struct opc_hash_control *opc_conditions_hash = NULL;

/* Handle of OPC_REGISTERS. */
static struct opc_hash_control *opc_registers_x_hash = NULL;
static struct opc_hash_control *opc_registers_y_hash = NULL;
static struct opc_hash_control *opc_registers_m_hash = NULL;
static struct opc_hash_control *opc_registers_c_hash = NULL;

/* Handle of OPC_REGISTER_PAIRS. */
static struct opc_hash_control *opc_register_pairs_x_hash = NULL;
static struct opc_hash_control *opc_register_pairs_y_hash = NULL;

/* Handle of OPC_REGISTER_PAIR_PAIRS. */
static struct opc_hash_control *opc_register_pair_pairs_x_hash = NULL;
static struct opc_hash_control *opc_register_pair_pairs_y_hash = NULL;

/* Handle of OPC_ADDR_MOD_SIGNS. */
static struct opc_hash_control *opc_addr_mod_signs_hash = NULL;

const pseudo_typeS md_pseudo_table[] =
{
  {"align",   s_align_bytes,  4 },
  {"space",   s_space,        0 },
  {"cputype", s_ignore,       0 },
  {"reg",     s_lsym,         0 },  /* Register equate, same as equ.  */
  {"sect",    s_ignore,       0 },  /* Creation of coff sections.  */
  {"proc",    s_ignore,       0 },  /* Start of a function.  */
  {"endproc", s_ignore,       0 },  /* Function end.  */
  {"word",    cons,           4 },
  {NULL,      0,              0 },
};

int md_short_jump_size  = 4;
int md_long_jump_size   = 4;

/* This array holds the chars that always start a comment.
   If the pre-processor is disabled, these aren't very useful.  */
const char comment_chars[] = "#";

/* This array holds the chars that only start a comment at the beginning of
   a line.  If the line seems to have the form '# 123 filename'
   .line and .file directives will appear in the pre-processed output.  */
/* Note that input_file.c hand checks for '#' at the beginning of the
   first line of the input file.  This is because the compiler outputs
   #NO_APP at the beginning of its output.  */
/* Also note that comments like this one will always work.  */
const char line_comment_chars[] = "#";

/* We needed an unused char for line separation to work around the
   lack of macros, using sed and such.  */
const char line_separator_chars[] = ";";

/* Chars that can be used to separate mant from exp in floating point nums.  */
const char EXP_CHARS[] = "eE";

/* Chars that mean this number is a floating point constant.
   As in 0f12.456
   or    0d1.2345e12.  */
const char FLT_CHARS[] = "rRsSfFdDxXpP";

/* "l.jalr r9" precalculated opcode.  */
static unsigned long jalr_r9_opcode;

static int machine_ip (char *);

/* Some useful macros and functions. */
/* Skip the whitespaces before the string. */
#define skip_whitespace(str) while(ISSPACE(*str)) ++(str)

/* Operand separator in operand list. */
#define is_operand_separator(C) ((C) == ',' || \
                                 (C) == '(' || \
                                 (C) == ')' || \
                                 (C) == '[' || \
                                 (C) == ']' || \
                                 (C) == '\0'    )

/* Indicator of a relocation label. */
#define is_operand_label(C) (ISALPHA (C))

/* Initializes hash tables. */
static int opc_hash_init (void)
{
    int i, lose = 0;
    char *retval = NULL;

    /* OPC_OPCODE_A. */
    opc_opcodes_a_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_opcodes_a; ++i) {

        const char *key = lily2_opc_opcodes_a[i].name;

        retval = opc_hash_insert
            (opc_opcodes_a_hash, key, & (lily2_opc_opcodes_a[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_opcodes_a.\n");
        }
    }

    /* OPC_OPCODE_M. */
    opc_opcodes_m_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_opcodes_m; ++i) {

        const char *key = lily2_opc_opcodes_m[i].name;

        retval = opc_hash_insert
            (opc_opcodes_m_hash, key, & (lily2_opc_opcodes_m[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_opcodes_m.\n");
        }
    }

    /* OPC_OPCODE_D. */
    opc_opcodes_d_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_opcodes_d; ++i) {

        const char *key = lily2_opc_opcodes_d[i].name;

        retval = opc_hash_insert
            (opc_opcodes_d_hash, key, & (lily2_opc_opcodes_d[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_opcodes_d.\n");
        }
    }

    /* OPC_CONDITION. */
    opc_conditions_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_conditions; ++i) {

        const char* key = lily2_opc_conditions[i].name;

        retval = opc_hash_insert
            (opc_conditions_hash, key, & (lily2_opc_conditions[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_conditions.\n");
        }
    }

    /* OPC_FUNCTIONAL_UNIT. */
    opc_functional_units_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_functional_units; ++i) {

        const char *key = lily2_opc_functional_units[i].name;

        retval = opc_hash_insert
            (opc_functional_units_hash, key, & (lily2_opc_functional_units[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_functional_units.\n");
        }
    }

    /* OPC_REGISTER_X. */
    opc_registers_x_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_registers_x; ++i) {

        const char *key = lily2_opc_registers_x[i].name;

        retval = opc_hash_insert
            (opc_registers_x_hash, key, & (lily2_opc_registers_x[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_registers_x.\n");
        }
    }

    /* OPC_REGISTER_Y. */
    opc_registers_y_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_registers_y; ++i) {

        const char *key = lily2_opc_registers_y[i].name;

        retval = opc_hash_insert
            (opc_registers_y_hash, key, & (lily2_opc_registers_y[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_registers_y.\n");
        }
    }

    /* OPC_REGISTER_M. */
    opc_registers_m_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_registers_m; ++i) {

        const char *key = lily2_opc_registers_m[i].name;

        retval = opc_hash_insert
            (opc_registers_m_hash, key, & (lily2_opc_registers_m[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_registers_m.\n");
        }
    }

    /* OPC_REGISTER_C. */
    opc_registers_c_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_registers_c; ++i) {

        const char *key = lily2_opc_registers_c[i].name;

        retval = opc_hash_insert
            (opc_registers_c_hash, key, & (lily2_opc_registers_c[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_registers_c.\n");
        }
    }

    /* OPC_REGISTER_PAIR_X. */
    opc_register_pairs_x_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_register_pairs_x; ++i) {

        const char *key = lily2_opc_register_pairs_x[i].name;

        retval = opc_hash_insert
            (opc_register_pairs_x_hash, key, & (lily2_opc_register_pairs_x[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_register_pairs_x.\n");
        }
    }

    /* OPC_REGISTER_PAIR_Y. */
    opc_register_pairs_y_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_register_pairs_y; ++i) {

        const char *key = lily2_opc_register_pairs_y[i].name;

        retval = opc_hash_insert
            (opc_register_pairs_y_hash, key, & (lily2_opc_register_pairs_y[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_register_paris_y.\n");
        }
    }

    /* OPC_REGISTER_PAIR_PAIR_X. */
    opc_register_pair_pairs_x_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_register_pair_pairs_x; ++i) {

        const char *key = lily2_opc_register_pair_pairs_x[i].name;

        retval = opc_hash_insert
            (opc_register_pair_pairs_x_hash, key, & (lily2_opc_register_pair_pairs_x[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_register_pair_pairs_x.\n");
        }
    }

    /* OPC_REGISTER_PAIR_PAIR_Y. */
    opc_register_pair_pairs_y_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_register_pair_pairs_y; ++i) {

        const char *key = lily2_opc_register_pair_pairs_y[i].name;

        retval = opc_hash_insert
            (opc_register_pair_pairs_y_hash, key, & (lily2_opc_register_pair_pairs_y[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_register_pair_pairs_y.\n");
        }
    }

    /* OPC_ADDR_MOD_SIGN. */
    opc_addr_mod_signs_hash = opc_hash_new ();

    for (i = 0; i != lily2_num_opc_addr_mod_signs; ++i) {

        const char *key = lily2_opc_addr_mod_signs[i].name;

        retval = opc_hash_insert
            (opc_addr_mod_signs_hash, key, & (lily2_opc_addr_mod_signs[i]));

        if (retval != NULL) {
            lose = 1;
            sprintf (the_insn.error, "internal error: can't hash opc_addr_mod_signs.\n");
        }
    }

    return (lose) ? 0 : 1;
}

/* Gets handle of OPC_OPCODES hash table. On success, returns the handle.
   On failure, returns NULL. */
static struct opc_hash_control *
which_opc_opcodes_hash (struct lily2_opcode *opcodes)
{
    struct opc_hash_control *opc_opcodes_hash = NULL;

    if (opcodes == lily2_opc_opcodes_a) {
        opc_opcodes_hash = opc_opcodes_a_hash;
    } else if (opcodes == lily2_opc_opcodes_m) {
        opc_opcodes_hash = opc_opcodes_m_hash;
    } else if (opcodes == lily2_opc_opcodes_d) {
        opc_opcodes_hash = opc_opcodes_d_hash;
    } else {
        opc_opcodes_hash = NULL;
        sprintf (the_insn.error,
                 "internal error: can't get opcode hash table.\n");
    }

    return opc_opcodes_hash;
}

/* Gets handle of OPC_REGISTERS hash table. On success, returns the handle.
   On failure, returns NULL. */
static struct opc_hash_control *
which_opc_registers_hash (char param_ch, unsigned long cluster)
{
    struct opc_hash_control *opc_registers_hash = NULL;

    switch (param_ch) {

        case 'A': /* Fall through. */
        case 'B': /* Fall through. */
        case 'D': /* Same cluster. */
            opc_registers_hash =
                (cluster) ? opc_registers_y_hash : opc_registers_x_hash;
            break;

        case 'X': /* Across cluster. */
            opc_registers_hash =
                (cluster) ? opc_registers_x_hash : opc_registers_y_hash;
            break;

        default : /* Unknown args. */
            opc_registers_hash = NULL;
            sprintf (the_insn.error,
                     "internal error: unknown args letter `%c'.\n", param_ch);
            break;
    }

    return opc_registers_hash;
}

/* Gets handle of OPC_REGISTER_PAIRS hash table. On success, returns the
   handle. On failure, returns NULL. */
static struct opc_hash_control *
which_opc_register_pairs_hash (char param_ch, unsigned long cluster)
{
    struct opc_hash_control *opc_register_pairs_hash = NULL;

    switch (param_ch) {

        case 'A': /* Fall through. */
        case 'B': /* Fall through. */
        case 'D': /* Same cluster. */
            opc_register_pairs_hash =
                (cluster) ? opc_register_pairs_y_hash : opc_register_pairs_x_hash;
            break;

        case 'X': /* Across cluster. */
            opc_register_pairs_hash =
                (cluster) ? opc_register_pairs_x_hash : opc_register_pairs_y_hash;
            break;

        default : /* Unknown args. */
            opc_register_pairs_hash = NULL;
            sprintf (the_insn.error,
                     "internal error: unknown args letter `%c'.\n", param_ch);
            break;
    }

    return opc_register_pairs_hash;
}

/* Gets handle of OPC_REGISTER_PAIR_PAIRS hash table. On success, returns
   the handle. On failure, returns NULL. */
static struct opc_hash_control *
which_opc_register_pair_pairs_hash (char param_ch, unsigned long cluster)
{
    struct opc_hash_control *opc_register_pair_pairs_hash = NULL;

    switch (param_ch) {

        case 'A': /* Fall through. */
        case 'B': /* Fall through. */
        case 'D': /* Same cluster. */
            opc_register_pair_pairs_hash =
                (cluster) ? opc_register_pair_pairs_y_hash : opc_register_pair_pairs_x_hash;
            break;

        case 'X': /* Across cluster. */
            opc_register_pair_pairs_hash =
                (cluster) ? opc_register_pair_pairs_x_hash : opc_register_pair_pairs_y_hash;
            break;

        default : /* Unknown args. */
            opc_register_pair_pairs_hash = NULL;
            sprintf (the_insn.error,
                     "internal error: unknown args letter `%c'.\n", param_ch);
            break;
    }

    return opc_register_pair_pairs_hash;
}

/* Removes the whitespaces in S. */
static void
remove_whitespace (char *s)
{
    char *t = NULL;

    if ((t = (char *) malloc (strlen (s))) == NULL) {
        fprintf (stderr, "internal error: remove_whitespace allocation error.\n");
        as_fatal (_("Broken assembler. Assemble abort."));
    }

    char *sp = s;
    char *tp = t;

    for (; *sp; ++sp) if (!ISSPACE (*sp)) *tp++ = *sp;

    *tp = '\0'; /* Terminates S. */

    strcpy (s, t);

    if (t) free (t);
}

int hex2bin (char *str)
{
    char *hex = str;
    char *bin = (char *) malloc (4 * strlen (hex));
    if (!bin) {
        fprintf (stderr, "internal error: malloc error in function ``hex2bin''.");
        as_fatal (_("Broken assembler. Assemble abort."));
    }
    char *s = bin;

    for (; *hex; ++hex) {
        TOLOWER (*hex);
        switch (*hex) {
            case '0': *s++ = '0'; *s++ = '0'; *s++ = '0'; *s++ = '0'; break;
            case '1': *s++ = '0'; *s++ = '0'; *s++ = '0'; *s++ = '1'; break;
            case '2': *s++ = '0'; *s++ = '0'; *s++ = '1'; *s++ = '0'; break;
            case '3': *s++ = '0'; *s++ = '0'; *s++ = '1'; *s++ = '1'; break;
            case '4': *s++ = '0'; *s++ = '1'; *s++ = '0'; *s++ = '0'; break;
            case '5': *s++ = '0'; *s++ = '1'; *s++ = '0'; *s++ = '1'; break;
            case '6': *s++ = '0'; *s++ = '1'; *s++ = '1'; *s++ = '0'; break;
            case '7': *s++ = '0'; *s++ = '1'; *s++ = '1'; *s++ = '1'; break;
            case '8': *s++ = '1'; *s++ = '0'; *s++ = '0'; *s++ = '0'; break;
            case '9': *s++ = '1'; *s++ = '0'; *s++ = '0'; *s++ = '1'; break;
            case 'a': *s++ = '1'; *s++ = '0'; *s++ = '1'; *s++ = '0'; break;
            case 'b': *s++ = '1'; *s++ = '0'; *s++ = '1'; *s++ = '1'; break;
            case 'c': *s++ = '1'; *s++ = '1'; *s++ = '0'; *s++ = '0'; break;
            case 'd': *s++ = '1'; *s++ = '1'; *s++ = '0'; *s++ = '1'; break;
            case 'e': *s++ = '1'; *s++ = '1'; *s++ = '1'; *s++ = '0'; break;
            case 'f': *s++ = '1'; *s++ = '1'; *s++ = '1'; *s++ = '1'; break;
        }
    }
    *s = '\0';

    int bin_count;
    for (bin_count = 0; *bin && *bin == '0'; ++bin) {
        ++bin_count;
        *bin = ' ';
    }
    bin_count = 4 * strlen (str) - bin_count;

    remove_whitespace (bin);
    strcpy (str, bin);

    return bin_count;
}

/* Converts an integer VALUE to a terminated string using the specific BASE
   and stores the result in the array given by STR parameter. On success the
   total number of characters written is returned. On failure, a negative
   number is returned. Supported BASE: 2,8,10,16. */
int itoa (int value, char *str, int base)
{
    switch (base) {
        case 2 : sprintf (str, "%x", value);
                 return hex2bin (str);
        case 8 : return sprintf (str, "%o", value);
        case 10: return sprintf (str, "%d", value);
        case 16: return sprintf (str, "%x", value);
    }
}

static int
lily2_encode (char param_ch, char *encoding, unsigned long insn)
{
    int retval = 1;
    struct lily2_letter *letter = letter_find (param_ch);

    /* Checks the overflow of INSN. */
    if (letter == NULL) {
        retval = 0;
        sprintf (the_insn.error,
                 "internal error: unknown encoding letter `%c'.\n",
                 param_ch);

    } else if (check_overflow (insn, letter->len, letter->sign)) {
        retval = 0;
        sprintf (the_insn.error,
                 "external error: insn of letter `%c' is overflowed(0x%x).\n",
                 param_ch, insn);
    } else {
        char insn_str[64];
        char *insn_ptr = insn_str;
        int insn_str_len = 0;

        /* Converts the value of INSN into a string. */
        insn_str_len = itoa (insn, insn_str, BASE_2);

        if (insn_str_len < 0) {
            /* The conversion is failure or the INSN string is illegal. */
            retval = 0;
            sprintf (the_insn.error,
                     "internal error: insn(0x%x) is illegal.\n", insn);

        } else {
            /* Replaces the corresponding characters in encoding with
               the INSN string. */
            int diff_len = letter->len - insn_str_len;
            for (; *encoding; ++encoding) {
                if (*encoding == param_ch) {
                    *encoding = (diff_len-- > 0) ? '0' : *insn_ptr++;
                }
            }
        }
    }

    return retval;
}

/* Extracts the string of functional unit from the instruction S and
   puts it in the FUNCTIONAL_UNIT_STR. The string should be at the start
   of S. On success, the pointer to the character past the string is
   returned. On failure, a NULL pointer is returned. */
static char *
lily2_extract_functional_unit (char *functional_unit_str, char *s)
{
#if DEBUG
    printf ("<---- Enter function (lily2_extract_functional_unit).\n");
    printf ("      src string: ``%s''.\n", s);
#endif

    char *endptr;

    if (!s) {
        sprintf (the_insn.error, "external error: " \
                 "A NULL string is given when extracting functional unit.\n");
        endptr = NULL;

    } else {
        /* Functional unit starts with `[' and ends with `]'. */
        char *prefix = strchr (s, '[');
        char *suffix = strchr (s, ']');

        if (prefix == s && suffix) {
            /* `[' should be the start of S. */
            ptrdiff_t len = suffix - prefix + 1;
            strncpy (functional_unit_str, prefix, len);

            /* STRNCPY generates a null-terminated string, so terminates
               it manually. */
            functional_unit_str[len] = '\0';

            endptr = suffix + 1;

        } else {
            /* Instructions without a functional unit are illegal. */
            sprintf (the_insn.error,
                     "external error: functional unit not found.\n");
            endptr = NULL;
        }
   }

#if DEBUG
    printf ("      dest string: ``%s''.\n", functional_unit_str);
    printf ("----> Leave function (lily2_extract_functional_unit).\n");
    printf ("\n");
#endif

    return endptr;
}

/* Searches the functional unit code in hash table by the given STR and
   encodes it into the ENCODING according to PARAM_CH. */
static int
lily2_parse_functional_unit (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_functional_unit).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: ``%s''.\n", str);
    printf ("      early encode: ``%s''.\n", encoding);
#endif

    int retval = 1;
    struct lily2_functional_unit *functional_unit =
        opc_hash_find (opc_functional_units_hash, str);

    if (functional_unit) {
        /* Actually encodes the cluster only because the functional unit
           field has already contained in the opcode table. */
        retval = lily2_encode (param_ch, encoding, (functional_unit->cluster).code);

    } else {
        sprintf (the_insn.error,
                 "external error: illegal functional unit ``%s''.\n", str);
        retval = 0;
    }

#if DEBUG
    printf ("      later encode: ``%s''.\n", encoding);
    printf ("----> Leave function (lily2_parse_functional_unit).\n");
    printf ("\n");
#endif

    return retval;
}

/* Extracts the string of condition from the instruction S and puts
   it in the CONDITION_STR. The string should be at the start of S and
   can be omitted. On success, the pointer to the character past the
   string is returned. On failure, a NULL pointer is returned. */
static char *
lily2_extract_condition (char *condition_str, char *s)
{
#if DEBUG
    printf ("<---- Enter function (lily2_extract_condition).\n");
    printf ("      src string: ``%s''.\n", s);
#endif

    char *endptr;

    if (!s) {
        sprintf (the_insn.error, "external error: " \
                 "A NULL string is given when extracting condition.\n");
        endptr = NULL;

    } else {
        skip_whitespace (s);

        /* Condition starts with `{' and ends with `}'. */
        char *prefix = strchr (s, '{');
        char *suffix = strchr (s, '}');

        if (prefix == s && suffix) {
            /* If exists, `{' should be the start of S. */
            ptrdiff_t len = suffix - prefix + 1;
            strncpy (condition_str, prefix, len);
            /* STRNCPY generates a null-terminated string, so terminates
               it manually. */
            condition_str[len] = '\0';

            endptr = suffix + 1;

        } else {
            /* Condition can be omitted. */
            condition_str[0] = '\0';
            endptr = s;
        }
   }

#if DEBUG
    printf ("      dest string: ``%s''.\n", condition_str);
    printf ("----> Leave function (lily2_extract_condition).\n");
    printf ("\n");
#endif

    return endptr;
}

/* Searches the condition code in hash table by the given STR and
   encodes it into the ENCODING according to PARAM_CH. */
static int
lily2_parse_condition (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_condition).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: ``%s''.\n", str);
    printf ("      early encode: ``%s''.\n", encoding);
#endif

    int retval = 1;
    struct lily2_condition *condition =
        opc_hash_find (opc_conditions_hash, str);

    if (condition) {
        retval = lily2_encode (param_ch, encoding, condition->code);

    } else {
        sprintf (the_insn.error,
                 "external error: illegal condition ``%s''.\n", str);
        retval = 0;
    }

#if DEBUG
    printf ("      later encode: ``%s''.\n", encoding);
    printf ("----> Leave function (lily2_parse_condition).\n");
    printf ("\n");
#endif

    return retval;
}

/* Extracts the string of instruction name from the instruction S and
   puts it in the NAME_STR. The string should be at the start of S. On
   success, the pointer to the character past the string is returned.
   On failure, a NULL pointer is returned. */
static char *
lily2_extract_name (char *name_str, char *s)
{
#if DEBUG
    printf ("<---- Enter function (lily2_extract_name).\n");
    printf ("      src string: ``%s''.\n", s);
#endif

    char *endptr;

    if (!s) {
        sprintf (the_insn.error, "external error: " \
                 "A NULL string is given when extracting name.\n");
        endptr = NULL;

    } else {
        char *name_ptr = name_str;

        skip_whitespace (s);
        for (; *s && (ISALNUM (*s) || (*s == '.'));)
            *name_ptr++ = *s++;
        *name_ptr = '\0';

        endptr = s;
    }

#if DEBUG
    printf ("      dest string: ``%s''.\n", name_str);
    printf ("----> Leave function (lily2_extract_name).\n");
    printf ("\n");
#endif

    return endptr;
}

/* Extracts the string of address modification sign from the instruction
   S and puts it in the ADDR_MOD_SIGN_STR. The string should be at the
   start of S and can be omitted. On success, the pointer to the character
   past the string is returned. On failure, a NULL pointer is returned. */
static char *
lily2_extract_addr_mod_sign (char *addr_mod_sign_str, char *s)
{
#if DEBUG
    printf ("<---- Enter function (lily2_extract_addr_mod_sign).\n");
    printf ("      given string: `%s'.\n", s);
#endif

    char *endptr = s;

    if (!s) {
        sprintf (the_insn.error, "external error: " \
                 "null operands list when extracting address modification sign.\n");
        endptr = NULL;

    } else {
        int addr_mod_sign_count = 0;

        char *incr_prefix = strstr (s, "(+");
        char *incr_suffix = strstr (s, "+)");
        char *decr_prefix = strstr (s, "(-");
        char *decr_suffix = strstr (s, "-)");

        if (incr_prefix) { strcpy (addr_mod_sign_str, "(+"); ++addr_mod_sign_count; }
        if (incr_suffix) { strcpy (addr_mod_sign_str, "+)"); ++addr_mod_sign_count; }
        if (decr_prefix) { strcpy (addr_mod_sign_str, "(-"); ++addr_mod_sign_count; }
        if (decr_suffix) { strcpy (addr_mod_sign_str, "-)"); ++addr_mod_sign_count; }

        if (addr_mod_sign_count == 0) {
            sprintf (the_insn.error, "external error: " \
                     "no address modification sign found.\n");
            endptr =  NULL;

        } else if (addr_mod_sign_count > 1) {
            sprintf (the_insn.error, "external error: " \
                     "multiple address modification signs found.\n");
            endptr =  NULL;

        } else {
            endptr = s;
        }
    }

#if DEBUG
    printf ("      found string: `%s'.\n", addr_mod_sign_str);
    printf ("----> Leave function (lily2_extract_addr_mod_sign).\n");
    printf ("\n");
#endif

    return endptr;
}

/* Searches the address modification sign  code in hash table by the
   given STR and encodes it into the ENCODING according to PARAM_CH. */
static int
lily2_parse_addr_mod_sign (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_addr_mod_sign).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: `%s'.\n", str);
    printf ("      early encoding: `%s'.\n", encoding);
#endif

    int retval = 1;

    struct lily2_addr_mod_sign *addr_mod_sign = NULL;

    addr_mod_sign = opc_hash_find (opc_addr_mod_signs_hash, str);

    if (addr_mod_sign) {
        retval = lily2_encode (param_ch, encoding, addr_mod_sign->code);

    } else {
        sprintf (the_insn.error, "external error: " \
                 "illegal address modification sign `%s'.\n", str);
        retval = 0;
    }

#if DEBUG
    printf ("      later encoding: `%s'.\n", encoding);
    printf ("----> Leave function (lily2_parse_addr_mod_sign).\n");
    printf ("\n");
#endif

    return retval;
}

/* Extracts the string of operands from the instruction S and puts it in
   the OPERAND_STR. The string should be at the start of S and can be omitted.
   On success, the pointer to the character past the string is returned.
   On failure, a NULL pointer is returned. */
static char *
lily2_extract_operand (char *operand_str, char *s, char **args,
                       char *operand_type, char *param_ch)
{
#if DEBUG
    printf ("<---- Enter function (lily2_extract_operand).\n");
    printf ("      given string: `%s'.\n", s);
    printf ("      early args: `%s'.\n", *args);
#endif

    char *endptr = NULL;

    char *operand_ptr = operand_str;
    char *args_ptr = *args;

    int operand_found = 0;

    if (!s) {
        sprintf (the_insn.error, "external error: " \
                 "given null operand list when extracting operand.");
        endptr = NULL;

    } else {

        switch (*args_ptr) {

            case 'r': /* Registers. */
                for (++args_ptr; !is_operand_separator (*s);)
                    *operand_ptr++ = *s++;
                *operand_ptr = '\0';
                *operand_type = 'r';
                break;

            case 'd': /* Register-Pairs. */
                for (++args_ptr; !is_operand_separator (*s);)
                    *operand_ptr++ = *s++;
                *operand_ptr = '\0';
                *operand_type = 'd';
                break;

            case 'q': /* Register-Pair-Pairs. */
                for (++args_ptr; !is_operand_separator (*s);)
                    *operand_ptr++ = *s++;
                *operand_ptr = '\0';
                *operand_type = 'q';
                break;

            case 'i': /* Immediates or labels. */

                ++args_ptr;

                if (is_operand_label (*s)) {
                    ;
                } else {
                    for (; !is_operand_separator (*s);)
                        *operand_ptr++ = *s++;
                    *operand_ptr = '\0';
                }
                *operand_type = 'i';

                break;

            case '(':
            case ')':
            case '[':
            case ']':
            case ',': /* Operands separator, match them
                         and keep on searching operands. */
                if (*s != *args_ptr) { /* Match failure. */
                    sprintf (the_insn.error, "internal error: " \
                             "operand separator missed.\n");
                    s = NULL;
                } else { /* Match success. */
                    ++s;
                    *operand_ptr = '\0';
                }

                break;

            case '#':
            case '*': /* Base address modification signs, skip them
                         and keep on searching operands. */
                /* S remains unchanged. */
                *operand_ptr = '\0';

                break;

            default : /* Internal error. */
                sprintf (the_insn.error, "internal error: " \
                         "unknown args character `%c'.\n", *args_ptr);
                s = NULL;
                break;
        }

        *param_ch = *args_ptr;
        *args = ++args_ptr;

        endptr = s;
    }

#if DEBUG
    printf ("      found string: `%s'.\n", operand_str);
    printf ("      param_ch: `%c'.\n", *param_ch);
    printf ("      later args: `%s'.\n", *args);
    printf ("----> Leave function (lily2_extract_operand).\n");
    printf ("\n");
#endif

    return endptr;
}

static int
lily2_parse_register (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_register).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: `%s'.\n", str);
    printf ("      early encoding: `%s'.\n", encoding);
#endif

    int retval = 1;

    struct opc_hash_control *opc_register_hash =
        which_opc_registers_hash (param_ch, the_insn.cluster.code);

    if (opc_register_hash) {
        struct lily2_register *reg = opc_hash_find (opc_register_hash, str);

        if (reg) {
            retval = lily2_encode (param_ch, encoding, reg->code);
        } else {
            retval = 0;
            sprintf (the_insn.error,
                     "external error: illegal register `%s'.\n", str);
        }

    } else {
        retval = 0;
    }

#if DEBUG
    printf ("      later encoding: `%s'.\n", encoding);
    printf ("----> Leave function (lily2_parse_register).\n");
    printf ("\n");
#endif

    return retval;
}

static int
lily2_parse_register_pair (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_register_pair).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: `%s'.\n", str);
    printf ("      early encoding: `%s'.\n", encoding);
#endif

    int retval = 1;

    struct opc_hash_control *opc_register_pairs_hash =
        which_opc_register_pairs_hash (param_ch, the_insn.cluster.code);

    if (opc_register_pairs_hash) {
        struct lily2_register *reg = opc_hash_find (opc_register_pairs_hash, str);

        if (reg) {
            retval = lily2_encode (param_ch, encoding, reg->code);
        } else {
            retval = 0;
            sprintf (the_insn.error,
                     "external error: illegal register pair `%s'.\n", str);
        }

    } else {
        retval = 0;
    }

#if DEBUG
    printf ("      later encoding: `%s'.\n", encoding);
    printf ("----> Leave function (lily2_parse_register_pair).\n");
    printf ("\n");
#endif

    return retval;
}

static int
lily2_parse_register_pair_pair (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_register_pair_pair).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: `%s'.\n", str);
    printf ("      early encoding: `%s'.\n", encoding);
#endif

    int retval = 1;

    struct opc_hash_control *opc_register_pair_pairs_hash =
        which_opc_register_pair_pairs_hash (param_ch, the_insn.cluster.code);

    if (opc_register_pair_pairs_hash) {
        struct lily2_register *reg = opc_hash_find (opc_register_pair_pairs_hash, str);

        if (reg) {
            retval = lily2_encode (param_ch, encoding, reg->code);
        } else {
            retval = 0;
            sprintf (the_insn.error,
                     "external error: illegal register-pair-pair `%s'.\n", str);
        }

    } else {
        retval = 0;
    }

#if DEBUG
    printf ("      later encoding: `%s'.\n", encoding);
    printf ("----> Leave function (lily2_parse_register_pair_pair).\n");
    printf ("\n");
#endif

    return retval;
}

static int
lily2_parse_immediate (char param_ch, char *encoding, char *str)
{
#if DEBUG
    printf ("<---- Enter function (lily2_parse_immediate).\n");
    printf ("      param_ch: `%c'.\n", param_ch);
    printf ("      str: `%s'.\n", str);
    printf ("      early encoding: `%s'.\n", encoding);
#endif

    int retval = 1;

    char *endptr = str;
    unsigned long value;

    struct lily2_letter *letter = letter_find (param_ch);

    if (letter == NULL) {
        sprintf (the_insn.error, "internal error: " \
                 "unknown encoding letter `%c'.\n", param_ch);
        return retval = 0;
    }

    /* To now, we only support the following formats immediate value.

       For HEX: 0x + ISXDIGIT(a).
       For DEC: (+/-) + ISDIGIT(a). */

    if (*str == '0' && *(str + 1) == 'x') { /* HEX. */

        /* For HEX format immediate, we should sign-extend the immediate
           properly when we find the sign bit is '1'.
           e.g. 0xff, no matter it is signed or unsigned, it can fill 8-bit
           encoding field. */

        value = (unsigned long) strtol (str, &endptr, BASE_16);

        if (*endptr) {
            sprintf (the_insn.error, "external error: " \
                     "illegal immediate value `%s'.\n", str);
            retval = 0;
        }

        if (retval && letter->sign) { /* Properly sign-extend. */

            if (bits (value, letter->len - 1, letter->len - 1) == 1 &&
                    (value & ~mask (letter->len)) == 0) {
                value = sign_extend (value, letter->len, 1); /* sign-extend. */
            }
        }

    } else { /* DEC. */

        value = (unsigned long) strtol (str, &endptr, BASE_10);

        if (*endptr) {
            sprintf (the_insn.error, "external error: " \
                     "illegal immediate value `%s'.\n", str);
            retval = 0;
        }
    }

    if (retval) retval = lily2_encode (param_ch, encoding, value);

#if DEBUG
    printf ("      later encoding: `%s'.\n", encoding);
    printf ("----> Leave function (lily2_parse_immediate).\n");
    printf ("\n");
#endif

    return retval;
}

static int
lily2_parse_operand (char operand_type, char param_ch, char *encoding, char *str)
{
    int retval;

    switch (param_ch) {
        case 'A': /* Fall through. */
        case 'B': /* Fall through. */
        case 'D': /* Fall through. */
        case 'C': /* Fall through. */
        case 'X':
            switch (operand_type) {
                case 'r': retval =
                          lily2_parse_register (param_ch, encoding, str); break;
                case 'd': retval =
                          lily2_parse_register_pair (param_ch, encoding, str); break;
                case 'q': retval =
                          lily2_parse_register_pair_pair (param_ch, encoding, str); break;
                default : fprintf (stderr, "illegal operand type `%c'.", operand_type);
                          as_fatal (_("Broken assembler. Assemble abort."));

            }
            break;

        default : retval = lily2_parse_immediate (param_ch, encoding, str); break;
    }

    return retval;
}

/* This function is called once, at assembler startup time.  It should
   set up all the tables, etc., that the MD part of the assembler will
   need.  */

void
md_begin (void)
{
    opc_hash_init ();
}

/* Returns non zero if instruction is to be used.  */

static int
check_invalid_opcode (unsigned long opcode)
{
  return opcode == jalr_r9_opcode;
}

/* Assemble a single instruction.  Its label has already been handled
   by the generic front end.  We just parse opcode and operands, and
   produce the bytes of data and relocation.  */

void
md_assemble (char *str)
{
#if DEBUG
    printf ("********************** NEW INSTRUCTION (%s) *********************\n", str);
#endif

    know (str);
    if (!machine_ip (str)) {
        fprintf (stderr, the_insn.error);
        as_fatal (_("Broken assembler."));
    }

    return;
}


/* Removes the address modification signs in S. */
static void
remove_addr_mod_sign (char *s)
{
    char *incr_prefix = strstr (s, "(+");
    char *decr_prefix = strstr (s, "(-");
    char *incr_suffix = strstr (s, "+)");
    char *decr_suffix = strstr (s, "-)");

    if (incr_prefix) *(incr_prefix + 1) = ' ';
    if (decr_prefix) *(decr_prefix + 1) = ' ';
    if (incr_suffix) *(incr_suffix    ) = ' ';
    if (decr_suffix) *(decr_suffix    ) = ' ';

    remove_whitespace (s);
}



/* Instruction parsing.  Takes a string containing the opcode.
   Operands are at input_line_pointer.  Output is in the_insn.
   Warnings or errors are generated.  */

static int
machine_ip (char *str)
{
    int retval = 1;
    char *s = str;
    const int extract_str_len = 20;
    const int max_operands = 4;


    /* Deals with the functional units. */
    char functional_unit_str[extract_str_len];
    if (retval) {
        s = lily2_extract_functional_unit (functional_unit_str, s);
        if (!s) retval = 0;
    }

    /* Deals with the conditions. */
    char condition_str[extract_str_len];
    if (retval) {
        s = lily2_extract_condition (condition_str, s);
        if (!s) retval = 0;
    }

    /* Deals with the instruction names. */
    char name_str[extract_str_len];
    if (retval) {
        s = lily2_extract_name (name_str, s);
        if (!s) retval = 0;
    }

    /* Gets the opcode table and operand list args first. */
    char encoding[64]; /* Needs to be written. */
    char *args; /* Needs to be read. */
    struct lily2_opcode *opcodes;
    struct lily2_cluster cluster;
    struct lily2_opcode *opcode;
    struct lily2_functional_unit *functional_unit = NULL;
    if (retval) {
        functional_unit = opc_hash_find (opc_functional_units_hash, functional_unit_str);
        if (functional_unit) {
            opcodes = functional_unit->opcodes;
            cluster = functional_unit->cluster;

            opcode = opc_hash_find (which_opc_opcodes_hash (opcodes), name_str);
            if (opcode) {
                strcpy (encoding, opcode->encoding);
                args = opcode->args;
            } else {
                retval = 0;
            }
        } else {
            retval = 0;
        }
    }

    /* Until here, we gets the encoding and args. */
    memset (&the_insn, 0, sizeof (the_insn));
    the_insn.cluster = cluster;
    the_insn.reloc = BFD_RELOC_NONE;

    /* Before dealing with the operands, removes the whitespaces first. */
    remove_whitespace (s);

    /* Deals with the address modification signs. */
    char addr_mod_sign_str[extract_str_len];
    if (retval) {
        if (opcode->flag == ADDR_MOD) {
            s = lily2_extract_addr_mod_sign (addr_mod_sign_str, s);
            if (s) {
                remove_addr_mod_sign (s);
            } else {
                retval = 0;
            }
        }
    }

    /* Deals with the operand lists. */
    char operand_str[max_operands][extract_str_len];
    char operand_type[max_operands];
    char param_ch[max_operands];
    int operand_count = 0;
    if (retval) {
        while (*s != '\0' && retval != 0) {
            s = lily2_extract_operand (operand_str[operand_count],
                    s, &args, &operand_type[operand_count], &param_ch[operand_count]);
            if (!s) {
                retval = 0;
            } else if (*(operand_str[operand_count]) != '\0') {
                ++operand_count;
            }
        }
    }

    /* Now all the extracting works are done.
       Do parsing! */
    if (retval) {
        retval = lily2_parse_functional_unit ('E', encoding, functional_unit_str);
    }
    if (retval) {
        retval = lily2_parse_condition ('Z', encoding, condition_str);
    }
    if (retval) {
        if (opcode->flag == ADDR_MOD) {
            retval = lily2_parse_addr_mod_sign ('S', encoding, addr_mod_sign_str);
        }
    }
    if (retval) {
        int i;
        for (i = 0; i != operand_count && retval; ++i) {
            retval = lily2_parse_operand (operand_type[i], param_ch[i],
                encoding, operand_str[i]);
        }
    }

    return retval;
}

char *
md_atof (int type, char * litP, int *  sizeP)
{
  return ieee_md_atof (type, litP, sizeP, TRUE);
}

/* Write out big-endian.  */

void
md_number_to_chars (char *buf, valueT val, int n)
{
  number_to_chars_bigendian (buf, val, n);
}

void
md_apply_fix (fixS * fixP, valueT * val, segT seg ATTRIBUTE_UNUSED)
{
  char *buf = fixP->fx_where + fixP->fx_frag->fr_literal;
  long t_val;

  t_val = (long) *val;

#if DEBUG
  printf ("md_apply_fix val:%x\n", t_val);
#endif

  fixP->fx_addnumber = t_val; /* Remember value for emit_reloc.  */

  switch (fixP->fx_r_type)
    {
    case BFD_RELOC_32:      /* XXXXXXXX pattern in a word.  */
#if DEBUG
      printf ("reloc_const: val=%x\n", t_val);
#endif
      buf[0] = t_val >> 24;
      buf[1] = t_val >> 16;
      buf[2] = t_val >> 8;
      buf[3] = t_val;
      break;

    case BFD_RELOC_16:      /* XXXX0000 pattern in a word.  */
#if DEBUG
      printf ("reloc_const: val=%x\n", t_val);
#endif
      buf[0] = t_val >> 8;
      buf[1] = t_val;
      break;

    case BFD_RELOC_8:      /* XX000000 pattern in a word.  */
#if DEBUG
      printf ("reloc_const: val=%x\n", t_val);
#endif
      buf[0] = t_val;
      break;

    case BFD_RELOC_LO16:      /* 0000XXXX pattern in a word.  */
#if DEBUG
      printf ("reloc_const: val=%x\n", t_val);
#endif
      buf[2] = t_val >> 8;  /* Holds bits 0000XXXX.  */
      buf[3] = t_val;
      break;

    case BFD_RELOC_HI16:    /* 0000XXXX pattern in a word.  */
#if DEBUG
      printf ("reloc_consth: val=%x\n", t_val);
#endif
      buf[2] = t_val >> 24; /* Holds bits XXXX0000.  */
      buf[3] = t_val >> 16;
      break;

    case BFD_RELOC_32_GOT_PCREL:  /* 0000XXXX pattern in a word.  */
      if (!fixP->fx_done)
        ;
      else if (fixP->fx_pcrel)
        {
          long v = t_val >> 28;

          if (v != 0 && v != -1)
            as_bad_where (fixP->fx_file, fixP->fx_line,
                          _("call/jmp target out of range (2)"));
        }
      else
        /* This case was supposed to be handled in machine_ip.  */
        abort ();

      buf[0] |= (t_val >> 26) & 0x03; /* Holds bits 0FFFFFFC of address.  */
      buf[1] = t_val >> 18;
      buf[2] = t_val >> 10;
      buf[3] = t_val >> 2;
      break;

    case BFD_RELOC_VTABLE_INHERIT:
    case BFD_RELOC_VTABLE_ENTRY:
      fixP->fx_done = 0;
      break;

    case BFD_RELOC_NONE:
    default:
      as_bad (_("bad relocation type: 0x%02x"), fixP->fx_r_type);
      break;
    }

  if (fixP->fx_addsy == (symbolS *) NULL)
    fixP->fx_done = 1;
}

/* Should never be called for lily2.  */

void
md_create_short_jump (char *    ptr       ATTRIBUTE_UNUSED,
		      addressT  from_addr ATTRIBUTE_UNUSED,
		      addressT  to_addr   ATTRIBUTE_UNUSED,
		      fragS *   frag      ATTRIBUTE_UNUSED,
		      symbolS * to_symbol ATTRIBUTE_UNUSED)
{
  as_fatal ("lily2_create_short_jmp\n");
}

/* Should never be called for lily2.  */

void
md_convert_frag (bfd *   headers ATTRIBUTE_UNUSED,
		 segT    seg     ATTRIBUTE_UNUSED,
		 fragS * fragP   ATTRIBUTE_UNUSED)
{
  as_fatal ("lily2_convert_frag\n");
}

/* Should never be called for lily2.  */

void
md_create_long_jump (char *    ptr       ATTRIBUTE_UNUSED,
		     addressT  from_addr ATTRIBUTE_UNUSED,
		     addressT  to_addr   ATTRIBUTE_UNUSED,
		     fragS *   frag      ATTRIBUTE_UNUSED,
		     symbolS * to_symbol ATTRIBUTE_UNUSED)
{
  as_fatal ("lily2_create_long_jump\n");
}

/* Should never be called for lily2.  */

int
md_estimate_size_before_relax (fragS * fragP   ATTRIBUTE_UNUSED,
			       segT    segtype ATTRIBUTE_UNUSED)
{
  as_fatal ("lily2_estimate_size_before_relax\n");
  return 0;
}

/* Translate internal representation of relocation info to target format.

   On sparc/29k: first 4 bytes are normal unsigned long address, next three
   bytes are index, most sig. byte first.  Byte 7 is broken up with
   bit 7 as external, bits 6 & 5 unused, and the lower
   five bits as relocation type.  Next 4 bytes are long addend.  */
/* Thanx and a tip of the hat to Michael Bloom, mb@ttidca.tti.com.  */

#ifdef OBJ_AOUT
void
tc_aout_fix_to_chars (char *where,
		      fixS *fixP,
		      relax_addressT segment_address_in_file)
{
  long r_symbolnum;

#if DEBUG
  printf ("tc_aout_fix_to_chars\n");
#endif

  know (fixP->fx_r_type < BFD_RELOC_NONE);
  know (fixP->fx_addsy != NULL);

  md_number_to_chars
    (where,
     fixP->fx_frag->fr_address + fixP->fx_where - segment_address_in_file,
     4);

  r_symbolnum = (S_IS_DEFINED (fixP->fx_addsy)
     ? S_GET_TYPE (fixP->fx_addsy)
     : fixP->fx_addsy->sy_number);

  where[4] = (r_symbolnum >> 16) & 0x0ff;
  where[5] = (r_symbolnum >> 8) & 0x0ff;
  where[6] = r_symbolnum & 0x0ff;
  where[7] = (((!S_IS_DEFINED (fixP->fx_addsy)) << 7) & 0x80) | (0 & 0x60) | (fixP->fx_r_type & 0x1F);

  /* Also easy.  */
  md_number_to_chars (&where[8], fixP->fx_addnumber, 4);
}

#endif /* OBJ_AOUT */

const char *md_shortopts = "";

struct option md_longopts[] =
{
  { NULL, no_argument, NULL, 0 }
};
size_t md_longopts_size = sizeof (md_longopts);

int
md_parse_option (int c ATTRIBUTE_UNUSED, char * arg ATTRIBUTE_UNUSED)
{
  return 0;
}

void
md_show_usage (FILE * stream ATTRIBUTE_UNUSED)
{
}

/* This is called when a line is unrecognized.  This is used to handle
   definitions of lily2 style local labels.  */

int
lily2_unrecognized_line (int c)
{
  int lab;
  char *s;

  if (c != '$'
      || ! ISDIGIT ((unsigned char) input_line_pointer[0]))
    return 0;

  s = input_line_pointer;

  lab = 0;
  while (ISDIGIT ((unsigned char) *s))
    {
      lab = lab * 10 + *s - '0';
      ++s;
    }

  if (*s != ':')
    /* Not a label definition.  */
    return 0;

  if (dollar_label_defined (lab))
    {
      as_bad (_("label \"$%d\" redefined"), lab);
      return 0;
    }

  define_dollar_label (lab);
  colon (dollar_label_name (lab, 0));
  input_line_pointer = s + 1;

  return 1;
}

/* Default the values of symbols known that should be "predefined".  We
   don't bother to predefine them unless you actually use one, since there
   are a lot of them.  */

symbolS *
md_undefined_symbol (char *name ATTRIBUTE_UNUSED)
{
  return NULL;
}

/* Parse an operand that is machine-specific.  */

void
md_operand (expressionS *expressionP)
{
#if DEBUG
  printf ("  md_operand(input_line_pointer = %s)\n", input_line_pointer);
#endif

  if (input_line_pointer[0] == REGISTER_PREFIX && input_line_pointer[1] == 'r')
    {
      /* We have a numeric register expression.  No biggy.  */
      input_line_pointer += 2;  /* Skip %r */
      (void) expression (expressionP);

      if (expressionP->X_op != O_constant
          || expressionP->X_add_number > 255)
        as_bad (_("Invalid expression after %%%%\n"));
      expressionP->X_op = O_register;
    }
  else if (input_line_pointer[0] == '&')
    {
      /* We are taking the 'address' of a register...this one is not
         in the manual, but it *is* in traps/fpsymbol.h!  What they
         seem to want is the register number, as an absolute number.  */
      input_line_pointer++; /* Skip & */
      (void) expression (expressionP);

      if (expressionP->X_op != O_register)
        as_bad (_("invalid register in & expression"));
      else
        expressionP->X_op = O_constant;
    }
  else if (input_line_pointer[0] == '$'
           && ISDIGIT ((unsigned char) input_line_pointer[1]))
    {
      long lab;
      char *name;
      symbolS *sym;

      /* This is a local label.  */
      ++input_line_pointer;
      lab = (long) get_absolute_expression ();

      if (dollar_label_defined (lab))
        {
          name = dollar_label_name (lab, 0);
          sym = symbol_find (name);
        }
      else
        {
          name = dollar_label_name (lab, 1);
          sym = symbol_find_or_make (name);
        }

      expressionP->X_op = O_symbol;
      expressionP->X_add_symbol = sym;
      expressionP->X_add_number = 0;
    }
  else if (input_line_pointer[0] == '$')
    {
      char *s;
      char type;
      int fieldnum, fieldlimit;
      LITTLENUM_TYPE floatbuf[8];

      /* $float(), $doubleN(), or $extendN() convert floating values
         to integers.  */
      s = input_line_pointer;

      ++s;

      fieldnum = 0;
      if (strncmp (s, "double", sizeof "double" - 1) == 0)
        {
          s += sizeof "double" - 1;
          type = 'd';
          fieldlimit = 2;
        }
      else if (strncmp (s, "float", sizeof "float" - 1) == 0)
        {
          s += sizeof "float" - 1;
          type = 'f';
          fieldlimit = 1;
        }
      else if (strncmp (s, "extend", sizeof "extend" - 1) == 0)
        {
          s += sizeof "extend" - 1;
          type = 'x';
          fieldlimit = 4;
        }
      else
	return;

      if (ISDIGIT (*s))
        {
          fieldnum = *s - '0';
          ++s;
        }
      if (fieldnum >= fieldlimit)
        return;

      SKIP_WHITESPACE ();
      if (*s != '(')
        return;
      ++s;
      SKIP_WHITESPACE ();

      s = atof_ieee (s, type, floatbuf);
      if (s == NULL)
        return;
      s = s;

      SKIP_WHITESPACE ();
      if (*s != ')')
        return;
      ++s;
      SKIP_WHITESPACE ();

      input_line_pointer = s;
      expressionP->X_op = O_constant;
      expressionP->X_unsigned = 1;
      expressionP->X_add_number = ((floatbuf[fieldnum * 2]
                                    << LITTLENUM_NUMBER_OF_BITS)
                                   + floatbuf[fieldnum * 2 + 1]);
    }
}

/* Round up a section size to the appropriate boundary.  */

valueT
md_section_align (segT segment ATTRIBUTE_UNUSED, valueT size ATTRIBUTE_UNUSED)
{
  return size;      /* Byte alignment is fine.  */
}

/* Exactly what point is a PC-relative offset relative TO?
   On the 29000, they're relative to the address of the instruction,
   which we have set up as the address of the fixup too.  */

long
md_pcrel_from (fixS *fixP)
{
  return fixP->fx_where + fixP->fx_frag->fr_address;
}

/* Generate a reloc for a fixup.  */

arelent *
tc_gen_reloc (asection *seg ATTRIBUTE_UNUSED, fixS *fixp)
{
  arelent *reloc;

  reloc = xmalloc (sizeof (arelent));
  reloc->sym_ptr_ptr = xmalloc (sizeof (asymbol *));
  *reloc->sym_ptr_ptr = symbol_get_bfdsym (fixp->fx_addsy);
  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where;
  /*  reloc->address = fixp->fx_frag->fr_address + fixp->fx_where + fixp->fx_addnumber;*/
  reloc->howto = bfd_reloc_type_lookup (stdoutput, fixp->fx_r_type);

  if (reloc->howto == (reloc_howto_type *) NULL)
    {
      as_bad_where (fixp->fx_file, fixp->fx_line,
		    _("reloc %d not supported by object file format"),
		    (int) fixp->fx_r_type);
      return NULL;
    }

  if (fixp->fx_r_type == BFD_RELOC_VTABLE_ENTRY)
    reloc->address = fixp->fx_offset;

  reloc->addend = fixp->fx_addnumber;
  return reloc;
}
