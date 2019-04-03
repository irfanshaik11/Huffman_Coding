//
//  heap.h
//  Huffman_Encoder
//
//  Created by 206452150 on 3/31/19.
//  Copyright © 2019 206452150. All rights reserved.
//

#ifndef heap_h
#define heap_h

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct Node{
    int val;
    char word[50];
    struct Node *heap_left;
    struct Node *heap_right;
    struct Node *huff_left;
    struct Node *huff_right;
};

int heap[1000000], heapSize, nextAvailableLevel, capacity, nextAvailableLeaf, prevFilledLeaf, prevFilledLevel;
struct Node *root = NULL;
void Init() {
//    printf("0");
    root = NULL;
    heapSize = 0;
    heap[0] = -INT_MAX;
    prevFilledLeaf = 0;
    prevFilledLevel = 0;
    nextAvailableLevel = 0;
    capacity = 1;
    nextAvailableLeaf = 1;
}

struct Node create(int val, char word[50], struct Node *hl, struct Node *hr, struct Node *hfl, struct Node *hfr){
//    printf("3");
    struct Node newNode;
    strcpy(newNode.word, word);
    newNode.val = val;
    newNode.heap_left = hl;
    newNode.heap_right = hr;
    newNode.huff_left = hfl;
    newNode.huff_right = hfr;
    
    return newNode;
}

void Increment(){
    prevFilledLeaf = nextAvailableLeaf;
    prevFilledLevel = nextAvailableLevel;
    
    heapSize++;
    nextAvailableLeaf++;
    
    if(heapSize == capacity){
        capacity = capacity + pow(2, nextAvailableLevel+1);
        nextAvailableLeaf = 1;
        nextAvailableLevel++;
    }
};

void Decrement(){
    if(prevFilledLevel == 0){
        printf("\nCannot Decrement Further");
    }else{
        heapSize --;
        nextAvailableLeaf --;
        
        if(nextAvailableLeaf == 0){
            capacity = capacity - pow(2, nextAvailableLevel);
            nextAvailableLevel--;
            nextAvailableLeaf = pow(2,nextAvailableLevel);
        }
        
        prevFilledLeaf--;

        if(prevFilledLeaf == 0){
            prevFilledLevel--;
            prevFilledLeaf = pow(2,prevFilledLevel);
        }
        
        if (prevFilledLevel == -1){
            prevFilledLevel = NULL;
        }
        
    }
};

void swapNode(struct Node *n1, struct Node *n2){
//    if (n1 == NULL || n2 == NULL){
//        printf("\nERROR: ONE OF THE NODES IS INVALID");
//        return;
//    }
    
    int temp = n1->val;
    n1->val = n2->val;
    n2->val = temp;
    
    char temp_string[50];
    strcpy(temp_string,n1->word);
    strcpy(n1->word, n2->word);
    strcpy(n2->word, temp_string);
    
    struct Node *temp_node_left = n1->heap_left;
    struct Node *temp_node_right = n1->heap_right;

//    n2->heap_left = n1->heap_left;
//    n2->heap_right = n1->heap_right;
//
//    n1->heap_left = temp_node_left;
//    n1->heap_right = temp_node_right;
    
    temp_node_right = n1->huff_right;
    temp_node_left = n1->huff_left;
//
    n2->huff_right = n1->huff_right;
    n2->huff_left = n1->huff_left;
//
    n1->huff_right = temp_node_right;
    n2->huff_left = temp_node_left;
    
}
void insert(int val){
//    printf("4");
    printf("\n %d level %d\n", val, nextAvailableLevel);
    if(root == 0){
        char emptyword[50];
        struct Node newNode = create(val, emptyword, NULL, NULL, NULL, NULL);
        struct Node *newNodeptr;
        newNodeptr = malloc (sizeof(struct Node));
        *newNodeptr = newNode;
        root = newNodeptr;
        
    }else{
        //      1
        //   2      3
        // 4  5   6  7
        char emptyword[50];
        struct Node newNode = create(val,emptyword, NULL, NULL, NULL, NULL);
        struct Node *newleaf;
        newleaf = malloc (sizeof(struct Node));
        *newleaf = newNode;
        
        struct Node *rptr = root;
        int curr_pos = nextAvailableLeaf;
        int leaf_space = pow(2, nextAvailableLevel);
        int curr_level = 0;
        
        
        while(1==1){
//            printf("\n%d|%d", curr_level, level);
            if (curr_level + 1 == nextAvailableLevel){
                if (curr_pos == 1){rptr->heap_left = newleaf;}
                else{rptr->heap_right = newleaf;};
                break;
            }
            
            if(rptr->val > val){
                swapNode(rptr, newleaf);
            }
            
            int mid = (int) ((float) leaf_space / 2.00);
            if (curr_pos > mid){
                rptr = rptr->heap_right;
                curr_pos = curr_pos - mid;
            }else{
                rptr = rptr->heap_left;
            }
            leaf_space = leaf_space - mid;
            curr_level++;
            printf("\n leaf space %d | curr pos %d\n", leaf_space, curr_pos);
        }
    }
    
    printf("here");
    Increment();
    printf("done");
    
};

