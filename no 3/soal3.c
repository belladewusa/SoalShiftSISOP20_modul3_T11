#include <stdio.h> 
#include <string.h> 
#include <unistd.h>  
#include <stdlib.h>
#include <dirent.h> 
#include <errno.h> 
#include <sys/stat.h> 

char menudir[100]; 
char dirlama[100];
char barufile[100]; 

const char *get_filename_ext(const char *filename) { 
    const char *dot = strrchr(filename, '.'); 
    if(!dot || dot == filename) return ""; 
    return dot + 1; 
}

int main(int argc, char **argv){  
 
if (strcmp(argv[1],"-f")==0){ 
    printf("NO\n");
} 
 
else if (strcmp(argv[1],"*")==0){ 
	char direc[100];	 
	struct dirent *de; 
	DIR *dir = opendir("/home/kecoak/Desktop/");
	
	if(!dir){ 
        printf("direktori salah\n");
        return 1; 
    	}
    else {
        printf("OK\n"); 
        return 1;
    }
    	closedir(dir); 
    return 0;
}
else if (strcmp(argv[1], "-d") == 0){ 
    printf("NO\n");
}
else{printf("Input salah \n");}

}