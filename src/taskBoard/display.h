/*   display.h - Number display driver   */
/*   Yuster, Liangyu Chen   */

#pragma once

#include <cstdint>

enum text_height {
    single_line,
    double_line,
    triple_line,
    quad_line
};

static constexpr enum text_height header_height = text_height::single_line;
static constexpr enum text_height value_height = text_height::double_line;

#define HEADER_START_LINE   0
#define VALUE_START_LINE    2
#define UNIT_POSITION       110

#define C0507(ch) ((ch) >= 'A' && (ch) <= 'Z')?cs0507_up[(ch)-'A']:((ch) >= 'a' && (ch) <= 'z')?cs0507_low[(ch)-'a']:s0507[0]
#define C1014(ch) ((ch) >= 'A' && (ch) <= 'Z')?cs1014_up[(ch)-'A']:((ch) >= 'a' && (ch) <= 'z')?cs1014_up[(ch)-'a']:s1014[0]
#define C1521(ch) ((ch) >= 'A' && (ch) <= 'Z')?cs1521_up[(ch)-'A']:((ch) >= 'a' && (ch) <= 'z')?cs1521_up[(ch)-'a']:s1521[0]
#define N0507(num) (num < 10)?nums0507[num]:s0507[0]
#define N1014(num) (num < 10)?nums1014[num]:s1014[0]
#define N1521(num) (num < 10)?nums1521[num]:s1521[0]
#define N2028(num) (num < 10)?nums2028[num]:s2028[0]

struct font0507 {
    const uint8_t col[5];
};

struct font1014 {
    const uint16_t col[10];
};

struct font1521 {
    const uint32_t col[15];
};

struct font2028 {
    const uint32_t col[20];
};

#define c0507_UNKN {{0x7F, 0x41, 0x41, 0x41, 0x7F}}

// Numbers
#define n0507_0 {{0x3E, 0x51, 0x49, 0x45, 0x3E}}
#define n0507_1 {{0x00, 0x42, 0x7F, 0x40, 0x00}}
#define n0507_2 {{0x42, 0x61, 0x51, 0x49, 0x46}}
#define n0507_3 {{0x22, 0x41, 0x49, 0x49, 0x36}}
#define n0507_4 {{0x18, 0x14, 0x12, 0x7F, 0x10}}
#define n0507_5 {{0x27, 0x45, 0x45, 0x45, 0x39}}
#define n0507_6 {{0x3E, 0x49, 0x49, 0x49, 0x32}}
#define n0507_7 {{0x01, 0x71, 0x09, 0x05, 0x03}}
#define n0507_8 {{0x36, 0x49, 0x49, 0x49, 0x36}}
#define n0507_9 {{0x26, 0x49, 0x49, 0x49, 0x3E}}

// Upper case characters
#define c0507_A {{0x7C, 0x12, 0x11, 0x12, 0x7C}}
#define c0507_B {{0x41, 0x7F, 0x49, 0x49, 0x36}}
#define c0507_C {{0x3E, 0x41, 0x41, 0x41, 0x22}}
#define c0507_D {{0x41, 0x7F, 0x41, 0x41, 0x3E}}
#define c0507_E {{0x7F, 0x49, 0x49, 0x49, 0x41}}
#define c0507_F {{0x7F, 0x09, 0x09, 0x09, 0x01}}
#define c0507_G {{0x3E, 0x41, 0x41, 0x49, 0x7A}}
#define c0507_H {{0x7F, 0x08, 0x08, 0x08, 0x7F}}
#define c0507_I {{0x00, 0x41, 0x7F, 0x41, 0x00}}
#define c0507_J {{0x20, 0x40, 0x41, 0x3F, 0x01}}
#define c0507_K {{0x7F, 0x08, 0x14, 0x22, 0x41}}
#define c0507_L {{0x7F, 0x40, 0x40, 0x40, 0x40}}
#define c0507_M {{0x7F, 0x02, 0x0C, 0x02, 0x7F}}
#define c0507_N {{0x7F, 0x04, 0x08, 0x10, 0x7F}}
#define c0507_O {{0x3E, 0x41, 0x41, 0x41, 0x3E}}
#define c0507_P {{0x7F, 0x09, 0x09, 0x09, 0x06}}
#define c0507_Q {{0x3E, 0x41, 0x51, 0x21, 0x5E}}
#define c0507_R {{0x7F, 0x09, 0x19, 0x29, 0x46}}
#define c0507_S {{0x26, 0x49, 0x49, 0x49, 0x32}}
#define c0507_T {{0x01, 0x01, 0x7F, 0x01, 0x01}}
#define c0507_U {{0x3F, 0x40, 0x40, 0x40, 0x3F}}
#define c0507_V {{0x1F, 0x20, 0x40, 0x20, 0x1F}}
#define c0507_W {{0x3F, 0x40, 0x38, 0x40, 0x3F}}
#define c0507_X {{0x63, 0x14, 0x08, 0x14, 0x63}}
#define c0507_Y {{0x07, 0x08, 0x70, 0x08, 0x07}}
#define c0507_Z {{0x61, 0x51, 0x49, 0x45, 0x43}}

