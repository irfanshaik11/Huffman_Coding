//
//  hashmap.c
//  Huffman_Encoder
//
//  Created by 206452150 on 4/1/19.
//  Copyright © 2019 206452150. All rights reserved.
//

#include <fcntl.h>
#include "hashmap.h"
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *name = "/Users/HabibullahShaik/Desktop/Projects/Huffman_Encoder/Huffman_Encoder/heap.h";
    
    int fd = open( name, O_RDONLY);
    
    if (fd == 1){
        printf("One file wouldn't open!\n");
        return -1;
    }
    
    char c;
    int bytes;
    while ((bytes = read(fd, &c, sizeof(c))) > 0){
        if (c == '.'){
            printf("%d", bytes);
        }
        printf("\n%d", bytes);
        close(fd);
    }
    printf("\n");
    

    
//    char newword[50] = "abacd";
    
//    printf("%d", hash(newword));
    
    
}
