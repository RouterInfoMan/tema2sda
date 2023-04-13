#include "imagefuncs.h"

const TPixel NULL_PIXEL = {0, 0, 0};

QNode *alloc_QNode(void *data) {
   QNode *node = malloc(sizeof(QNode));
   node->data = data;
   node->next = NULL; 
   return node;
}
void *delete_QNode(QNode **nod) {
    void *aux = (*nod)->data;
    free(*nod);
    *nod = NULL;
    return aux;
}
TQueue *init_Q() {
    TQueue *q = malloc(sizeof(TQueue));
    q->front = q->back = NULL;
    return q;
}
void push_q(TQueue *q, void *data) {
    QNode *nod = alloc_QNode(data);
    if(q->front == q->back && q->front == NULL) {
        q->front = q->back = nod;
    } else {
        q->back->next = nod;
        q->back = nod;
    }
}
void *pop_q(TQueue *q) {
    if(q->front == NULL) {
        return NULL;
    } else {
        QNode *aux = q->front;
        q->front = q->front->next;
        if (!q->front) {
            q->back = q->front;
        }
        return delete_QNode(&aux);
    }
}
void destroy_q(TQueue **q) {
    while((*q)->front) {
        QNode *aux = (*q)->front;
        (*q)->front = (*q)->front->next;
        delete_QNode(&aux);
    }
    free(*q);
    *q = NULL;
}




TPixel compute_mean(TPixel **pixels, int l, int c, int size) {
    double mr, mg, mb;
    mr = mg = mb = 0;
    
    int i, j;
    for (i = l; i < l + size; i++) {
        for (j = c; j < c + size; j++) {
            mr += (double)(pixels[i][j].r);
            mg += (double)(pixels[i][j].g);
            mb += (double)(pixels[i][j].b);
        }
    }

    TPixel mean;
    mean.r = mr / (size * size);
    mean.g = mg / (size * size);
    mean.b = mb / (size * size);

    return mean;
}

