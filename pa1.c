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

/* TODO: Implement this function */
int encode(const u8* src, int width, int height, u8* result)
{
  if (width == 0 || height == 0) return 0;

  //Phase 1: Simplified Paeth Filtering
  for(int i = 0; i < width * height; i++) {
    if(i == 0) {
      *(result + i) = 0;
    } else if(i > 0 && i < width) {
      *(result + i) += *(src + i - 1);
    } else if((i % width) == 0) {
      *(result + i) = *(src + i - width);
    } else {
      *(result + i) = (*(src + i - 1) + *(src + i - (1 + width)) + *(src + i - width)) / 3;
    }
  }


  for(int i = 0; i < width * height; i++) {
    if(*(src + i) >= *(result + i)) {
      *(result + i) = *(src + i) + 256 - *(result + i);
    } else {
      *(result + i) = *(src + i) - *(result + i);
    }
  }

  //make location for base , n;
  for(int i = width * height - 1; i >= 0; i--) {
    *(result + i + 2 * (i / width + 1)) = *(result + i);
    *(result + i) = 0;
  }


  //Phase 2: Encoding Filtered Values
  for (int i = 0; i < height; i++) {
    //find min, max
    for(int j = 2; j < width + 2; j++) {
      if (j == 2) {
        //min
        *(result + (i * (width+2))) = *(result + (i * (width+2)) + j);
        //max
        *(result + (i * (width+2)) + 1) = *(result + (i * (width+2)) + j);
        //printf("I = %d, J = %d, INDEX: %d, %d\n", i, j , (i * (width+2)) + j ,*(result + (i * j) + j));
        
        
        continue;
      }

      //min
      if(*(result + (i * (width+2))) >= *(result + (i * (width+2)) + j))  {
        *(result +(i * (width+2))) = *(result + (i * (width+2)) + j);
      }
      //max
      if(*(result + (i * (width+2)) + 1) <= *(result + (i * (width+2)) + j)) {
        *(result + (i * (width+2)) + 1) = *(result + (i * (width+2)) + j);
      } 
    }
  }

  //make dela
  for(int i = 0; i < (width + 2) * height; i++) {
    if(i % (width +2) == 0) continue;
    else {
      *(result + i) = *(result + i) - *(result + (i - (i % (width + 2))));
    }
    if( i % (width + 2) == 1) {

      if(*(result + i) == 0) *(result + i) = 0;
      else if(*(result + i) == 1) *(result + i) = 1;
      else if(*(result + i) < 4) *(result + i) = 2;
      else if(*(result + i) < 8) *(result + i) = 3;
      else if(*(result + i) < 16) *(result + i) = 4;
      else if(*(result + i) < 32) *(result + i) = 5;
      else if(*(result + i) < 64) *(result + i) = 6;
      else if(*(result + i) < 128) *(result + i) = 7;
      else *(result + i) = 8;
      
    }
  }

  //Bit Compression
  int length = (width + 2) * height * 8;
  int _n = 0;
  for(int i = (width + 2) * height - 1; i >= 0; i--) {
    int shift;

    if(i % (width + 2) == (width + 2 - 1)) _n =  *(result + ((i / (width + 2)) * (width + 2) + 1));

    if(i % (width + 2) == 0) shift = 0;
    else if(i % (width + 2) == 1) shift = 4;
    else shift = 8 - _n;

    if(shift == 0) continue;

    *(result + i) = *(result + i) << shift;

    for(int j = i + 1; j < (width + 2) * height; j++) {
      u8 temp = *(result + j);
      temp = temp >> (8 - shift);
      *(result + j - 1) = *(result + j - 1) | temp;
      *(result + j) = *(result + j) << shift;
    }
    length -= shift;
  }

  if(length % 8 == 0) {
    length = length / 8;
  } else {
    length = length / 8 + 1;
  }

  return length;
}