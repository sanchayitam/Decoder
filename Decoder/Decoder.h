/* Decoder.h */

#define CALL_PILOT "ELAN"
#define MERIDIAN_MAIL "CSLM"
#define REFMSG_FILE "RefMsg.txt"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct LINK
{
  char *Msg_Type;
  int  start_block;
  int  end_block; 
  struct LINK *next;
}LINK;
LINK* Msg_List = NULL;


void Construct_MsgList();
void Scan_Msg();
void Search_MsgType(char*,LINK**);
void Print_Msg(FILE*,char*,LINK**);
void Print_LinkedList();
void Tokenize( FILE*);
void Print_FieldName(char*);
void Free_LinkedList();
void GoTo_LineNo(FILE*,LINK**);
void ParamID_Match(FILE*,char*,LINK**);

int Msg_No = 4;
int File_OffsetList[]={0,34,108,66};
int File_BlockList[]={30,63,139,104}; 
char *Message_Arr[]={"ICC","ITR","ITS","PCI"};
char *delimiters = {" \n\t"}; 
char *MsgFile_Name = 0,*Decoder_Type = 0 ,*Msg_Header = 0;
int Field_Len = 0,Octet = 0,Line_Count = 0,Match_Flag = 1;
char *Param_ID = 0,Field_Name[80]; 
