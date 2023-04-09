#include "imagefuncs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc <= 4 || argc > 5) {
        printf("Invalid number of arguments!\n");
        printf("Usage: \n");
        printf("./quadtree [-c1 factor | -c2 factor | -d] [fisier_intrare] [fisier_iesire]\n");
        return -1;
    }
    if (strcmp(argv[1], "-c1") == 0) {
        double factor;
        sscanf(argv[2], "%lf", &factor);

    }
    if (strcmp(argv[1], "-c2") == 0) {
        double factor;
        sscanf(argv[2], "%lf", &factor);

        FILE *ppm_in = fopen(argv[3], "r");
        FILE *out_file = fopen(argv[4], "w");

        unsigned int size;
        TPixel **pixels = get_pixels(ppm_in, &size);
        //printf("%d\n", size);
        TNode *tree = construct_tree(pixels, 0, 0, size, factor);

        fwrite(&size, sizeof(unsigned int), 1, out_file);

        binary_print_tree(out_file, tree);
        
        //deallocs
        delete_tree(tree);
        delete_pixels(pixels, size);
        fclose(ppm_in);
        fclose(out_file);
    }
    if (strcmp(argv[1], "-d") == 0) {

    }
}