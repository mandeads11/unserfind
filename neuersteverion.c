/*
 * myfind.c
 *
 *    Programmname: myfind
 *    Aufgabenstellung: Funktionsprogrammierung des "find" Befehl
 *  Created on: Mar 16, 2020 20:40
 *      Author: Mahir Tulic   | 1910258009(ic19b009)
 *              David Rüb     | 1910258081(ic19b081)
 *              Manuel Houzar | 1910258089(ic19b089)
 *              Ibrahim Alnaif| 1910258033(ic19b033)
*/


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <error.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <unistd.h>		
#include <ctype.h>	


#define MAXLEN 256
#define NULLCHAR 1


//ls:
static void ls(const char *path, struct stat *buf);
static /*char**/void print_permission_string(struct stat *buf);

//-----------------------------



static void no_argv(int argc, char ** parms);
//do_entry(const char * entry_name, const char * const * parms);

static void do_entry(const char * entry_name, char ** parms);
static void do_dir(const char * dir_name, char ** parms,int ls_mode);
static void do_nogroup(char *parms);
static void do_group(char *parms);
static void do_nouser(char *parms);
static void do_user(char *parms);
static void do_name(const char * entry_name, char *parms);
static void do_type(char *parms);
static void do_path(char *parms);
static void do_print(char *parms);
static void do_ls(char *parms);

