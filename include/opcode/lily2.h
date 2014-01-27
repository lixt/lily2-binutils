/* Table of opcodes for the LILY2 ISA.
   Copyright(C) 2014 DSP Group, Institute of Microelectronics, Tsinghua University
   All rights reserved.

   Contributed by Xiaotian Li <lixiaotian07@gmail.com>.

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

/* We treat all letters the same in encode/decode routines so
   we need to assign some characteristics to them like signess etc.  */

#ifndef LILY2_H_ISA
#define LILY2_H_ISA

#define NUM_SIGNED (1)
#define NUM_UNSIGNED (0)

#define BASE_2  (2)
#define BASE_8  (8)
#define BASE_10 (10)
#define BASE_16 (16)

#define ADDR_MOD (1)

/* Attributes of encoding letters. */
struct lily2_letter
{
    char ch;
    int len;
    int sign;
};

struct lily2_opcode
{
    const char *name;

    /* A string of characters describing the operands of instructions.
       Valid characters are:
       rA   Register operand.
       rB   Register operand.
       rD   Register operand.
       dA   Register-Pair operand.
       dB   Register-Pair operand.
       dD   Register-Pair operand.
       qA   Register-Pair-Pair operand.
       qB   Register-Pair-Pair operand.
       qD   Register-Pair-Pair operand.
       rC   Miscellaneous register operand.
       rX   Cross register operand.
       dX   Cross register-pair operand.
       qX   Cross register-pair-pair operand.
       iG   An immediate operand, range 0 to 1.
       iH   An immediate operand, range 0 to 3.
       iI   An immediate operand, range -128 to 127.
       iJ   An immediate operand, range 0 to 255.
       iK   An immediate operand, range -16 to 15.
       iL   An immediate operand, range 0 to 31.
       iM   An immediate operand, range 0 to 65535.
       iN   An immediate operand, range -1048576 to 1048575.
       *    Address modification prefix operator.
       #    Address modification suffix operator.
       */
    char *args;

    /* A string of characters describing the encoding of instructions.
       Valid characters are:
       A   Register/Register-Pair/Register-Pair-Pair operand.
       B   Register/Register-pair/Register-pair-Pair operand.
       D   Register/Register-pair/Register-pair-Pair operand.
       C   Miscellaneous register operand.
       X   Across register/register-pair/register-pair-pair operand.
       G   An immediate operand, range 0 to 1.
       H   An immediate operand, range 0 to 3.
       I   An immediate operand, range -128 to 127.
       J   An immediate operand, range 0 to 255.
       K   An immediate operand, range -16 to 15.
       L   An immediate operand, range 0 to 31.
       M   An immediate operand, range 0 to 65535.
       N   An immediate operand, range -1048576 to 1048575.
       S   Address modification prefix or suffix operator(S stands for *).
       E   Cluster bitfield(used in gas).
       F   Functional unit bitfield(used in dis).
       Z   Condition bitfield.
       0   Bit 0.
       1   Bit 1.
       -   Reserved bit. */
    char *encoding;

    unsigned int flag;
};

/* lily2 cluster. */
struct lily2_cluster
{
    const char *name;
    unsigned long code;
};

/* lily2 functional unit. */
struct lily2_functional_unit
{
    const char *name;
    unsigned long code;

    struct lily2_cluster cluster;
    struct lily2_opcode *opcodes;
};

/* lily2 condition. */
struct lily2_condition
{
    const char *name;
    unsigned long code;
};

/* lily2 register. */
struct lily2_register
{
    const char *name;
    unsigned long code;
};

/* lily2 address modification sign. */
struct lily2_addr_mod_sign
{
    const char *name;
    unsigned long code;
};

/* All the tables beginning with ``OPC_'' are used in GAS.
   All the tables beginning with ``DIS_'' are used in DIS.
   Those without ``OPC_'' or ``DIS_'' are used in both. */

/* Table of LETTER. */
extern const struct lily2_letter lily2_letters[];
extern const size_t lily2_num_letters;

/* Table of OPC_OPCODE.
   OPC_OPCODES_A for opcode table of functional unit A.
   OPC_OPCODES_M for opcode table of functional unit M.
   OPC_OPCODES_D for opcode table of functional unit D. */
extern const struct lily2_opcode lily2_opc_opcodes_a[];
extern const size_t lily2_num_opc_opcodes_a;
extern const struct lily2_opcode lily2_opc_opcodes_m[];
extern const size_t lily2_num_opc_opcodes_m;
extern const struct lily2_opcode lily2_opc_opcodes_d[];
extern const size_t lily2_num_opc_opcodes_d;

/* Table of OPC_FUNCTIONAL_UNIT. */
extern const struct lily2_functional_unit lily2_opc_functional_units[];
extern const size_t lily2_num_opc_functional_units;

