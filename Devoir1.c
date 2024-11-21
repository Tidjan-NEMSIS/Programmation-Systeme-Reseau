#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

int f(int n) {
    return pow(2, n);
}

int g(int n) {
    return 2 * n;
}

int main(int argc, char* argv[]) {
    int opt;
    int n;

    while ((opt = getopt(argc, argv, "f:g:")) != -1) {
        if (argc == 3) {
            switch (opt) {
                case 'f':
                    n = atoi(optarg);
                    printf("Calcul de 2^%d = %d\n", n, f(n));
                    break;
                case 'g':
                    n = atoi(optarg);
                    printf("Calcul de 2*%d = %d\n", n, g(n));
                    break;
                default:
                    printf("Procédé: %s -f entier ou -g entier\n", argv[0]);
                    exit(1);
            }
        }
        else if (argc > 3) {
           
            if ((strcmp(argv[3], "-f") == 0 || strcmp(argv[3], "-g") == 0) && 
                isdigit(atoi(argv[2]) + '0')) {
                switch (opt) {
                    case 'f':
                        n = atoi(optarg);
                        printf("Calcul de gof(%d) = %d\n", n, g(f(n)));
                        break;
                    case 'g':
                        n = atoi(optarg);
                        printf("Calcul de fog(%d) = %d\n", n, f(g(n)));
                        break;
                    default:
                        printf("Procédé: -g (ou -f) entier -f (ou -g)\n");
                        exit(1);
                }
            }
        }
    }
    return 0;
}