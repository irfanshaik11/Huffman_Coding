#include "heap.h"
#include "dir_reader.h"
#include "wordfreq.h"
#include "inorder.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>


int checkFilepath(char *);
int encode_and_write(char *, char *, FILE **);

struct huff_node{
    char word[50];
    char code[50];
    int rightsize, leftsize;
    struct huff_node *left, *right;
};

struct huff_node *huff_root = NULL;
struct huff_node *original = NULL;

void buildHuffmanTreeFromCode(char word[50], int freq, char code[100]){
    char nonterminal[50] = "!NONTERMINAL!";
    if (huff_root == NULL){
        huff_root = malloc(sizeof(struct huff_node));
        strcpy(huff_root->word,nonterminal);
        huff_root->left = huff_root->right = NULL;
    }
    
    struct huff_node *myroot = huff_root;
//    printf("%s", word);
    fflush(stdout);
    
    int i = 0;
    int j = 0;
    for(i=0; i < 99; i++){
//        printf("%c \n", code[i]);
        if (code[i+1] == '\0'){
            j = i;
            break;
        }
        
        if (code[i] == '0'){
//            printf("Did not make it here \n");
            //printf("ROOT %ld\n", myroot);
            if(myroot->left == NULL){
//                printf("here 1\n");
                fflush(stdout);
                myroot->left = malloc(sizeof(struct huff_node));
                myroot = myroot->left;
                myroot->left = NULL;
                myroot->right = NULL;
//                printf("here 1 close\n");
//                printf("here 2\n");
                strcpy(myroot->word, nonterminal);

//                printf("here 2 close\n");
            }else{
//                printf("here 3\n");
                myroot = myroot->left;
//                printf("here 3 close\n");
                
            }
            
            
        }else{
            if(myroot->right == NULL){
//                printf("here 4\n");
                myroot->left = malloc(sizeof(struct huff_node));
                myroot = myroot->left;
                myroot->left = NULL;
                myroot->right = NULL;

//                printf("here4 close\n");
//                printf("here 5 open\n");
                strcpy(myroot->word, nonterminal);
//                printf("here 5 close\n");
            }else{
//                printf("here 6\n");
                myroot = myroot->right;
//                printf("here 6 close\n");
            }
        }
    }
    
//    printf("exited while loop %ld \n", myroot);
    
    if (code[j] == '0'){
        myroot->left = malloc(sizeof(struct huff_node));
        myroot = myroot->left;
        myroot->left = myroot->right = NULL;

    }else{
        myroot->right = malloc(sizeof(struct huff_node));
        myroot = myroot->right;
        myroot->left = myroot->right = NULL;

    }

    strcpy(myroot->word, word);
    
}


int decode_and_write(char *word, char filename[100], FILE **file_ptr){

    printf("have entered decode and write \n");

    struct huff_node *myroot;
    myroot = huff_root;
    printf("have reached huff_root: %p %s %s\n", myroot, myroot->word, word);
    
    for(char *inputPtr = word; *inputPtr; ++inputPtr){
        printf("*inputPtr is %c, (%d)\n", *inputPtr, *inputPtr);
        if (*inputPtr == '0'){
            printf("myroot is %p \n", myroot);
            if(myroot->left == NULL){
                printf("DAW: Could not decode %s\n", word);
                return -1;
            }
            myroot = myroot->left;
        }else if (*inputPtr == '1'){
            printf("myroot right is %p \n", myroot);
            if(myroot->right == NULL){
                printf("DAW: Could not decode %s\n", word);
                return -1;
            }
            myroot = myroot->right;
        }else{
            printf("unrecognized char: %c (%d)\n", *inputPtr);
        }
    }
    
    printf("DAW: writing %s to %s\n", myroot->word, filename);
    fprintf(*file_ptr, "%s", myroot->word);

    return 0;
    
//    for (word = word; word <strlen(word); word++){
//        if (strcmp(word,'0')==0){
//            if(myroot->left == NULL){
//                printf("Could not decode %s", word);
//                return -1;
//            }
//            myroot = myroot->left;
//        }else{
//            if(myroot->right == NULL){
//                printf("Could not decode %s", word);
//                return -1;
//            }
//            myroot = myroot->right;
//        }
//    }
//    printf("writing %s to %s\n",myroot->word, filename);
//    fprintf(*file_ptr, "%s ", myroot->word);
//
//    return 0;
}

// PART OF HASH FUNCTION buildLookupTreeFromCode
unsigned int hash(char *str)
{
    int hash = 5381;
    int c;
    
    while ((c = *str++)){
        
        hash = ((hash << 5) + hash) + c; }
    
    return hash;
}

