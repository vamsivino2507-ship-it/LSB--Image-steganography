#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include "common.h"
#include<stdlib.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);   
    
    
    fread(&width,sizeof(int),1,fptr_image);


     fread(&height,sizeof(int),1,fptr_image);
 
 //   printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr,0,SEEK_END); //Seek to last byte

    int count=ftell(fptr);   //ftell return current position of the cursor
  

   return count; //return count

}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status copy_bmp_header(FILE*fptr_src_image,FILE*fptr_dest_image)
{
   
    rewind(fptr_src_image);  //rewind the cursor to 0 position
    
    char str[54];  //declare array of string

    fread(str,1,54,fptr_src_image);   //read 54 bytes from source image

    fwrite(str,1,54,fptr_dest_image);  //write 54 bytes from destination image
   
    return e_success;  // return success
    
   

}
Status encode_magic_string(const char*magic_string,EncodeInfo*encInfo)
{
    char str[8];  //declare array of string
    
    
    for(int i=0;i<strlen(magic_string);i++)
    {
        fread(str,8,1,encInfo->fptr_src_image);     //read the 8 byte from source img

        encode_byte_to_lsb(magic_string[i],str);      //call encode byte to lsb

        fwrite(str,1,8,encInfo->fptr_stego_image);  //write 8 byte to stego img
       
    }

    return e_success;
}

Status encode_byte_to_lsb(char data,char*image_buffer)
{
    int arr[8],p=0;
    for(int i=7;i>=0;i--)
    {
        if(data&(1<<i))
        {
           arr[p++]=1;                  //encode magic string to lsb bit of source byte
        }
        else{
            arr[p++]=0;
        }

    }
    
   
   
     for(int i=0;i<8;i++)
    {
       
         image_buffer[i]=(image_buffer[i]&(~1))|(arr[i]);
       
         
      
    }
   
   

   
   
    
}
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char str1[32];  //declare character array
  
    fread(str1,1,32,encInfo->fptr_src_image);  //read 32 bytes from source image

    encode_size_to_lsb(size,str1);  //call encode size to lsb

    fwrite(str1,1,32,encInfo->fptr_stego_image); //write 32 byte to stego img

    return e_success;
   
   


}
 Status encode_size_to_lsb(int size,char*imageBuffer)
  {
    int arr[32],p=0;

    for(int i=31;i>=0;i--)
    {
        if(size&(1<<i))
        {
            arr[p++]=1;                       //encode the bits 
        }
        else{
            arr[p++]=0;
        }
    }
    
    
   
        for(int i=0;i<32;i++)
        {
            imageBuffer[i]=imageBuffer[i]&(~1)|arr[i];
          
            
        }
   
       
  }

  Status encode_secret_file_extn(const char*file_extn,EncodeInfo*encInfo)
  {
    char str[8]; // declare character array
    for(int i=0;i<strlen(file_extn);i++)
    {
        fread(str,1,8,encInfo->fptr_src_image);//read 8 byte from source
        encode_byte_to_lsb(file_extn[i],str);  //call byte to lsb
        fwrite(str,1,8,encInfo->fptr_stego_image); //write 8 bit to destination img
        
    }

    return e_success;
  }
  Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
  {
     char str[32];  //declare character array
     fread(str,1,32,encInfo->fptr_src_image);  //read  32 bytes from source img 
     encode_size_to_lsb(file_size,str); //call encode size to lsb
     fwrite(str,1,32,encInfo->fptr_stego_image); //write 32 bytes to destination img
   
     return e_success;

  }
   Status encode_secret_file_data(EncodeInfo *encInfo)
   {
    
   char str[8];// declare character array
   
   char secret_data_bit; //declare character variable

   rewind(encInfo->fptr_secret); //rewind the secret file
   
   
  
  
   for(int i=0;i<encInfo->size_secret_file;i++)
   {
    fread(&secret_data_bit,1,1,encInfo->fptr_secret); //read 1 byte of character from secret file
    
     fread(str,1,8,encInfo->fptr_src_image); // read 1 byte of character from source img

    encode_byte_to_lsb(secret_data_bit,str); //call encode byte to lsb

    fwrite(str,1,8,encInfo->fptr_stego_image);//write 1 byte to destination img
   
   }
   return e_success;




   }

   Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
   {
    int n; // declare a variable 

    char str[1000]; //declare character array
   
    while((n=fread(str,1,sizeof(str),fptr_src))> 0)
    {
        fwrite(str,1,n,fptr_dest);  //write to destination img
    }
    return e_success;
   }

 Status read_and_validate_encode_args(char*argv[],EncodeInfo*encInfo)
 {
    int i=0;  
    int count=0;  //declare a count variable initialize with 0
    while(argv[i]!=NULL)
    {
      count++; //count increment by 1
      i++;  //increment i by 1
    }

    if(count<4||count>5)  
    {
        return e_failure;     //if the condition is true return failure
    }
    
    int z=0;  //declare a z variable initialize with 0
    int c=0;   //declare a c variable initialize with 0
    int size=0;  //declare a size variable initialize with 0
   for(int j=0;argv[2][j]!=0;j++)
   {
    if(argv[2][j]=='.')
    {
        z=j;
    }
   }
   for(int j=z;argv[2][j]!=0;j++)
   {
    size++;
     
    if(argv[2][j]=='.')
    {
        c++;
    }
    else if(argv[2][j]=='b')
    {
        c++;
    }                                               //check the argument source file has .bmp or not 
    else if(argv[2][j]=='m')     
    {
        c++;
    }
    else if(argv[2][j]=='p')
    {
        c++;
    }
    
   }
  

   if(size!=c)
   {
    return e_failure;
   }

   int b=0;
   
   int d=0;
   int size2=0;

   for(int j=0;argv[3][j]!=0;j++)
   {
    if(argv[3][j]=='.')
    {
        b=j;
    }
    if(j=='\0')                                         //validating argument file
    {
        b=strlen(argv[3]);
    }
   }
   
   for(int j=b;argv[3][j]!=0;j++)
   {
    size2++;
     
    
    
   }


   if(size2<=1)
   {
    return e_failure;
   }
 
   int l=0;
    int m=0;
    int size3=0;
   if(count==5)
   {
    
    for(int j=0;argv[4][j]!=0;j++)
    {
        
        if(argv[4][j]=='.')
        {
            l=j;
        }
    }
    for(int j=l;argv[4][j]!=0;j++)
    {
        size3++;
        if(argv[4][j]=='.')
        {
            m++;
        }
        else if(argv[4][j]=='b')
        {
            m++;
        }
        else if(argv[4][j]=='m')                         //check the argument destination file is .bmp or not
        {
            m++;
        }
        else if(argv[4][j]=='p')
        {
            m++;
        }
    }
   }

   if(size==c&&size3==m)
   {
    if(count==5)
    {
        encInfo->src_image_fname=argv[2];
        //encInfo->secret_fname=argv[3];                       //arguments are stored in variable
        encInfo->stego_image_fname=argv[4];
    }
    else
    {
        encInfo->src_image_fname=argv[2];
       // encInfo->secret_fname=argv[3];
        encInfo->stego_image_fname="stego_img.bmp";
        
    }
    if(size2>0)
    {
      encInfo->secret_fname=argv[3];
    }
    

    return e_success;
   }
   else
   {
    return e_failure;
   }
   

 }

