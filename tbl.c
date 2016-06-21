/*
 * dis6502 by Robert Bond, Udi Finkelstein, and Eric Smith
 *
 * $Id: tbl.c 26 2004-01-17 23:28:23Z eric $
 * Copyright 2001-2014 Eric Smith <eric@brouhaha.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.  Note that permission is
 * not granted to redistribute this program under the terms of any
 * other version of the General Public License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 */


#include <stdint.h>
#include <stdio.h>

#include "dis.h"

struct info optbl[256] = {
	[0x00] = { "brk", 1, IMP|JUMP },
	[0x01] = { "ora", 2, INX },

	[0x05] = { "ora", 2, ZPG },
	[0x06] = { "asl", 2, ZPG },

	[0x08] = { "php", 1, IMP },
	[0x09] = { "ora", 2, IMM },
	[0x0a] = { "asl", 1, ACC },

	[0x0d] = { "ora", 3, ABS },
	[0x0e] = { "asl", 3, ABS },

	[0x10] = { "bpl", 2, REL|FORK },
	[0x11] = { "ora", 2, INY },

	[0x15] = { "ora", 2, ZPX },
	[0x16] = { "asl", 2, ZPX },

	[0x18] = { "clc", 1, IMP },
	[0x19] = { "ora", 3, ABY },

	[0x1d] = { "ora", 3, ABX },
	[0x1e] = { "asl", 3, ABX },

	[0x20] = { "jsr", 3, ABS|FORK },
	[0x21] = { "and", 2, INX },

	[0x24] = { "bit", 2, ZPG },
	[0x25] = { "and", 2, ZPG },
	[0x26] = { "rol", 2, ZPG },

	[0x28] = { "plp", 1, IMP },
	[0x29] = { "and", 2, IMM },
	[0x2a] = { "rol", 1, ACC },

	[0x2c] = { "bit", 3, ABS },
	[0x2d] = { "and", 3, ABS },
	[0x2e] = { "rol", 3, ABS },

	[0x30] = { "bmi", 2, REL|FORK },
	[0x31] = { "and", 2, INY },

	[0x35] = { "and", 2, ZPX },
	[0x36] = { "rol", 2, ZPX },

	[0x38] = { "sec", 1, IMP },
	[0x39] = { "and", 3, ABY },

	[0x3d] = { "and", 3, ABX },
	[0x3e] = { "rol", 3, ABX },

	[0x40] = { "rti", 1, IMP|STOP },
	[0x41] = { "eor", 2, INX },

	[0x45] = { "eor", 2, ZPG },

        [0x48] = { "pha", 1, IMP },
	[0x49] = { "eor", 2, IMM },

	[0x4a] = { "lsr", 1, ACC },

	[0x4c] = { "jmp", 3, ABS|JUMP },
	[0x4d] = { "eor", 3, ABS },
	[0x4e] = { "lsr", 3, ABS },

	[0x50] = { "bvc", 2, REL|FORK },
	[0x51] = { "eor", 2, INY },

	[0x55] = { "eor", 2, ZPX },
	[0x56] = { "lsr", 2, ZPX },

	[0x58] = { "cli", 1, IMP },
	[0x59] = { "eor", 3, ABY },

	[0x5d] = { "eor", 3, ABX },
	[0x5e] = { "lsr", 3, ABX },

	[0x60] = { "rts", 1, IMP|STOP },
	[0x61] = { "adc", 2, INX },

	[0x65] = { "adc", 2, ZPG },
	[0x66] = { "ror", 2, ZPG },

	[0x68] = { "pla", 1, IMP },
	[0x69] = { "adc", 2, IMM },
	[0x6a] = { "ror", 1, ACC },

	[0x6c] = { "jmp", 3, IND|STOP },
	[0x6d] = { "adc", 3, ABS },
	[0x6e] = { "ror", 3, ABS },

	[0x70] = { "bvs", 2, REL|FORK },
	[0x71] = { "adc", 2, INY },

	[0x75] = { "adc", 2, ZPX },
	[0x76] = { "ror", 2, ZPX },

	[0x78] = { "sei", 1, IMP },
	[0x79] = { "adc", 3, ABY },

	[0x7d] = { "adc", 3, ABX },
	[0x7e] = { "ror", 3, ABX },

	[0x81] = { "sta", 2, INX },

	[0x84] = { "sty", 2, ZPG },
	[0x85] = { "sta", 2, ZPG },
	[0x86] = { "stx", 2, ZPG },

	[0x88] = { "dey", 1, IMP },

	[0x8a] = { "txa", 1, IMP },

	[0x8c] = { "sty", 3, ABS },
	[0x8d] = { "sta", 3, ABS },
	[0x8e] = { "stx", 3, ABS },

	[0x90] = { "bcc", 2, REL|FORK },
	[0x91] = { "sta", 2, INY },

	[0x94] = { "sty", 2, ZPX },
	[0x95] = { "sta", 2, ZPX },
	[0x96] = { "stx", 2, ZPY },

