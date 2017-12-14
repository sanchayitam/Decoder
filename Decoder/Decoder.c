
#include "Decoder.h"


void Construct_MsgList()
{
   int index=0;
   LINK* header = NULL;
   LINK* temp = NULL;
 
  for(index=0;index<Msg_No;index++)
  {  
     /* First Node */ 
     if(Msg_List == NULL) 
     {

        temp = (LINK*)malloc(sizeof(LINK));
        Msg_List =  header = temp;
     }
     else
     {
        temp = (LINK*)malloc(sizeof(LINK));
        Msg_List->next = temp;
        Msg_List= Msg_List->next;
     }
 
      temp->Msg_Type = Message_Arr[index];
      temp->start_block= File_OffsetList[index];
      temp->end_block = File_BlockList[index];
      temp->next = NULL;
 } 
  Msg_List = header;
}

void Print_LinkedList()
{
 LINK *temp = Msg_List;
 while(temp != NULL)
 {
   printf("%s",temp->Msg_Type);
   printf("%d",temp->start_block);
   printf("%d",temp->end_block); 

   temp = temp->next;
 }
}


void Free_LinkedList()
{
 LINK *temp = Msg_List ,*prev ;
 
 while(temp != NULL)
 {
   prev = temp;
   temp = temp->next;
   free(prev);
 }
}  

void Print_FieldName(char *Field_Name)
{

 int i = 0;
 char buffer[80];

 memset(buffer,'\0',sizeof(buffer));
 strcpy(buffer,Field_Name);
 printf("\n");

 while(buffer[i] != '\0')
 {
   if(buffer[i] == '_' && buffer[i+1] == '_')
   { 
     printf(" ");
   }
   else
   {
    if(buffer[i] == '_')
    {
       printf(" ");
       i++;
    } 
    printf("%c",buffer[i]);
   }
   i++; 
  }
  printf(" : ");
}  


 
void Scan_Msg()
{

   FILE *file_ptr;
   char buffer[80]; 
   char *token = 0 , *Mtype = 0 ,*tmp;
   int  Msg_count = 0 ; 
   LINK* Node = Msg_List; 

   file_ptr = fopen(MsgFile_Name,"r");
 
   if(file_ptr == NULL)
   {
     printf("\n Cannot open the Message File '%s' .Please try again \n" ,MsgFile_Name);
     exit(0);
   }
  
   while ( !feof(file_ptr))
  {
    memset(buffer, '\0' ,sizeof(buffer));
    if(fgets(buffer,80,file_ptr) == NULL)
    {
      continue;
    }
 
    if(buffer[0] == '\n' || buffer[0] == '/')
      continue;
 
       token = strtok_r( buffer, delimiters,&tmp);
       Msg_Header = token; 
       if( strncasecmp(token, CALL_PILOT ,strlen(CALL_PILOT)) == 0 || strncasecmp(token , MERIDIAN_MAIL,strlen(MERIDIAN_MAIL)) == 0)
       {
          printf(" \n\nMsg %d ", ++Msg_count); 
          while(token != NULL)
          { 
                if(strcasecmp(token, "O") == 0)
                  printf(" Outgoing Message Type -");
         
              else
             {
                if(strcasecmp(token, "I") == 0)
                   printf(" Incoming Message Type - ");
             }  
 
             if(strncasecmp(token,"MTYP",4)== 0)
             {
                Mtype =  strtok_r( NULL,delimiters,&tmp);
                printf("'%s'",Mtype);
             } 
          
             if(strncasecmp(token,"TIME",4) == 0)
             {
                 printf(" %s\n ",token); 
             }   
            
             token = strtok_r(NULL,delimiters,&tmp);     
           }
        }
        else
        {
          printf("\nInvalid Message Format\n");
          exit(0); 
        } 
  
   Search_MsgType(Mtype, &Node);  
   Print_Msg(file_ptr,Mtype,&Node); 
   printf("\n");
 } 
 fclose(file_ptr);
}

void Search_MsgType(char* Mtype,LINK** Node)
{
   *Node = Msg_List; 
    while (*Node != NULL )
    {  
     if( strcasecmp((*Node)->Msg_Type,Mtype) == 0)
       {
          return ; 
       }
       else
       {
         *Node = (*Node)->next;
       }
   }
  /* Invalid Message */
  printf("\n '%s' Message not recognised \n",Mtype);
  exit(0); 
}

