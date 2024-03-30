#include "../base.c"
#include <stdio.h>
#include "stub.c"
#include "fixture16.c"
#include "fixture32.c"
#include "fixture32hex.c"
#include "fixture64.c"

#define BUFLEN 680000

mu8  buf[BUFLEN];
mu32 len;

void assert(i8 val[], i8 exp[], u32 len) {
  mu32 j;
  mi8 sign[3] = "==";
  mi8 res     = '+';

  buf[len] = 0;
  for (j = 0; j < len && exp[j] == buf[j]; j++);
  if (j != len || exp[j] != buf[j]) {
    sign[0] = '!';
    res     = '-';
  }

  printf("  %c \"%s\": \"%s\" %s \"%s\"\n", res, val, exp, sign, buf);
}

void test(u8 exp[], u32 len) {
  mu32 j;
  mi8 sign[3] = "==";
  mi8 res     = '+';

  for (j = 0; j < len && exp[j] == buf[j]; j++);
  if (j != len || exp[j-1] != buf[j-1]) {
    sign[0] = '!';
    res     = '-';
  }

  printf("  %c \"%s\"\n", res, exp);
}

int main(void) {
  mu32 j;
  mu8  data[2];

  puts("BASE16");
  assert("",       "",             b16e("", 0, buf, BUFLEN));
  assert("f",      "66",           b16e("f", 1, buf, BUFLEN));
  assert("fo",     "666F",         b16e("fo", 2, buf, BUFLEN));
  assert("foo",    "666F6F",       b16e("foo", 3, buf, BUFLEN));
  assert("foob",   "666F6F62",     b16e("foob", 4, buf, BUFLEN));
  assert("fooba",  "666F6F6261",   b16e("fooba", 5, buf, BUFLEN));
  assert("foobar", "666F6F626172", b16e("foobar", 6, buf, BUFLEN));


  data[0] = 0xff;
  data[1] = 0xff;

  len = b16e(data, 1, buf, BUFLEN);
  buf[len] = 0;
  printf("base16(0xff) = %s\n", buf);

  len = b16e(data, 2, buf, BUFLEN);
  buf[len] = 0;
  printf("base16(0xffff) = %s\n", buf);

  len = b64e(data, 2, buf, BUFLEN);
  buf[len] = 0;
  printf("base64(0xffff) = %s\n", buf);

  len      = b16e(stub, stublen, buf, BUFLEN);
  buf[len] = 0;
  for (j = 0; j < len && j < fixt16len && fixt16[j] == buf[j]; j++);
  if (j != len || fixt16[j] != buf[j]) {
    printf("  - stub.c: fixt16 != buf\n");
  } else {
    printf("  + stub.c: fixt16 == buf\n");
  }

  puts("\nBASE32");
  assert("", "", b32e("", 0, buf, BUFLEN));
  assert("f", "MY======", b32e("f", 1, buf, BUFLEN));
  assert("fo", "MZXQ====", b32e("fo", 2, buf, BUFLEN));
  assert("foo", "MZXW6===", b32e("foo", 3, buf, BUFLEN));
  assert("foob", "MZXW6YQ=", b32e("foob", 4, buf, BUFLEN));
  assert("fooba", "MZXW6YTB", b32e("fooba", 5, buf, BUFLEN));
  assert("foobar", "MZXW6YTBOI======", b32e("foobar", 6, buf, BUFLEN));
  len      = b32e(stub, stublen, buf, BUFLEN);
  buf[len] = 0;
  for (j = 0; j < len && fixt32[j] == buf[j]; j++);
  if (j != len || fixt32[j] != buf[j]) {
    printf("  - stub.c: fixt32 != buf\n");
  } else {
    printf("  + stub.c: fixt32 == buf\n");
  }

  puts("\nBASE32HEX");
  assert("", "", b32hexe("", 0, buf, BUFLEN));
  assert("f", "CO======", b32hexe("f", 1, buf, BUFLEN));
  assert("fo", "CPNG====", b32hexe("fo", 2, buf, BUFLEN));
  assert("foo", "CPNMU===", b32hexe("foo", 3, buf, BUFLEN));
  assert("foob", "CPNMUOG=", b32hexe("foob", 4, buf, BUFLEN));
  assert("fooba", "CPNMUOJ1", b32hexe("fooba", 5, buf, BUFLEN));
  assert("foobar", "CPNMUOJ1E8======", b32hexe("foobar", 6, buf, BUFLEN));
  len      = b32hexe(stub, stublen, buf, BUFLEN);
  buf[len] = 0;
  for (j = 0; j < len && fixt32hex[j] == buf[j]; j++);
  if (j != len || fixt32hex[j] != buf[j]) {
    printf("  - stub.c: fixt32hex != buf\n");
  } else {
    printf("  + stub.c: fixt32hex == buf\n");
  }

  puts("\nBASE64");
  assert("", "", b64e("", 0, buf, BUFLEN));
  assert("f", "Zg==", b64e("f", 1, buf, BUFLEN));
  assert("fo", "Zm8=", b64e("fo", 2, buf, BUFLEN));
  assert("foo", "Zm9v", b64e("foo", 3, buf, BUFLEN));
  assert("foob", "Zm9vYg==", b64e("foob", 4, buf, BUFLEN));
  assert("fooba", "Zm9vYmE=", b64e("fooba", 5, buf, BUFLEN));
  assert("foobar", "Zm9vYmFy", b64e("foobar", 6, buf, BUFLEN));
  len      = b64e(stub, stublen, buf, BUFLEN);
  buf[len] = 0;
  for (j = 0; j < len && fixt64[j] == buf[j]; j++);
  if (j != len || fixt64[j] != buf[j]) {
    printf("  - stub.c: fixt64 != buf\n");
  } else {
    printf("  + stub.c: fixt64 == buf\n");
  }

  puts("\nDECODE BASE16");
  test("f",      b16d("66",           2, buf, BUFLEN));
  test("fo",     b16d("666F",         4, buf, BUFLEN));
  test("foo",    b16d("666F6F",       6, buf, BUFLEN));
  test("foob",   b16d("666F6F62",     8, buf, BUFLEN));
  test("fooba",  b16d("666F6F6261",   10, buf, BUFLEN));
  test("foobar", b16d("666F6F626172", 12, buf, BUFLEN));
  len      = b16d(fixt16, fixt16len, buf, BUFLEN);
  buf[len] = 0;
  for (j = 0; j < len && j < stublen && stub[j] == buf[j]; j++);
  if (j != len || stub[j-1] != buf[j-1]) {
    printf("  - fixt16.c: stub != buf\n");
  } else {
    printf("  + fixt16.c: stub == buf\n");
  }

  return 0;
}
