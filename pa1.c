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

typedef unsigned char u8;

#include <stdio.h>

void print_u8(u8* u8, int width, int height);
void print_const_u8(const u8* u8, int width, int height);
void print_bit(u8* u8, int width, int height);

/* TODO: Implement this function */
int encode(const u8* src, int width, int height, u8* result)
{

  if (width == 0 || height == 0) return 0;

  u8* src_ptr = (u8*)src;
  u8* dst_ptr = (u8*)result;
  u8* temp_ptr = (u8*)(result+(width * height));
  int index = 0;

  //Phase 1: Simplified Paeth Filtering
  while(index < (width * height)) {
    if(index == 0) {
      *dst_ptr = 0;
    } else if(index > 0 && index < width) {
      *dst_ptr += *(src_ptr-1);
    } else if((index % width) == 0) {
      *dst_ptr += *(src_ptr-width);
    } else {
      *dst_ptr = (*(src_ptr - 1) + *(src_ptr -(1 + width)) + *(src_ptr - width)) / 3;
    }
    index++;
    src_ptr++;
    dst_ptr++;
  }

  src_ptr = (u8*)src;
  dst_ptr = (u8*)result;
  index = 0;

  while(index < (width * height)) {
    if(*src_ptr >= *dst_ptr) {
      *dst_ptr = *src_ptr + 256 - *dst_ptr;
    } else {
      *dst_ptr = *src_ptr - *dst_ptr;
    }
    index++;
    src_ptr++;
    dst_ptr++;
  }

  dst_ptr = (u8*)result;
  temp_ptr = (u8*)(result+(width * height));

  //Phase 2: Encoding Filtered Values
  //From the width * height of the result, it will be used to store base, n, and diff.
  #define TEMP_WIDTH width + 2

  for (int i = 0; i < height; i++) {
    src_ptr = (u8*)(src + i * width);
    dst_ptr = (u8*)(result + i * width);
    u8 min = 0;
    u8 max = 0;

    //Find max, min
    for(int j = 0; j < width; j++) {
      if(j == 0) {
        min = *(dst_ptr+j);
        max = *(dst_ptr +j);
        continue;
      } 
      if(*(dst_ptr+j) <= min) min = *(dst_ptr+j);
      if(*(dst_ptr+j) >= max) max = *(dst_ptr+j);
    }

    //make Delta
    for(int j = 0; j < width; j++) {
      *(dst_ptr+j) = *(dst_ptr+j) - min;
    }

    max = max - min;
    
    //find n
    u8 n;
    if(max == 0) n = 0;
    else if(max == 1) n = 1;
    else if(max < 4) n = 2;
    else if(max < 8) n = 3;
    else if(max < 16) n = 4;
    else if(max < 32) n = 5;
    else if(max < 64) n = 6;
    else if(max < 128) n = 7;
    else n = 8;
    
    for(int j = 0; j < (TEMP_WIDTH); j++) {
      u8 temp;
      if(j == 0) temp = min;
      else if(j == 1) temp = n;
      else temp = *(dst_ptr + (j - 2));

      *(temp_ptr + j)= temp;
    }
    dst_ptr += width;
    temp_ptr += TEMP_WIDTH;
  }

  //BIT COMPRESSION
  dst_ptr = (u8*)result;
  temp_ptr = (u8*)(result+(width * height));

  int _n = 0;
  int length = (TEMP_WIDTH) * height * 8;
  for(int i = (TEMP_WIDTH) * height - 1; i >= 0; i--) {
    int shift;

    if(i % (TEMP_WIDTH) == (TEMP_WIDTH - 1)) _n =  *(temp_ptr + ((i / (TEMP_WIDTH)) * (TEMP_WIDTH) + 1));

    if(i % (TEMP_WIDTH) == 0) shift = 0;
    else if(i % (TEMP_WIDTH) == 1) shift = 4;
    else shift = 8 - _n;

    if(shift == 0) continue;

    *(temp_ptr + i) = *(temp_ptr + i) << shift;

    for(int j = i + 1; j < (TEMP_WIDTH) * height; j++) {
      u8 temp = *(temp_ptr + j);
      temp = temp >> (8 - shift);
      *(temp_ptr + j - 1) = *(temp_ptr + j - 1) | temp;
      *(temp_ptr + j) = *(temp_ptr + j) << shift;
    }

    length -= shift;
  }

  if(length % 8 == 0) {
    length = length / 8;
  } else {
    length = length / 8 + 1;
  }

  dst_ptr = result;
  temp_ptr = (result + width * height);
  for(int i = 0; i < length; i++) {
    *(dst_ptr + i) = *(temp_ptr + i);
  }

  return length;
}


void print_bit(u8* u8, int width, int height) {

  printf("BIT:\n");
  for(int j = 0; j < (TEMP_WIDTH) * height; j++) {
    if(j > 0 && j % (TEMP_WIDTH) * height == 0) printf("\n");
    printf("%.3d ", *(u8 + (width * height) + j));
  }
  printf("\n");
}

void print_u8(u8* u8, int width, int height) {
  printf("DESTINATION:\n");
  for(int i = 0; i < width * height; i++) {
    if(i > 0 && i % width == 0) {
      printf("\n");
    }
    printf("%.3d ", u8[i]);
  }
  printf("\n");
}

void print_const_u8(const u8* u8, int width, int height) {
  printf("SRC:\n");
    for(int i = 0; i < width * height; i++) {
    if(i > 0 && i % width == 0) {
      printf("\n");
    }
    printf("%.3d ", u8[i]);
  }
  printf("\n");
}
