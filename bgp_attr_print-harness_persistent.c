#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netdissect-stdinc.h"
#include "netdissect.h"
#include "print-bgp.c"

#define MAXIMUM_SNAPLEN 262144
#define DEFAULT_SNAPLEN MAXIMUM_SNAPLEN

/* this lets the source compile without afl-clang-fast/lto */
#ifndef __AFL_FUZZ_TESTCASE_LEN

ssize_t       fuzz_len;
unsigned char fuzz_buf[1024000];

  #define __AFL_FUZZ_TESTCASE_LEN fuzz_len
  #define __AFL_FUZZ_TESTCASE_BUF fuzz_buf
  #define __AFL_FUZZ_INIT() void sync(void);
  #define __AFL_LOOP(x) \
    ((fuzz_len = read(0, fuzz_buf, sizeof(fuzz_buf))) > 0 ? 1 : 0)
  #define __AFL_INIT() sync()

#endif

__AFL_FUZZ_INIT();

/* Main entry point. */

/* To ensure checks are not optimized out it is recommended to disable
   code optimization for the fuzzer harness main() */
#pragma clang optimize off
#pragma GCC            optimize("O0")


int main(int argc, char **argv) {
    // persistent_mode https://github.com/AFLplusplus/AFLplusplus/blob/stable/utils/persistent_mode/persistent_demo_new.c
    ssize_t        len;                        /* how much input did we read? */
    unsigned char *buf;                        /* test case buffer pointer    */

    // from tcpdump.c
    netdissect_options Ndo;
    netdissect_options *ndo = &Ndo;
    memset(ndo, 0, sizeof(*ndo));
    ndo_set_function_pointers(ndo);
    ndo->ndo_snaplen = DEFAULT_SNAPLEN;
    ndo->program_name = argv[0];
    // end of ndo

    __AFL_INIT();

    buf = __AFL_FUZZ_TESTCASE_BUF;  // this must be assigned before __AFL_LOOP!

    while (__AFL_LOOP(UINT_MAX)) {  // increase if you have good stability

      len = __AFL_FUZZ_TESTCASE_LEN;  // do not use the macro directly in a call!

      if (len < 5) {
        printf("No enough data\n");
        return -1;
      }


      // generate atype value based on fuzzer's data
      u_int atype_value = ((unsigned char)buf[0] << 24) | 
                ((unsigned char)buf[1] << 16) | 
                ((unsigned char)buf[2] << 8)  | 
                ((unsigned char)buf[3]);

      // fuzz function :)
      int result = bgp_attr_print(ndo, atype_value, buf, len);

      if (result > 0) {
        printf("Result: %d\n", result);
      } else {
          printf("An error occurred: %d\n", result);
      }

    }

    return 0;
}
