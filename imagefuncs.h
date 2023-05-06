/*ROTARU Razvan-Andrei 315CB*/
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NR_FII 4

typedef struct {
    unsigned char r, g, b;
}TPixel;



typedef struct Node{
    TPixel pixel;
    unsigned char type;
    struct Node *next[4];
}TNode;


typedef struct qnode{
    void *data;
    struct qnode *next;
}QNode;
typedef struct {
    QNode *front, *back;
}TQueue;

TPixel **get_pixels(FILE *ppm_in, unsigned int *size);


TNode *construct_tree(TPixel **pixels, int l, int c, int size, __uint64_t prag);
void binary_print_tree(FILE *out_file, TNode *node);
void delete_tree(TNode *node);
void delete_pixels(TPixel **pixels, unsigned int size);
TNode *get_tree(FILE *ppm_compressed, unsigned int *size);
TPixel **get_pixels_from_tree(TNode *base, unsigned int size);
void write_pixels_to_ppm(FILE *ppm_out, TPixel **pixels, unsigned int size);

unsigned int count_leaves(TNode *tree);
unsigned int find_min_depth(TNode *tree);
unsigned int find_max_depth(TNode *tree);