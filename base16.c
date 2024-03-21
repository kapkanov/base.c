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

const i8 alph16[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

u32 b16e(const u8 data[], const u32 datalen, u8 buf[], const u32 buflen) {
  u32 j;

  if (buflen < 2 * datalen)
    return 0;

  for (j = 0; j < datalen; j++) {
    buf[2*j+1] = alph16[data[j]        & 0xf];
    buf[2*j]   = alph16[(data[j] >> 4) & 0xf ];
  }

  return 2*j;
}