// Lower case characters
#define c0507_a {{0x20, 0x54, 0x54, 0x54, 0x78}}
#define c0507_b {{0x7F, 0x48, 0x44, 0x44, 0x38}}
#define c0507_c {{0x38, 0x44, 0x44, 0x44, 0x20}}
#define c0507_d {{0x38, 0x44, 0x44, 0x48, 0x7F}}
#define c0507_e {{0x38, 0x54, 0x54, 0x54, 0x18}}
#define c0507_f {{0x08, 0x7E, 0x09, 0x01, 0x02}}
#define c0507_g {{0x08, 0x54, 0x54, 0x54, 0x3C}}
#define c0507_h {{0x7F, 0x08, 0x04, 0x04, 0x78}}
#define c0507_i {{0x00, 0x48, 0x7D, 0x40, 0x00}}
#define c0507_j {{0x20, 0x40, 0x44, 0x3D, 0x00}}
#define c0507_k {{0x7F, 0x10, 0x28, 0x44, 0x00}}
#define c0507_l {{0x00, 0x41, 0x7F, 0x40, 0x00}}
#define c0507_m {{0x7C, 0x04, 0x78, 0x04, 0x78}}
#define c0507_n {{0x7C, 0x08, 0x04, 0x04, 0x78}}
#define c0507_o {{0x38, 0x44, 0x44, 0x44, 0x38}}
#define c0507_p {{0x7C, 0x14, 0x14, 0x14, 0x08}}
#define c0507_q {{0x08, 0x14, 0x14, 0x18, 0x7C}}
#define c0507_r {{0x7C, 0x08, 0x04, 0x04, 0x08}}
#define c0507_s {{0x48, 0x54, 0x54, 0x54, 0x20}}
#define c0507_t {{0x04, 0x3F, 0x44, 0x40, 0x20}}
#define c0507_u {{0x3C, 0x40, 0x40, 0x20, 0x7C}}
#define c0507_v {{0x1C, 0x20, 0x40, 0x20, 0x1C}}
#define c0507_w {{0x3C, 0x40, 0x30, 0x40, 0x3C}}
#define c0507_x {{0x44, 0x28, 0x10, 0x28, 0x44}}
#define c0507_y {{0x0C, 0x50, 0x50, 0x50, 0x3C}}
#define c0507_z {{0x44, 0x64, 0x54, 0x4C, 0x44}}

// Special characters
#define s0507_1  {{0x00, 0x00, 0x00, 0x00, 0x00}}  // space
#define s0507_2  {{0x00, 0x60, 0x60, 0x00, 0x00}} // period
#define s0507_3  {{0x00, 0xA0, 0x60, 0x00, 0x00}} // comma
#define s0507_4  {{0x00, 0x36, 0x36, 0x00, 0x00}} // colon
#define s0507_5  {{0x00, 0x56, 0x36, 0x00, 0x00}} // semicolon
#define s0507_6  {{0x00, 0x08, 0x08, 0x08, 0x00}} // minus
#define s0507_7  {{0x08, 0x08, 0x3E, 0x08, 0x08}} // plus big
#define s0507_8  {{0x00, 0x08, 0x1C, 0x08, 0x00}} // plus
#define s0507_9  {{0x40, 0x40, 0x40, 0x40, 0x40}} // underscore
#define s0507_10 {{0x00, 0x00, 0x02, 0x05, 0x02}} // degree
#define s0507_11 {{0x23, 0x13, 0x08, 0x64, 0x62}} // percent
#define s0507_12 {{0x20, 0x10, 0x08, 0x04, 0x02}} // slash
#define s0507_13 {{0x02, 0x01, 0x51, 0x09, 0x06}} // question
#define s0507_14 {{0x00, 0x22, 0x14, 0x08, 0x00}} // >
#define s0507_15 {{0x00, 0x08, 0x14, 0x22, 0x00}} // <

// 
#define c1014_UNKN {{0x7FFE, 0x4002, 0x4002, 0x4002, 0x4002, 0x4002, 0x4002, 0x4002, 0x4002, 0x7FFE}} // unknown

