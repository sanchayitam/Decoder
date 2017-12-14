
#include "Decoder.h"
#include "strtok_r.c"


typedef struct LINK
{
  char *Msg_Type;
  int  start_block;
  int  end_block; 
  struct LINK *next;
}LINK;
LINK* Msg_List = NULL;

int Msg_No = 4;
int File_OffsetList[]={0,34,108,66};
int File_BlockList[]={30,63,139,104}

; 
char *Message_Arr[]={"ICC","ITR","ITS","PCI"};
char *delimiters = {" \n\t"}; 
char *MsgFile_Name = 0,*Decoder_Type = 0 ,*Msg_Header = 0;
int Field_Len = 0,Octet = 0,Line_Count = 0 ,Match_Flag = 1;
char Param_ID[20];
char Field_Name[80]; 
FILE *filewrite_ptr;
int InvalidMsg = 0;
char* Dir_Name = 0;

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

/*void Print_LinkedList()
{
 LINK *temp = Msg_List;
 while(temp != NULL)
 {
   print_text("%s",temp->Msg_Type);
   print_text("%d",temp->start_block);
   print_text("%d",temp->end_block); 

   temp = temp->next;
 }
}*/


void Free_LinkedList()
{
 LINK *temp = Msg_List ,*prev ;
 
 while(temp != NULL)
 {
   prev = temp;
   temp = temp->next;
   free(prev);
 }
 Msg_List = NULL;
}  

void Print_FieldName(char *Field_Name)
{

 int i = 0 , j = 0;
 char buffer[80];
 char text_buffer[80];

 memset(text_buffer,'\0',sizeof(text_buffer));
 memset(buffer,'\0',sizeof(buffer));
 strcpy(buffer,Field_Name);
 print_text("\n");

 while(buffer[i] != '\0')
 {
   if(buffer[i] == '_' && buffer[i+1] == '_')
   { 
		text_buffer[j] = ' ';
   }
	else
	{
		if(buffer[i] == '_')
		{
		   text_buffer[j] = ' ';
		   i++;
		   j++;
		}
		 if(buffer[i] != ' ')
			text_buffer[j] = buffer[i];
	}
	i++;
	j++;
 }
  print_text(text_buffer);
  print_text(" : ");
}  


 
void Scan_Msg()
{

   FILE *file_ptr;
   char buffer[80]; 
   char *token = 0 , *Mtype = 0 ,*tmp ,MsgNo[10] ;
   int  Msg_count = 0 ; 
   LINK* Node = Msg_List; 
   
   memset(MsgNo, '\0' ,sizeof(MsgNo));
   file_ptr = fopen(MsgFile_Name,"r");
 
   if(file_ptr == NULL)
   {
     print_text("\n Cannot open the Message File ");
	 print_text( MsgFile_Name );
	 print_text(" .Please try again ");
	 return;
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
 
       token = (char*)strtok_r(buffer, delimiters,&tmp); 
	   Msg_Header = token;

       if( strnicmp(token, CALL_PILOT,strlen(CALL_PILOT)) == 0 || strnicmp(token , MERIDIAN_MAIL,strlen(MERIDIAN_MAIL)) == 0)
       {
		  Msg_count++;
          print_text("\n\nMsg : ");
		  sprintf(MsgNo,"%d",Msg_count);
		  print_text(MsgNo); 

          while(token != NULL)
          { 
                if(stricmp(token, "O") == 0)
                  print_text(" Outgoing Message Type - ");
         
              else
             {
                if(stricmp(token, "I") == 0)
                   print_text(" Incoming Message Type - ");
             }  
 
             if(strnicmp(token,"MTYP",4)== 0)
             {
                Mtype =  (char*)strtok_r( NULL,delimiters,&tmp);
				print_text("'");
                print_text(Mtype);
				print_text("'");
             } 
          
             if(strnicmp(token,"TIME",4) == 0)
             {
                 print_text("  ");
				 print_text(token); 
				 print_text("\n");
             }   
            
             token = (char*)strtok_r(NULL,delimiters,&tmp);     
           }
        }
        else
        {
          print_text("\nInvalid Message Format\n");
          return; 
        } 
  
		if(!Search_MsgType(Mtype, &Node))
		{
			Print_Msg(file_ptr,Mtype,&Node);
			 if(InvalidMsg)
			 {
				 break;
			 }
		}
		else
		{
			   break;
		}
		print_text("\n");
		
 } 
 fclose(file_ptr);
}

int Search_MsgType(char* Mtype,LINK** Node)
{
  *Node = Msg_List;
 
    while (*Node != NULL )
    {  
     if( stricmp((*Node)->Msg_Type,Mtype) == 0)
       {
		  return 0; 
       }
       else
       {
         *Node = (*Node)->next;
       }
   }
  /* Invalid Message */ 
  print_text("\n");
  print_text("'");	
  print_text(Mtype);
  print_text("'");
  print_text(" Message not recognised \n");
  return 1;
 // exit(0); 
}

