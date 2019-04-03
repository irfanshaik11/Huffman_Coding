//
//  huffman_tree.h
//  Huffman_Encoder
//
//  Created by 206452150 on 3/31/19.
//  Copyright © 2019 206452150. All rights reserved.
//

#ifndef huffman_tree_h
#define huffman_tree_h
struct h_Node{
    char word[50];
    unsigned int frequency;
    struct MinHeapNode *left;
    struct MinHeapNode *right;
};

typedef struct h_Node huffman_node;

huffman_node *huffman_node_create(char word[50], unsigned int frequency){
    huffman_node *node = malloc(sizeof(huffman_node));
    if (node){
        strcpy(node->word, word);
        node->frequency = frequency;
        node->left = 0;
        node->right = 0;
    }
    return node;
}

void huffman_node_delete(huffman_node *node){
    if (node){
        huffman_node_delete(node->left);
        huffman_node_delete(node->right);
        free(node);
    }
}




void HuffmanCodes(char data[], int freq[], int size){
    
}

#include <stdio.h>



#endif /* huffman_tree_h */
