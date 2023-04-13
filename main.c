#include "imagefuncs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc < 4 || argc > 5) {
        printf("Invalid number of arguments!\n");
        printf("Usage: \n");
        printf("./quadtree [-c1 factor | -c2 factor | -d] [fisier_intrare] [fisier_iesire]\n");
        return -1;
    }
    if (strcmp(argv[1], "-c1") == 0) {
        double factor;
        sscanf(argv[2], "%lf", &factor);

        FILE *in = fopen(argv[3], "rb");
        FILE *out = fopen(argv[4], "wb");

        if (!in || !out) {
            return -1;
        }
        
        unsigned int size;
        TPixel **pixels = get_pixels(in, &size);
        //printf("%d\n", size);
        TNode *tree = construct_tree(pixels, 0, 0, size, factor);

        fprintf(out, "%u\n", find_max_depth(tree));
        fprintf(out, "%u\n", count_leaves(tree));
        fprintf(out, "%u\n", size >> find_min_depth(tree));

        delete_tree(tree);
        delete_pixels(pixels, size);

        fclose(in);
        fclose(out);             

    }
    if (strcmp(argv[1], "-c2") == 0) {
        double factor;
        sscanf(argv[2], "%lf", &factor);

        FILE *in = fopen(argv[3], "rb");
        FILE *out = fopen(argv[4], "wb");

        if (!in || !out) {
            return -1;
        }

        unsigned int size;
        TPixel **pixels = get_pixels(in, &size);
        //printf("%d\n", size);
        TNode *tree = construct_tree(pixels, 0, 0, size, factor);

        fwrite(&size, sizeof(unsigned int), 1, out);

        binary_print_tree(out, tree);
        
        //deallocs
        delete_tree(tree);
        delete_pixels(pixels, size);

        fclose(in);
        fclose(out);
    }
    if (strcmp(argv[1], "-d") == 0) {
        FILE *in = fopen(argv[2], "rb");
        FILE *out = fopen(argv[3], "w");

        if (!in || !out) {
            return -1;
        }

        unsigned int size;
        TNode *tree = get_tree(in, &size);
        TPixel **pixels = get_pixels_from_tree(tree, size);
        write_pixels_to_ppm(out, pixels, size);

        delete_tree(tree);
        delete_pixels(pixels, size);

        fclose(in);
        fclose(out);
    }
}