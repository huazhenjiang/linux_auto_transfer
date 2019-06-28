#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
static void _split_whole_name(const char *whole_name, char *fname, char *ext);
char *str_replace (char *source, char *find,  char *rep);

// str_replace :input char *source , find specific string , replace by *rep
char *str_replace (char *source, char *find,  char *rep){  
   //   
   int find_L=strlen(find);  
   //   
   int rep_L=strlen(rep);  
   // 
   int length=strlen(source)+1;  
   //   
   int gap=0;  
     
   //   
   char *result = (char*)malloc(sizeof(char) * length);  
   strcpy(result, source);      
     
   // 
   char *former=source;  
   //   
   char *location= strstr(former, find);  
     
   //   
   while(location!=NULL){  
       //   
       gap+=(location - former);  
       //   
       result[gap]='\0';  
         
       //  
       length+=(rep_L-find_L);  
       //  
       result = (char*)realloc(result, length * sizeof(char));  
       //   
       strcat(result, rep);  
       // 
       gap+=rep_L;  
         
       //   
       former=location+find_L;  
       // 
       strcat(result, former);  
         
       // 
       location= strstr(former, find);  
   }      
  
   return result;  
  
} 

//splitpath with dir, file's name and extension name
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
	char *p_whole_name;
 
	drive[0] = '\0';
	if (NULL == path)
	{
		dir[0] = '\0';
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}
 
	if ('/' == path[strlen(path)])
	{
		strcpy(dir, path);
		fname[0] = '\0';
		ext[0] = '\0';
		return;
	}
 
	p_whole_name = rindex(path, '/');
	if (NULL != p_whole_name)
	{
		p_whole_name++;
		_split_whole_name(p_whole_name, fname, ext);
 
		snprintf(dir, p_whole_name - path, "%s", path);
	}
	else
	{
		_split_whole_name(path, fname, ext);
		dir[0] = '\0';
	}
}


static void _split_whole_name(const char *whole_name, char *fname, char *ext)
{
	char *p_ext;
 
	p_ext = rindex(whole_name, '.');
	if (NULL != p_ext)
	{
		strcpy(ext, p_ext);
		snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
	}
	else
	{
		ext[0] = '\0';
		strcpy(fname, whole_name);
	}
}

//example E:\code\auto_transfer$ auto_transfer E:\\code\\DevC\\auto_transfer\\css\\switch.css
int main (int argc, char *argv[]){
	
	char ch=0,ch2=0;
	unsigned long i=0,length,j;
	unsigned long index=0,count=0;
	unsigned char ten=0, one=0, sum=0;
	
	char filename[128]={0}, *head, *ptr;
	char arrayname[128]={0};
	const char cmp = '.';
	
    char drive[16];
    char dir[128];
    char fname[128];
    char ext[16];

	//char pwd_path[128]={0}, output_folder[128]={0};
	//size_t size;

	
	FILE *fp_in, *fp_out, *fp_trans;

	memset(filename,0,sizeof(filename));
	memset(arrayname,0,sizeof(arrayname));
	
	if(argc >2){
		printf("\r\n%s, %s",argv[0],argv[1]);
		return -2;
	}

//get filename and trans '.' , '-' to '_' , add '.c' at final
    _splitpath( argv[1], drive, dir, fname, ext );

    printf("\r\nDrive:%s\n file name: %s\n file type: %s\n",drive,fname,ext);
	strcat(fname,ext);
    printf("File name with extension :%s\n",fname);			

	head=str_replace(fname,".","_");
	head=str_replace(head,"-","_");

	//strncpy(filename, argv[1], strlen(argv[1]));
	strncpy(filename,head,strlen(head));
	strcat(filename,".c");
	printf("\r\nOutput_filename: %s",filename);
	
//get arrayname
	strncpy(arrayname, head , strlen(head));
	printf("\r\nArrayname:%s",arrayname);	
	
//get pwd
	//getcwd(pwd_path, sizeof(pwd_path));
	//printf("\r\nPWD_path=%s\n", pwd_path);
//make out folder
	//strncpy(output_folder, pwd_path, strlen(pwd_path));	
	//strcat(output_folder,"\\out");
	//printf("\r\nOutput_folder=%s\n", output_folder);	
	
	if((access( "./out", 0 )) ==0){
		printf( "\r\n%s exists ", "./out");
	}
	else{
		printf( "\r\ncan not find %s , create it", "./out");
		if (mkdir("./out",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0){ 
	        printf("\r\ncan not create out folder, Error:%d\n", __LINE__);
			//return -1;
		}		
	}
	//if( (access( ACCESS.C, 2 )) == 0 ){
	//	printf( "File ACCESS.C has write permission " );
	//}		

	fp_in=fopen(argv[1],"r");
	if( NULL == fp_in ){
        printf( "\r\n%s open failure",argv[1] );
        return -1;
    }

    if(chdir("./out") ==0)
    	printf("\r\ncurrent working directory: %s\n", getcwd(NULL, NULL));
    else
    	printf("\r\ncan not enter /out");
				
	fp_out=fopen(filename,"w");
	if( NULL == fp_out ){
        printf( "\r\n%s open failure",filename );
        return -1;
    }
	//char *index.html.c="3c6874...";	
	fprintf(fp_out,"char *%s=\"", arrayname);	
	while( (ch=fgetc(fp_in)) != EOF){	
		fprintf(fp_out,"%02x",ch);				
	}
	fprintf(fp_out,"\";");

	fclose(fp_in);
	fclose(fp_out);
	
	return 0;	

	
}	
