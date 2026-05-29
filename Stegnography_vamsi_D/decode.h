#ifndef DECODE_H
#define DECODE_H
#include "decodetype.h"
#include<math.h>

typedef struct DecodeInfo
{
  //decode file info
  char*source_img;
  FILE*fptr_src_decode_img;

  //output file info
  char output_file[100];
  FILE*fptr_output;


  
}DecodeInfo;

/* Read and validate decode args from argv */
Dstatus read_and_validate_decode_args(char*argv[],DecodeInfo*decInfo);

/* Perform the decoding */
Dstatus do_decoding(DecodeInfo*decInfo); 

/* Get File pointers for i/p and o/p files */
Dstatus open_decode_files(DecodeInfo*decInfo);

/* decode Magic String */
Dstatus decode_magic_string(const char*magic_string,DecodeInfo*decInfo);

/*decode magic string from lsb*/
char decode_magic_string_from_lsb(FILE*fptr_src_decode_img);

/*check magic string match */
Dstatus check_magic_string_match(char*ptr);

/*decode extn file size*/
int decode_extn_size(DecodeInfo*decInfo);

/*decode_extn_size_from_lsb*/
int decode_extn_size_from_lsb(int m,char*str,DecodeInfo*decInfo);

/*decode_extn_data*/
Dstatus decode_extn_data(int v,DecodeInfo*decInfo);

/*decode_extn_data_from_lsb*/
char decode_extn_data_from_lsb(char*str,DecodeInfo*decInfo);

/*open_decode_output_extn_file*/
Dstatus open_decode_output_extn_file(DecodeInfo*decInfo);

/*decode_secret_file_data_size*/
int decode_secret_file_data_size(DecodeInfo*decInfo);

/*decode_secret_file_data*/
Dstatus decode_secret_file_data(int p,DecodeInfo *decInfo);


#endif

