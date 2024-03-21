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

const u8 alph64[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/',
};

union quant64 {
  u32 num;
  u8  byte[3];
} q64;

const u8 offset64[] = {18, 12, 6, 0};

u32 b64e(const u8 data[], const u32 datalen, u8 buf[], const u32 buflen) {
  u32 j, k, cur, len;

  len = 4 * (datalen / 3 + !!(datalen % 3));
  if (buflen < len)
    return 0;

  cur = 0;
  for (j = 0; j + 3 <= datalen; j += 3) {
    for (k = 0; k < 3; k++)
      q64.byte[2 - k] = data[j + k];
    for (k = 0; k < 4; k++)
      buf[cur + k] = alph64[q64.num >> offset64[k] & 0b111111];
    cur += 4;
  }

  if (j == datalen)
    return len;

  for (k = 0; k < 2 && j + k < datalen; k++)
    q64.byte[2 - k] = data[j + k];
  q64.byte[2 - k] = 0;

  for (j = 0; j < 4 * k / 3 + 1; j++)
    buf[cur + j] = alph64[q64.num >> offset64[j] & 0b111111];

  for (; j < 4; j++)
    buf[cur + j] = '=';

  return len;
}