void insertNode(struct Node *newNodeptr){
    //    printf("4");
    if(root == 0){
        char emptyword[50];
        root = newNodeptr;
    }else{
        
        //      1
        //   2      3
        // 4  5   6  7
    
        

        struct Node *rptr = root;
        int curr_pos = nextAvailableLeaf;
        int leaf_space = pow(2, nextAvailableLevel);
        int curr_level = 0;
        
        printf("newNodeptr->val %d", newNodeptr->val);
        while(1==1){
            printf("rptr->val %d", rptr->val);

            //            printf("\n%d|%d", curr_level, level);
            if (curr_level + 1 == nextAvailableLevel){
                if (curr_pos == 1){rptr->heap_left = newNodeptr;}
                else{rptr->heap_right = newNodeptr;};
                break;
            }
            
            if(rptr->val > newNodeptr->val){
                swapNode(rptr, newNodeptr);
            }
            
            int mid = (int) ((float) leaf_space / 2.00);
            if (curr_pos > mid){
                rptr = rptr->heap_right;
                curr_pos = curr_pos - mid;
            }else{
                rptr = rptr->heap_left;
            }
            leaf_space = leaf_space - mid;
            curr_level++;
            printf("\n leaf space %d | curr pos %d\n", leaf_space, curr_pos);
        }
    }
    
    
    printf("here");
    Increment();
    printf("done");
    
    
};

void heapify_helper(struct Node *rptr){
    
    if(rptr->heap_right==NULL && rptr->heap_left == NULL){
        //pass
    }else if(rptr->heap_right==NULL){
        
        if(rptr->heap_left->val < rptr->val){
            printf("Case 6.1");
            fflush(stdout);
            swapNode(rptr->heap_left, rptr);
            heapify_helper(rptr->heap_left);
        }
        
    }else if (rptr->heap_left==NULL){
        
        if(rptr->heap_right->val < rptr->val){
            printf("Case 6.2");
            fflush(stdout);
            swapNode(rptr->heap_right, rptr);
            heapify_helper(rptr->heap_right);
        }
        
    }
    else{
        if (rptr->heap_right->val < rptr->val || rptr->heap_left->val < rptr->val){
            if (rptr->heap_right->val < rptr->heap_left->val){
                printf("Case 6.3");
                fflush(stdout);
                swapNode(rptr, rptr->heap_right);
                heapify_helper(rptr->heap_right);
            }else{
                printf("Case 6.4");
                fflush(stdout);
                printf("rptr->val %d rptr %p rptr->heap_left %p", rptr->val, &rptr, &rptr->heap_left);
                printf("rptr");
                fflush(stdout);
                
                swapNode(rptr, rptr->heap_left);
                printf("hhhh");
                fflush(stdout);
                heapify_helper(rptr->heap_left);
            }
        }
    }
    
};

void heapify(struct Node *rptr){
    struct Node *original = rptr;
    for (int target_level = nextAvailableLevel-1; target_level >= 0; target_level--){
        for (int curr_pos = pow(2,target_level); curr_pos > 0; curr_pos--){
            printf("\ntarget level %d curr_pos %d", target_level, curr_pos);
            int curr_level = 0;
            int leaf_space = pow(2,target_level);
            rptr = original;


            while(rptr != NULL){
                if(curr_level == target_level){
                    break;
                }

                int mid = (int) ( (float) leaf_space / 2.00);
                printf("\n mid: %d curr_pos: %d curr_level %d target_level %d", mid, curr_pos, curr_level, target_level);

                if(curr_pos > mid){

                    printf("\nright");
                    rptr = rptr->heap_right;
                    curr_pos = curr_pos - mid;
                }else{
                    printf("\nleft");
                    rptr = rptr->heap_left;
                }
                leaf_space = leaf_space - mid;
                curr_level++;
            }

            printf("heapify here");
            if(rptr->heap_right==NULL && rptr->heap_left == NULL){
                //pass
            }else if(rptr->heap_right==NULL){
                if(rptr->heap_left->val < rptr->val){

                    printf("case 2");
                    fflush(stdout);
                    swapNode(rptr->heap_left, rptr);
                    heapify_helper(rptr->heap_left);
                }

            }else if (rptr->heap_left==NULL){
                if(rptr->heap_right->val < rptr->val){
                    printf("case 3");
                    fflush(stdout);
                    swapNode(rptr->heap_right, rptr);
                    heapify_helper(rptr->heap_right);
                }

            }
            else{
                if (rptr->heap_right->val < rptr->val || rptr->heap_left->val < rptr->val){
                    if (rptr->heap_right->val < rptr->heap_left->val){
                        printf("case 4.56");
                        fflush(stdout);
                        swapNode(rptr, rptr->heap_right);
                        heapify_helper(rptr->heap_right);
                    }else{
                        printf("case 4.75");
                        fflush(stdout);
                        swapNode(rptr, rptr->heap_left);
                        heapify_helper(rptr->heap_left);
                    }
                }
            }

        }
    }

    printf("heqpify quit");
};
//
struct Node *pop(){
    if (capacity == 0){
        printf("Tree is Empty");
        return NULL;
    }
    
