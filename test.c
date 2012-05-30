#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>

int main(){
FILE   *stream;
FILE    *wstream;
char   buf[10240]; 
//vector <int> PIDdata;
char ch;
int i = 0;
int data;
int PIDdata[100];
memset( buf, '/0', sizeof(buf) );
stream = popen( "lsof /dev", "r" );
wstream = fopen( "testfile", "w+"); 
fread( buf, sizeof(char), sizeof(buf),  stream);
fwrite( buf, 1, sizeof(buf), wstream );
//char *c;
//c = "lsof /dev\n";
//printf("hello world\n");
//system(c);
ch=fgetc(wstream);
int j = 0;
while(j < 1000)
{

if(ch >= 48 && ch<=57){

data = ch - '0';
ch = fgetc(wstream);
while(ch >= 48 && ch<=57)
{
data = data*10 + (ch - '0');
ch = fgetc(wstream);
//printf("%d",data);
}
if (i = 0)
   { PIDdata[i] = data;
       i++;
   }
else
  { if(data!= PIDdata[i-1])
  {
       PIDdata[i] = data;
        i ++;
   }
     }

}
ch=fgetc(wstream);
j ++;
  } 
while(i<100){
printf("%d\n",PIDdata[i]);
i++;
}
pclose( stream );
fclose( wstream );
return 0;
}