void Print_Msg( FILE* MsgFile_ptr , char* Mtype , LINK** Node)
{
  FILE *RefMsgFile_ptr;
  char buffer[80], *token = 0, *tmp ,Ref_File[80];
  int cnt = 0,flag = 0, Check_Flag = 0 ,i = 0 ,Len_Flag = 0 , offset = 0;
  int calling_DN_len = 0,called_DN_len = 0;
  Line_Count = 0;
  InvalidMsg = 0;
  memset(Ref_File,'\0',sizeof(Ref_File));

  strcpy(Ref_File,Dir_Name);
  strcat(Ref_File,REFMSG_FILE);

  RefMsgFile_ptr = fopen( Ref_File , "r");
  if(RefMsgFile_ptr == NULL)
  {
     print_text("\n Cannot open the Reference File ");
	 print_text( Ref_File );
	 print_text(" .Please try again \n");
	 InvalidMsg = 1;
     return;
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
    if((stricmp(Decoder_Type,MERIDIAN_MAIL) == 0 )&& (memcmp(buffer,CALL_PILOT,strlen(CALL_PILOT)) == 0)) 
    { 
      print_text(" \n This message is not a Meridian Mail Message Format\n");
      InvalidMsg = 1;
      return;
    }

   /* If Decoder is of Call Pilot and message to be decoded is Meridian mail then exit out of the program */
   if((stricmp(Decoder_Type, CALL_PILOT)  == 0) && (memcmp(buffer,MERIDIAN_MAIL,strlen(MERIDIAN_MAIL)) == 0)) 
   {
      print_text("\n This message is not a Call Pilot /Symposium Message Format\n");
	  InvalidMsg = 1;
      return;
   }
    
   /* Replace the header with blank spaces */ 
    memset(buffer,' ',strlen(Msg_Header)); 

	token = (char*)strtok_r(buffer,delimiters,&tmp);

    if(stricmp(token,"O") == 0 || stricmp(token,"I") == 0)
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
        memset(Param_ID,'\0',sizeof(Param_ID)); 
        Tokenize(RefMsgFile_ptr);
	    Line_Count++; 
       } 
      
       Check_Flag = 0;

       if(Len_Flag)
       {
         Field_Len = (int) strtol(token,NULL,16);
         token = (char*)strtok_r(NULL,delimiters,&tmp); 
         Len_Flag = 0;
       }

       if(Octet == 2 || Octet == 7 || Octet == 14 || Octet == 18)
       {

          token = (char*)strtok_r(NULL,delimiters,&tmp);
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
         /* token = strtok_r(NULL,delimiters,&&tmp);*/
          print_text(token); 
          calling_DN_len = (int) strtol(token,NULL,16);
          Tokenize(RefMsgFile_ptr); 
          token = (char*)strtok_r(NULL,delimiters,&tmp);
          print_text(token);
          Tokenize(RefMsgFile_ptr); 
          called_DN_len = (int) strtol(token,NULL,16);
          Field_Len = calling_DN_len; 
          token = (char*)strtok_r(NULL,delimiters,&tmp); 
        }
 
       if(Field_Len >= 1 )
       {
          cnt = 0;
              while(token != NULL && cnt < Field_Len)
             { 
                  print_text(token);
                  cnt++;
               token = (char*)strtok_r(NULL,delimiters,&tmp); 
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

         if(Param_ID[0] != '\0' && stricmp(token,Param_ID) == 0)
         {
		   Print_FieldName(Field_Name); 
           token = (char*)strtok_r(NULL ,delimiters,&tmp);
		   
           if( token != NULL)
           { 
              Field_Len = (int) strtol(token,NULL,16);
              token = (char*)strtok_r(NULL,delimiters,&tmp); 
           }
           else
           {
             Len_Flag = 1;
             Check_Flag = 1;
             Line_Count --;  
             break; 
           }  
           cnt = 0; 
 
           while( token != NULL && cnt < Field_Len )
           {
                 print_text(token); 
                 cnt++;

              token = (char*)strtok_r(NULL,delimiters,&tmp);
           } 
         
           if(token == NULL)
           {
                 Check_Flag = 1;
                 Field_Len = Field_Len - cnt;
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
 memset(Param_ID,'\0',sizeof(Param_ID)); 

  while(Line_Count <= (*Node)->end_block)
  {
     Match_Flag = 0; 
     Tokenize(RefMsgFile_ptr);
     if( Param_ID[0] != '\0' && (stricmp(token,Param_ID) == 0))
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
 
 memset(Param_ID,'\0',sizeof(Param_ID)); 
 memset(Refbuffer,'\0', sizeof(Refbuffer));
 memset(Field_Name, '\0', sizeof(Field_Name));
 
  if(fgets(Refbuffer,80,RefMsgFile_ptr) != NULL && Refbuffer[0] != '%')
  {
       token = (char*)strtok_r(Refbuffer,delimiters,&tmp);
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
                    Field_Len = (int)strtol(token,NULL,16) ;
                    break;
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
                     strcpy(Param_ID,token);
					 
             }
             cnt++;
              token = (char*)strtok_r(NULL,delimiters,&tmp);
      }
 }
}


void print_text(char *Message)
{
 if(filewrite_ptr == NULL)
 {
	 printf("\nError in opening DecodedMessages.txt File.Please try again\n");
	 return;
 }
 else
 {
	 fwrite(Message,1,strlen(Message),filewrite_ptr);
 }
}


int Decode( char* File_Name , char* Decode_Type , char* Output_FilePath)
{
	char buffer[80];
	memset(buffer,'\0',sizeof(buffer));
	Construct_MsgList();
	if((File_Name == NULL)||(Decode_Type == NULL))
		return 1;
		/* Print_LinkedList();*/
	MsgFile_Name  = File_Name ;
	Decoder_Type = Decode_Type; 
    Dir_Name = Output_FilePath;
	strcpy(buffer,Output_FilePath);
	strcat(buffer,"\\DecodedMessages.txt");
	filewrite_ptr = fopen(buffer, "w+");
	Scan_Msg();
	Free_LinkedList();
	MsgFile_Name = 0;
	Decoder_Type = 0;
    Msg_Header = 0;
	Dir_Name = 0;
	fclose(filewrite_ptr);
	return 0;
}