#define n1014_0 {{0x1FF8, 0x3FFC, 0x760E, 0x6706, 0x6386, 0x61C6, 0x60E6, 0x706E, 0x3FFC, 0x1FF8}}
#define n1014_1 {{0x0000, 0x0000, 0x6018, 0x601C, 0x7FFE, 0x7FFE, 0x6000, 0x6000, 0x0000, 0x0000}}
#define n1014_2 {{0x6018, 0x701C, 0x780E, 0x7C06, 0x6E06, 0x6706, 0x6386, 0x61CE, 0x60FC, 0x6078}}
#define n1014_3 {{0x1818, 0x381C, 0x700E, 0x6006, 0x6186, 0x6186, 0x6186, 0x73CE, 0x3FFC, 0x1E78}}
#define n1014_4 {{0x0780, 0x07C0, 0x06E0, 0x0670, 0x0638, 0x061C, 0x7FFE, 0x7FFE, 0x0600, 0x0600}}
#define n1014_5 {{0x187E, 0x387E, 0x7066, 0x6066, 0x6066, 0x6066, 0x6066, 0x70E6, 0x3FC6, 0x1F86}}
#define n1014_6 {{0x1FF8, 0x3FFC, 0x718E, 0x6186, 0x6186, 0x6186, 0x6186, 0x738E, 0x3F1C, 0x1E18}}
#define n1014_7 {{0x0006, 0x0006, 0x7E06, 0x7F06, 0x0386, 0x01C6, 0x00E6, 0x0076, 0x003E, 0x001E}}
#define n1014_8 {{0x1E78, 0x3FFC, 0x73CE, 0x6186, 0x6186, 0x6186, 0x6186, 0x73CE, 0x3FFC, 0x1E78}}
#define n1014_9 {{0x1878, 0x38FC, 0x71CE, 0x6186, 0x6186, 0x6186, 0x6186, 0x718E, 0x3FFC, 0x1FF8}}

// Upper case characters
#define c1014_A {{0x7FE0, 0x7FF0, 0x0638, 0x061C, 0x060E, 0x060E, 0x061C, 0x0638, 0x7FF0, 0x7FE0}}
#define c1014_B {{0x6006, 0x6006, 0x7FFE, 0x7FFE, 0x6186, 0x6186, 0x6186, 0x73CE, 0x3FFC, 0x1E78}}
#define c1014_C {{0x1FF8, 0x3FFC, 0x700E, 0x6006, 0x6006, 0x6006, 0x6006, 0x700E, 0x381C, 0x1818}}
#define c1014_D {{0x6006, 0x6006, 0x7FFE, 0x7FFE, 0x6006, 0x6006, 0x6006, 0x700E, 0x3FFC, 0x1FF8}}
#define c1014_E {{0x7FFE, 0x7FFE, 0x6186, 0x6186, 0x6186, 0x6186, 0x6186, 0x6186, 0x6006, 0x6006}}
#define c1014_F {{0x7FFE, 0x7FFE, 0x0186, 0x0186, 0x0186, 0x0186, 0x0186, 0x0186, 0x0006, 0x0006}}
#define c1014_G {{0x1FF8, 0x3FFC, 0x700E, 0x6006, 0x6006, 0x6006, 0x6186, 0x618E, 0x7F9C, 0x7F98}}
#define c1014_H {{0x7FFE, 0x7FFE, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x7FFE, 0x7FFE}}
#define c1014_I {{0x0000, 0x0000, 0x6006, 0x6006, 0x7FFE, 0x7FFE, 0x6006, 0x6006, 0x0000, 0x0000}}
#define c1014_J {{0x1800, 0x3800, 0x7000, 0x6000, 0x6006, 0x7006, 0x3FFE, 0x1FFE, 0x0006, 0x0006}}
#define c1014_K {{0x7FFE, 0x7FFE, 0x0180, 0x03C0, 0x07E0, 0x0E70, 0x1C38, 0x381C, 0x700E, 0x6006}}
#define c1014_L {{0x7FFE, 0x7FFE, 0x6000, 0x6000, 0x6000, 0x6000, 0x6000, 0x6000, 0x6000, 0x6000}}
#define c1014_M {{0x7FFE, 0x7FFE, 0x0018, 0x0018, 0x01E0, 0x01E0, 0x0018, 0x0018, 0x7FFE, 0x7FFE}}
#define c1014_N {{0x7FFE, 0x7FFE, 0x0060, 0x0060, 0x0180, 0x0180, 0x0600, 0x0600, 0x7FFE, 0x7FFE}}
#define c1014_O {{0x1FF8, 0x3FFC, 0x700E, 0x6006, 0x6006, 0x6006, 0x6006, 0x700E, 0x3FFC, 0x1FF8}}
#define c1014_P {{0x7FFE, 0x7FFE, 0x0186, 0x0186, 0x0186, 0x0186, 0x0186, 0x01CE, 0x00FC, 0x0078}}
#define c1014_Q {{0x1FF8, 0x3FFC, 0x700E, 0x6006, 0x6606, 0x6E06, 0x5C06, 0x380E, 0x77FC, 0x6FF8}}
#define c1014_R {{0x7FFE, 0x7FFE, 0x0186, 0x0386, 0x0786, 0x0F86, 0x1D86, 0x39CE, 0x70FC, 0x6078}}
#define c1014_S {{0x1878, 0x38FC, 0x71CE, 0x6186, 0x6186, 0x6186, 0x6186, 0x738E, 0x3F1C, 0x1E18}}
#define c1014_T {{0x0006, 0x0006, 0x0006, 0x0006, 0x7FFE, 0x7FFE, 0x0006, 0x0006, 0x0006, 0x0006}}
#define c1014_U {{0x1FFE, 0x3FFE, 0x7000, 0x6000, 0x6000, 0x6000, 0x6000, 0x7000, 0x3FFE, 0x1FFE}}
#define c1014_V {{0x07FE, 0x0FFE, 0x1C00, 0x3800, 0x7000, 0x7000, 0x3800, 0x1C00, 0x0FFE, 0x07FE}}
#define c1014_W {{0x1FFE, 0x3FFE, 0x7000, 0x7000, 0x3F80, 0x3F80, 0x7000, 0x7000, 0x3FFE, 0x1FFE}}
#define c1014_X {{0x781E, 0x7C3E, 0x0E70, 0x07E0, 0x03C0, 0x03C0, 0x07E0, 0x0E70, 0x7C3E, 0x781E}}
#define c1014_Y {{0x001E, 0x003E, 0x0070, 0x00E0, 0x7FC0, 0x7FC0, 0x00E0, 0x0070, 0x003E, 0x001E}}
#define c1014_Z {{0x7806, 0x7C06, 0x6E06, 0x6706, 0x6386, 0x61C6, 0x60E6, 0x6076, 0x603E, 0x601E}}