void Print_Msg( FILE* MsgFile_ptr , char* Mtype , LINK** Node)
{
  FILE *RefMsgFile_ptr;
  char buffer[80], *token = 0, *tmp;
  int cnt = 0,flag = 0, Check_Flag = 0 ,i = 0 ,Len_Flag = 0 , offset = 0;
  int calling_DN_len = 0,called_DN_len = 0;
  Line_Count = 0;

  RefMsgFile_ptr = fopen( REFMSG_FILE , "r");
  if(RefMsgFile_ptr == NULL)
  {
     printf("\n Cannot open the Reference File '%s' .Please try again \n" ,REFMSG_FILE);
     exit(0);
   }

   GoTo_LineNo(RefMsgFile_ptr,Node);


  while( Line_Count <= (*Node)->end_block && !feof(MsgFile_ptr))
  {
    memset(buffer,'\0',sizeof(buffer)); 

     offset = ftell(MsgFile_ptr);  

    if((fgets(buffer,80, MsgFile_ptr) == NULL || buffer[0] == '/' || buffer[0] == '\n')|| !flag )
    { 
      /* Skip reading the second line of message */ 
      flag = 1; 
      continue; 
    }

    /* If Decoder is Meridian Mail and Message Type is Call Pilot / Symposium -- give an Invalid Message and exit out of the program */ 
    if((strcasecmp(Decoder_Type,MERIDIAN_MAIL) == 0 )&& (memcmp(buffer,CALL_PILOT,strlen(CALL_PILOT)) == 0)) 
    { 
     printf(" \n This message is not a Meridian Mail Message Format\n");
      exit(0);
    }

   /* If Decoder is of Call Pilot and message to be decoded is Meridian mail then exit out of the program */
   if((strcasecmp(Decoder_Type, CALL_PILOT)  == 0) && (memcmp(buffer,MERIDIAN_MAIL,strlen(MERIDIAN_MAIL)) == 0)) 
   {
      printf("\n This message is not a Call Pilot /Symposium Message Format\n");
      exit(0);
   }
    
   /* Replace the header with blank spaces */
    memset(buffer,' ',strlen(Msg_Header)); 

    token = strtok_r(buffer,delimiters,&tmp);

    if(strcasecmp(token,"O") == 0 || strcasecmp(token,"I") == 0)
    {
       /* Next Message in the buffer */
       fseek(MsgFile_ptr,offset,SEEK_SET);
       fclose(RefMsgFile_ptr);
       return;
    }
 
    while(token != NULL)
    {
     if(!Check_Flag)
      {
        Field_Len = Octet = 0;
        Param_ID = 0; 
        Tokenize(RefMsgFile_ptr);
        Line_Count++; 
       } 
      
       Check_Flag = 0;

       if(Len_Flag)
       {
         Field_Len = (int) strtol(token,NULL,16);
         token = strtok_r(NULL,delimiters,&tmp); 
         Len_Flag = 0;
       }

       if(Octet == 2 || Octet == 7 || Octet == 14 || Octet == 18)
       {

          token = strtok_r(NULL,delimiters,&tmp);
          continue;
       }

       if(called_DN_len != 0) 
       {
         Print_FieldName(Field_Name); 
         Field_Len = called_DN_len;
         called_DN_len = 0; 
       }
 
       if(Octet == 22)
       {
         /* token = strtok_r(NULL,delimiters,&tmp);*/
          printf("%s" , token); 
          calling_DN_len =  (int) strtol(token,NULL,16);
          Tokenize(RefMsgFile_ptr); 
          token = strtok_r(NULL,delimiters,&tmp);
          printf("%s" , token);
          Tokenize(RefMsgFile_ptr); 
          called_DN_len = (int) strtol(token,NULL,16);
          Field_Len = calling_DN_len; 
          token = strtok_r(NULL,delimiters,&tmp); 
        }
 
       if(Field_Len >= 1 )
       {
          cnt = 0;
              while(token != NULL && cnt < Field_Len)
             { 
                  printf("%s" , token);
                  cnt++;
               token = strtok_r(NULL,delimiters,&tmp); 
              }
              if(token == NULL)
              {
                 Check_Flag = 1;
                 Field_Len = Field_Len - cnt;
                 break;
              }

           if(Line_Count >= (*Node)->end_block && token != NULL)
           {
              rewind(RefMsgFile_ptr);
              GoTo_LineNo(RefMsgFile_ptr,Node);
              ParamID_Match(RefMsgFile_ptr,token,Node);
              Check_Flag = 1;
           }

      }
      else
       {
      
         
          if(Line_Count >= (*Node)->end_block && token != NULL)
          {
              rewind(RefMsgFile_ptr);
              GoTo_LineNo(RefMsgFile_ptr,Node);
              ParamID_Match(RefMsgFile_ptr,token,Node);
          }
 
         if(Param_ID != 0  && strcasecmp(token ,Param_ID) == 0)
         {
           Print_FieldName(Field_Name); 
           token = strtok_r(NULL ,delimiters,&tmp);
           if( token != NULL)
           { 
              Field_Len = (int) strtol(token,NULL,16);
              token = strtok_r(NULL,delimiters,&tmp); 
           }
           else
           {
             Len_Flag = 1;
             Check_Flag = 1; 
             Line_Count--; 
             break; 
           }  
           cnt = 0; 

 
           while( token != NULL && cnt < Field_Len )
           {
                 printf("%s" , token); 
                 cnt++;

              token = strtok_r(NULL,delimiters,&tmp);
           } 
         
         
           if(token == NULL)
           {
                 Check_Flag = 1;
                 Field_Len = Field_Len - cnt;
              
                 /* If IEs are not in sequence */
                 if(Line_Count <= (*Node)->end_block)
                 {
                   break; 
                 /*IEs not in sequence */
                 }
                 else
                 {
                   Line_Count --; 
                 } 
         
            }

          /*  if(Line_Count >= (*Node)->end_block && token != NULL) 
           {
              rewind(RefMsgFile_ptr);
              GoTo_LineNo(RefMsgFile_ptr,Node);
              ParamID_Match(RefMsgFile_ptr,token,Node);
           }*/
       }
        
       } 
 
     }
   }
 fclose(RefMsgFile_ptr);
}

