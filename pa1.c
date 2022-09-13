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

/* TODO: Implement this function */
int encode(const u8* src, int width, int height, u8* result)
{
  u8* src_ptr = (u8*)src;
  u8* dst_ptr = (u8*)result;
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

  //Phase 2: Encoding Filtered Values
  for (int i = 0; i < height; i++) {
    src_ptr = (u8*)(src + i * width);
    dst_ptr = (u8*)(result + i * width);
    u8 min = 0;
    u8 max = 0;

    //Find min
    for(int j = 0; j < width; j++) {
      if(j == 0) {
        min = *(dst_ptr+j);
        continue;
      } 
      if(*(dst_ptr+j) < min) min = *(dst_ptr+j);
      if(*(dst_ptr+j) > max) max = *(dst_ptr+j);
    }

    //make Delta
    for(int j = 0; j < width; j++) {
      *(dst_ptr+j) = *(dst_ptr+j) - min;
    }
    
    max = max - min;
    
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

    printf("n(%d): %d\n", i, n);
  }

  //print_const_u8(src, width, height);
  print_u8(result, width, height);

  return 0;
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
