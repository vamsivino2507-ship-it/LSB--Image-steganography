#include<stdio.h>

#include"decode.h"

#include"decodetype.h"


#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include<stdlib.h>
#include<math.h>


Dstatus read_and_validate_decode_args(char*argv[],DecodeInfo*decInfo)
{
    int m=0,c=0,count=0;
     // char str[20];

    int l=0,argv_count=0;
    while (argv[l])
    {
        argv_count++;
        l++;
    }
    if(argv_count<3&&argv_count>4)
    {
        return d_failure;
    }
    
    for(int i=0;argv[2][i]!=0;i++)
    {
        if(argv[2][i]=='.')
        {
              m=i;
        }
    }
    for(int j=m;argv[2][j]!=0;j++)
    {
        count++;
        if(argv[2][j]=='.')
        {
            c++;
        }
        else if(argv[2][j]=='b')
        {
            c++;
        }
        else if(argv[2][j]=='m')                   //validating decode arguments
        {
            c++;
        }
        else if(argv[2][j]=='p')
        {
            c++;
        }
    }
    if(argv_count==4)
    {
        int f=0,p=0;
       
        for(int i=0;argv[3][i]!=0;i++)
        {
            if(argv[3][i]=='.')
            {
                 f=i;
            }
        }
        for(int i=0;i<f;i++)
        {
          decInfo->output_file[p++]=argv[3][i];
        }
        decInfo->output_file[p]=0;

    }
    else{
      
      strcpy(decInfo->output_file, "output");


        
    }
    
    if(count==c)
    {
        decInfo->source_img=argv[2];
        

        return d_success;
    }
    else{
        return d_failure;
    }


}
Dstatus open_decode_files(DecodeInfo*decInfo)
{
    decInfo->fptr_src_decode_img=fopen(decInfo->source_img,"r");  //open file in read mode
    if(decInfo->fptr_src_decode_img==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->source_img); // print error if file not open
        return d_failure;
    }
    return d_success; //return success if successfully open the file
}

Dstatus check_magic_string_match(char*ptr)
{
   if(strcmp(ptr,MAGIC_STRING)==0)  // check the magic str matching or not
   {
    return d_success;  //if matching return success
   }
   else
   {
    return d_failure;  //if  not matching return failure
   }
}
char decode_magic_string_from_lsb(FILE*fptr)
{
    char str[8];
    fread(str,1,8,fptr);
    int sum=0;
                                       //decode magic sting from destination file
    for(int i=0;i<8;i++)
    {
       sum<<=1;
       sum|=str[i]&1;
        
    }
    
    return (char)sum;            
    
    


}
Dstatus decode_magic_string(const char*magic_string,DecodeInfo*decInfo)
{
    fseek(decInfo->fptr_src_decode_img,54,SEEK_SET);        //place the cursor in 54th position
    int count=ftell(decInfo->fptr_src_decode_img);
   // printf("%d",count);
   char ptr[100],p=0;
   for(int i=0;magic_string[i]!=0;i++)
   {
     ptr[p++]= decode_magic_string_from_lsb(decInfo->fptr_src_decode_img);    // call decode magic string from lsb
   }
   ptr[p]=0;

   if(d_success==check_magic_string_match(ptr))  //call magic string match function
   {
      return d_success; //return success if matching

   }
   else{
    return d_failure; //return success if not matching
   }
   
}
int decode_extn_size_from_lsb(int m,char*str,DecodeInfo*decInfo)
{
  //  printf("%d",m);
   int sum=0;
   for(int i=0;i<m;i++)
   {
     sum<<=1;                       //decode extn size from lsb to destination file
     sum|=str[i]&1;
   }

    return sum;
}

int decode_extn_size(DecodeInfo*decInfo)
{
    char str[32];
    fread(str,1,32,decInfo->fptr_src_decode_img);  // read the size from destination file
    int m=sizeof(str);
   
    int n=decode_extn_size_from_lsb(m,str,decInfo); // call decode extn size from lsb
    
     return n;
   

}
char decode_extn_data_from_lsb(char*str,DecodeInfo*decInfo)
{
    int sum=0;
    for(int i=0;i<8;i++)
    {
        sum<<=1;                          //decode extn data from lsb from encoded file
        sum|=str[i]&1;
    }
    return (char)sum;
}

Dstatus decode_extn_data(int v,DecodeInfo*decInfo)
{
    char str[8],p=0;
    char ptr[v];
    for(int i=0;i<v;i++)
    {
        fread(str,1,8,decInfo->fptr_src_decode_img);   //read 8 bytes from encoded file
        ptr[p++]=decode_extn_data_from_lsb(str,decInfo); // call decode extn data from lsb 
    }
    ptr[p]=0;
    strcat(decInfo->output_file,ptr);
    return d_success;
    
   
    
}

Dstatus open_decode_output_extn_file(DecodeInfo*decInfo)
{
    decInfo->fptr_output=fopen(decInfo->output_file,"w");  // opening file in write mode
    if(decInfo->fptr_output==NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->output_file ); //print if error
        return d_failure;
    }
    return d_success; // return success if file open successfully
}

int decode_secret_file_data_size(DecodeInfo*decInfo)
{
    char str[32];
    fread(str,1,32,decInfo->fptr_src_decode_img); //read 32 byte from encoded file

    int m=sizeof(str);
    
     int n=decode_extn_size_from_lsb(m,str,decInfo);  //call decode extn size from lsb
     return n;

}

Dstatus decode_secret_file_data(int p,DecodeInfo*decInfo)
{
   char str[8];
   for(int i=0;i<p;i++)
   {
    fread(str,1,8,decInfo->fptr_src_decode_img); //read 8 bytes from encoded file
    fprintf(decInfo->fptr_output,"%c",decode_extn_data_from_lsb(str,decInfo));//call decode extn data from lsb and the return value print int output file

   }
   return d_success;
}



Dstatus do_decoding(DecodeInfo*decInfo)
{
    int result1=open_decode_files(decInfo);  //call open decode file
    if(result1==d_success)
    {
        printf("decode file open successfull\n"); //print if success
    }
    else
    {
        printf("issue in open file\n"); //print if not success
    }

    int result2=decode_magic_string(MAGIC_STRING,decInfo);  //call decode magic string function
    if(result2==e_success)
    {
        printf("magicstring decode successfull\n");//print if success
    }
    else
    {
        printf("issue in magicstring decode\n");//print if not success
    }
   int v= decode_extn_size(decInfo); // call decode extn size
  

   int result3=decode_extn_data(v,decInfo); //call decode extn data
   if(result3==d_success)
   {
    printf("decode extn data successful\n");//print if success
   }
   else
   {
    printf("issue in decode extn data successful\n");//print if not success
   }
  int result4=open_decode_output_extn_file(decInfo); //call open decode output extn file
  if(result4==d_success)
  {
    printf("output file open successfull\n");//print if success
  }
  else
  {
    printf("issue in output file opening\n");//print if not success
  }

  int p=decode_secret_file_data_size(decInfo); //call decode secret file data size

  int result5=decode_secret_file_data(p,decInfo); //call decode secret file data
  if(result5==d_success)
  {
    printf("decode the successfull\n");//print if success
  }
  else
  {
    printf("issue in decoding the data\n");//print if not success
  }

}