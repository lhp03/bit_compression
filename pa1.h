//---------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2022)
//
//  Project #1:
//
//  September 6, 2022
//
//  Seongyeop Jeong (seongyeop.jeong@snu.ac.kr)
//  Jaehoon Shim (mattjs@snu.ac.kr)
//  IlKueon Kang (kangilkueon@snu.ac.kr)
//  Wookje Han (gksdnrwp@snu.ac.kr)
//  Jinsol Park (jinsolpark@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//---------------------------------------------------------------

#include <stdio.h>

#define LEN_GUARD 8
#define LEN_DST 45
#define GUARD_WORD 0xDEADBEEFDEADBEEF
typedef unsigned char u8;

u8 input0[] = {
  0, 0, 0, 0,
  50, 75, 100, 120,
  75, 100, 120, 0
};
const int input_height_0 = 3;
const int input_width_0 = 4;
u8 ans0[] = {
  0x00, 0x03, 0x26, 0x00, 0x96, 0x65, 0x19, 0x70, 0x02, 0x42, 0x76
};

u8 input1[] = {
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const int input_height_1 = 5;
const int input_width_1 = 10;

u8 ans1[] = {
  0x00, 0x8f, 0xf0, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0x01, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xc0, 0x56, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x1a, 0xb0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00
};


u8 input2[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const int input_height_2 = 5;
const int input_width_2 = 10;
u8 ans2[] = {
  0x00, 0x8f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

u8 input3[] = {
  0x32, 0x45, 0x64, 0x78, 0x25, 0x26, 0x27, 0x29, 0x16, 0x12,
  0x18, 0x17, 0x57, 0x98, 0xA3, 0xF0, 0x21, 0x4A, 0x5B, 0x20,
  0x44, 0x56, 0x78, 0x90, 0x32, 0x45, 0x64, 0x78, 0x25, 0x26,
  0x27, 0x29, 0x16, 0x12, 0x21, 0x4A, 0x5B, 0x20, 0x44, 0x56,
  0x18, 0x17, 0x57, 0x98, 0xA3, 0xF0, 0x21, 0x4A, 0x5B, 0x20,

};
const int input_height_3 = 5;
const int input_width_3 = 10;
u8 ans3[] = {
  0x01, 0x83, 0x11, 0x21, 0xe1, 0x3a, 0xc0, 0x00, 0x00, 0x1e,
  0xcf, 0xb1, 0x78, 0xcf, 0xd1, 0x00, 0x1b, 0x25, 0x8a, 0xa1,
  0x0e, 0x17, 0xde, 0x19, 0x81, 0x31, 0x71, 0xe0, 0x08, 0x09,
  0x5d, 0x91, 0xaa, 0xdd, 0x80, 0x58, 0xde, 0xe4, 0xbf, 0xae,
  0xd6, 0x13, 0x05, 0xb4, 0x00, 0x22, 0x16, 0x8d, 0xbd, 0xf2,
  0x55, 0x84, 0xa8, 0x08, 0x40, 0x00, 0xbb, 0x90
};

u8 input4[] = {
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
};
const int input_height_4 = 5;
const int input_width_4 = 10;
u8 ans4[] = {
  0x01, 0x00, 0x01, 0x7f, 0xc0, 0x05, 0xff, 0x00, 0x17, 0xfc, 0x00, 0x5f, 0xf0
};

u8 input5[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};
const int input_height_5 = 5;
const int input_width_5 = 10;
u8 ans5[] = {
  0x00, 0x8f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x17, 0x01, 0x56, 0xad, 0x5a, 0xb5, 0x6a, 0xd5,
  0xab, 0x56, 0xa9, 0xea, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x70, 0x15, 0x6a, 0xd5, 0xab, 0x56, 0xad,
  0x5a, 0xb5, 0x6a, 0x9e, 0xa8, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00
};

const int input_height_6 = 5;
const int input_width_6 = 10;
u8 input6[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

u8 ans6[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const int input_height_7 = 9;
const int input_width_7 = 8;
u8 input7[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xde,0xa3,0x00,0x00,0x00,0xa3,
  0x00,0x00,0x00,0xde,0xe7,0xe3,0xe7,0xe7,
  0xa3,0xc1,0x00,0xe7,0x00,0xe7,0xe3,0x00,
  0xc1,0xa3,0x00,0x68,0xe7,0xe3,0xe7,0xe7,
  0x00,0xc1,0x00,0xe3,0xa3,0xe7,0xa3,0x00,
  0x00,0xa3,0xe3,0xa3,0xe3,0xa3,0xe7,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

u8 ans7[] = {
  0x00, 0x00, 0x08, 0x00, 0x00, 0xde, 0x59, 0xca, 0x00, 0x00,
  0xa3, 0x00, 0x80, 0x00, 0x0b, 0x65, 0xe6, 0x79, 0x69, 0xc6,
  0x41, 0xb8, 0x88, 0x70, 0xa5, 0x82, 0x01, 0x34, 0xe3, 0x00,
  0x03, 0x81, 0xbe, 0x98, 0x71, 0x87, 0x54, 0x60, 0x04, 0xc1,
  0x88, 0x27, 0x33, 0x72, 0xa9, 0xd0, 0x00, 0xa6, 0x18, 0x00,
  0x80, 0x06, 0x36, 0xd0, 0xc2, 0xbd, 0x42, 0xe7, 0xd0, 0x08,
  0x00, 0xca, 0x7e, 0x7e, 0x7e, 0x7e, 0x7d, 0xb3, 0x00, 0x00
};

typedef struct {
  const void *input;
  const int input_height;
  const int input_width;

  const u8 *ans;
  const int ans_len;
} testcase;

#define TESTCASE(num) {						    \
  .input = input##num,				      \
  .input_height = input_height_##num,	\
  .input_width = input_width_##num,   \
  .ans = ans##num,					        \
  .ans_len = sizeof(ans##num),		  \
}


const testcase tc[] = {
  TESTCASE(0),
  TESTCASE(1),
  TESTCASE(2),
  TESTCASE(3),
  TESTCASE(4),
  TESTCASE(5),
  TESTCASE(6),
  TESTCASE(7),
};

int encode(const u8* src, const int width, const int height, u8 *dst);