    if (root == NULL){
        printf("Cannot Pop from Empty Tree");
        return root;
    }
    
    if (root->heap_left == NULL && root->heap_right == NULL){
        struct Node * temp = root;
        root = NULL;
        return temp;
        
    }
    
    int target_level = prevFilledLevel;
    int curr_level = 0;
    int leaf_space = pow(2, prevFilledLevel);
    int curr_pos = prevFilledLeaf;

    struct Node *rptr = root;
    struct Node *returnNode = NULL;
//    printf("pop %d", curr_pos);
    while (rptr != NULL){

        if (curr_level + 1 == target_level){
            if (curr_pos == 1){
                if (rptr->heap_left != NULL){
                    printf("heap_left is NULL");
                    fflush(stdout);
                    break;
                }
                
                returnNode = rptr->heap_left;
                rptr->heap_left = NULL;
                break;
                
            }
            else{
                swapNode(rptr->heap_right, root);
                returnNode = rptr->heap_right;
                rptr->heap_right = NULL;

                break;
            };

        }

        int mid = (int) ((float) leaf_space / 2.00);
        if (curr_pos > mid){
            rptr = rptr->heap_right;
            curr_pos = curr_pos - mid;
        }else{
            rptr = rptr->heap_left;
        }
        leaf_space = leaf_space - mid;
        curr_level++;
    }

    Decrement();
    return returnNode;
};



//struct Node *search(int val, struct Node *leaf){
//    if(leaf != 0){
//
//        if(val == leaf->val){
//            return leaf;
//        }
//        else if(val < leaf->val){
//            return search(val, leaf->val);
//        }
//        else{
//            return search(val, leaf->heap_right);
//        }
//
//    }
//
//    else{
//        return 0;
//    }
//
//}

//int Insert(int element) {
//    if(root == NULL){
//        struct Node newNode;
//        newNode.val = element;
//        newNode.heap_left = NULL;
//        newNode.heap_right = NULL;
//        newNode.huff_left = NULL;
//        newNode.huff_right = NULL;
//        heapSize++;
//        return 1;
//    }
//
//
//
////    heapSize++;
////    heap[heapSize] = element; /*Insert in the last place*/
////    /*Adjust its position*/
////    int now = heapSize;
////    while (heap[now / 2] > element) {
////        heap[now] = heap[now / 2];
////        now /= 2;
////    }
////    heap[now] = element;
//    return 1;
//}

//int DeleteMin() {
//    /* heap[1] is the minimum element. So we remove heap[1]. Size of the heap is decreased.
//     Now heap[1] has to be filled. We put the last element in its place and see if it fits.
//     If it does not fit, take minimum element among both its children and replaces parent with it.
//     Again See if the last element fits in that place.*/
//    int minElement, lastElement, child, now;
//    minElement = heap[1];
//    lastElement = heap[heapSize--];
//    /* now refers to the index at which we are now */
//    for (now = 1; now * 2 <= heapSize; now = child) {
//        /* child is the index of the element which is minimum among both the children */
//        /* Indexes of children are i*2 and i*2 + 1*/
//        child = now * 2;
//        /*child!=heapSize beacuse heap[heapSize+1] does not exist, which means it has only one
//         child */
//        if (child != heapSize && heap[child + 1] < heap[child]) {
//            child++;
//        }
//        /* To check if the last element fits ot not it suffices to check if the last element
//         is less than the minimum element among both the children*/
//        if (lastElement > heap[child]) {
//            heap[now] = heap[child];
//        } else /* It fits there */
//        {
//            break;
//        }
//    }
//    heap[now] = lastElement;
//    return minElement;
//}
#endif /* heap_h */


//        while(1==1){
////            printf(".");
//            if(val > rptr->val){
//
//                if(rptr->heap_right == NULL){
//                    struct Node newNode = create(val, NULL, NULL, NULL, NULL);
//                    struct Node *newNodeptr;
//                    newNodeptr = malloc(sizeof(struct Node));
//                    *newNodeptr = newNode;
//                    rptr->heap_right = newNodeptr;
//                    break;
//                }else{
//                    rptr = rptr -> heap_right;
//                }
//
//            }else{
//
//                if(rptr->heap_left == NULL){
//                    struct Node newNode = create(val, NULL, NULL, NULL, NULL);
//                    struct Node *newNodeptr;
//                    newNodeptr = malloc(sizeof(struct Node));
//                    *newNodeptr = newNode;
//                    rptr->heap_left = newNodeptr;
//                    break;
//                }else{
//                    rptr = rptr->heap_left;
//                }
//            }
//        }