#define s1014_1 {{0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000}}  // space
#define s1014_2 {{0x0000, 0x0000, 0x3C00, 0x3C00, 0x3C00, 0x3C00, 0x0000, 0x0000, 0x0000, 0x0000}}  // period
#define s1014_3 {{0x0000, 0x0000, 0xCC00, 0xEC00, 0x7C00, 0x3C00, 0x0000, 0x0000, 0x0000, 0x0000}}  // comma
#define s1014_4 {{0x0000, 0x0000, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0180, 0x0000, 0x0000}}  // dash
#define s1014_5 {{0x0000, 0x0000, 0x1818, 0x1C38, 0x0E70, 0x07E0, 0x03C0, 0x0180, 0x0000, 0x0000}}  // >
#define s1014_6 {{0x0000, 0x0000, 0x0180, 0x03C0, 0x07E0, 0x0E70, 0x1C38, 0x1818, 0x0000, 0x0000}}  // <

// 
#define c1521_UNKN {{0x7FFFFC, 0x7FFFFC, 0x60000C, 0x60000C, 0x60000C, 0x60000C, 0x60000C, 0x60000C, 0x60000C, 0x60000C,0x60000C, 0x60000C, 0x60000C, 0x7FFFFC, 0x7FFFFC}} // unknown

#define n1521_0 {{0x0FFFE0, 0x1FFFF0, 0x3FFFF8, 0x7FC03C, 0x73E01C, 0x71F01C, 0x70F81C, 0x707C1C, 0x703E1C, 0x701F1C, 0x700F9C, 0x7807FC, 0x3FFFF8, 0x1FFFF0, 0x0FFFE0}}
#define n1521_1 {{0x000000, 0x000000, 0x000000, 0x7000E0, 0x7000E0, 0x7000F0, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x700000, 0x700000, 0x700000, 0x000000, 0x000000, 0x000000}}
#define n1521_2 {{0x7000E0, 0x7800F0, 0x7C00F8, 0x7E003C, 0x7F001C, 0x77801C, 0x73C01C, 0x71E01C, 0x70F01C, 0x70781C, 0x703C1C, 0x701E3C, 0x700FF8, 0x7007F0, 0x7003E0}}
#define n1521_3 {{0x0E00E0, 0x1E00F0, 0x3E00F8, 0x78003C, 0x70001C, 0x70001C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x787C3C, 0x3FFFF8, 0x1FEFF0, 0x0FC7E0}}
#define n1521_4 {{0x01F800, 0x01FC00, 0x01FE00, 0x01CF00, 0x01C780, 0x01C3C0, 0x01C1E0, 0x01C0F0, 0x01C078, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x01C000, 0x01C000, 0x01C000}}
#define n1521_5 {{0x0E07FC, 0x1E07FC, 0x3E07FC, 0x7C071C, 0x78071C, 0x70071C, 0x70071C, 0x70071C, 0x70071C, 0x70071C, 0x780F1C, 0x7C1F1C, 0x3FFE1C, 0x1FFC1C, 0x0FF81C}}
#define n1521_6 {{0x0FFF00, 0x1FFF80, 0x3FFFC0, 0x7CFBE0, 0x7879F0, 0x7038F8, 0x70387C, 0x70383C, 0x70381C, 0x70381C, 0x78781C, 0x7CF81C, 0x3FF000, 0x1FE000, 0x0FC000}}
#define n1521_7 {{0x00001C, 0x00001C, 0x00001C, 0x7F801C, 0x7FC01C, 0x7FE01C, 0x00F01C, 0x00781C, 0x003C1C, 0x001E1C, 0x000F1C, 0x00079C, 0x0003FC, 0x0001FC, 0x0000FC}}
#define n1521_8 {{0x0FC7E0, 0x1FEFF0, 0x3FFFF8, 0x787C3C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x787C3C, 0x3FFFF8, 0x1FEFF0, 0x0FC7E0}}
#define n1521_9 {{0x0007E0, 0x000FF0, 0x001FF8, 0x703E7C, 0x703C3C, 0x70381C, 0x70381C, 0x78381C, 0x7C381C, 0x3E381C, 0x1F3C3C, 0x0FBE7C, 0x07FFF8, 0x03FFF0, 0x01FFE0}}