	[0x98] = { "tya", 1, IMP },
	[0x99] = { "sta", 3, ABY },
	[0x9a] = { "txs", 1, IMP },

	[0x9d] = { "sta", 3, ABX },

	[0xa0] = { "ldy", 2, IMM },
	[0xa1] = { "lda", 2, INX },
	[0xa2] = { "ldx", 2, IMM },

	[0xa4] = { "ldy", 2, ZPG },
	[0xa5] = { "lda", 2, ZPG },
	[0xa6] = { "ldx", 2, ZPG },

	[0xa8] = { "tay", 1, IMP },
	[0xa9] = { "lda", 2, IMM },
	[0xaa] = { "tax", 1, IMP },

	[0xac] = { "ldy", 3, ABS },
	[0xad] = { "lda", 3, ABS },
	[0xae] = { "ldx", 3, ABS },

	[0xb0] = { "bcs", 2, REL|FORK },
	[0xb1] = { "lda", 2, INY },

	[0xb4] = { "ldy", 2, ZPX },
	[0xb5] = { "lda", 2, ZPX },
	[0xb6] = { "ldx", 2, ZPY },

	[0xb8] = { "clv", 1, IMP },
	[0xb9] = { "lda", 3, ABY },
	[0xba] = { "tsx", 1, IMP },

	[0xbc] = { "ldy", 3, ABX },
	[0xbd] = { "lda", 3, ABX },
	[0xbe] = { "ldx", 3, ABY },

	[0xc0] = { "cpy", 2, IMM },
	[0xc1] = { "cmp", 2, INX },

	[0xc4] = { "cpy", 2, ZPG },
	[0xc5] = { "cmp", 2, ZPG },
	[0xc6] = { "dec", 2, ZPG },

	[0xc8] = { "iny", 1, IMP },
	[0xc9] = { "cmp", 2, IMM },
	[0xca] = { "dex", 1, IMP },

	[0xcc] = { "cpy", 3, ABS },
	[0xcd] = { "cmp", 3, ABS },
	[0xce] = { "dec", 3, ABS },

	[0xd0] = { "bne", 2, REL|FORK },
	[0xd1] = { "cmp", 2, INY },

	[0xd5] = { "cmp", 2, ZPX },
	[0xd6] = { "dec", 2, ZPX },

	[0xd8] = { "cld", 1, IMP },
	[0xd9] = { "cmp", 3, ABY },

	[0xdd] = { "cmp", 3, ABX },
	[0xde] = { "dec", 3, ABX },

	[0xe0] = { "cpx", 2, IMM },
	[0xe1] = { "sbc", 2, INX },

	[0xe4] = { "cpx", 2, ZPG },
	[0xe5] = { "sbc", 2, ZPG },
	[0xe6] = { "inc", 2, ZPG },

	[0xe8] = { "inx", 1, IMP },
	[0xe9] = { "sbc", 2, IMM },
	[0xea] = { "nop", 1, IMP },

	[0xec] = { "cpx", 3, ABS },
	[0xed] = { "sbc", 3, ABS },
	[0xee] = { "inc", 3, ABS },

	[0xf0] = { "beq", 2, REL|FORK },
	[0xf1] = { "sbc", 2, INY },

	[0xf5] = { "sbc", 2, ZPX },
	[0xf6] = { "inc", 2, ZPX },

	[0xf8] = { "sed", 1, IMP },
	[0xf9] = { "sbc", 3, ABY },

	[0xfd] = { "sbc", 3, ABX },
	[0xfe] = { "inc", 3, ABX },

#if 1
// 65C02
	[0x04] = { "tsb", 2, ZPG },
	[0x0c] = { "tsb", 3, ABS },
	[0x12] = { "ora", 2, INZ },
	[0x14] = { "trb", 2, ZPG },
	[0x1a] = { "inc", 1, ACC },
	[0x1c] = { "trb", 3, ABS },
	[0x32] = { "and", 2, INZ },
	[0x34] = { "bit", 2, ZPX },
	[0x3a] = { "dec", 1, ACC },
	[0x3c] = { "bit", 3, ABX },
	[0x52] = { "eor", 2, INZ },
	[0x5a] = { "phy", 1, IMP },
	[0x64] = { "stz", 2, ZPG },
	[0x72] = { "adc", 2, INZ },
	[0x74] = { "stz", 2, ZPX },
	[0x7a] = { "ply", 1, IMP },
	[0x7c] = { "jmp", 3, ABX|STOP },
	[0x80] = { "bra", 2, REL|JUMP },
	[0x89] = { "bit", 2, IMM },
	[0x92] = { "sta", 2, INZ },
	[0x9c] = { "stz", 3, ABS },
	[0x9e] = { "stz", 3, ABX },
	[0xb2] = { "lda", 2, INZ },
	[0xd2] = { "cmp", 2, INZ },
	[0xda] = { "phx", 1, IMP },
	[0xf2] = { "sbc", 2, INZ },
	[0xfa] = { "plx", 1, IMP },
#endif

#if 0
// Rockwell bit instructions
#endif
};