//HASHMAP FUNCTION THAT BUILDS A FUNCTION TO LOOKUP A WORD
struct huff_node *word2code[1000]; // DECLARE THE HASHMAP
int buildLookupTreeFromCode(char *word, int freq, char *code){
    
    struct huff_node *myroot;
    
    int key = hash(word) % 1000;
    printf("Key is %d\n", key);
    if (word2code[key] == NULL){
        word2code[key] = malloc(sizeof(struct huff_node));
        word2code[key]->right = word2code[key]->left = NULL;
        strcpy(word2code[key]->word,word);
        strcpy(word2code[key]->code, code);
        return 1;
    }

    myroot = word2code[key];
    
    while (myroot != NULL){
//        printf("3 w\n");
//        printf("myword->word %p word %p\n", myroot->word, word);
//        printf("word %s myroot->word %s\n", word, myroot->word);
        if (strcmp(word, myroot->word) != 0){
//            printf("nomatch\n");
            if (myroot->right == NULL){
                printf("right is null\n");
                myroot->right = malloc(sizeof(struct huff_node));
                strcpy(myroot->right->code, code);
                strcpy(myroot->right->word, word);
                break;
            
            }else{
//                printf("right is not null\n");
                myroot = myroot->right;}
            
        }else{
//            printf("nomatch\n");
//            printf("Found a collision code for the word\n");
            strcpy(myroot->code, code);
            break;
        }
    }
    
    return 1;
} // end build lookup tree

int encodeFile(char *filename){
    char nonterminal[50] = "!NONTERMINAL!";
    Init();
    int i = 0;
    char word[50];
    FILE *ptr_file;
    char buf[1000];
    int linenum = 1;
    char delim[5] = " | ";
    int curr_freq;
    char curr_code[50];
    // READ HUFFMAN CODEBOOK TO EXTRACT THE CODE AND THE WORD
    ptr_file = fopen("HuffmanCodebook", "r");
    //        printf("opening huffman codebook");
    fflush(stdout);
    
    while (fgets(buf,1000, ptr_file)!=NULL){
        //            printf("\nline number: %d\n", linenum);
        fflush(stdout);
        
        if (linenum != 1){
            
            char *ptr = strtok(buf, delim);
            int cntr = 1;
            while(ptr!=NULL){
                if (cntr == 1){
                    strcpy(word, ptr);
                    printf("word: %s\n", word);
                    
                }else if (cntr == 2){
                    curr_freq = atoi(ptr);
                    
                }else if (cntr == 3){
                    //                        printf("SEGFAULT HERE\n");
                    strcpy(curr_code, ptr);
                    //                        printf("jk\n");
                    break;
                }
                //                    printf("NEXT\n");
                fflush(stdout);
                
                ptr = strtok(NULL, delim);
                cntr++;
            }
            //                printf("error here\n");
            buildLookupTreeFromCode(word, curr_freq, curr_code);
            //                printf("error not here\n");
        }
        
        linenum++;
    }
    fclose(ptr_file);
    //        printf("done opening codebook");
    fflush(stdout);
    
    FILE *ptr_file2;
    
    ptr_file2 = fopen(filename, "r");
    
    // CHECK IF FILEPATH IS VALID
    if (checkFilepath(filename) != 0){
        return -1;}
    
    //        printf("\nopening %s to be encoded\n", filename);
    char outfile[100];
    strcpy(outfile, filename);
    strcat(outfile, ".hcz");
    
    FILE *encodeFilePtr = fopen(outfile, "wb");
    
    while(fgets(buf,1000, ptr_file2)!=NULL){
        //            printf("\n Exec1kjljkl;j");
        char *ptr = strtok(buf, delim);
        int cntr;
        
        while(ptr!=NULL){
            //                printf("Executed %s \n", ptr);
            strcpy(word, ptr);
            //                printf("Got here %s\n", filename);
            encode_and_write(word, filename, &encodeFilePtr);
            //                printf("Got here 2\n");
            ptr = strtok(NULL, delim);
        }
    }
    fclose(encodeFilePtr);
    
    //        printf("\ndone opening file to be encoded");
    fflush(stdout);
    return 0;
}