#define c1521_A {{0x7FFF00, 0x7FFF80, 0x7FFFC0, 0x01C3E0, 0x01C1F0, 0x01C0F8, 0x01C07C, 0x01C03C, 0x01C07C, 0x01C0F8, 0x01C1F0, 0x01C3E0, 0x7FFFC0, 0x7FFF80, 0x7FFF00}}
#define c1521_B {{0x70001C, 0x70001C, 0x70001C, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x787C3C, 0x7CFE7C, 0x3FFFF8, 0x1FEFF0, 0x0FC7E0}}
#define c1521_C {{0x0FFFE0, 0x1FFFF0, 0x3FFFF8, 0x7C007C, 0x78003C, 0x70001C, 0x70001C, 0x70001C, 0x70001C, 0x70001C, 0x78003C, 0x7C007C, 0x3E00F8, 0x1E00F0, 0x0E00E0}}
#define c1521_D {{0x70001C, 0x70001C, 0x70001C, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x70001C, 0x70001C, 0x70001C, 0x70001C, 0x78003C, 0x7C007C, 0x3FFFF8, 0x1FFFF0, 0x0FFFE0}}
#define c1521_E {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70001C, 0x70001C, 0x70001C}}
#define c1521_F {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00001C, 0x00001C, 0x00001C}}
#define c1521_I {{0x000000, 0x000000, 0x000000, 0x70001C, 0x70001C, 0x70001C, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x70001C, 0x70001C, 0x70001C, 0x000000, 0x000000, 0x000000}}
#define c1521_K {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x007C00, 0x007C00, 0x00FE00, 0x01FF00, 0x03EF80, 0x07C7C0, 0x0F83E0, 0x1F01F0, 0x3E00F8, 0x7C007C, 0x78003C, 0x70001C}}
#define c1521_L {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000, 0x700000}}
#define c1521_N {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x0007C0, 0x000F80, 0x001F00, 0x003E00, 0x007C00, 0x00F800, 0x01F000, 0x03E000, 0x07C000, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC}}
#define c1521_O {{0x0FFFE0, 0x1FFFF0, 0x3FFFF8, 0x7C007C, 0x78003C, 0x70001C, 0x70001C, 0x70001C, 0x70001C, 0x70001C, 0x78003C, 0x7C007C, 0x3FFFF8, 0x1FFFF0, 0x0FFFE0}}
#define c1521_P {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x00381C, 0x003C3C, 0x003E7C, 0x001FF8, 0x000FF0, 0x0007E0}}
#define c1521_R {{0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x00381C, 0x00781C, 0x00F81C, 0x01F81C, 0x03F81C, 0x07F81C, 0x0FB81C, 0x1F3C3C, 0x3E3E7C, 0x7C1FF8, 0x780FF0, 0x7007E0}}
#define c1521_S {{0x0E07E0, 0x1E0FF0, 0x3E1FF8, 0x7C3E7C, 0x783C3C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x70381C, 0x78783C, 0x7CF87C, 0x3FF0F8, 0x1FE0F0, 0x0FC0E0}}
#define c1521_Y {{0x0007FC, 0x000FFC, 0x001FFC, 0x003E00, 0x007C00, 0x00F800, 0x7FF000, 0x7FE000, 0x7FF000, 0x00F800, 0x007C00, 0x003E00, 0x001FFC, 0x000FFC, 0x0007FC}}
#define n1521_T {{0x00001C, 0x00001C, 0x00001C, 0x00001C, 0x00001C, 0x00001C, 0x7FFFFC, 0x7FFFFC, 0x7FFFFC, 0x00001C, 0x00001C, 0x00001C, 0x00001C, 0x00001C, 0x00001C}}

