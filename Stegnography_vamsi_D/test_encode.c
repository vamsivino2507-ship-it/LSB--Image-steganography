#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include<stdlib.h>
#include "common.h"
#include"decode.h"
#include"decodetype.h"
#include<math.h>

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)                     //validate the given argument is valid or not
    {
        return e_decode;
    }
    else if((strcmp(argv[1],"-e")!=0)||(strcmp(argv[1],"-d"))!=0)
    {
        return e_unsupported;
    }
    
}






int main(int argc,char*argv[])
{
    EncodeInfo encInfo;
  
    DecodeInfo decInfo;

    int y=check_operation_type(argv);  //call check operation type
    switch(y)
    {
        case 0:
        {
                int result=read_and_validate_encode_args(argv,&encInfo); //call read and validation encode args
                if(result==e_success)
                {
                    printf("file loaded successful\n"); //print if success
                   do_encoding(&encInfo); //call do encodeing
                   
                }
                else
                {
                    
                    printf("insuffecient argument"); //print if not success
                    exit(0);
                }
                    break;
        }
        case 1:
        {
            int result=read_and_validate_decode_args(argv,&decInfo); //call read and validation encode args
            if(result==d_success)
            {
                printf("decode validation done successfull\n");//print if  success
                do_decoding(&decInfo); // call do decoding
            }
            else
            {
                printf("decode validate not done successfull\n"); //print if not success
            }
            break;
        }
        case 2:
        {
            printf("invalid argument for -e - encoding -d -decoding");  
            exit(0);
        }
    }

    
}
