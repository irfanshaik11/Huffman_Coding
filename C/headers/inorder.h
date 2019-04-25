#ifndef inorder_h
#define inorder_h

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

void printToCodebook(char word[50], int freq, char curr_code[50]){
    FILE *fptr;
    
    char line[100];
    
    sprintf(line, "%s | %d | %s\n", word, freq, curr_code);
    
    fptr = fopen("HuffmanCodebook", "a");
    
    if(fptr == NULL)
    {
//        printf("Error!");
        exit(1);
    }
    
    fprintf(fptr,"%s", line);
    fclose(fptr);
    
}

// INODER TRAVERSAL  TO BUILD CODEBOOK
void huff_inorder(struct Node *ptr, int cnt, char curr_code[100]){
    
    if (ptr != NULL){
//        printf("\nword %s| freq %d | level %d\n", ptr->word,ptr->val, cnt);
        
        char temp_code1[100], temp_code2[100];
        strcpy(temp_code1, curr_code);
        strcpy(temp_code2, curr_code);
        
        strcat(temp_code1, "0");
        strcat(temp_code2, "1");
        
        if(strcmp(ptr->word,"!NONTERMINAL!")){
            printToCodebook(ptr->word, ptr->val, curr_code);
        }
        
        if (ptr->huff_left != NULL){huff_inorder(ptr->huff_left, cnt+1, temp_code1);}
        
        if (ptr->huff_right != NULL){huff_inorder(ptr->huff_right, cnt+1, temp_code2);}
        
    }
}

// PART OF INORDER TRAVERSAL
int height(struct Node* node)
{
    if (node==NULL)
        return 0;
    else
    {
        /* compute the height of each subtree */
        int lheight = height(node->huff_left);
        int rheight = height(node->huff_right);
        
        /* use the larger one */
        if (lheight > rheight)
            return(lheight+1);
        else return(rheight+1);
    }
}

// PART OF INORDER TRAVERSAL
void printGivenLevel(struct Node* root, int level, char curr_code[100])
{
    if (root == NULL)
        return;
    
    if (level == 1){
        
        if(strcmp(root->word, "!NONTERMINAL!")!=0 && strcmp(root->word, "")!=0){
//            printf("\n      %d %s %s", root->val, root->word, curr_code);
            printToCodebook(root->word, root->val, curr_code);
        }else{
//            printf("\n      %d %s %s", root->val, root->word, curr_code);
        }
        
    }
    else if (level > 1)
    {
        
        char temp1[100], temp2[100];
        
        strcpy(temp1, curr_code);
        strcpy(temp2, curr_code);
        strcat(temp1, "0");
        strcat(temp2, "1");
        
        printGivenLevel(root->huff_left, level-1, temp1);
        printGivenLevel(root->huff_right, level-1, temp2);
    }
}

// PART OF INORDER TRAVERSAL
void printLevelOrder(struct Node* root)
{
    int h = height(root);
    int i;
    char null_code[100] = "";


    for (i=1; i<=h; i++)
        printGivenLevel(root, i, null_code);
}

// CALCULATES THE FREQUENCY OF EVERY WORD
int wordfreq(char filename[100], int recursive) {
    
    filename = strtok(filename, "\n");
    
//    printf("filename %s", filename);
    fflush(stdout);
    word words[MAXWORDS];
    int i,n, m;
    
    n = 0;
    
    if (recursive == 0){
        FILE *inFilePtr;
        
        inFilePtr = fopen("dir_reader_filepaths.txt", "r");
        char buf[1000];
        char delim[3] = "\n";
        char filepath[100];
        
        
        
        while(fgets(buf, sizeof(buf), inFilePtr) != NULL){
            char *ptr = strtok(buf, delim);
            while (ptr != NULL){
//                printf("trying to strcpy\n");
                strcpy(filepath, ptr);
                int j = 0;
                char properextension[4] = ".txt";
                int breaktok = -1;
                for(int i = strlen(filepath)-4; i < strlen(filepath); i++){
                    if (filepath[i] != properextension[j]){
                        breaktok = 0;
                        break;
                    }
                    j++;
                }
                if (breaktok == 0){
                    ptr = strtok(NULL, delim);
                    continue;
                }
//                printf("opening file %s\n", filepath);
                
                FILE *fp = fopen(filepath,"r");
                
                char *buf = malloc(100*sizeof(char));
    
                while( fscanf(fp, "%s", buf) != EOF ){
//                    printf("insert_words\n");
//                  printf("\n%s cnt %d", buf, n);
                    insert_word(words,&n, buf);
                }
//                printf("ptr\n");
                ptr = strtok(NULL, delim);
//                printf("exiting ptr\n");
            }
        }
        
    }else{
        
        FILE *fp = fopen(filename,"r");
        
        char *buf = malloc(100*sizeof(char));
        
        while( fscanf(fp, "%s", buf) != EOF ){
            //                  printf("\n%s cnt %d", buf, n);
            insert_word(words,&n, buf);
        }
        
        
    }
    
//    printf("exited while loop\n");
    qsort((void *) words, n, sizeof (word),
          (int (*) (const void *, const void *)) wordcmp);
//    printf("qsorted\n");
//    printf("n is %d", n);fflush(stdout);
    
    for (i=0; i<=n; i++){
//        printf("%s %d\n", words[i].s, words[i].count);
        insert(words[i].count, words[i].s);
//        printf("i\n");
    }
//    printf("exiting for loop\n");
    fflush(stdout);
    
    return 1;
}

#endif /* hashmap_h */