#define s1521_1 {{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000}}    // space
#define s1521_2 {{0x000000, 0x000000, 0x000000, 0x180000, 0x3C0000, 0x7E0000, 0x7E0000, 0x3C0000, 0x180000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000}}    // period
#define s1521_3 {{0x000000, 0x000000, 0x000000, 0xC30000, 0xE78000, 0xF78000, 0x7F8000, 0x3F8000, 0x1F0000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000}}    // comma
#define s1521_4 {{0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000, 0x0F0F00, 0x0F0F00, 0x0F0F00, 0x0F0F00, 0x000000, 0x000000, 0x000000, 0x000000, 0x000000}}    // colon
#define s1521_5 {{0x000000, 0x000000, 0x000000, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x003800, 0x000000, 0x000000}}    // dash
#define s1521_6 {{0x000000, 0x000000, 0x000000, 0x0E00E0, 0x0F01E0, 0x0F83E0, 0x07C7C0, 0x03EF80, 0x01FF00, 0x00FE00, 0x007C00, 0x003800, 0x000000, 0x000000, 0x000000}}    // >
#define s1521_7 {{0x000000, 0x000000, 0x000000, 0x003800, 0x007C00, 0x00FE00, 0x01FF00, 0x03EF80, 0x07C7C0, 0x0F83E0, 0x0F01E0, 0x0E00E0, 0x000000, 0x000000, 0x000000}}    // <

// 
#define c2028_UNKN {{0x7FFFFFFC, 0x7FFFFFFC, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C,0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x6000000C, 0x7FFFFFFC, 0x7FFFFFFC}} // unknown

#define n2028_0 {{0x07FFFF80, 0x0FFFFFC0, 0x1FFFFFE0, 0x3FFFFFF0, 0x7FF803F8, 0x7FFC01F8, 0x7CFE00F8, 0x787F0078, 0x783F8078, 0x781FC078, 0x780FE078, 0x7807F078, 0x7803F878, 0x7C01FCF8, 0x7E00FFF8, 0x7F007FF8, 0x3FFFFFF0, 0x1FFFFFE0, 0x0FFFFFC0, 0x07FFFF80}}
#define n2028_1 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x78000780, 0x78000780, 0x780007C0, 0x7C0007E0, 0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x7C000000, 0x78000000, 0x78000000, 0x78000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}
#define n2028_2 {{0x78000780, 0x780007C0, 0x7C0007E0, 0x7E0003F0, 0x7F0001F8, 0x7F8000F8, 0x7FC00078, 0x7FE00078, 0x7BF00078, 0x79F80078, 0x78FC0078, 0x787E0078, 0x783F0078, 0x781F8078, 0x780FC0F8, 0x7807E1F8, 0x7803FFF0, 0x7801FFE0, 0x7800FFC0, 0x78007F80}}
#define n2028_3 {{0x07800780, 0x0F8007C0, 0x1F8007E0, 0x3F0003F0, 0x7E0001F8, 0x7C0000F8, 0x78000078, 0x78000078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x7C0FC0F8, 0x7E1FE1F8, 0x3FFFFFF0, 0x1FFCFFE0, 0x0FF87FC0, 0x07F87F80}}
#define n2028_4 {{0x007F8000, 0x007FC000, 0x007FE000, 0x007FF000, 0x0079F800, 0x0078FC00, 0x00787E00, 0x00783F00, 0x00781F80, 0x00780FC0, 0x007807E0, 0x007803F0, 0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x00780000, 0x00780000, 0x00780000, 0x00780000}}
#define n2028_5 {{0x07807FF8, 0x0F807FF8, 0x1F807FF8, 0x3F807FF8, 0x7E007878, 0x7C007878, 0x78007878, 0x78007878, 0x78007878, 0x78007878, 0x78007878, 0x78007878, 0x78007878, 0x78007878, 0x7C00F878, 0x7E01F878, 0x3FFFF078, 0x1FFFE078, 0x0FFFC078, 0x07FF8078}}
#define n2028_6 {{0x07FFF800, 0x0FFFFC00, 0x1FFFFE00, 0x3FFFFF00, 0x7E1F9F80, 0x7C0F8FC0, 0x780787E0, 0x780783F0, 0x780781F8, 0x780780F8, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x7C0F8078, 0x7E1F8078, 0x3FFF0000, 0x1FFE0000, 0x0FFC0000, 0x07F80000}}
#define n2028_7 {{0x00000078, 0x00000078, 0x00000078, 0x00000078, 0x7FF00078, 0x7FF80078, 0x7FFC0078, 0x7FFE0078, 0x003F0078, 0x001F8078, 0x000FC078, 0x0007E078, 0x0003F078, 0x0001F878, 0x0000FC78, 0x00007E78, 0x00003FF8, 0x00001FF8, 0x00000FF8, 0x000007F8}}
#define n2028_8 {{0x07F03F80, 0x0FF87FC0, 0x1FFCFFE0, 0x3FFFFFF0, 0x7E1FE1F8, 0x7C0FC0F8, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x7C0FC0F8, 0x7E1FE1F8, 0x3FFFFFF0, 0x1FFCFFE0, 0x0FF87FC0, 0x07F03F80}}
#define n2028_9 {{0x00007F80, 0x0000FFC0, 0x0001FFE0, 0x0003FFF0, 0x7807E1F8, 0x7807C0F8, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x7C078078, 0x7E078078, 0x3F078078, 0x1F878078, 0x0FC780F8, 0x07E781F8, 0x03FFFFF0, 0x01FFFFE0, 0x00FFFFC0, 0x007FFF80}}

