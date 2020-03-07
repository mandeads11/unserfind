/*
 * myfind.c
 *
 *    Programmname: myfind
 *    Aufgabenstellung: Funktionsprogrammierung des "find" Befehl
 *  Created on: Feb 26, 2020
 *      Author: Mahir Tulic   | 1910258009(ic19b009)
 *              David Rüb     | 1910258081(ic19b081)
 *              Manuel Houzar | 1910258089(ic19b089)
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <sys/stat.h>
#include <fnmatch.h>

#define MAXLEN 256
#define NULLCHAR 1

static void no_argv(int argc);
//do_entry(const char * entry_name, const char * const * parms);

static void do_entry(const char * entry_name, char ** parms);
static void do_dir(const char * dir_name, char ** parms);

int main (int argc, char* argv[])
{
    //char* dirname
    no_argv(argc);
    int argvlength = 0;
    char* dirname = NULL;
    dirname = argv[1];
    //dirname = ".";
    if (argv[1] != NULL) argvlength=strlen(argv[1]);
    //fprintf(stderr, "Test Länge: %d\n%c\n",argvlength,argv[1]);
    do_entry(argv[1],argv);
    //do_dir(dirname,argv);

    return 0;
}

static void no_argv(int argc){

    if(argc<2){
        fprintf(stderr, "something went wrong!!! - ARGUMENTS 0");
    exit(1);
    }

}
void do_dir(const char * dir_name, char ** parms) {

    struct stat st;
    char wholepath[MAXLEN];
    DIR *dirp = NULL;
    dirp = opendir(dir_name);
    //int is_dir_flag=0;
    char temp_dir[MAXLEN];
    if (dirp == NULL){
    //if ((dirp = opendir(dir_name)) == NULL) {

        error(0,errno, "Fehler!: %s\n",dir_name);
        //EXIT_FAILURE;
      //  exit(1);
    }
    else {
        printf("wir haben %s gefunden \n",dir_name);
    }
    const struct dirent *dirent;
    errno = 0;
    int testnr = 0;
    while ((dirent = readdir(dirp)) != NULL) {
        testnr++;
        //printf("%d\n", testnr);
        if (strcmp(dirent->d_name, ".") != 0 && (strcmp(dirent->d_name, "..") != 0)) {
            printf("%s/%s\n", dir_name, dirent->d_name);
            snprintf(wholepath, (sizeof(dir_name)+sizeof(dirent->d_name) +NULLCHAR), "%s/%s", dir_name, dirent->d_name);
            if (lstat(wholepath, &st) == -1) {
                error(0,errno, "stat - no such file or directory");
                exit(1);
            }

        }
        if (S_ISDIR(st.st_mode)) {
            do_dir(wholepath,parms);
        }


        errno=0;
    }
    if (errno!=0){
        error(0,errno, "Fault while readdir");
    }
    
    

    if (closedir(dirp) == -1) {
                error(0,errno, "closedir");
        exit(1);
    }
}


void do_entry(const char * entry_name, char ** parms)
{
    struct stat entry_data;
    errno=0;
    int i=0;
    char buffer[MAXLEN];
    const char possible_entry[10][MAXLEN] = {"-nogroup","-group", "-nouser", "-user", "-name", "-type", "-path", "-print", "-ls"};
          
    while (parms[++i] != NULL){
        if (*parms[i] == '-'){
            for (int j = 0; j < 10; j++) {
            strcpy (buffer,parms[i]);
                if ((strcmp(possible_entry[j], buffer)) == 0) {
                printf("Gefunden:%s!!\n",possible_entry[j]); //
                } 
                switch (j)
                {
                case 0:
                    do_nogroup(parms[j+1])
                    break;
                case 1:
                    do_group(parms[j+1])
                    break;
                case 2:
                    do_nouser(parms[j+1])
                    break;
                case 3:
                    do_user(parms[j+1])
                    break;
                case 4:
                    do_name(parms[j+1])
                    break;
                case 5:
                    do_type(parms[j+1])
                    break;
                case 6:
                    do_path(parms[j+1])
                    break;    
                case 7:
                    do_print(parms[j+1])
                    break;    
                case 8:
                    do_ls(parms[j+1])
                    break;
                        

                default:
                error(0,errno, "switch-case-default");
                exit(1);
                    break;
                }
            }
        }
        else{
            do_dir(parms[i],parms);
        }
    }
}