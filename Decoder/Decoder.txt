
#include "Decoder.h"


void Construct_MsgList()
{
   int index=0;
   LINK *header = Msg_list;
   LINK *temp;
 
  for(index=0;index<MsgNo;index++)
  {  
     /* First Node */ 
     if(Msg_list == NULL) 
     {

        temp = (LINK*)malloc(sizeof(LINK));
        Msg_list = temp;
     }
     else
     {
        temp = (LINK*)malloc(sizeof(LINK));
        Msg_list->next = temp;
        Msg_list= Msg_list->next;
     }
 
      strcpy(temp->Msg_Type,Message_Arr[index];
      temp->Offset = File_OffsetList[index];
      temp->Block_len = File_BlockList[index];
      temp->next = NULL;
   } 
  Msg_list = header;
}
 
void Scan_Msg()
{

   FILE *file_ptr;
   char buffer[80]; 
   char *token = 0 , *Mtype = 0;
   int  Msg_count = 0; 
   char Decoder_ Type[10]; 
   int file_offset = 0;
   LINK **Node = NULL; 

   file_ptr = fopen(MsgFile_Name,"r");
   memset(Decoder_Type,'\0', sizeof(Decoder_Type));
 
   if(file_ptr == NULL)
   {
     printf("\n Cannot open the file : % s \t Please try again " ,MsgFile_Name);
     exit(0);
   }
  
   while ( !feof(file_ptr))
  {
    memset(buffer, '\0' ,sizeof(buffer)); 
    if(fgets(buffer,80,file_ptr) == NULL)
    {
      printf("\n Unable to read from the file : %s ",MsgFile_Name);
      exit(0);
    }
    if(buffer[0] == '\n' || buffer[0] == "\\")
      continue;
 
   token = strtok( buffer, delimiters);  
    while(token != NULL)
    {
       if( strcmpi(token, CALL_PILOT) == 0 || strcmpi(token , MERIDIAN_MAIL) == 0)
       {
          strcpy(Decoder_Type,token);   
          token = (NULL ,delimiters); 
          if(strcmpi(token,"0") == 0)
              printf("\n Outgoing Message ");
           
          else
          {
            if(strcmpi(token, "I") == 0)
               printf("\n Incoming Message ");
           }  

          printf(" Msg %d ", ++Msg_count);
 
           if(strncmpi(token,"MTYP",4)
           {
              Mtype = ( NULL , delimiters); 
           } 
          
           if(strncmpi(token,"TIME",4)
              printf(" %s ",token); 

        }
     } /* end of tokenizer loop * /
  
  file_offset = ftell(file_ptr);
  Node = Search_MsgType(Mtype)  
  Print_Msg( file_offset, Mtype, Decoder_Type , Node); 
 } /* end of EOF detection loop */ 
 fclose( file_ptr);
}


LINK ** Search_MsgType(char *Mtype)
{

    LINK **Node = Msg_List;
    while (NODE != NULL )
    {  
     if( strcmpi(*NODE->Msg_Type,Mtype) == 0)
       {
          return Node; 
       }
       else
       {
         Node = Node->next;
       }
   }
}
 
int Print_Msg( int file_offset , char * Mtype , char *Decoder_Type , LINK ** Node)
{
  FILE * MsgFile_ptr , *RefMsgFile_ptr;
  char buffer[80], Refbuffer[80] ,* token;
  int cnt = Field_Len = Octet = 0;
  char *Param_ID = 0; 

  MsgFile_ptr = fopen( MsgFile_Name , "r");   
  fseek(MsgFile_ptr,file_offset,SEEK_SET);

  RefMsgFile_ptr = fopen( REFMSG_FILE , "r");
  fseek(RefMsgFile_ptr, *Node->Offset,SEEK_SET);

  while( ftell(RefMsgFile_ptr) != *Node->end_block)
  {
    memset(buffer, '\0' , sizeof(buffer)); 
    if(fgets(buffer,80, Msgfile_ptr) == NULL)
    {
      printf("\n Unable to read from the file : %s ",MsgFile_Name);
      exit(0);
    }
    if(buffer[0] == '\n' || buffer[0] == "\\")
      continue;
   
    /* If Decoder is Meridian Mail and Message Type is Call Pilot / Symposium -- give an Invalid Message and exit out of the program */ 
    if((strcmpi(Decoder_Type,MERIDIAN_MAIL) == 0 )&& (memcmp(buffer,CALL_PILOT,strlen(CALL_PILOT)) == 0)) 
    { 
     printf(" \n This message is not a Meridian Mail Message ");
      exit(0);
    }

   /* If Decoder is of Call Pilot and message to be decoded is Meridian mail then exit out of the program */
   if((strcmpi(Decoder_Type, CALL_PILOT ) && (memcmp(buffer,MERIDIAN_MAIL,strlen(MERIDIAN_MAIL) == 0)) 
   {
      printf("\n This message is not a Call Pilot /Symposium Message ");
      exit(0);
   }

  if( strchr( 

    memset(buffer,'\0',strlen(Decoder_Type));
    
    while( buffer[Octet_Count] != '\0')
    {
        if(fgets(Refbuffer,80,RefMsgFile_ptr) == NULL)
           exit(0);

        if(Refbuffer[0] == '\n' || Refbuffer[0] == "%")
           continue;
   
        token = strtok(Refbuffer,delimiters);
       while(token != NULL)
       {
            switch(cnt)
            {
               case 0:
                    if( *token != '-') 
                    Field_Len = *token ;
                    break;
               case 1 :
                    if( *token != '-') 
                    Octet = *token; 
                    break;
               case 2:
                    printf("\n %s : " , token);
                    break;
               case 3:
                    if( *token != '-') 
                     strcpy(Param_ID,token);
             }
             cnt++;
              token = strtok(NULL,delimiters);
      }


        if(Field_Len >= 1 && Octet_Count ! = 2  && Octet_Count != 7)
       {
          for(cnt = 0; cnt < Field_Len ; cnt++)
          { 
             switch(Octet)
             {
            
             }
              printf("%c" ,buffer[Octet_Count + cnt]);
       }
       else
       {
         if(Param_ID && ( strcmpi(buffer[Octet_Count],Param_ID) == 0)
         {
           Field_Len = atoi(buffer[Octet_Count + 1]);
           for(cnt = 0; cnt < Field_Len ; cnt++)
               printf("%c" ,buffer[Octet_Count + cnt + 2]);
         }
       } 
        Octet_Count += Field_Len;

   }
}       