#define c2028_Y {{0x00007FF8, 0x0000FFF8, 0x0001FFF8, 0x0003FFF8, 0x0007F000, 0x000FE000, 0x001FC000, 0x003F8000, 0x7FFF0000, 0x7FFE0000, 0x7FFE0000, 0x7FFF0000, 0x003F8000, 0x001FC000, 0x000FE000, 0x0007F000, 0x0003FFF8, 0x0001FFF8, 0x0000FFF8, 0x00007FF8}}
#define c2028_E {{0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x7FFFFFF8, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78000078, 0x78000078, 0x78000078, 0x78000078}}
#define c2028_S {{0x07807F80, 0x0F80FFC0, 0x1F81FFE0, 0x3F83FFF0, 0x7F07F3F8, 0x7E07E1F8, 0x7C07C0F8, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x78078078, 0x7C0F80F8, 0x7E1F81F8, 0x7F3F83F8, 0x3FFF07F0, 0x1FFE07E0, 0x0FFC07C0, 0x07F80780}}
#define c2028_N {{0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x00000FF0, 0x00001FE0, 0x00007F80, 0x0001FE00, 0x0003FC00, 0x000FF000, 0x003FC000, 0x007F8000, 0x01FE0000, 0x07F80000, 0x0FF00000, 0x1FC00000, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8}}
#define c2028_O {{0x07FFFF80, 0x0FFFFFC0, 0x1FFFFFE0, 0x3FFFFFF0, 0x7F0003F8, 0x7E0001F8, 0x7C0000F8, 0x78000078, 0x78000078, 0x78000078, 0x78000078, 0x78000078, 0x78000078, 0x7C0000F8, 0x7E0001F8, 0x7F0003F8, 0x3FFFFFF0, 0x1FFFFFE0, 0x0FFFFFC0, 0x07FFFF80}}
#define c2028_M {{0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x000007F0, 0x00000FE0, 0x00001FC0, 0x00003F80, 0x0000FF00, 0x0001FE00, 0x0001FE00, 0x0000FF00, 0x00003F80, 0x00001FC0, 0x00000FE0, 0x000007F0, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8}}
#define c2028_A {{0x1FFFF800, 0x1FFFFC00, 0x1FFFFE00, 0x1FFFFF00, 0x00783F80, 0x00781FC0, 0x00780FE0, 0x007807F0, 0x007803F8, 0x007801F8, 0x007801F8, 0x007803F8, 0x007807F0, 0x00780FE0, 0x00781FC0, 0x00783F80, 0x1FFFFF00, 0x1FFFFE00, 0x1FFFFC00, 0x1FFFF800}}
#define c2028_X {{0x1F0003F8, 0x1F8007F8, 0x1FC00FF8, 0x1FE01FF8, 0x03F03F00, 0x01F87E00, 0x00FCFC00, 0x007FF800, 0x003FF000, 0x001FE000, 0x001FE000, 0x003FF000, 0x007FF800, 0x00FCFC00, 0x01F87E00, 0x03F03F00, 0x1FE01FF8, 0x1FC00FF8, 0x1F8007F8, 0x1F0003F8}}
#define c2028_I {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x18000078, 0x18000078, 0x18000078, 0x18000078, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x1FFFFFF8, 0x18000078, 0x18000078, 0x18000078, 0x18000078, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}