void GoTo_LineNo(FILE* RefMsgFile_ptr,LINK** Node)
{

 char buffer[80];

 memset(buffer,'\0',sizeof(buffer)); 
 Line_Count = 0;
  while(Line_Count != (*Node)->start_block)
  {
    if(fgets(buffer,80,RefMsgFile_ptr) != NULL)
       Line_Count++;
  }
 
}

void ParamID_Match(FILE* RefMsgFile_ptr,char* token, LINK** Node)
{

 Field_Len = Octet = 0;
 Param_ID = 0; 
  while(Line_Count <= (*Node)->end_block)
  {
     Match_Flag = 0; 
     Tokenize(RefMsgFile_ptr);
     if( Param_ID && (strcasecmp(token,Param_ID) == 0))
     {
        Match_Flag = 1; 
        return;
     }
     Line_Count++;
  } 

}
 
void Tokenize(FILE* RefMsgFile_ptr)
{

 char Refbuffer[80];
 int cnt = 0;
 char *token = 0, *tmp;
 
 memset(Refbuffer,'\0', sizeof(Refbuffer));
 memset(Field_Name, '\0', sizeof(Field_Name));
 
  if(fgets(Refbuffer,80,RefMsgFile_ptr) != NULL && Refbuffer[0] != '%')
  {
       token = strtok_r(Refbuffer,delimiters,&tmp);
       while(token != NULL)
       {
            switch(cnt)
            {
               case 0:
                    if(!Match_Flag)
                    {
                      Field_Len = 0;
                      break;
                    } 
                    
                    if( *token != '-')
                    { 
                       Field_Len = (int) strtol(token,NULL,16) ;
                       break;
                    }
               case 1 :
                    if( *token != '-')
                    Octet = atoi(token);
                    break;
               case 2:
                    if(Field_Len == 0)
                    { 
                       strcpy(Field_Name,token);
                     }
                     else
                     { 
                       if(Octet != 2 && Octet !=7 && Octet !=14 && Octet != 18) 
                          Print_FieldName(token); 
                     }
                    break;
               case 3:
                    if( *token != '-')
                     Param_ID = token;
             }
             cnt++;
              token = strtok_r(NULL,delimiters,&tmp);
      }
 }
}


int main( int argc , char* argv[])
{
 
  if(argc < 3)
  {
    exit(0);
  } 
  Construct_MsgList();
  /* Print_LinkedList();*/
  MsgFile_Name  = argv[1];
  Decoder_Type = argv[2]; 
  Scan_Msg();
  Free_LinkedList();
  Msg_Header = 0;
  MsgFile_Name = 0;
  Decoder_Type = 0;  

  return 0;

}
