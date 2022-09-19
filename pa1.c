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

u8 get_average(const u8* src, int width, int height, int x, int y) {
  if (x == 0 && y == 0) {
    return 0;
  } else if(x != 0 && y == 0) {
    return *(src + (width * y + x - 1));
  } else if(x == 0 && y != 0) {
    return *(src + (width * (y - 1)) + x);
  } else {
    return (*(src + (width * (y - 1)) + x) + *(src + (width * y) + x - 1) + *(src + (width * (y - 1)) + x - 1)) / 3;
  }
}

u8 get_filter(const u8* src, int width, int height, int x, int y) {
  u8 average = get_average(src, width, height, x, y);
  u8 result;

  if(*(src + (width * y) + x) >= average) {
    result = *(src + (width * y) + x) - average;
  } else {
    result = *(src + (width * y) + x) + 256 - average;
  }

  return result;
}

u8 get_base(const u8* src, int width, int height, int y) {
  u8 base = get_filter(src, width, height, 0, y);

  for(int i = 1; i < width; i++) {
    if(base > get_filter(src, width, height, i, y)) {
      base = get_filter(src, width, height, i, y);
    }
  }

  return base;
}

u8 get_delta(const u8* src, int width, int height, int x, int y) {
  return get_filter(src, width, height, x, y) - get_base(src, width, height, y);
}

u8 get_deltaMax(const u8* src, int width, int height, int y) {
  u8 deltaMax = get_delta(src, width, height, 0, y);

  for(int i = 1; i < width; i++) {
    if(deltaMax < get_delta(src, width, height, i, y)) {
      deltaMax = get_delta(src, width, height, i, y);
    }
  }

  return deltaMax;
}

u8 get_n(const u8* src, int width, int height, int y) {
  u8 deltaMax = get_deltaMax(src, width, height, y);
  if(deltaMax == 0) {
    return 0;
  } else if(deltaMax == 1) {
    return 1;
  } else if(deltaMax < 4) {
    return 2;
  } else if(deltaMax < 8) {
    return 3;
  } else if(deltaMax < 16){
    return 4;
  } else if(deltaMax < 32) {
    return 5;
  } else if(deltaMax < 64) {
    return 6;
  } else if(deltaMax < 128) {
    return 7;
  } else {
    return 8;
  }
}


void insert_value_in_result(u8* dst, int* bit_index, u8 value, int bit_len) {
  if(bit_len == 0 && value == 0) {
    return;
  }
  value = value << (8 - bit_len);

  while(bit_len) {
    if(*bit_index % 8 == 0) {
      *(dst + *bit_index / 8) = 0;
    }

    u8 temp = (value >> 7 << 7 >> (*bit_index % 8));

    *(dst + *(bit_index) / 8) |= temp;
    *bit_index = *bit_index + 1;
    value <<= 1;
    bit_len--;
  }
}

/* TODO: Implement this function */
int encode(const u8* src, int width, int height, u8* result)
{
  if (width == 0 || height == 0) {
    return 0;
  }

  int bit_index = 0;

  for(int i = 0; i < height; i++) {
    insert_value_in_result(result, &bit_index, get_base(src, width, height, i), 8);
    insert_value_in_result(result, &bit_index, get_n(src, width, height, i), 4);

    for(int j = 0; j < width; j++) {
      insert_value_in_result(result, &bit_index, get_delta(src, width, height, j, i), (int)get_n(src,width,height, i));
    }
  }

  int length; 
  if (bit_index % 8 == 0) {
    length = bit_index / 8;
  } else {
    length = bit_index / 8 + 1;
  }

  return length;
}