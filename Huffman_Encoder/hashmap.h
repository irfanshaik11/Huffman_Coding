//
//  hashmap.h
//  Huffman_Encoder
//
//  Created by 206452150 on 4/1/19.
//  Copyright © 2019 206452150. All rights reserved.
//

#ifndef hashmap_h
#define hashmap_h

#define SIZE 5000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct DataItem{
    int count;
    char key[50];
    struct DataItem *next;
};

struct DataItem *hashArray[SIZE];
struct DataItem *dummyItem;
struct DataItem *item;

unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    
    while (c = *str++){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */}
    
    return hash;
}

struct DataItem *search(int key) {
    //get the hash
    int hashIndex = hash(key);
    
    //move in array until an empty
    while(hashArray[hashIndex] != NULL) {
        
        if(hashArray[hashIndex]->key == key){
            return hashArray[hashIndex];}
        
        //go to next cell
        ++hashIndex;
        
        //wrap around the table
        hashIndex %= SIZE;
    }
    
    return NULL;
}

struct DataItem *create_data_item(char key[50]){
    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    strcpy(item->key, key);
    item->count = 1;
    
    return item;

}
void insert(char key[50]) {
    
//    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
//    strcpy(item->data, key);
//    item->count++;
    
    //get the hash
    int hashIndex = hash(key);
    
    
    struct DataItem *root = hashArray[hashIndex];
    struct DataItem *prev = NULL;
    while(root != NULL){
        if (strcmp(root->key, key) == 0){
            break;
        }
        prev = root;
        root = root->next;
    }
    
    if (root == NULL){
        
        struct DataItem *newNode = create_data_item(key);
        hashArray[hashIndex] = newNode;
        
        if(prev == NULL){
            hashArray[hashIndex] = newNode;
            
        }else{
            newNode->next = root;
            prev->next = newNode;
            
        }
        
    }else{
        root->count++;
    }
    
}








#endif /* hashmap_h */
