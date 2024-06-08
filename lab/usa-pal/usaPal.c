#include <stdio.h>
#include "palindroma.h"

int main(int argc, char *argv[]) {
    if (palindroma(argv[1])) printf("La parola \"%s\" e' palindroma\n", argv[1]);
    else printf("La parola \"%s\" non e' palindroma\n", argv[1]);
}