double compute_similarity(TPixel **pixels, int l, int c, int size, TPixel mean) {
    double similarity = 0;
    
    int i, j;
    for (i = l; i < l + size; i++) {
        for (j = c; j < c + size; j++) {
            double difr = (double)mean.r - (double)pixels[i][j].r;
            double difg = (double)mean.g - (double)pixels[i][j].g;
            double difb = (double)mean.b - (double)pixels[i][j].b;
            //printf("%lf %lf %lf\n", difr, difg, difb);
            similarity += difr * difr;
            similarity += difg * difg;
            similarity += difb * difb;
        }
    }

    similarity /= (3 * size * size);

    return similarity;
}
TNode *alloc_TNode(int type, TPixel pixel) {
    TNode *nod = (TNode *) malloc(sizeof(TNode));
    nod->type = type;
    nod->pixel = pixel;
    int i;
    for (i = 0; i < NR_FII; i++) {
        nod->next[i] = NULL;
    }
    return nod;
}
TNode *construct_tree(TPixel **pixels, int l, int c, int size, double prag) {
    if(size == 1) { 
        return alloc_TNode(1, pixels[l][c]);
    }
    
    TPixel mean = compute_mean(pixels, l, c, size);
    double similarity = compute_similarity(pixels, l, c, size, mean);
    
    TNode *nod;
    if (similarity <= prag) {
        nod = alloc_TNode(1, mean);
    } else {
        nod = alloc_TNode(0, NULL_PIXEL);
        nod->next[0] = construct_tree(pixels, l         , c         , size/2, prag);
        nod->next[1] = construct_tree(pixels, l         , c + size/2, size/2, prag);
        nod->next[2] = construct_tree(pixels, l + size/2, c + size/2, size/2, prag);
        nod->next[3] = construct_tree(pixels, l + size/2, c         , size/2, prag);
    }
    return nod;
}
void delete_tree(TNode *node) {
    if (node->type == 1) {
        free(node);
    } else {
        int i;
        for (i = 0; i < NR_FII; i++) {
            delete_tree(node->next[i]);
        }
        free(node);
    }
}
void binary_print_tree(FILE *out_file, TNode *node) {
    TQueue *q = init_Q();
    
    push_q(q, node);
    
    QNode *level_end = q->back;
    TNode *current;
    int level = 0;
    
    while(q->front) {
        while(q->front != level_end) {
            current = pop_q(q);
            if (current->type == 0) {
                fputc(0, out_file);
                int i;
                for (i = 0; i < NR_FII; i++) {
                    push_q(q, current->next[i]);
                }
            } else {
                fputc(1               , out_file);
                fputc(current->pixel.r, out_file);
                fputc(current->pixel.g, out_file);
                fputc(current->pixel.b, out_file);
            }
        }
        current = pop_q(q); 
        if (current->type == 0) {
            fputc(0, out_file);
            int i;
            for (i = 0; i < NR_FII; i++) {
                push_q(q, current->next[i]);
            }
        } else {
            fputc(1               , out_file);
            fputc(current->pixel.r, out_file);
            fputc(current->pixel.g, out_file);
            fputc(current->pixel.b, out_file);
        }
        //fprintf(out_file, "\n");
        level_end = q->back;
        level++;
    }
    destroy_q(&q);
}
void human_readable_print_tree(FILE *out_file, TNode *node) {
    
    TQueue *q = init_Q();
    
    push_q(q, node);
    
    QNode *level_end = q->back;
    TNode *current;
    int level = 0;
    
    while(q->front) {
        while(q->front != level_end) {
            current = pop_q(q);
            if (current->type == 0) {
                fprintf(out_file, "0 ");
                int i;
                for (i = 0; i < NR_FII; i++) {
                    push_q(q, current->next[i]);
                }
            } else {
                fprintf(out_file, "{1 %d %d %d} ", current->pixel.r, current->pixel.g, current->pixel.b);
            }
        }
        current = pop_q(q); 
        if (current->type == 0) {
            fprintf(out_file, "0 ");
            int i;
            for (i = 0; i < NR_FII; i++) {
                push_q(q, current->next[i]);
            }
        } else {
            fprintf(out_file, "{1 %d %d %d} ", current->pixel.r, current->pixel.g, current->pixel.b);
        }
        fprintf(out_file, "\n");
        level_end = q->back;
        level++;
    }
    destroy_q(&q);
}
void basic_tree_print(TNode *node) {
    if (!node) {
        return;
    }
    if (node->type == 0) {
        printf("0\n");
        int i;
        for (i = 0; i < NR_FII; i++) {
            basic_tree_print(node->next[i]);
        }
    } else {
        printf("{1 %d %d %d}\n", node->pixel.r, node->pixel.g, node->pixel.b);
    }
}
TPixel **get_pixels(FILE *ppm_in, unsigned int *size) {
    char dump[256];
    unsigned int width, height;
    int max_value;

    fgets(dump, 256, ppm_in); //antet
    fscanf(ppm_in, "%u %u", &width, &height);
    fscanf(ppm_in, "%d", &max_value);

    if (width != height || max_value != 255) {
        return NULL;
    }
    *size = width;
    fgetc(ppm_in);
    //fgets(dump, 256, ppm_in);

    TPixel **img =(TPixel **) malloc(sizeof(TPixel *) * *size);
    int i, j;
    for (i = 0; i < *size; i++) {
        img[i] = malloc(sizeof(TPixel) * *size);
    } 
    for (i = 0; i < *size; i++) {
        for (j = 0; j < *size; j++) {
            fread(&(img[i][j].r), sizeof(unsigned char), 1, ppm_in);
            fread(&(img[i][j].g), sizeof(unsigned char), 1, ppm_in);
            fread(&(img[i][j].b), sizeof(unsigned char), 1, ppm_in);
        }
    }

    //printf("%hhu %hhu %hhu\n", img[128][0].r, img[128][0].g, img[128][0].b);
    return img;
}
TNode *get_tree(FILE *ppm_compressed, unsigned int *size) {
    fread(size, sizeof(unsigned int), 1, ppm_compressed);

    TQueue *q = init_Q();
    unsigned char type;
    TNode *base, *current;
    TPixel tmp;
    fread(&type, sizeof(unsigned char), 1, ppm_compressed);
    if (type == 1) {
        fread(&tmp.r, sizeof(unsigned char), 1, ppm_compressed);
        fread(&tmp.g, sizeof(unsigned char), 1, ppm_compressed);
        fread(&tmp.b, sizeof(unsigned char), 1, ppm_compressed);
        base = alloc_TNode(1, tmp);
        destroy_q(&q);
        return base;
    }
    base = alloc_TNode(0, NULL_PIXEL);
    push_q(q, base);
    while (q->front) {
        current = pop_q(q);
        int i;
        for (i = 0; i < NR_FII; i++) {
            fread(&type, sizeof(unsigned char), 1, ppm_compressed);
            if (type == 1) {
                fread(&tmp.r, sizeof(unsigned char), 1, ppm_compressed);
                fread(&tmp.g, sizeof(unsigned char), 1, ppm_compressed);
                fread(&tmp.b, sizeof(unsigned char), 1, ppm_compressed);
                current->next[i] = alloc_TNode(1, tmp);
            } else {
                current->next[i] = alloc_TNode(0, NULL_PIXEL);
                push_q(q, current->next[i]);
            }
        }
    }
    destroy_q(&q);
    return base;
}
void fill_pixels(TPixel **pixels, TNode *tree, int l, int c, int curr_size) {
    if (tree->type == 1) {
        int i, j;
        for (i = l; i < l + curr_size; i++) {
            for (j = c; j < c + curr_size; j++) {
                pixels[i][j] = tree->pixel;
            }
        }
    } else {
        fill_pixels(pixels, tree->next[0], l                , c                , curr_size / 2);
        fill_pixels(pixels, tree->next[1], l                , c + curr_size / 2, curr_size / 2);
        fill_pixels(pixels, tree->next[2], l + curr_size / 2, c + curr_size / 2, curr_size / 2);
        fill_pixels(pixels, tree->next[3], l + curr_size / 2, c                , curr_size / 2);
    }
}
TPixel **get_pixels_from_tree(TNode *base, unsigned int size) {
    TPixel **pixels = malloc(sizeof(TPixel *) * size);
    int i;
    for (i = 0; i < size; i++) {
        pixels[i] = malloc(sizeof(TPixel) * size);
    }
    fill_pixels(pixels, base, 0, 0, size);
    return pixels;
}
void write_pixels_to_ppm(FILE *ppm_out, TPixel **pixels, unsigned int size) {
    fprintf(ppm_out, "P6\n");
    fprintf(ppm_out, "%u %u\n", size, size);
    fprintf(ppm_out, "%u\n", 255);

    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            fwrite(&(pixels[i][j].r), sizeof(unsigned char), 1, ppm_out);
            fwrite(&(pixels[i][j].g), sizeof(unsigned char), 1, ppm_out);
            fwrite(&(pixels[i][j].b), sizeof(unsigned char), 1, ppm_out);
        }
    }
}
unsigned int max(unsigned int a, unsigned int b) {
    if (a > b) {
        return a;
    }
    return b;
}
unsigned int min(unsigned int a, unsigned int b) {
    if (a < b) {
        return a;
    }
    return b;
}
unsigned int find_max_depth(TNode *tree) {
    if (tree->type == 1) {
        return 0;
    } else {
        unsigned int max_depth = 0;
        int i;
        for (i = 0; i < NR_FII; i++) {
            max_depth = max(max_depth, find_max_depth(tree->next[i]) + 1);
        }
        return max_depth;
    }
}
unsigned int count_leaves(TNode *tree) {
    if (tree->type == 1) {
        return 1;
    }
    unsigned int i, sum = 0;
    for (i = 0; i < NR_FII; i++) {
        sum += count_leaves(tree->next[i]);
    }
    return sum;
}
unsigned int find_min_depth(TNode *tree) {
    if (tree->type == 1) {
        return 0;
    } else {
        unsigned int min_depth = 9999999;
        int i;
        for (i = 0; i < NR_FII; i++) {
            min_depth = min(min_depth, find_min_depth(tree->next[i]) + 1);
        }
        return min_depth;
    }
}

void delete_pixels(TPixel **pixels, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(pixels[i]);
    }
    free(pixels);
}