#define s2028_1 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}              // space
#define s2028_2 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0F000000, 0x1F800000, 0x3FC00000, 0x3FC00000, 0x3FC00000, 0x3FC00000, 0x1F800000, 0x0F000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}  // period
#define s2028_3 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x60700000, 0x70F80000, 0x78F80000, 0x7CF80000, 0x3FF80000, 0x1FF80000, 0x0FF80000, 0x07F00000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}  // comma
#define s2028_4 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x0003C000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}  // dash
#define s2028_5 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x07800780, 0x07C00F80, 0x07E01F80, 0x07F03F80, 0x03F87F00, 0x01FCFE00, 0x00FFFC00, 0x007FF800, 0x003FF000, 0x001FE000, 0x000FC000, 0x00078000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}  // >
#define s2028_6 {{0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00078000, 0x000FC000, 0x001FE000, 0x003FF000, 0x007FF800, 0x00FFFC00, 0x01FCFE00, 0x03F87F00, 0x07F03F80, 0x07E01F80, 0x07C00F80, 0x07800780, 0x00000000, 0x00000000, 0x00000000, 0x00000000}}  // <

enum class Alignment {
    Left,
    Center,
    Right
};

enum UNITS {
    UNIT_P,     // bar
    UNIT_P2,    // kPa
    UNIT_T,     // C
    UNIT_T2,    // K
    UNIT_V,     // k/h
    UNIT_V2,    // mph
    UNIT_p,     // %
    UNIT_f,     // rpm
    UNIT_U,     // V
    UNIT_gps,   // g/s
    UNIT_nm,    // Nm
    UNIT_lh,    // l/h
    UNIT_deg,   // degrees
    UNIT_s,     // s
    UNIT_min,   // min
    UNIT_l,     // l
    UNIT_lL,    // l
    UNIT_mm,    // mm
    UNIT_cm,    // cm
    UNIT_none,  // none

    LAST_UNIT
};

struct unit_struct {
    char def_value[6];
    char unit_txt[4];
    uint8_t decimals;
};

struct unit_struct *get_units(void);

int set_cursor(uint8_t line, uint8_t col);
int set_line(uint8_t line);
int set_col(uint8_t col);
bool put_font0507(uint32_t *data, struct font0507 ch);
bool put_font1014(uint32_t *data, struct font1014 ch);
bool put_font1521(uint32_t *data, struct font1521 ch);
bool put_font2028(uint32_t *data, struct font2028 ch);
bool put_space0507(uint32_t *data);
bool put_space1014(uint32_t *data);
bool put_space1521(uint32_t *data);
bool put_space2028(uint32_t *data);
void print_font0507(uint32_t *data, const char *text);
void print_font1014(uint32_t *data, const char *text);
void print_font1521(uint32_t *data, const char *text);
void print_font2028(uint32_t *data, const char *text);
void print_header(uint32_t *data, const char *text, Alignment alignment = Alignment::Left);
void print_value(uint32_t *data, const char *text);
void print_unit(uint32_t *data, enum UNITS unit);
void debug_data(uint32_t *data);
void init_display(uint8_t cols, uint8_t lines);

static constexpr uint8_t get_header_maxlength(enum text_height height) {
    return height == text_height::single_line ? 21U : height == text_height::double_line ? 11U : 0U;
    // switch(height) {
    //     case text_height::single_line:
    //         return 21U;
    //     case text_height::double_line:
    //         return 11U;
    //     default:
    //         break;
    // }
    // return 0U;
}

static constexpr uint8_t get_value_maxlength(enum text_height height) {
    return height == text_height::single_line ? 18U : height == text_height::double_line ? 10U : height == text_height::triple_line ? 6U : height == text_height::quad_line ? 4U : 0U;
    // switch(height) {
    //     case text_height::single_line:
    //         return 18U;
    //     case text_height::double_line:
    //         return 9U;
    //     case text_height::triple_line:
    //         return 6U;
    //     case text_height::quad_line:
    //         return 4U;
    //     default:
    //         break;
    // }
    // return 0U;
}

static constexpr uint8_t get_value_start_column(enum text_height height) {
    return height == text_height::single_line ? 0U : height == text_height::double_line ? 0U : height == text_height::triple_line ? 2U : height == text_height::quad_line ? 14U : 0U;
    // switch(height) {
    //     case text_height::single_line:
    //         return 0U;
    //     case text_height::double_line:
    //         return 0U;
    //     case text_height::triple_line:
    //         return 2U;
    //     case text_height::quad_line:
    //         return 14U;
    //     default:
    //         break;
    // }
    // return 0U;
}