int buildCodebook(char *filename){
    printf("\n      CREATING HUFFMANCODEBOOK FILE\n");
    printf("\n      Frequency | Word | Huffman Code");
    
//    printf("build Codebook\n");
    char nonterminal[50] = "!NONTERMINAL!";
    Init();
    int i = 0;
    char word[50];
    
//    printf("\nPassing %s to wordfreq\n", filename);
    
    wordfreq(filename);
    
    struct Node *origin;
    origin = malloc(sizeof(struct Node));
    
    while(1==1){
        if(heapSize >= 2){
//            printf("fails node1\n");
            struct Node *node1 = pop();
//            printf("fails node2\n");
            struct Node *node2 = pop();
//            printf("fails after node2\n");
            struct Node *combinedNode;
            combinedNode = malloc(sizeof(struct Node));
            combinedNode->huff_right = node1;
            combinedNode->huff_left = node2;
            if(node2 == NULL && node1 == NULL){printf("Both nodes are null error");return -1;}
            if(node2 == NULL){origin = node1; break;}
//            printf("node1 %p, node2 %p \n", node1, node2);
            combinedNode->val = node1->val + node2->val;
            strcpy(combinedNode->word, nonterminal);
//            printf("fails on insertNode\n");
            insertNode(combinedNode);
//            printf("fails after insertNode\n");
//            printf("heap size is %d\n", heapSize);
//        }else if(heapSize == 2){
//            origin = pop();
//            break;
        }else if (heapSize == 1){
            origin = pop();
            break;
        }
        else{
            printf("\nERROR: Heap is Empty\n");
            printf("heap size is %d\n", heapSize);
            break;
        }
    }
    
    FILE *fptr;
    fptr = fopen("HuffmanCodebook", "w");
    fprintf(fptr,"Word | Frequency | Code\n");
    fclose(fptr);
    
    printLevelOrder(origin);
    
    
    printf("\n\n      FILE HUFFMANCODEBOOK HAS BEEN CREATED\n\n");

    return 0;
}


int encode_and_write(char *word, char *filename, FILE **file_ptr){
    printf("encode_and_write opened\n");
    char outfile[100];
    
    strcpy(outfile, filename);
    strcat(outfile, ".hcz");
    struct huff_node *myroot;
    int key = hash(word) % 1000;
    printf("\nkey is %d", key);
    myroot = word2code[key];
//    printf("asd 1\n");
    if (myroot == NULL){
        printf("\nCould not encode word: %s", word);
        return -1;
    }
//    printf("encode&write 2\n");
    char outword[50];
    while(myroot != NULL){
//        printf("%p\n", myroot);
        if (strcmp(myroot->word, word) == 0){
//            printf("strcpy failed\n");
            strcpy(outword, myroot->code);
//            printf("strcpy succeeded\n");
            break;
        }
//        printf("\ncomparison succeeded\n");
        myroot = myroot->right;
    }
    
    printf("\nencode&write 3 %s\n", outfile);
//    FILE *file_ptr;
//    file_ptr = fopen(outfile, "r+");

    
    if (outword[0] != '\0'){
        fprintf(*file_ptr, "%s ", outword);
        
    }else{
        printf("ERROR: Could not encode word\n");
    }
    
//    fclose(file_ptr);
    return 0;
}

int decodeFile(char *filename){
    // BUILD HUFFMAN TREE
//    printf("Decode File\n");
    char nonterminal[50] = "!NONTERMINAL!";
    Init();
    int i = 0;
    
    FILE *ptr_file;
    char buf[1000];
    ptr_file = fopen("HuffmanCodebook", "r");
    
    
    int linenum = 1;
    char delim[] = " | ";
    char word[100];
    int curr_freq;
    char curr_code[100];
    
//    printf("We   while loop");
    //BUILD A LOOKUP TREE TO FIND THE WORD GIVEN A CODE
    while (fgets(buf,1000, ptr_file)!=NULL){
        if (linenum != 1){
            char *ptr = strtok(buf, delim);
            fflush(stdout);
            int cntr = 1;
            while(ptr!=NULL){
                if (cntr == 1){
                    strcpy(word, ptr);
                }else if (cntr == 2){
                    curr_freq = (int) ptr - '0';
                }else if (cntr == 3){
                    strcpy(curr_code, ptr);
                }
                ptr = strtok(NULL, delim);
                cntr++;
            }
            buildHuffmanTreeFromCode(word, curr_freq, curr_code);
        }
        linenum++;
    }
    fclose(ptr_file);
    //DECODE BASED ON HUFFMAN TREE
//    printf("Have reached file pointer\n");
    FILE *file_ptr2;
    char buf2[1000];
    char outfile[100];
    strcpy(outfile, filename);

    int eof = strlen(outfile);
    // Removing .hcz to output filepath
    for(i=eof-4; i < eof; i++){
        printf("outfile[%d]: %c\n", i, outfile[i]);
        outfile[i] = '\0';
    }
//    for(i=0; i < eof; i++){
//        printf("outfile[%d]: %c\n", i, outfile[i]);
////        outfile[i] = '\0';
//    }

    printf("reading file: %s\n", filename);
    printf("writing to file: %s\n", outfile);
    file_ptr2 = fopen(filename, "r");
    FILE *file_to_write_ptr;
    file_to_write_ptr = fopen(outfile,"w");
    printf("attempting to open file\n");
    while (fgets(buf2, 1000, file_ptr2)!= NULL){
//        printf("made it to while loop\n");
        
//        char *ptr = strtok(buf, delim2);
        
//        int cntr = 1;
//        while(ptr!=NULL){
        buf2[strcspn(buf2, "\n")] = '\0';

        printf("1. writing %s to %s\n", buf2, outfile);
        decode_and_write(buf2, outfile, &file_to_write_ptr);
//            printf("have passed decode and write \n");
//            ptr = strtok(NULL, delim2);
//            cntr++;
//        }
        //fclose(ptr);
    }
    printf("closing file");
    fclose(file_ptr2);
    
    return 0;
}

