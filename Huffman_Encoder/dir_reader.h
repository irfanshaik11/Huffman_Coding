#ifndef dir_reader_h
#define dir_reader_h


#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>


void init_listdir(){
    FILE *fptr;
    fptr = fopen("dir_reader_filepaths.txt", "w");
    fclose(fptr);
}

//void printfilepath(const char path[200], char file[100]){
//    FILE *fptr;
//
//    char line[300];
//    strcpy(line,path);
//    strcat(line,"/");
//    strcat(line,file);
//
//    fptr = fopen("dir_reader_filepaths.txt", "a");
//
//    if(fptr == NULL)
//    {
//        printf("Error!");
//        exit(1);
//    }
//
//    fprintf(fptr,"%s\n", line);
//    fclose(fptr);
//
//}


void listdirRecursively(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(name))){
        return;}
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR){
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
//            printf("%*s[%s]\n", indent, "", entry->d_name);
//            listdir(path, indent + 2);
            listdirRecursively(path, indent);
        } else {
//            printf("%*s- %s\n", indent, "", entry->d_name);
//            printf("%s/%s\n", name, entry->d_name);
            FILE *fptr;
            fptr = fopen("dir_reader_filepaths.txt","a");
            if(fptr!= NULL){
                if (strcmp(name, "./") == 0){
                    fprintf(fptr, "%s/%s\n", name,entry->d_name);
                }
                else{
                    fprintf(fptr, "%s/%s\n", name,entry->d_name);
                }
                
                
            }
                fclose(fptr);
            }
//            printfilepath(name, entry->d_name);
            
        }
    closedir(dir);
}


void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(name))){
        return;}
    
    while ((entry = readdir(dir)) != NULL) {
//        if (entry->d_type == DT_DIR){
//            char path[1024];
//            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
//                continue;
//            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
//            //            printf("%*s[%s]\n", indent, "", entry->d_name);
//            //            listdir(path, indent + 2);
//            listdirRecursively(path, indent);
//        } else {
//            //            printf("%*s- %s\n", indent, "", entry->d_name);
        if (entry->d_type != DT_DIR){
            FILE *fptr;
            fptr = fopen("dir_reader_filepaths.txt","a");
            if(fptr!= NULL){
                if (strcmp(name, "./") == 0){
                    fprintf(fptr, "%s/%s\n", name, entry->d_name);
                }
                else{
                    fprintf(fptr, "%s/%s\n", name, entry->d_name);
                }

                fclose(fptr);
            }

        }
    }
    closedir(dir);
}






#endif /* hashmap_h */
