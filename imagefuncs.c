#include "imagefuncs.h"

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

TNode *construct_tree(TPixel **pixels, int l, int c, int size, double prag) {
    if(size == 1) {
        TNode *nod = (TNode *) malloc(sizeof(TNode));
        nod->pixel = pixels[l][c];
        nod->type = 1;
        nod->node1 = nod->node2 = nod->node3 = nod->node4 = NULL;
        return nod;
    }
    TNode *nod = (TNode *) malloc(sizeof(TNode));
    TPixel mean = compute_mean(pixels, l, c, size);
    double similarity = compute_similarity(pixels, l, c, size, mean);
    //printf("%lf\n", similarity);
    if (similarity <= prag) {
        nod->pixel = mean;
        nod->node1 = nod->node2 = nod->node3 = nod->node4 = NULL;
        nod->type = 1;
    } else {
        nod->type = 0;
        nod->node1 = construct_tree(pixels, l         , c         , size/2, prag);
        nod->node2 = construct_tree(pixels, l         , c + size/2, size/2, prag);
        nod->node3 = construct_tree(pixels, l + size/2, c + size/2, size/2, prag);
        nod->node4 = construct_tree(pixels, l + size/2, c         , size/2, prag);
    }
    return nod;
}
void delete_tree(TNode *node) {
    if (node->type == 1) {
        free(node);
    } else {
        delete_tree(node->node1);
        delete_tree(node->node2);
        delete_tree(node->node3);
        delete_tree(node->node4);
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
                push_q(q, current->node1);
                push_q(q, current->node2);
                push_q(q, current->node3);
                push_q(q, current->node4);
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
            push_q(q, current->node1);
            push_q(q, current->node2);
            push_q(q, current->node3);
            push_q(q, current->node4);
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
                push_q(q, current->node1);
                push_q(q, current->node2);
                push_q(q, current->node3);
                push_q(q, current->node4);
            } else {
                fprintf(out_file, "{1 %d %d %d} ", current->pixel.r, current->pixel.g, current->pixel.b);
            }
        }
        current = pop_q(q); 
        if (current->type == 0) {
            fprintf(out_file, "0 ");
            push_q(q, current->node1);
            push_q(q, current->node2);
            push_q(q, current->node3);
            push_q(q, current->node4);
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
        basic_tree_print(node->node1);
        basic_tree_print(node->node2);
        basic_tree_print(node->node3);
        basic_tree_print(node->node4);
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
void delete_pixels(TPixel **pixels, unsigned int size) {
    int i;
    for (i = 0; i < size; i++) {
        free(pixels[i]);
    }
    free(pixels);
}