int checkFilepath(char *filename){
    char filepath[100];
    strcpy(filepath, filename);
    
    if( access(filepath, F_OK ) == -1 ) {
        printf("\n      Filepath Invalid");
        printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
        return -1;
    }
    
    return 0; // FILEPATH IS VALID
}



int main(int argc, char **argv){
    
    int debug = 1;
    
    if ((argc - 1) < 2){
        printf("\n      Incorrect Input");
        printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
        return -1;}
    
    int bflag, cflag, dflag, Rflag;
    bflag = cflag = dflag = Rflag = 0;
    
    for (int i = 0; i < argc; ++i){
        if(strcmp(argv[i],"-b")==0){bflag = 1;};if(strcmp(argv[i],"-R")==0){Rflag = 1;};
        if(strcmp(argv[i], "-c")==0){cflag = 1;};if(strcmp(argv[i], "-d")==0){dflag = 1;};}
    
   // THIS CODE RUNS WHEN R FLAG AND ANOTHER FLAG ARE BOTH UP
    if (Rflag == 1){
        init_listdir();
        listdirRecursively(argv[argc-1], 0);
        
        if(bflag + cflag + dflag > 1){
            printf("\n      Can only have one of flags: -b, -c, -d");
            printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
            return -1;
        }else if (cflag == 1){
            if(strcmp(argv[argc-1],"HuffmanCodebook")!=0){
                printf("\n      Filepath in Last Argument is Incorrect");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");

                return -1;}
            
            return encodeFile(argv[argc-2]);
        }else if(bflag == 1){
            
            if(checkFilepath(argv[argc-1])!=0){
            printf("\n      Filepath in Last Argument is Incorrect");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|");
            
            }
            return buildCodebook(argv[argc-2]);
        }else if (dflag == 1){
            if(checkFilepath(argv[argc-2])!= 0 || strcmp(argv[argc-1],"HuffmanCodebook")!=0 ){
                printf("\n      Incorrect Input");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
                return -1;}
            
            return decodeFile(argv[argc-2]);
        }
        return 0;
    }
    
    // THIS CODE RUNS WHEN ONLY ONE FLAG IS UP
    if ( (bflag + cflag + dflag > 1) ){
        printf("\n      Can only have one of flags: -b, -c, -d");
        printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
        return -1;
        
    }else if (bflag == 1){
        if(checkFilepath(argv[argc-1])!= 0){
            printf("\n      Filepath in Last Argument is Incorrect");
            printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
            return -1;}
        buildCodebook(argv[argc-1]);

    }else if (dflag == 1){
        if(checkFilepath(argv[argc-2])!= 0 || strcmp(argv[argc-1],"HuffmanCodebook")!=0){
            printf("\n      Incorrect Input");
            printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
            return -1;}

        decodeFile(argv[argc-2]);
    }else if (cflag == 1){
        if(checkFilepath(argv[argc-2])!= 0 || strcmp(argv[argc-1], "HuffmanCodebook")!=0){
            printf("\n      Incorrect Input");
            printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
            return -1;}
        
        encodeFile(argv[argc-2]);
    }else{
        printf("\n      No flags given or only R flag given");
        printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
        return -1;
    }
    
    return 0;
}







    
//    char nonterminal[50] = "!NONTERMINAL!";
//    Init();
//    int i = 0;
//    char word[50];
//    for(i=1; i < 10; i++){
//    insert(i, word);
//    }
//
//    struct Node *origin;
//    origin = malloc(sizeof(struct Node));
//    int while_counter = 0;
//    while(1==1){
//        while_counter++;
//        printf("while_counter: %d, heapSize: %d", while_counter, heapSize);
//        if(heapSize > 2){
//            struct Node *node1 = pop();
//            struct Node *node2 = pop();
//            struct Node *combinedNode;
//            combinedNode = malloc(sizeof(struct Node));
//            combinedNode->huff_right = node1;
//            combinedNode->huff_left = node2;
//            combinedNode->val = node1->val + node2->val;
//            strcpy(combinedNode->word, nonterminal);
//
//            insertNode(combinedNode);
//
//        }else if(heapSize == 2){
//            origin = pop();
//            break;
//        }else if (heapSize == 1){
//            origin = pop();
//            break;
//        }
//        else{
//            printf("\nERROR: Heap is Empty\n");
//            break;
//        }
//    }
////    inorder(root,1);
//
//    FILE *fptr;
//    fptr = fopen("HuffmanCodebook", "w");
//    fprintf(fptr,"Word | Frequency | Code\n");
//
//    fclose(fptr);
//
////    char null_code[50];
////    huff_inorder(origin, 1, null_code);
//    printLevelOrder(origin);