int main (int argc, char* argv[])
{
    //char* dirname
    no_argv(argc,argv);
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

static void no_argv(int argc, char ** parms){
    if (argc == 1) {
         printf ("default directory ");
    parms[1]=".";
    parms[2]=NULL;}

    else if(argc >= 2 ){
        if (*parms[1] == '-'){
            char *temp;
            int n= argc+2;
                 printf ("default directory ... %s sollte gegeben sein", parms[1]);
                 for (int i= n-2;i>=1; i--){
                     temp = parms[i];
                     parms[i+1]=temp;
                 }

                 parms[1]=".";
                 parms[n]=NULL;}}
    printf("\n arguments are:");
    for (int i=argc ; i>0; i--){
    printf("\n argv[%d]: %s \n",i,parms[i]);
    }

}


void do_dir(const char * dir_name, char ** parms, int ls_mode) {
	//printf("TestTestTestMahir");
    struct stat st;
    const struct dirent *dirent;
    char wholepath[sizeof(dir_name)+sizeof(dirent->d_name)+1];
    errno=0;
    DIR *dirp;
    dirp = opendir(dir_name);
    //int is_dir_flag=0;
    char temp_dir[MAXLEN];
    if (errno != 0){
    //if ((dirp = opendir(dir_name)) == NULL) {
        
        error(1,errno, "Fehler!: %s\n",dir_name);
        return;
        //EXIT_FAILURE;
        
    }
    else {
        //printf("wir haben %s gefunden \n",dir_name);
        errno = 0;
        while ((dirent = readdir(dirp)) != NULL) {
            
            //printf("%d\n", testnr);
            if (strcmp(dirent->d_name, ".") != 0 && (strcmp(dirent->d_name, "..") != 0)) {
           if(ls_mode==0){
		   //printf("%s/%s\n", dir_name, dirent->d_name);
			snprintf(wholepath, (sizeof(dir_name)+sizeof(dirent->d_name) +NULLCHAR), "%s/%s", dir_name, dirent->d_name);
		   }
			
		   strcpy(wholepath,dir_name);
		   strcat(wholepath, "/");
		   strcat(wholepath, dirent->d_name);
		   
		   
			   if (lstat(wholepath, &st) == -1) {
                    error(0,errno, "stat - no such file or directory");
                    exit(1);
                }
		   

do_entry(wholepath, parms);

            }
			
			 
			/*
            if (S_ISDIR(st.st_mode)) {
                do_dir(wholepath,parms);
            }
			*/
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
}


void do_entry(const char * entry_name, char ** parms){                                                                                                                                                                 
    struct stat entry_data;
	//printf("\nHabe Entry-Funktion aufgerufen");
    // get information about the file and/or directory
    if (lstat(entry_name, &entry_data) == -1)
    {
       //  error(0,errno,"lstat failed");
        return;
    }
    errno=0;                                                                                                                                                      
    int i=0;
	int ls_mode=0;
    //int do_dir_flag=1;                                                                                                                                                      
    char buffer[MAXLEN];                                                                                                                                          
    const char possible_entry[10][MAXLEN] = {"-nogroup","-group", "-nouser", "-user", "-name", "-type", "-path", "-print", "-ls"};                                
                                                                                                                                                                  
    while (parms[++i] != NULL){
        if (*parms[i] == '-'){ 
            strcpy(buffer, parms[i]);                                                                                                                                   
            for (int j = 0; j < 10; j++) {                                                                                                                        
                                                                                                                                        
                if ((strcmp(possible_entry[j], buffer)) == 0) {                                                                                                   
                  //  printf("Gefunden:%s!!\n", possible_entry[j]); //                                                                                              
                    switch (j)
                {
                case 0:
                    do_nogroup(parms[i+1]);
                    break;
                case 1:
                    do_group(parms[i+1]);
                    break;
                case 2:
                    do_nouser(parms[i+1]);
                    break;
                case 3:
                    do_user(parms[i+1]);
                    break;
                case 4:
                    do_name(entry_name,parms[i + 1]); 
                    break;
                case 5:
                    do_type(parms[i+1]);
                    break;
                case 6:
                    do_path(parms[i+1]);
                    break;    
                case 7:
                    do_print(parms[i+1]);
                    break;    
                case 8:
                    //do_ls(parms[i-1]);
					ls(entry_name,&entry_data);
					ls_mode=1;
                    break;


                default:
                error(0,errno, "switch-case-default");
                exit(1);
                    break;                                                                                                                                       
                   
                /*    if (j == 0) {                                                                                                                                      
                                                                                                                                                                    
                    do_nogroup(parms[i + 1]);                                                                                                                      
                                                                                                                                                                    
                    } else if (j == 1) {                                                                                                                               
                                                                                                                                                                    
                        do_group(parms[i + 1]);                                                                                                                       
                                                                                                                                                                    
                    } else if (j == 2) {                                                                                                                               
                                                                                                                                                                    
                        do_nouser(parms[i + 1]);                                                                                                                      
                                                                                                                                                                    
                    } else if (j == 3) {                                                                                                                               
                                                                                                                                                                    
                        do_user(parms[i + 1]);                                                                                                                        
                                                                                                                                                                    
                    } else if (j == 4) {                                                                                                                               
                        do_dir_flag=0; 
                        printf("TTUTUUTU");                                                                                          
                        do_name(entry_name,parms[i + 1]);                                                                                                                        
                                                                                                                                                                                                                                                                                                                    
                    } else if (j == 6) {                                                                                                                               
                                                                                                                                                                    
                        do_path(parms[i+1]);                                                                                                                     
                                                                                                                                                                    
                    } else if (j == 7) {                                                                                                                               
                                                                                                                                                                    
                    do_print(parms[i]);                                                                                                                                                 
                    } else if (j == 8) {                                                                                                                               
                                                                                                                                                                    
                    do_ls(parms[i]);
                    }
                }                                                                                                                              
                else{  */ 
				//	if (S_ISDIR(entry_data.st_mode)){		
                    //if (do_dir_flag==1){
                    
                   // }                                                                                                                               
                }                                                                                                                                                         
             }                                                                                                                                                             
        
        }   
 	
    }
	   
					
    }
	//printf("\n nr:%i parameter:%s\n",i,parms[i]); 
	printf("%s\n", entry_name);
						if (S_ISDIR(entry_data.st_mode)){					
                       do_dir(entry_name,parms,ls_mode);
						//printf("\nMöchte do-Funktion aufrufen");	
						}
}

    static void do_group(char *parms){
        printf("Group: Gesucht wird nach: %s", parms);
    }
    static void do_nogroup(char *parms) {
        printf("NoGroup:Gesucht wird nach: %s", parms);
    }
    static void do_nouser(char *parms) {
        printf("do_nouser Gesucht wird nach: %s", parms);
    }
    static void do_user(char *parms) {
        printf(" do_user Gesucht wird nach: %s", parms);
    }
    static void do_name(const char *entry_name, char *parms) {
        printf("do_name Gesucht wird nach: %s", parms);
        char *buff = NULL;
		if(strrchr(parms, '/') != 0){
		    buff = (strrchr(parms, '/') + 1);

		    if (fnmatch(entry_name, buff, FNM_NOESCAPE) == 0){ 
                printf("%s",buff);
            }
	    }
	    else{
		    if (fnmatch(entry_name, parms, FNM_NOESCAPE) == 0){ 
                printf("%s",buff);
            }            
		    else {
                printf("No match %s\n",parms);
            }
	    }	
    }
    static void do_type(char *parms) {
        printf(" do_type Gesucht wird nach: %s", parms);
    }
    static void do_path(char *parms) {
        printf("path Gesucht wird nach: %s", parms);
    }
    static void do_print(char *parms) {
        printf("print Gesucht wird nach: %s", parms);
    }
    static void do_ls(char *parms) {
      //  printf("ls Gesucht wird nach: %s", parms);
    }
 


    static void ls(const char *path, struct stat *buf) {

    		struct passwd *pwd;
    		struct group *grp;

    		pwd = getpwuid(buf->st_uid);
    		grp = getgrgid(buf->st_gid);
                struct tm * time;
			time = localtime (&(buf->st_mtime));
			char buff [30];
			strftime(buff, sizeof(buff), "%b %d %H:%M", time); 
    		printf("    %lu  %5lu ",
    			buf->st_ino,
				buf->st_blocks);
				print_permission_string(buf);
				//buf->st_mode,
				printf(" %3ld %2s %10s %10ld %s %s ",
				buf->st_nlink,
				pwd->pw_name,
				grp->gr_name,
				buf->st_size,
				buff,
				path);
				printf("\n");


    }

	static /*char**/void print_permission_string(struct stat *buf){
		struct stat fileStat = *buf;
		// copy paste von stackoverflow  
		//(source :https://stackoverflow.com/questions/38561/what-is-the-argument-for-printf-that-formats-a-long )
		// Entweder oben printfaufteillen und das dazwischen geben oder 
		// hier ein char* zurückgeben mit snprintf ausgabe.
		printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
		printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
		printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
		printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
		printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
		printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
		
	}

