#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    unsigned char r, g, b;
}TPixel;

typedef struct Node{
    TPixel pixel;
    unsigned char type;
    struct Node *node1, *node2, *node3, *node4;
}TNode;


typedef struct qnode{
    void *data;
    struct qnode *next;
}QNode;
typedef struct {
    QNode *front, *back;
}TQueue;

TPixel **get_pixels(FILE *ppm_in, unsigned int *size);


TNode *construct_tree(TPixel **pixels, int l, int c, int size, double prag);
void basic_tree_print(TNode *node);
void human_readable_print_tree(FILE *out_file, TNode *node);
void binary_print_tree(FILE *out_file, TNode *node);
void delete_tree(TNode *node);
void delete_pixels(TPixel **pixels, unsigned int size);