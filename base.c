typedef const char                   i8;
typedef const unsigned char          u8;
typedef const short                  i16;
typedef const unsigned short         u16;
typedef const int                    i32;
typedef const unsigned int           u32;
typedef const long long int          i64;
typedef const unsigned long long int u64;
typedef char                         mi8;
typedef unsigned char                mu8;
typedef short                        mi16;
typedef unsigned short               mu16;
typedef int                          mi32;
typedef unsigned int                 mu32;
typedef long long int                mi64;
typedef unsigned long long int       mu64;
void asserttypes(void) {
  (void)sizeof(char[!!(1 - sizeof(i8))  * (-1)]);
  (void)sizeof(char[!!(2 - sizeof(i16)) * (-1)]);
  (void)sizeof(char[!!(4 - sizeof(i32)) * (-1)]);
  (void)sizeof(char[!!(8 - sizeof(i64)) * (-1)]);
}

enum ALPHTYPE {
  ALPH16 = 1,
  ALPH32,
  ALPH32HEX,
  ALPH64,
  ALPH64SAFE
};

u32 b16e(u8 data[], u32 datalen, mu8 buf[], u32 buflen);
u32 b32e(u8 data[], u32 datalen, mu8 buf[], u32 buflen);
u32 b32hexe(u8 data[], u32 datalen, mu8 buf[], u32 buflen);
u32 b64e(u8 data[], u32 datalen, mu8 buf[], u32 buflen);
u32 b64se(u8 data[], u32 datalen, mu8 buf[], u32 buflen);
u32 benc(
  u8  alph[],
  u32 alphlen,
  u8  data[],
  u32 datalen,
  mu8 buf[],
  u32 buflen,
  u32 grouplen);
u32 bdec(
  const enum ALPHTYPE alpht,
  u8  alph[],
  u32 alphlen,
  u8  data[],
  u32 datalen,
  mu8 buf[],
  u32 buflen,
  u32 grouplen);

u8 alph16[16] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

u8 alph32[32] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', '2', '3', '4', '5', '6', '7',
};

u8 alph32hex[32] = {
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
  'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
  'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
};

u8 alph64[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/',
};

u8 alph64safe[64] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '-', '_',
};

u32 alphdlen   = 128;
mu8 alphd[129] = {0};
u8  alpherr    = 0xff;

u32 benc(
  u8  alph[],
  u32 alphlen,
  u8  data[],
  u32 datalen,
  mu8 buf[],
  u32 buflen,
  u32 grouplen)
{
  mu32 j, k, m;
  mu32 offset;
  mu32 index;
  mu64 cur;

/*
       6 bits grouplen -> 3 common bytes -> 4 curlen bytes
       5 bits grouplen -> 5 common bytes -> 8 curlen bytes
       4 bits grouplen -> 1 common byte  -> 2 curlen bytes
*/
  u32  common = grouplen / (!(grouplen % 2) + 1) / (!(grouplen % 4) + 1);
  u32  curlen = 8 * common / grouplen;
  u8   mask   = 0xff >> (8 - grouplen);
  u32  len    = curlen * (datalen / common + !!(datalen % common));

  if (buflen < len)
    return 0;

  m   = 0;
  cur = 0;
  for (j = 0; j + common <= datalen && m < len; j += common) {
    for (k = 0; k < common; k++)
      cur      = cur << 8 | data[j + k];
    for (k = 0; k < curlen; k++) {
      offset   = grouplen * (curlen - k - 1);
      index    = cur >> offset & mask;
      if (index >= alphlen)
        return 0;

      buf[m++] = alph[index];
    }
  }

  if (j == datalen)
    return (m == len) * m;

  for (k = 0; k < curlen - 1 && j + k < datalen; k++)
      cur = cur << 8 | data[j + k];
  cur <<= 8;

  for (j = 0; j < 8 * k / grouplen + 1 && m < len; j++) {
    offset   = 8 * (k + 1) - grouplen * (j + 1);
    index    = cur >> offset & mask;
    if (index >= alphlen)
      return 0;

    buf[m++] = alph[index];
  }

  for (; j < curlen && m < len; j++)
    buf[m++] = '=';

  return (m == len) * m;
}

u32 b16e(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return benc(alph16, 16, data, datalen, buf, buflen, 4);
}

u32 b32e(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return benc(alph32, 32, data, datalen, buf, buflen, 5);
}

u32 b32hexe(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return benc(alph32hex, 32, data, datalen, buf, buflen, 5);
}

u32 b64e(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return benc(alph64, 64, data, datalen, buf, buflen, 6);
}

u32 b64se(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return benc(alph64safe, 64, data, datalen, buf, buflen, 6);
}

u32 bdec(
  const enum ALPHTYPE alpht,
  u8  alph[],
  u32 alphlen,
  u8  data[],
  u32 datalen,
  mu8 buf[],
  u32 buflen,
  u32 grouplen)
{
  mu32 j, k, m;
  mu32 offset;
  mu32 index;
  mu32 len;
  mu32 padlen;
  mu64 cur;

  u32  common = grouplen / (!(grouplen % 2) + 1) / (!(grouplen % 4) + 1);
  u32  curlen = 8 * common / grouplen;
  u8   mask   = 0xff >> (8 - grouplen);

  if (datalen % curlen != 0)
    return 0;

  for (padlen = 0; padlen < datalen && data[datalen - 1 - padlen] == '='; padlen++);
  if (grouplen == 4 && padlen != 0)
    return 0;
  if (padlen >= curlen)
    return 0;

  len      = (datalen - padlen) / curlen * common + !!padlen * (curlen - padlen) * grouplen / 8;

  if (len > buflen)
    return 0;

  if (alphd[128] != alpht) {
    for (j = 0; j < alphdlen; j++)
      alphd[j]     = alpherr;
    for (j = 0; j < alphlen; j++) {
      index        = alph[j];
      if (index >= alphdlen)
        return 0;
      alphd[index] = j;
    }
    alphd[128] = alpht;
  }

  m   = 0;
  cur = 0;
  for (j = 0; j + curlen <= datalen - padlen && m < len; j += curlen) {
    for (k = 0; k < curlen; k++) {
      index  = data[j + k];
      if (index >= alphdlen)
        return 0;
      if (alphd[index] == alpherr)
        return 0;
      cur    = cur << grouplen | alphd[index];
    }
    for (k = 0; k < common; k++) {
      offset   = 8 * (common - k - 1);
      buf[m++] = cur >> offset;
    }
  }

  if (j == datalen)
    return (m == len) * m;

  for (k = 0; k < curlen - 1 && j + k < datalen - padlen; k++) {
    index  = data[j + k];
    if (index >= alphdlen)
      return 0;
    cur    = cur << grouplen | alphd[index];
  }

  for (j = 0; j < grouplen * k / 8; j++) {
    offset   = k * grouplen - 8 * (j + 1);
    buf[m++] = cur >> offset;
  }

  return (m == len) * m;
}

u32 b16d(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return bdec(ALPH16, alph16, 16, data, datalen, buf, buflen, 4);
}

u32 b32d(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return bdec(ALPH32, alph32, 32, data, datalen, buf, buflen, 5);
}

u32 b32hexd(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return bdec(ALPH32HEX, alph32hex, 32, data, datalen, buf, buflen, 5);
}


u32 b64d(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return bdec(ALPH64, alph64, 64, data, datalen, buf, buflen, 6);
}

u32 b64sd(u8 data[], u32 datalen, mu8 buf[], u32 buflen) {
  return bdec(ALPH64SAFE, alph64, 64, data, datalen, buf, buflen, 6);
}