Status do_encoding(EncodeInfo*encInfo)
{
   int result= open_files(encInfo);       //call open files function

   if(result==e_success)  
   {

    printf("file open successfully\n");  //print if it success

   }

   else
   {

    printf("file not open\n"); //print if not success

    exit(0);
   }

  int capacityresult= check_capacity(encInfo);   //call check capacity

  if(capacityresult==e_success)
  {

    printf("suffient space is there in image\n");  //printf if success

  }

  else
  {

   printf("not have sufficient space in image\n"); //print if not success
  
  }

  int y=copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);   // call copy bmp header

  if(y==e_success)
  {

    printf("successfully copied\n"); //print if success

  }

  else
  {

    printf("not copied");  //print if not success

  }

  int resultmagicstring=encode_magic_string(MAGIC_STRING,encInfo); //call encode magic function

  if(resultmagicstring==e_success)
  {

    printf("magic encoded successful\n");//print if success

  }
  else{
    printf("issue in magic string"); //print if not success
  }

  int v=0;

  for(int i=0;encInfo->secret_fname[i]!=0;i++)
  {

    if(encInfo->secret_fname[i]=='.')
    {

       v=i;

    }

  }
  int p=0;

  for(int i=v;encInfo->secret_fname[i]!=0;i++)
  {

    encInfo->extn_secret_file[p++]=encInfo->secret_fname[i];

  }

  encInfo->extn_secret_file[p]=0;

  

  int resultextnsize=encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo);  //call secret file extn size function

  if(resultextnsize==e_success)
  {

    printf("extn file size loaded successfull\n");  //print if success

  }

  else
  {

    printf("issue in extn file size\n");   //print if not success

  }

  int resultextnfile=encode_secret_file_extn(encInfo->extn_secret_file,encInfo);   //call encode secret file extn

  if(resultextnfile==e_success)
  {

    printf(".txt extn file loaded successfull\n");  //print if success

  }

  else
  {

    printf("issue in .txt extn file\n");//print if not success

  }

  int resultfiledatasize=encode_secret_file_size(encInfo->size_secret_file,encInfo); //call encode secret file size function

    if(resultfiledatasize==e_success)
    {

        printf("file data size encoded successfull\n"); //print if success

    }
    else
    {

        printf("issue in file data size\n"); //print if not success

    }

   int resultfiledata= encode_secret_file_data(encInfo);  //call encode secret file data

   if(resultfiledata==e_success)
   {

    printf("data encoded successfull\n");  //print if success

   }
   else
   {

    printf("issue in data encoded\n"); //print if not success

   }

  int resultcopydata= copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);  //call copy remaining img data function
  if(resultcopydata==e_success)
  {
    printf("copy data successfull\n"); //print if   success
  }
  else{
    printf("issue in copy data\n");  // //print if not success
  }

}


Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");//file open in read mode
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)// checking file open or not
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");  //file open in read mode
    // Do Error handling
    if (encInfo->fptr_secret == NULL) // checking file open or not
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");  //file open in write mode
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL) // checking file open or not
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status check_capacity(EncodeInfo*encInfo)
{
    // step1 -> encInfo->image_capacity =get_image_size_for_bmp(source_file_pointer)
   // printf("%d",get_image_size_for_bmp(encInfo->fptr_src_image));
    encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
     // step2 -> find secret file size encInfo -> size_secret_file = get_file_size(secret file pointer)
     encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
     


      
      if((encInfo->image_capacity)>16+32+32+32+54+encInfo->size_secret_file*8)  //chaeck the image capacity is greateryhen file size
      {
        return e_success;  //return success
        
      }
      else
      {
        return e_failure;  //return failure
      }


    
}