//    return 0;
//}


//void Init_huff(){
//
//    huff_root = malloc(sizeof(struct huff_node));
//
//    char nonterminal[50] = "!NONTERMINAL!";
//    strcpy(huff_root->word,nonterminal);
//
//    original = huff_root;
//
//    FILE *fptr;
//    fptr = fopen("HuffmanCodebook.txt", "w");
//    fclose(fptr);
//
//}



//void insert_huff(char insertword[50], int freq){
//
//    char nonterminal[50] = "!NONTERMINAL!";
//
//    struct huff_node *huff_rootptr;
//    huff_rootptr = malloc(sizeof(struct huff_node));
//    strcpy(huff_rootptr->word, insertword);
//    huff_root->left = huff_rootptr;
//
//    huff_root->right = malloc(sizeof(struct huff_node));
//    huff_root = huff_root->right;
//    strcpy(huff_root->word, nonterminal);
//    strcat(code, "1");
//
//}


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


//
//char nonterminal[50] = "!NONTERMINAL!";
//Init();
//int i = 0;
//
//FILE *ptr_file;
//char buf[1000];
//ptr_file = fopen("HuffmanCodebook", "r");
//
//int linenum = 1;
//char delim[] = " | ";
//char word[100];
//int curr_freq;
//char curr_code[100];
////BUILD LOOKUP TREE FROM CODE
//while (fgets(buf,1000, ptr_file)!=NULL){
//    if (linenum != 1){
//        char *ptr = strtok(buf, delim);
//        fflush(stdout);
//        int cntr = 1;
//        while(ptr!=NULL){
//            if (cntr == 1){
//                strcpy(word, ptr);
//            }else if (cntr == 2){
//                curr_freq = (int) ptr - '0';
//            }else if (cntr == 3){
//                strcpy(curr_code, ptr);
//            }
//            ptr = strtok(NULL, delim);
//            cntr++;
//        }
//        //                    fclose(ptr);
//        buildHuffmanTreeFromCode(word, curr_freq, curr_code);
//    }
//
//    linenum++;
//}
//fclose(ptr_file);
////DECODE BASED ON HUFFMAN TREE
//FILE *file_ptr2;
//char buf2[1000];
//
//while (fgets(buf2, 1000, file_ptr2)!= NULL){
//    char *ptr = strtok(buf, delim);
//    fflush(stdout);
//    int cntr = 1;
//    while(ptr!=NULL){
//        //                    encode_and_write(ptr, argv[argc-2]);
//        ptr = strtok(NULL, delim);
//        cntr++;
//    }
//    //                fclose(ptr);
//}
//
//
//fclose(file_ptr2);


//void inorder(struct Node *ptr, int cnt){
//
//    if (ptr != NULL){
////        printf("inorder");
////        fflush(stdout);
//        printf("%d | level %d\n", ptr->val, cnt);
////        printToCodebook(ptr->word, cnt);
////        insert_huff(ptr->word, cnt);
//
//
//        if (ptr->heap_left != NULL){inorder(ptr->heap_left, cnt+1);}
//        if (ptr->heap_right != NULL){inorder(ptr->heap_right, cnt+1);}
//    }
//
//}