/* Table of OPC_CONDITION. */
extern const struct lily2_condition lily2_opc_conditions[];
extern const size_t lily2_num_opc_conditions;

/* Table of OPC_REGISTER.
   OPC_REGISTERS_X for general register table of cluster X.
   OPC_REGISTERS_Y for general register table of cluster Y.
   OPC_REGISTERS_M for miscellaneous register table.
   OPC_REGISTERS_C for condition register table. */
extern const struct lily2_register lily2_opc_registers_x[];
extern const size_t lily2_num_opc_registers_x;
extern const struct lily2_register lily2_opc_registers_y[];
extern const size_t lily2_num_opc_registers_y;
extern const struct lily2_register lily2_opc_registers_m[];
extern const size_t lily2_num_opc_registers_m;
extern const struct lily2_register lily2_opc_registers_c[];
extern const size_t lily2_num_opc_registers_c;

/* Table of OPC_REGISTER_PAIR.
   OPC_REGISTER_PAIRS_X for general register-pair of cluster X.
   OPC_REGISTER_PAIRS_Y for genreal register-pair of cluster Y.*/
extern const struct lily2_register lily2_opc_register_pairs_x[];
extern const size_t lily2_num_opc_register_pairs_x;
extern const struct lily2_register lily2_opc_register_pairs_y[];
extern const size_t lily2_num_opc_register_pairs_y;

/* Table of OPC_REGISTER_PAIR_PAIR.
   OPC_REGISTER_PAIR_PAIRS_X for general register-pair-pair of cluster X.
   OPC_REGISTER_PAIR_PAIRS_Y for general register-pair-pair of cluster Y.*/
extern const struct lily2_register lily2_opc_register_pair_pairs_x[];
extern const size_t lily2_num_opc_register_pair_pairs_x;
extern const struct lily2_register lily2_opc_register_pair_pairs_y[];
extern const size_t lily2_num_opc_register_pair_pairs_y;

/* Table of OPC_ADDR_MOD_SIGN. */
extern const struct lily2_addr_mod_sign lily2_opc_addr_mod_signs[];
extern const size_t lily2_num_opc_addr_mod_signs;

/* Table of DIS_OPCODE. */
extern const struct lily2_opcode lily2_dis_opcodes[];
extern const size_t lily2_num_dis_opcodes;

/* Table of DIS_FUNCTIONAL_UNIT. */
extern const struct lily2_functional_unit lily2_dis_functional_units[];
extern const size_t lily2_num_dis_functional_units;

/* Table of DIS_CONDITION. */
extern const struct lily2_condition lily2_dis_conditions[];
extern const size_t lily2_num_dis_conditions;

/* Table of DIS_REGISTER. */
extern const struct lily2_register lily2_dis_registers[];
extern const size_t lily2_num_dis_registers;

/* Table of DIS_REGISTER_PAIR. */
extern const struct lily2_register lily2_dis_register_pairs[];
extern const size_t lily2_num_dis_register_pairs;

/* Table of DIS_REGISTER_PAIR_PAIR. */
extern const struct lily2_register lily2_dis_register_pair_pairs[];
extern const size_t lily2_num_dis_register_pair_pairs;

/* Table of DIS_ADDR_MOD_SIGN_PREFIX. */
/* Table of DIS_ADDR_MOD_SIGN_SUFFIX. */
extern const struct lily2_addr_mod_sign lily2_dis_addr_mod_sign_prefix[];
extern const size_t lily2_num_dis_addr_mod_sign_prefix;
extern const struct lily2_addr_mod_sign lily2_dis_addr_mod_sign_suffix[];
extern const size_t lily2_num_dis_addr_mod_sign_suffix;

/* Letter table isn't put into the hash table and it will be used by
   both opc and dis. Therefore we list letter_find functions alone
   for convenience. */
extern struct lily2_letter *letter_find (char);

/* Generates a NBITS mask. */
extern unsigned long mask (int nbits);

/* Extracts FIRST to LAST bits of INSN and puts them in the LSBs. */
extern unsigned long bits (unsigned long insn, int first, int last);

/* Replaces FIRST to LAST bits of OLD_INSN with NEW_INSN. */
extern unsigned long
replace_bits (unsigned long old_insn, int first, int last, unsigned long new_insn);

extern unsigned long
sign_extend (unsigned long insn, int nbits, int sign);

/* Checks whether INSN is overflow according to the NBITS and SIGN.
   SIGN = 0 represents positive value. Returns 0 if bits (MSB) to (nbits)
   are all 0s to represents not overflow.
   SIGN = 1 represents negative value. Returns 0 if bits (MSB) to (nbits)
   are all 1s to represents not overflow. */
extern int check_overflow (unsigned long insn, int nbits, int sign);

#endif
