/*
 * char_map.h
 *
 *  Created on: 12 dec 2017
 *      Author: Calle Melander
 *      TinyFont 8x8
 */

#ifndef CHAR_MAP_H_
#define CHAR_MAP_H_

#include <alt_types.h>

const alt_u32 font32upper[]={

		0x00000000, // <Space
		0x183C3C18, // !
		0x66662400, // "
		0x6C6CFE6C, // #
		0x183E603C, // $
		0x00C6CC18, // %
		0x386C3876, // &
		0x18183000, // '
		0x0C183030, // (
		0x30180C0C, // )
		0x00663CFF, // *
		0x0018187E, // +
		0x00000000, // ,
		0x0000007E, // -
		0x00000000, // .
		0x060C1830, // /
		0x7CC6CED6, // 0
		0x18381818, // 1
		0x7CC6061C, // 2
		0x7CC6063C, // 3
		0x1C3C6CCC, // 4
		0xFEC0C0FC, // 5
		0x3860C0FC, // 6
		0xFEC60C18, // 7
		0x7CC6C67C, // 8
		0x7CC6C67E, // 9
		0x00181800, // :
		0x00181800, // ;
		0x060C1830, // <
		0x00007E00, // =
		0x6030180C, // >
		0x7CC60C18, // ?
		0x7CC6DEDE, // @
		0x386CC6FE, // A
		0xFC66667C, // B
		0x3C66C0C0, // C
		0xF86C6666, // D
		0xFE626878, // E
		0xFE626878, // F
		0x3C66C0C0, // G
		0xC6C6C6FE, // H
		0x3C181818, // I
		0x1E0C0C0C, // J
		0xE6666C78, // K
		0xF0606060, // L
		0xC6EEFEFE, // M
		0xC6E6F6DE, // N
		0x7CC6C6C6, // O
		0xFC66667C, // P
		0x7CC6C6C6, // Q
		0xFC66667C, // R
		0x7CC66038, // S
		0x7E7E5A18, // T
		0xC6C6C6C6, // U
		0xC6C6C6C6, // V
		0xC6C6C6D6, // W
		0xC6C66C38, // X
		0x6666663C, // Y
		0xFEC68C18, // Z
		0x3C303030, // [
		0xC0603018, // <BackSpace
		0x3C0C0C0C, // ]
		0x10386CC6, // ^
		0x00000000, // _
		0x30180C00, // '
		0x0000780C, // a
		0xE0607C66, // b
		0x00007CC6, // c
		0x1C0C7CCC, // d
		0x00007CC6, // e
		0x3C6660F8, // f
		0x000076CC, // g
		0xE0606C76, // h
		0x18003818, // i
		0x06000606, // j
		0xE060666C, // k
		0x38181818, // l
		0x0000ECFE, // m
		0x0000DC66, // n
		0x00007CC6, // o
		0x0000DC66, // p
		0x000076CC, // q
		0x0000DC76, // r
		0x00007EC0, // s
		0x3030FC30, // t
		0x0000CCCC, // u
		0x0000C6C6, // v
		0x0000C6D6, // w
		0x0000C66C, // x
		0x0000C6C6, // y
		0x00007E4C, // z
		0x0E181870, // {
		0x18181818, // |
		0x7018180E, // }
		0x76DC0000, // ~
};
const alt_u32 font32lower[]={

		0x00000000, // <Space>
		0x18001800, // !
		0x00000000, // "
		0xFE6C6C00, // #
		0x067C1800, // $
		0x3066C600, // %
		0xDCCC7600, // &
		0x00000000, // '
		0x30180C00, // (
		0x0C183000, // )
		0x3C660000, // *
		0x18180000, // +
		0x00181830, // ,
		0x00000000, // -
		0x00181800, // .
		0x60C08000, // /
		0xE6C67C00, // 0
		0x18187E00, // 1
		0x3066FE00, // 2
		0x06C67C00, // 3
		0xFE0C1E00, // 4
		0x06C67C00, // 5
		0xC6C67C00, // 6
		0x30303000, // 7
		0xC6C67C00, // 8
		0x060C7800, // 9
		0x00181800, // :
		0x00181830, // ;
		0x180C0600, // <
		0x007E0000, // =
		0x18306000, // >
		0x18001800, // ?
		0xDEC07800, // @
		0xC6C6C600, // A
		0x6666FC00, // B
		0xC0663C00, // C
		0x666CF800, // D
		0x6862FE00, // E
		0x6860F000, // F
		0xCE663A00, // G
		0xC6C6C600, // H
		0x18183C00, // I
		0xCCCC7800, // J
		0x6C66E600, // K
		0x6266FE00, // L
		0xD6C6C600, // M
		0xCEC6C600, // N
		0xC6C67C00, // O
		0x6060F000, // P
		0xC6CE7C0E, // Q
		0x6C66E600, // R
		0x0CC67C00, // S
		0x18183C00, // T
		0xC6C67C00, // U
		0xC66C3800, // V
		0xD6FE6C00, // W
		0x6CC6C600, // X
		0x18183C00, // Y
		0x3266FE00, // Z
		0x30303C00, // [
		0x0C060200, // <Backslash>
		0x0C0C3C00, // ]
		0x00000000, // ^
		0x000000FF, // _
		0x00000000, // '
		0x7CCC7600, // a
		0x6666DC00, // b
		0xC0C67C00, // c
		0xCCCC7600, // d
		0xFEC07C00, // e
		0x6060F000, // f
		0xCC7C0CF8, // g
		0x6666E600, // h
		0x18183C00, // i
		0x0666663C, // j
		0x786CE600, // k
		0x18183C00, // l
		0xD6D6D600, // m
		0x66666600, // n
		0xC6C67C00, // o
		0x667C60F0, // p
		0xCC7C0C1E, // q
		0x6060F000, // r
		0x7C06FC00, // s
		0x30361C00, // t
		0xCCCC7600, // u
		0xC66C3800, // v
		0xD6FE6C00, // w
		0x386CC600, // x
		0xC67E06FC, // y
		0x18327E00, // z
		0x18180E00, // {
		0x18181800, // |
		0x18187000, // }
		0x00000000, // ~

};



alt_u32 upper_char_line(alt_u8 char_pos){
	return font32upper[char_pos];
};

alt_u32 lower_char_line(alt_u8 char_pos){
	return font32lower[char_pos];
};


#endif /* CHAR_MAP_H_ */
