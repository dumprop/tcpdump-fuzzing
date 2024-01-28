#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "netdissect-stdinc.h"
#include "netdissect.h"
#include "print-bgp.c"

#define MAXIMUM_SNAPLEN 262144
#define DEFAULT_SNAPLEN MAXIMUM_SNAPLEN

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return -1;
    }

    // from tcpdump.c
    netdissect_options Ndo;
    netdissect_options *ndo = &Ndo;
    memset(ndo, 0, sizeof(*ndo));
    ndo_set_function_pointers(ndo);
    ndo->ndo_snaplen = DEFAULT_SNAPLEN;
    ndo->program_name = argv[0];
    // end of ndo

    // params
    unsigned char buffer[10240];
    // end of params

    // read value from file (fuzzer)
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        perror("Unable to open file");
        return -1;
    }
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    // generate atype value based on fuzzer's data
    u_int atype_value = ((unsigned char)buffer[0] << 24) | 
              ((unsigned char)buffer[1] << 16) | 
              ((unsigned char)buffer[2] << 8)  | 
              ((unsigned char)buffer[3]);

    // fuzz function :)
    int result = bgp_attr_print(&ndo, atype_value, buffer, sizeof(buffer));

    if (result > 0) {
        printf("Result: %d\n", result);
    } else {
        printf("An error occurred: %d\n", result);
    }

    return 0;
}
