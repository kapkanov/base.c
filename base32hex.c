#ifndef TYPES_C
#define TYPES_C
typedef char                   i8;
typedef unsigned char          u8;
typedef short                  i16;
typedef unsigned short         u16;
typedef int                    i32;
typedef unsigned int           u32;
typedef long long int          i64;
typedef unsigned long long int u64;
void asserttypes(void) {
  (void)sizeof(char[!!(1 - sizeof(i8))  * (-1)]);
  (void)sizeof(char[!!(2 - sizeof(i16)) * (-1)]);
  (void)sizeof(char[!!(4 - sizeof(i32)) * (-1)]);
  (void)sizeof(char[!!(8 - sizeof(i64)) * (-1)]);
}
#endif

const u8 alph32hex[32] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

union quant32hex {
  u64 num;
  u8  byte[5];
} q32hex;

const u8 offset32hex[] = {35, 30, 25, 20, 15, 10, 5, 0};

u32 b32hexe(const u8 data[], const u32 datalen, u8 buf[], const u32 buflen) {
  u32 j, k, cur, len;

  len = 8 * (datalen / 5 + !!(datalen % 5));
  if (buflen < len)
    return 0;

  cur = 0;
  for (j = 0; j + 5 <= datalen; j += 5) {
    for (k = 0; k < 5; k++)
      q32hex.byte[4 - k] = data[j + k];
    for (k = 0; k < 8; k++)
      buf[cur + k] = alph32hex[q32hex.num >> offset32hex[k] & 0b11111];
    cur += 8;
  }

  if (j == datalen)
    return len;

  for (k = 0; k < 4 && j + k < datalen; k++)
    q32hex.byte[4 - k] = data[j + k];
  q32hex.byte[4 - k] = 0;

  for (j = 0; j < 8 * k / 5 + 1; j++)
    buf[cur + j] = alph32hex[q32hex.num >> offset32hex[j] & 0b11111];

  for (; j < 8; j++)
    buf[cur + j] = '=';

  return len;
}
