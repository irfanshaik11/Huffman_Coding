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
char *encode_and_write(char *, char *, FILE **);
void view_huff_root_inorder(struct Node *);

struct huff_node{
    char word[50];
    char code[100];
    int rightsize, leftsize;
    struct huff_node *left, *right;
};

struct huff_node *huff_root = NULL;
struct huff_node *original = NULL;

void buildHuffmanTreeFromCode(char word[50], int freq, char code[100]){
//    printf("Entering build huffman tree from code word: %s | freq: %d | code %s\n", word, freq, code);
    char nonterminal[50] = "!NONTERMINAL!";

    if (huff_root == NULL){
     
        printf("huff_root is NULL\n");
        huff_root = malloc(sizeof(struct huff_node));
        strcpy(huff_root->word,nonterminal);
        huff_root->left = huff_root->right = NULL;
    
    }
    
    struct huff_node *myroot;
    myroot = huff_root;
//    printf("reached here\n");
    
    int i = 0;
    int j = 0;
    
    for(i=0; i < 99; i++){
        
        printf("%c", code[i]);
        if (code[i+1] == '\0'){
            j = i;
            break;
        }
        
        if (code[i] == '0'){
            
            if(myroot->left == NULL){
                printf("left is Null\n");
                myroot->left = malloc(sizeof(struct huff_node));
                myroot = myroot->left;
                myroot->left = myroot->right = NULL;
                strcpy(myroot->word, nonterminal);
                
            }else{
            printf("left\n");
                myroot = myroot->left;}

        }else if (code[i]=='1'){
            if(myroot->right == NULL){
                printf("right is Null\n");
                myroot->left = malloc(sizeof(struct huff_node));
                myroot = myroot->left;
                myroot->left = myroot->right = NULL;
                strcpy(myroot->word, nonterminal);
                
            }else{
                printf("right\n");
                myroot = myroot->right;}
        }else{
            printf("\n attempting to traverse an unrecognized char %c", code[i]);
        }
        
    }
    
//    printf("code[%d]: %c, code[%d]: %c", j-1, code[j-1], j, code[j]);
    if (code[j-1] == '0'){
        printf("Final is left\n");
        if(myroot->left ==NULL){
            printf("left is empty\n");
            myroot->left = malloc(sizeof(struct huff_node));
            myroot = myroot->left;
            myroot->left = myroot->right = NULL;
            strcpy(myroot->word,nonterminal);
        }else{
            myroot = myroot->left;
        }

    }else if(code[j-1]=='1'){
        printf("Final is right\n");
        if (myroot->right ==NULL){
            printf("Right is empty\n");
            myroot->right = malloc(sizeof(struct huff_node));
            myroot = myroot->right;
            myroot->left = myroot->right = NULL;
            strcpy(myroot->word,nonterminal);
        }else{
            myroot = myroot->right;
        }

    }else{
        printf("\n attempting to write an unrecognized char %c", code[j-1]);
        return;
    }

    strcpy(myroot->word, word);
    
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
int buildReverseLookupTreeFromCode(char *word, int freq, char *code){
    
    struct huff_node *myroot;
    code[strlen(code)-1] = '\0';
    int key = hash(code) % 1000;
    //    printf("Key is %d\n", key);
    printf("\nHash is %d for code %s", key, code);
    
    if (word2code[key] == NULL){
        word2code[key] = malloc(sizeof(struct huff_node));
        word2code[key]->right = word2code[key]->left = NULL;
        strcpy(word2code[key]->word,word);
        strcpy(word2code[key]->code, code);
        return 1;
    }
    //    printf("done editing the root\n");
    myroot = word2code[key];
    //    printf("entering while loop\n");
    while (myroot != NULL){
        
        if (strcmp(word, myroot->word) != 0){
            //            printf("entering root->right\n");
            if (myroot->right == NULL){
                //                printf("right is null\n");
                myroot->right = malloc(sizeof(struct huff_node));
                strcpy(myroot->right->code, code);
                strcpy(myroot->right->word, word);
                //                printf("creating the right root\n");
                break;
                
            }else{
                //                printf("moving right\n");
                myroot = myroot->right;}
            
        }else{
            //            printf("copying code\n");
            strcpy(myroot->code, code);
            //            printf("successfully copied code\n");
            break;
        }
        
    }
    //    printf("exiting build lookup tree\n");
    return 1;

}

int buildLookupTreeFromCode(char *word, int freq, char *code){
//    printf("entering building lookup tree from code\n");
    struct huff_node *myroot;
    
    int key = hash(word) % 1000;
//    printf("Key is %d\n", key);
    printf("\nKey for %s is %d", word, key);
    if (word2code[key] == NULL){
        word2code[key] = malloc(sizeof(struct huff_node));
        word2code[key]->right = word2code[key]->left = NULL;
        strcpy(word2code[key]->word,word);
        strcpy(word2code[key]->code, code);
        return 1;
    }
//    printf("done editing the root\n");
    myroot = word2code[key];
//    printf("entering while loop\n");
    while (myroot != NULL){

        if (strcmp(word, myroot->word) != 0){
//            printf("entering root->right\n");
            if (myroot->right == NULL){
//                printf("right is null\n");
                myroot->right = malloc(sizeof(struct huff_node));
                strcpy(myroot->right->code, code);
                strcpy(myroot->right->word, word);
//                printf("creating the right root\n");
                break;
            
            }else{
//                printf("moving right\n");
                myroot = myroot->right;}
            
        }else{
//            printf("copying code\n");
            strcpy(myroot->code, code);
//            printf("successfully copied code\n");
            break;
        }
        
    }
//    printf("exiting build lookup tree\n");
    return 1;
} // end build lookup tree

int encodeFile(char *filename){
    char nonterminal[50] = "!NONTERMINAL!";
    Init();
    int i = 0;
    char word[50];
    char buf[1000];
    FILE *ptr_file;
    int linenum = 1;
    char delim[5] = " | ";
    int curr_freq;
    char curr_code[100];
    // READ HUFFMAN CODEBOOK TO EXTRACT THE CODE AND THE WORD
    ptr_file = fopen("HuffmanCodebook", "r");
    
    while (fgets(buf,1000, ptr_file)!=NULL){
        
        fflush(stdout);
        
        if (linenum != 1){
            printf("tokenizing new line\n");
            char *ptr = strtok(buf, delim);
            int cntr = 1;
            printf("exiting new line\n");
            while(ptr!=NULL){
                if (cntr == 1){
                    printf("failed on strcpy: word %s\n", ptr);
                    strcpy(word, ptr);
//                    printf("word: %s\n", word);
                    printf("strcpy succeeded\n");
                }else if (cntr == 2){
                    printf("converting %s to %d\n", ptr, atoi(ptr));
                    curr_freq = atoi(ptr);
                    printf("successfully converted\n");
                }else if (cntr == 3){
                    
                    printf("entering strcpy: word %s\n", ptr);
                    strcpy(curr_code, ptr);
                    printf("exiting strcpy\n");
                    break;
                }
                
                fflush(stdout);
                
                ptr = strtok(NULL, delim);
                cntr++;
            }
            printf("entering build lookup tree, word %s | curr_freq %d | curr_code: %s\n", word, curr_freq, curr_code);
            
            buildLookupTreeFromCode(word, curr_freq, curr_code);
            printf("exiting build lookup tree\n");
        }
        printf("incrementing line\n");
        linenum++;
    }
    printf("closing file\n");
    fclose(ptr_file);
    
    fflush(stdout);
    
    FILE *ptr_file2;
    printf("opening file: %s\n", filename);
    ptr_file2 = fopen(filename, "r");
    
    // CHECK IF FILEPATH IS VALID
    if (checkFilepath(filename) != 0){
        printf("Invalid File %s", filename);
        return -1; }
    
    
    char outfile[100];

    strcpy(outfile, filename);
    strcat(outfile, ".hcz");
//    printf("creating outfile %s\n", outfile);
    
    FILE *encodeFilePtr = fopen(outfile, "w");
    char *outword = malloc(100*sizeof(char));
    char delim2[5] = " ";
    while(fgets(buf,sizeof(buf), ptr_file2)!=NULL){
        
        fflush(stdout);
        char *ptr = strtok(buf, delim2);
        int cntr;
        
        while(ptr!=NULL){
            
            strcpy(word, ptr);
            
            outword = encode_and_write(word, filename, &encodeFilePtr);
            printf("writing outword: %s", outword);
            outword[strlen(outword)-1] = '\0';
            for(i=0; i < strlen(outword); i++){
                printf("outword[%d]: %c\n", i, outword[i]);
            }
            fprintf(encodeFilePtr, "%s ", outword);
            
            ptr = strtok(NULL, delim2);
            
        }
        
        printf("current word %s\n", buf);
    }
    
    fclose(encodeFilePtr);
    
    return 0;
}

int buildCodebook(char *filename, int recursive){
    printf("\n      CREATING HUFFMANCODEBOOK FILE\n");
    printf("\n      Frequency | Word | Huffman Code\n");
    
    char nonterminal[50] = "!NONTERMINAL!";
    Init();
    int i = 0;
    char word[50];
    
    wordfreq(filename);
    
    struct Node *origin;
    origin = malloc(sizeof(struct Node));
    

    while(1==1){
        if(heapSize >= 2){
            struct Node *node1 = pop();
            struct Node *node2 = pop();
            struct Node *combinedNode;
            combinedNode = malloc(sizeof(struct Node));
            combinedNode->huff_right = node1;
            combinedNode->huff_left = node2;
            if(node2 == NULL && node1 == NULL){printf("Both nodes are null error");return -1;}
            if(node2 == NULL){origin = node1; break;}
            combinedNode->val = node1->val + node2->val;
            strcpy(combinedNode->word, nonterminal);
            insertNode(combinedNode);
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
    
//    printf("\nViewing Huff Tree\n --------------------\n");
//    printf("left->left->left %s", origin->huff_left->huff_left->huff_left->huff_left->word);
//    view_huff_root_inorder(origin);
    
    if (recursive != 0){
        FILE *fptr;
        fptr = fopen("HuffmanCodebook", "w");
        fprintf(fptr,"Word | Frequency | Code\n");
        fclose(fptr);
    }
    
    printLevelOrder(origin);
    
    
    printf("\n\n      FILE HUFFMANCODEBOOK HAS BEEN CREATED\n\n");

    return 0;
}


char *encode_and_write(char *word, char *filename, FILE **file_ptr){
//    printf("encode_and_write opened\n");
    char outfile[100];
    
    strcpy(outfile, filename);
    strcat(outfile, ".hcz");
    struct huff_node *myroot;
    int key = hash(word) % 1000;
//    printf("\nkey is %d", key);
    myroot = word2code[key];
    static char outword[100];
    char *outwordptr;
    outwordptr = outword;

    if (myroot == NULL){
//        printf("\nCould not encode word: %s", word);
//        return word;
        return outwordptr;
    }
//    printf("\nhere");
    while(myroot != NULL){
//        printf("failed on strcmp\n");
        if (strcmp(myroot->word, word) == 0){
//            printf("string compare: %s\n", myroot->code);
            strcpy(outword, myroot->code);
//            printf("string compare succeeded\n");
            break;
        }
//        printf("entering right\n");
        myroot = myroot->right;
//        printf("entering right succeeded\n");
    }
    
//    printf("\nencode&write 3 %s\n", outfile);

    
//    if (outword[0] != '\0'){
//        fprintf(*file_ptr, "%s ", outword);
//
//    }else{
//        printf("ERROR: Could not encode word\n");
//    }
    
    printf("outword is: %s\n", outword);
    return outwordptr;
}

char *decode_and_write(char mycode[100]){
    char outword[100];
    char *outwordptr;
    
    int key;
//    printf("my key is %d\n", key);
    key = hash(mycode) % 1000;
    
    struct huff_node *myroot;
    myroot = word2code[key];
    
    while(myroot != NULL){
        if (strcmp(mycode, myroot->code)==0){
            break;
        }
        
        myroot = myroot->right;
    }
    
    if (myroot!= NULL){
        strcpy(outword, myroot->word);
        outwordptr = outword;
        return outwordptr;
    }else{
        printf("\ncould not decode %s", mycode);
        return NULL;
    }

    return NULL;
    
    
    
//    int cnt = 0;
//    char c;
//    while((c=fgetc(inFilePtr))!=EOF){
//        if (c == ' ' || cnt >= 100){
//
//            int key = hash(curr_code) % 1000;
//            curr_code[cnt] = '\0';
////            printf("Code %s has hash %d", curr_code, key);
//            struct huff_node *myroot;
//            myroot = word2code[key];
//            while(myroot != NULL){
////                printf("Line 463");
//                if (strcmp(myroot->code,curr_code) == 0){
//                    break;
//                }else{
//                    myroot = myroot->right;
//                }
//            }
////            printf("Line 468");
//            if (myroot != NULL){
//                printf("\nprinting %s to file", myroot->word);
//                fprintf(outFilePtr,"%s ", myroot->word);
//            }else{
//                    printf("\ncould not print %s to file", curr_code);
////                for(int i2 = 0; i2 < strlen(curr_code); i2++){
////                    printf("\ncurr_code[%d]: %c", i2, curr_code[i2]);
////                }
//
//                }
//
//            cnt = 0;
//
//        }else{
//            curr_code[cnt] = c;
//        }
//        cnt++;
//    }
    

    
    
    return 0;
    
////    printf("have entered decode and write \n");
//
//    struct huff_node *myroot;
//    myroot = huff_root;
////    printf("have reached huff_root: %p %s\n", myroot, myroot->word);
//    FILE *inFilePtr;
//    inFilePtr = fopen(infile, "r");
//    FILE *outFilePtr;
//    outFilePtr = fopen(outfile, "w");
//    if(inFilePtr == NULL || outFilePtr==NULL){
//        printf("\n        Invalid Input File");
//        return -1;
//    }
//
//    char c;
////    printf("entering scanf for infile: %s with outfile: %s\n", infile, outfile);
////    printf("myroot %p", myroot);
//
//    while((c=fgetc(inFilePtr))!=EOF){
////        printf("entered scanf %c\n", c);
//
//        if (c == ' '){
//            fprintf(outFilePtr,"%s ", myroot->word);
//            myroot = huff_root;
//        }
//        else if(c == '0'){
//            if(myroot->left == NULL){
//                printf("Left DAW: Could not decode %c\n", c);
//                myroot = huff_root;
////                return -1;
//            }else{
//                myroot = myroot->left;
//            }
//        }else if (c == '1'){
////            printf("myroot %p", myroot);
//            if (myroot->right == NULL){
//                printf("Right DAW: Could not decode %c\n", c);
//                myroot = huff_root;
////                return -1;
//            }else{
//                myroot = myroot->right;
//            }
//        }else{
//            printf("Unrecognized char: %c (%d)\n", c, c);
//        }
//
//    }
//
//
//    fclose(inFilePtr);
//    fclose(outFilePtr);
//
////    for(char *inputPtr = word; *inputPtr; ++inputPtr){
////        printf("*inputPtr is %c, (%d)\n", *inputPtr, *inputPtr);
////        if (*inputPtr == '0'){
////            printf("myroot is %p \n", myroot);
////            if(myroot->left == NULL){
////                printf("DAW: Could not decode %s\n", word);
////                return -1;
////            }
////            myroot = myroot->left;
////        }else if (*inputPtr == '1'){
////            printf("myroot right is %p \n", myroot);
////            if(myroot->right == NULL){
////                printf("DAW: Could not decode %s\n", word);
////                return -1;
////            }
////            myroot = myroot->right;
////        }else{
////            printf("unrecognized char: %c (%d)\n", *inputPtr, *inputPtr);
////        }
////    }
////
////    printf("DAW: writing %s to %s\n", myroot->word, filename);
////    fprintf(*file_ptr, "%s", myroot->word);
//
//    return 0;
}

void view_huff_root_inorder(struct Node *myroot){
    if(myroot != NULL){
        view_huff_root_inorder(myroot->huff_left);
        printf("\n%s", myroot->word);
        view_huff_root_inorder(myroot->huff_right);
    }
}

int decodeFile(char *filename){
    // BUILD HUFFMAN TREE
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
//            printf("adding %s to tree\n", word);
//            buildHuffmanTreeFromCode(word, curr_freq, curr_code);
            buildReverseLookupTreeFromCode(word, curr_freq, curr_code);
        }
        linenum++;
    }
    
    struct huff_node *myroot = huff_root;
//    view_huff_root_inorder(myroot);

    fclose(ptr_file);

    //DECODE BASED ON HUFFMAN TREE
//    FILE *file_ptr2;
    char buf2[1000];
    char outfile[100];
    strcpy(outfile, filename);

    int eof = strlen(outfile);
    // Removing .hcz to output filepath
    for(i=eof-4; i < eof; i++){ outfile[i] = '\0'; }

    //printf("reading file: %s\n", filename);
    //printf("writing to file: %s\n", outfile);
//    file_ptr2 = fopen(filename, "r");
//    FILE *file_to_write_ptr;
//    file_to_write_ptr = fopen(outfile,"w");
    //printf("attempting to open file\n");
//    while (fgets(buf2, 1000, file_ptr2)!= NULL){
//        buf2[strcspn(buf2, "\n")] = '\0';

        //printf("1. writing %s to %s\n", buf2, outfile);

    
//    char curr_code[101];
    
    FILE *inFilePtr;
    inFilePtr = fopen(filename, "r");
    FILE *outFilePtr;
    outFilePtr = fopen(outfile, "w");
    if(inFilePtr == NULL || outFilePtr==NULL){
        printf("\n        Invalid Input File");
        return -1;
    }
    
    
    char delim2[5] = " ";
    char mycode[100];
    char *outword = malloc(sizeof(char)*100);
    
    while(fgets(buf2,sizeof(buf2), inFilePtr)!=NULL){
        
//        fflush(stdout);
        char *ptr = strtok(buf2, delim2);
        int cntr;
        
        while(ptr!=NULL){
            strcpy(mycode, ptr);
            outword = decode_and_write(mycode);
            if (outword != NULL){
                printf("\nwriting outword %s for code %s", outword, mycode);
//                outword[strlen(outword)-1] = '\0';
//                for(i=0; i < strlen(outword); i++){
////                    printf("outword[%d]: %c\n", i, outcode[i]);
//                }
                fprintf(outFilePtr, "%s ", outword);
            }else{
                printf("\nOutword is NULL for %s", mycode);
            }
            ptr = strtok(NULL, delim2);
        }
        
//        printf("current code %s\n", buf2);
    }
    
    
    fclose(inFilePtr);
    fclose(outFilePtr);

//    decode_and_write(outfile, filename);
//    }
    //printf("closing file");
//    fclose(file_ptr2);
    
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
        
        if(bflag + cflag + dflag > 1){
            printf("\n      Can only have one of flags: -b, -c, -d");
            printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
            return -1;
        }else if (cflag == 1){
            if(strcmp(argv[argc-1],"HuffmanCodebook")!=0){
                printf("\n      Filepath in Last Argument is Incorrect");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
                return -1;
            }
            
                init_listdir();
                listdirRecursively(argv[argc-2], 0);
            
                char buf[1000];
                FILE *inFilePtr;
                char delim[3] = "\n";
                inFilePtr = fopen("dir_reader_filepaths.txt", "r");
                char filepath[50];
            
                printf("line 775\n");
                while(fgets(buf,sizeof(buf), inFilePtr)!=NULL){
                    //        fflush(stdout);
                    printf("778\n");
                    char *ptr = strtok(buf, delim);
                    
                    while(ptr!=NULL){
                        strcpy(filepath, ptr);
                        printf("\nEncoding file %s", filepath);
                        encodeFile(filepath);
                        ptr = strtok(NULL, delim);
                    }
                }
            printf("line 788\n");
            
            return 0;
//            return encodeFile(argv[argc-2]);
        
        }else if(bflag == 1){
            
            if(checkFilepath(argv[argc-1])!=0){
                printf("\n      Filepath in Last Argument is Incorrect");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|");
                return -1;
            }
            
            init_listdir();
            listdirRecursively(argv[argc-1], 0);
            
            char buf[1000];
            FILE *inFilePtr;
            char delim[3] = "\n";
            inFilePtr = fopen("dir_reader_filepaths.txt", "r");
            char filepath[50];
            //Init Huffman Codebook for multiple writes
            FILE *fptr;
            fptr = fopen("HuffmanCodebook", "w");
            fprintf(fptr,"Word | Frequency | Code\n");
            fclose(fptr);

            
            while(fgets(buf,sizeof(buf), inFilePtr)!=NULL){
                //        fflush(stdout);
                char *ptr = strtok(buf, delim);
                
                while(ptr!=NULL){
                    printf("Reading file %s", filepath);
                    strcpy(filepath, ptr);
                    buildCodebook(filepath, 0);
                    ptr = strtok(NULL, delim);
                }
            }
            
            return 0;
//            return buildCodebook(argv[argc-1]);
        }else if (dflag == 1){
            if(checkFilepath(argv[argc-2])!= 0 || strcmp(argv[argc-1],"HuffmanCodebook")!=0 ){
                printf("\n      Incorrect Input");
                printf("\n      Proper Usage is ./fileCompressor <flag> <path or file> |codebook|\n");
                return -1;}
            
            init_listdir();
            listdirRecursively(argv[argc-2], 0);

            char buf[1000];
            FILE *inFilePtr;
            char delim[3] = "\n";
            inFilePtr = fopen("dir_reader_filepaths.txt", "r");
            char filepath[50];
            
            while(fgets(buf,sizeof(buf), inFilePtr)!=NULL){
                //        fflush(stdout);
                char *ptr = strtok(buf, delim);
                
                while(ptr!=NULL){
                    printf("Reading file %s", filepath);
                    strcpy(filepath, ptr);
                    decodeFile(filepath);
                    ptr = strtok(NULL, delim);
                }
            }

//            return decodeFile(argv[argc-2]);
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
        buildCodebook(argv[argc-1], -1);

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
