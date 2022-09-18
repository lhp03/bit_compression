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

u8 get_avg(const u8* src, int width, int height, int w, int h) {
  if (w == 0 && h == 0) {
    return 0;
  } else if(h != 0 && w == 0) {
    return *(src + (width * (h - 1)) + w);
  } else if(h == 0 && w != 0) {
    return *(src + (width * h + w - 1));
  } else {
    return (*(src + (width * (h - 1)) + w) + *(src + (width * h) + w - 1) + *(src + (width * (h - 1)) + w - 1)) / 3;
  }
}

u8 get_filter(const u8* src, int width, int height, int w, int h) {
  u8 avg = get_avg(src, width, height, w, h);
  if(*(src + (width * h) + w) >= avg) return *(src + (width * h) + w) - avg;
  else return *(src + (width * h) + w) + 256 - avg;
}

u8 get_min(const u8* src, int width, int height, int h) {
  u8 min = get_filter(src, width, height, 0, h);

  for(int i = 1; i < width; i++) {
    if(min > get_filter(src, width, height, i, h)) min = get_filter(src, width, height, i, h);
  }

  return min;
}

u8 get_max(const u8* src, int width, int height, int h) {
  u8 max = get_filter(src, width, height, 0, h);

  for(int i = 1; i < width; i++) {
    if(max < get_filter(src, width, height, i, h)) max = get_filter(src, width, height, i, h);
  }

  return max;
}

u8 get_n(const u8* src, int width, int height, int h) {
  u8 max = get_max(src, width, height, h) - get_min(src, width, height, h);
  if(max == 0) return 0;
  else if(max == 1) return 1;
  else if(max < 4) return 2;
  else if(max < 8) return 3;
  else if(max < 16) return 4;
  else if(max < 32) return 5;
  else if(max < 64) return 6;
  else if(max < 128) return 7;
  else return 8;
}


u8 get_delta(const u8* src, int width, int height, int w, int h) {
  u8 base = get_min(src, width, height, h);

  return get_filter(src, width, height, w, h) - base;
}

void insert_bit(u8* result, u8 value, int len_value, int* bit_index) {
  if(len_value == 0) return;

  value = value << (8 - len_value);
  if (*bit_index % 8 == 0) {
    *(result + *bit_index / 8) = value;
  } else {
    u8 temp = (value >> (*bit_index % 8));
    value = value << (8 - *bit_index % 8);
    *(result + *bit_index / 8) = *(result + *bit_index / 8) | temp;
    *(result + *bit_index / 8 + 1) = value;
  }
  *bit_index += len_value;
}

/* TODO: Implement this function */
int encode(const u8* src, int width, int height, u8* result)
{
  if (width == 0 || height == 0) return 0;

  int bit_index = 0;
  for(int i = 0; i < height; i++) {
    u8 base = get_min(src, width, height, i);
    u8 n = get_n(src, width, height, i);

    insert_bit(result, base, 8, &bit_index);
    insert_bit(result, n, 4, &bit_index);

    for(int j = 0; j < width; j++) {
      insert_bit(result, get_delta(src, width, height, j, i), (int)n, &bit_index);
    }
  }

  int length = bit_index / 8;

  if(bit_index % 8 != 0) length++;

  return length;
}