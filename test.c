#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>

int ReadData(char*rbuf,int *PID)
{
 //char *readbuf;
 char ch;
 unsigned int rchar,get,data;
 int i = 0; 
 FILE *fp;
 fp = fopen("testfile", "r+");  
 if (fp == NULL)
 {
  return -1;                         
 }
 rchar=0;
 
 while ((get = fgetc(fp)) != EOF)
 {
  if((char)get == '\n')     
  {
   rchar = ftell(fp);
   rbuf=(char*)malloc(rchar*sizeof(char));
    if (fgets(rbuf,rchar,fp)!=NULL)

   {
       int j=8;
       while(j<14)
       {
            ch = rbuf[j];
           if(ch <= 48 || ch>=57)
	    {
  	
   	   j++;
         
            }
          if(ch >= 48 && ch<=57)
              {
          	   data = ch - '0';
          	   j++;
     	  	   ch = rbuf[j];
       
           	  while(ch >= 48 && ch<=57)
           	      {
              	 	 data = data*10 + (ch - '0');
             
               		  j++;
               		  ch = rbuf[j];

                	}
     
              }

         }


    }
 
               PID[i] = data;
                i++;

   }
  rchar++;
 }
 return i ;
}



int main(){
FILE   *stream;
FILE    *wstream;
FILE    *fp;
char   buf[10240]; 
//vector <int> PIDdata;
char readbuf[81];
char ch;
int i = 0;
//int data;
int PIDdata[1024];
memset( buf, '/0', sizeof(buf) );

stream = popen( "lsof /dev", "r" );
wstream = fopen( "testfile", "w+"); 

fread( buf, sizeof(char), sizeof(buf),  stream);
fwrite( buf, 1, sizeof(buf), wstream );
char *c;
//c = "lsof /dev\n";
//printf("hello world\n");
//system(c);
pclose( stream );
fclose( wstream );
//fp = fopen("testfile","r");
//ch=fgetc(fp);
//int j = 0;
i = ReadData(readbuf,PIDdata);
while(i<100)
{
     printf("%d \n",PIDdata[i]);
        i++;
}

return 0;
}
