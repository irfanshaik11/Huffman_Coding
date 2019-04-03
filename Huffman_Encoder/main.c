#include <stdio.h>
#include "heap.h"

void inorder(struct Node *ptr, int cnt){
    
    if (ptr != NULL){

        if (ptr->heap_left != NULL){inorder(ptr->heap_left, cnt+1);}
        printf("%d | level %d\n", ptr->val, cnt);
        if (ptr->heap_right != NULL){inorder(ptr->heap_right, cnt+1);}

    }
    
}

void huff_inorder(struct Node *ptr, int cnt){
    if (ptr != NULL){
        
        if (ptr->heap_left != NULL){inorder(ptr->huff_left, cnt+1);}
        printf("%d | level %d\n", ptr->val, cnt);
        if (ptr->heap_right != NULL){inorder(ptr->huff_right, cnt+1);}
    
    }
    
}

int main() {
    Init();

    for(int i = 0; i < 10; i++){
        printf("Insert %d",i);
        insert(i);
    }

    for(int i = 0; i <= 9; i++){
        printf("Capacity %d, Level %d, pos %d\n", capacity, nextAvailableLevel, nextAvailableLeaf);
        fflush(stdout);
        pop();
    }
    
    
    
    inorder(root, 1);
    return 0;
}

//struct Node combineNodes(struct Node *n1, struct Node *n2){
//    int sumval = n1->val + n2->val;
//    char word[50];
//    struct Node returnNode = create(sumval, word, NULL, NULL, n1, n2);
//    return returnNode;
//};
//
//struct Node *create_HuffmanTree(){
//    if (root == 0){
//        printf("ERROR: Binary Heap Does Not Exist");
//        return root;
//    }
//
//    struct Node *rptr = root;
//    struct Node *Node1 = NULL;
//    struct Node *Node2 = NULL;
//    struct Node *returnNode = NULL;
//
//    while(rptr != NULL){
//        Node1 = pop();
//        returnNode = Node1;
//
//        if (Node1 == NULL){break;}
//        heapify(root);
//
//        Node2 = pop();
//        returnNode = Node2;
//
//        if (Node2 == NULL){break;}
//        heapify(root);
//
//        struct Node *combinedNode = NULL;
//        *combinedNode = combineNodes(Node1, Node2);
//        returnNode = combinedNode;
//    }
//
//    return returnNode;
//};


//    struct Node *huff_root = create_HuffmanTree();
//
//    printf("Huff_Root->val: %p", huff_root);
//    printf("\n Huff Inorder \n ----------- \n");
//    huff_inorder(huff_root, 1);

    
//    int number_of_elements;
//    printf("Program to demonstrate Heap:\nEnter the number of elements: ");
//    scanf("%d", &number_of_elements);
//    int iter, element;
//    Init();
//    printf("Enter the elements: ");
//    for (iter = 0; iter < number_of_elements; iter++) {
//        scanf("%d", &element);
//        Insert(element);
//    }
//
//    for (int i = 1; i < 5; i++){
//        printf(" %d %d", heap[i], i);
//    }

//    for (iter = 0; iter < number_of_elements; iter++) {
//        int x = DeleteMin();
//        printf("\n%d", x);
//        printf("\n%d", heapSize);
//
//    }


//        struct Node newNode = create(val, emptyword, NULL, NULL, NULL, NULL);
//        struct Node *newNodeptr;
//        newNodeptr = malloc (sizeof(struct Node));
//        *newNodeptr = newNode;
//        root = newNodeptr;


