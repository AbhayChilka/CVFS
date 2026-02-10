///////////////////////////////////////////////////////////////////
//
//  Header file inclusion 
//
///////////////////////////////////////////////////////////////////

#include<stdio.h>   //printf scanf
#include<stdlib.h>  //memory allocation
#include<unistd.h>  //system call 
#include<stdbool.h> //true or false
#include<string.h>  //for inbuilt string use 
#include<fcntl.h>   

///////////////////////////////////////////////////////////////////
//
//   User Defined Macros 
//
///////////////////////////////////////////////////////////////////

//maximum file size that we allow in the project
#define MAXFILESIZE 50

#define MAXOPENFILES 20

#define MAXINODE 5

#define READ 1
#define WRITE 2
#define EXECUTE 4

#define START 0
#define CURRENT 1
#define END 2

#define EXECUTE_SUCCESS 0

#define REGULARFILE 1
#define SPECIALFILE 2 

///////////////////////////////////////////////////////////////////
//
//   User Defined Macros for error handling
//
///////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

void BackupFile(char *Name, char *data);

///////////////////////////////////////////////////////////////////
//                                                               
//  User-defined structures                                      
//                                                              
///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
///////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
};

///////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
///////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about the file
//
///////////////////////////////////////////////////////////////////

#pragma pack(1)  //due to char array (padding can)
struct Inode
{
    char FileName[20]; 
    int InodeNUmber;    
    int FileSize;
    int ActualFileSize; 
    int FileType;
    int RefrenceCount;
    int Permission;
    char *Buffer;

    struct Inode *next;
};

typedef struct Inode INODE;

typedef struct Inode* PINODE;
typedef struct Inode** PPINODE;

///////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
///////////////////////////////////////////////////////////////////

struct FileTable
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

///////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about the process
//
///////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAXOPENFILES];
};

///////////////////////////////////////////////////////////////////
//
//  Global variable or objects used in the project
//
///////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

///////////////////////////////////////////////////////////////////
//
//  Function Name :     InitializeUAREA
//  Description :       It is usd to initialize UAREA members
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void InitializeUAREA()
{
    strcpy(uareaobj.ProcessName, "Myexe");

    int i = 0;

    for(i = 0; i < MAXOPENFILES; i++)
    {
        uareaobj.UFDT[i] = NULL;
    }

    printf("Marvellous CVFS : UAREA gets initialized successfully\n");
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     InitializeSuperBlock
//  Description :       It is usd to initialize Super block members
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void InitializeSuperBlock()
{
    superobj.TotalInodes = MAXINODE;
    superobj.FreeInodes = MAXINODE;

    printf("Marvellous CVFS : Super Block gets initialized successfully\n");
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is usd to create LinkedLise of inodes
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for ( i = 1; i <= MAXINODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName, "\0");
        newn->InodeNUmber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->RefrenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // ll is empty
        {
            head = newn;
            temp = head;
        }   
        else            // ll  contain atleast 1 node 
        {
            temp->next = newn;
            temp = temp->next;
        }
    }
 
    printf("Marvellous CVFS : DILB created successfully\n");
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitialization
//  Description :       It is usd to call all such functions which are 
//                      use to initialize auxillary data                        
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitialization()
{
    strcpy(bootobj.Information, "Booting process of Marvellous CVFS is done");

    printf("%s\n", bootobj.Information);

    InitializeSuperBlock();

    CreateDILB();

    InitializeUAREA();

    printf("Marvellous CVFS : Auxillary data initialized successfully\n");
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is usd to display help page                        
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-------------------------------------------------------------\n");
    printf("----------------Marvellous CVFS help page--------------------\n");
    printf("-------------------------------------------------------------\n");

    printf("clear : It is used to clear the terminal\n");
    printf("close : It is used to close file\n");
    printf("creat : It is used to create new file\n");
    printf("exit : It is used to terminate Marvellous CVFS\n");
    printf("lseek : It is used to set offset in the file\n");
    printf("ls : It is used to list all existing file\n");
    printf("man : It is used to display manual page\n");
    printf("open : It is used to open file\n");
    printf("read : It is used to read the data from file\n");
    printf("stat : It is used to display statistical information\n");
    printf("truncate : To Remove all data from file\n");
    printf("unlink : It is used to delete the file\n");
    printf("write : It is used to write the data into file\n");

    printf("-------------------------------------------------------------\n");

    printf("\nFor see Usage of above command type man _command name\n");   
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is usd to display man page                        
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void ManPageDisplay(char Name[])
{
    if(strcmp("clear", Name) == 0)
    {
        printf("About : It is used to clear the shell\n");
        printf("Usage : clear\n");
    }
    else if(strcmp("close", Name) == 0)
    {
        printf("About : It is used to close the file of given fd\n");
        printf("Usage : close __fd\n");
        printf("__fd : It is file decriptor number return from open file or create file\n");
    }
    else if(strcmp("creat", Name) == 0)
    {
        printf("About : It is used to creat file using file name + permission -(ie. 1,2,3)\n");
        printf("Usage : creat __filename __permission\n");
        printf("__filenamename : File name for creating that name file\n");
        printf("__permission : permission for files (1 for read, 2 for write, 3 = read+write)\n");
    }
    else if(strcmp("exit", Name) == 0)
    {
        printf("About : It is used to terminate shell\n");
        printf("Usage : exit\n");
    }
    else if(strcmp("lseek", Name) == 0)
    {
        printf("About : It is used to set offset in the file\n");
        printf("Usage : lseek __fd __size __from \n");
        printf("__fd : It is the file descriptor get by open or create call\n");
        printf("__size : It is size which want to move offset\n");
        printf("__from : It is the offset from which we want set offset to given size\n");
    }
    else if(strcmp("ls", Name) == 0)
    {
        printf("About : It is used to list the names of all files\n");
        printf("Usage : ls\n");
    }
    else if(strcmp("man", Name) == 0)
    {
        printf("About : It is used to display manual page\n");
        printf("Usage : man command_name\n");
        printf("command_name ; It is the name of command\n");
    }
    else if(strcmp("open", Name) == 0)
    {
        printf("About : It is used to open an existing file \n");
        printf("Usage : open __filename __mode\n");
        printf("__filename : It is the name of existing file\n");
        printf("__mode : It is the permission based in which mode we want to open file i.e(read = 1, write = 2)\n");
    }
    else if(strcmp("read", Name) == 0)
    {
        printf("About : It is used to read file using fd and read till given bytes\n");
        printf("__fd : It is the file descriptor get by open or create call\n");
        printf("__byte : It is number bytes that wewant to read from file\n");
        printf("\n");
    }
    else if(strcmp("unlink", Name) == 0)
    {
        printf("About : It is used to delete file\n");
        printf("Usage : unlink \n");
    }
    else
    {
        printf("No manual entry for %s\n", Name);
    }
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check wheather file is already exist or not  
//  Input :             It access file name 
//  Output :            It returns the true or false                      
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *Name      //File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(Name, temp->FileName) == 0) && (temp->FileType == REGULARFILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }

    return bFlag;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file  
//  Input :             It access file name and permision
//  Output :            It returns the file descriptor                        
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

int  CreateFile(
    char *Name,     //name of new file
    int permission  //permission for that file
)
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n", superobj.FreeInodes);

    //if name is missing
    if(Name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    //if permission value is wrong
    // permission -> 1-> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    //If inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    //if file is already present
    if(IsFileExist(Name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }
    
    //Search empt inode
    while (temp != NULL)
    {
        if(temp->FileType == 0)
        {
            break;
        }
        temp = temp->next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    //search for empty UFDT entry
    // Note : 0,1,2 are reserved
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    //UFDT is full
    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    //Allocate memory for file table 
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    //initialize file table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode =  permission;

    //connect file table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    //Initialize elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName, Name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAXFILESIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULARFILE;
    uareaobj.UFDT[i]->ptrinode->RefrenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    //Allocate memory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    superobj.FreeInodes--;

    return i;  //File Descriptor

}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all files
//  Input :            Nothing 
//  Output :            Nothing 
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void  LsFile()
{
    PINODE temp = head;

    printf("-------------------------------------------------------------\n");
    printf("------------Marvellous CVFS Files Information-------------\n");
    printf("-------------------------------------------------------------\n");

    while (temp != NULL)
    {
        if(temp->FileType != 0)
        {
            printf("%d\t%s\t%d\n", temp->InodeNUmber, temp->FileName, temp->ActualFileSize);
        }
        
        temp = temp->next;
    }

    printf("-------------------------------------------------------------\n");
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete file
//  Input :             File name  
//  Output :            Nothing 
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    //travel UFDT
    for (i = 0; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                //Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                //reset all values of inode
                //dont deallocate memory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->RefrenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                //Deallocate memory of file table
                free(uareaobj.UFDT[i]);

                //set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                //increment free inodes count
                superobj.FreeInodes++;

                break;  //imp
            }            //end of if 
        }        //end of if
    }    //end of for

    return EXECUTE_SUCCESS;
} //end of function      

///////////////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into file
//  Input :             File descriptor
//                      Address of Buffer which contains data
//                      size of data that we want to write
//  Output :            Number of bytes successfully written 
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

int WriteFile(
                    int fd,
                    char *data,
                    int size
            )
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n", data);
    printf("Number of bytes that we want to write : %d\n", size);
    
    //invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    //fd points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    ///there is no permission to write
    if (uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }
        
    //insufficient space
    if((MAXFILESIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    //write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    //update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    //udate the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data from the file
//  Input :             File descriptor
//                      Address of empty Buffer
//                      size of data that we want to read
//  Output :            Number of bytes successfully read 
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{
    //invalid fd
    if(fd < 0 || fd > MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    if(uareaobj.UFDT[fd]->ptrinode->ActualFileSize == 0)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    if((uareaobj.UFDT[fd]->ptrinode->ActualFileSize - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    //filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    //insufficient data
    if((MAXFILESIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    
    //read the data
    strncpy(data, uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    //update read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     OpenFile()
//  Description :       It is used to open the file in given mode 
//  Input :             It access file name and permision
//  Output :            It returns the file descriptor  
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

int OpenFile(char *name, int mode)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL || mode < 1 || mode > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // find inode
    while(temp != NULL)
    {
        if(strcmp(temp->FileName, name) == 0)
            break;
        temp = temp->next;
    }

    if(temp->Permission < mode)
    {
        return ERR_PERMISSION_DENIED;
    }

    // find free UFDT
    for(i = 3; i < MAXOPENFILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    if(i == MAXOPENFILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));
    uareaobj.UFDT[i]->Mode = mode;
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->ptrinode = temp;

    temp->RefrenceCount++;

    return i;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     CloseFileByFD()
//  Description :       It is used to close the file from open in ufdt
//  Input :             It access fd
//  Output :            It returns close fd  
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

int CloseFileByFD(int fd)
{    
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;   // FD was never opened
    }

    uareaobj.UFDT[fd]->ReadOffset = 0;
    uareaobj.UFDT[fd]->WriteOffset = 0;
   
    if((uareaobj.UFDT[fd]->ptrinode->RefrenceCount) > 0)
    {
        (uareaobj.UFDT[fd]->ptrinode->RefrenceCount)--;
    }

    free(uareaobj.UFDT[fd]);
    uareaobj.UFDT[fd] = NULL;

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     CloseAllFile()
//  Description :       It is used to close all the file from open in ufdt
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

void CloseAllFile()
{
    int i = 0;
    while (i < MAXOPENFILES)
    {
        if (uareaobj.UFDT[i] != NULL)
        {
            uareaobj.UFDT[i]->ReadOffset = 0;
            uareaobj.UFDT[i]->WriteOffset = 0;

            if((uareaobj.UFDT[i]->ptrinode->RefrenceCount) > 0)
            {
                (uareaobj.UFDT[i]->ptrinode->RefrenceCount)--;
            }

            free(uareaobj.UFDT[i]);
            uareaobj.UFDT[i] = NULL;
        }
        i++;
    }  
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     StatFile()
//  Description :       It is used to describe file information
//  Input :             file name 
//  Output :            It returns information about file 
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

int StatFile(char *name)
{
    PINODE temp = head;
    int i = 0;

    if(name == NULL ) 
    {
        return ERR_INVALID_PARAMETER;
    }
    
    while (temp != NULL)
    {
        if(strcmp(name, temp->FileName) ==0)
        {
            break;
        }
        temp = temp->next;
    }

    if(temp == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }
    
    printf("\n---------Statistical Information about file---------\n");
    printf("File name : %s\n", temp->FileName);
    printf("Inode Number %d\n", temp->InodeNUmber);
    printf("File size : %d\n", temp->FileSize);
    printf("Actual File size : %d\n", temp->ActualFileSize);
    printf("Refrence count : %d\n", temp->RefrenceCount);

    if(temp->Permission == 1)
    {
        printf("File Permission : Read only\n");
    }
    else if(temp->Permission == 2)
    {
        printf("File Permission : Write\n");
    }
    else if(temp->Permission == 3)
    {
        printf("File Permission : Read & Write\n");
    }
    printf("-----------------------------------------------------\n\n");    

    return 0;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     TruncateFile()
//  Description :       It is used to resize file data 
//  Input :             It access fd
//  Output :            It return file with removing all data
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

int TruncateFile(int fd)
{
    if(fd < 0 || fd >= MAXOPENFILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // permission check
    if(uareaobj.UFDT[fd]->ptrinode->Permission == READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    memset(uareaobj.UFDT[fd]->ptrinode->Buffer, 0, MAXFILESIZE);

    uareaobj.UFDT[fd]->ReadOffset = 0;
    uareaobj.UFDT[fd]->WriteOffset = 0;
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = 0;

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     LseekFile()
//  Description :       It is used to set offset to given size 
//  Input :             It access fd, size till which want to go, from where 
//  Output :            It return offset which we want to set
//  Author :            Abhay Bharat Chilka
//                 
///////////////////////////////////////////////////////////////////

int LseekFile(int fd, int size, int from)
{
    int newOffset = 0;

    if (fd < 0 || fd >= MAXOPENFILES)
        return ERR_INVALID_PARAMETER;

    if (uareaobj.UFDT[fd] == NULL)
        return ERR_FILE_NOT_EXIST;

    if (from != START && from != CURRENT && from != END)
        return ERR_INVALID_PARAMETER;

    PINODE inode = uareaobj.UFDT[fd]->ptrinode;
    
    /* ---------- READ MODE ---------- */
    if (uareaobj.UFDT[fd]->Mode == READ)
    {
        if (from == START)
        {
            newOffset = size;
        }
        else if (from == CURRENT)
        {
            newOffset = uareaobj.UFDT[fd]->ReadOffset + size;
        }
        else
        {
            newOffset = inode->ActualFileSize + size;
        }

        if (newOffset < 0 || newOffset > inode->ActualFileSize)
        {
            return ERR_INVALID_PARAMETER;
        }

        uareaobj.UFDT[fd]->ReadOffset = newOffset;
    }

    /* ---------- WRITE MODE ---------- */
    else if (uareaobj.UFDT[fd]->Mode == WRITE)
    {
        if (from == START)
        {
            newOffset = size;
        }
        else if (from == CURRENT)
        {
            newOffset = uareaobj.UFDT[fd]->WriteOffset + size;
        }
        else
        {
            newOffset = inode->ActualFileSize + size;
        }

        if (newOffset < 0 || newOffset > MAXFILESIZE)
        {
            return ERR_INVALID_PARAMETER;
        }

        uareaobj.UFDT[fd]->WriteOffset = newOffset;

        if (uareaobj.UFDT[fd]->WriteOffset > inode->ActualFileSize)
        {
            inode->ActualFileSize = uareaobj.UFDT[fd]->WriteOffset;
        }
    }

    /* ---------- READ + WRITE MODE ---------- */
    else if (uareaobj.UFDT[fd]->Mode == READ + WRITE)
    {
        if (from == START)
        {
            newOffset = size;
        }
        else if (from == CURRENT)
        {
            newOffset = uareaobj.UFDT[fd]->ReadOffset + size;
        }
        else
        {
            newOffset = inode->ActualFileSize + size;
        }

        if (newOffset < 0 || newOffset > MAXFILESIZE)
        {
            return ERR_INVALID_PARAMETER;
        }

        uareaobj.UFDT[fd]->ReadOffset  = newOffset;
        uareaobj.UFDT[fd]->WriteOffset = newOffset;

        if (newOffset > inode->ActualFileSize)
        {
            inode->ActualFileSize = newOffset;
        }
    }

    printf("[FD %d] RO=%d WO=%d SIZE=%d\n",
           fd,
           uareaobj.UFDT[fd]->ReadOffset,
           uareaobj.UFDT[fd]->WriteOffset,
           inode->ActualFileSize);

    return EXECUTE_SUCCESS;
}

///////////////////////////////////////////////////////////////////
//
//  Function Name :     BackupFile()
//  Description :       It is used to store file on harddisk
//  Input :             FileName, size of data that we want to read and write
//  Output :            Number of bytes successfully read 
//  Author :            Abhay Bharat Chilka
//
///////////////////////////////////////////////////////////////////

void BackupFile(char *Name, char *data)
{
    int fd = 0;
    int iWrite = 0;

    fd = open(Name, O_CREAT | O_WRONLY  , 0777);   

    if(fd == -1)
    {
        printf("Unable to create file ON HARDDISK\n");
    }
    else
    {
        printf("File gets succesfully created ON HARDISK with fd : %d\n",fd);
    }

    iWrite = write(fd, data, strlen(data));

    close(fd);
}


///////////////////////////////////////////////////////////////////
//
//  Entry point function of the project
//
///////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char command[5][20] = {{'\0'}};

    char *EmptyBuffer = NULL;
    int iCount = 0;
    int iRet = 0;
    char InputBuffer[MAXFILESIZE] = {'\0'};
    int i = 0;

    StartAuxillaryDataInitialization();

    printf("-------------------------------------------------------------\n");
    printf("------------Marvellous CVFS started successfully-------------\n");
    printf("-------------------------------------------------------------\n");

    // infinite listening shell
    while (1)  //unconditional loop
    {
        fflush(stdin); //keyboard buffer gets clean before use 

        strcpy(str, "");

        printf("\n Marvellous CVFS : > ");
        fgets(str, sizeof(str), stdin);   
           
        iCount = sscanf(str, "%s %s %s %s %s", command[0], command[1], command[2], command[3], command[4]);

        fflush(stdin);

        if (iCount == 1)
        {
            //Marvellous CVFS : > exit
            if(strcmp("exit", command[0]) == 0)
            {   
                PINODE temp = head;

                while(temp != NULL)
                {
                    if(temp->FileType != 0)
                    {
                        BackupFile(temp->FileName, temp->Buffer);
                    }
                    temp = temp->next;
                }

                CloseAllFile();               

                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");
                break;
            }

            //Marvellous CVFS : > ls
            else if(strcmp("ls", command[0]) == 0)
            {
                LsFile();
            }

            //Marvellous CVFS : > help
            else if(strcmp("help", command[0]) == 0)
            {
                DisplayHelp();
            }

            //Marvellous CVFS : > clear
            else if(strcmp("clear", command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else 
                    system("clear");
                #endif                
            }

            else
            {
                printf("There is no such command\n");
            }

        } // End of else 1

        else if(iCount == 2)
        {
            //Marvellous CVFS : > man, command (man ls)
            if(strcmp("man", command[0]) == 0)
            {
                ManPageDisplay(command[1]);
            }
        
            //Marvellous CVFS : > unlink, __filename (unlink Demo.txt)
            else if(strcmp("unlink", command[0]) == 0)
            {
                iRet = UnlinkFile(command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid Parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file\n");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets successfully deleted\n");
                }
            }

            // Marvellous CVFS: >  write, _fd   (write 3)  
            else if(strcmp("write", command[0]) == 0)
            {
                printf("Enter the data that we want to write : \n");
                fgets(InputBuffer, MAXFILESIZE, stdin);

                iRet = WriteFile(atoi(command[1]), InputBuffer, strlen(InputBuffer)-1);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no sufficient space\n");
                }
                else
                {
                    printf("%d bytes gets successfully written\n", iRet);
                }
            }

            // Marvellous CVFS: >  close, _fd   (close 3)
            else if(strcmp("close", command[0]) == 0)
            {
                iRet = CloseFileByFD(atoi(command[1]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if (iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Given fd is not exist\n");
                }
                else if (iRet == EXECUTE_SUCCESS)
                {
                    printf("File get close successfuly with of fd : %d\n", atoi(command[1]));
                }
            }  

            //  Marvellous CVFS: >  stat, _filename   (stat Demo.txt)
            else if(strcmp("stat", command[0]) == 0)
            {
                iRet = StatFile(command[1]);

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if (iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File Not exists\n");
                }
            }  

            //  Marvellous CVFS: >  truncate, _fd   (truncate 3)
            else if(strcmp("truncate", command[0]) == 0)
            {
                iRet = TruncateFile(atoi(command[1]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if (iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File Not exists\n");
                }
                else if (iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : permission denied\n");
                }
                else if (iRet == EXECUTE_SUCCESS)
                {
                    printf("File get truncate successfully\n");
                }
    
            }  

            else
            {
                printf("There is no such command\n");
            }

        } // End of else 2

        else if(iCount == 3)
        {
            //Marvellous CVFS : > creat, __filename, permission (creat Demo.txt 3)
            if(strcmp("creat", command[0]) == 0)
            {
                iRet = CreateFile(command[1], atoi(command[2]));   //atoi = ascii to interger inbuilt function
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invaid\n");
                    printf("Please refer man page\n");
                }
                else if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create the file as there is no inode\n");
                }
                else if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Uable to create file because file is already present\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened file limit reached\n");
                }
                else
                {
                    printf("File gets successfully created with FD %d\n", iRet);
                }
            }

            //Marvellous CVFS : > read, _fd, _bytes  (read 3 10)
            else if(strcmp("read", command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(command[2])+1));

                iRet = ReadFile(atoi(command[1]), EmptyBuffer, atoi(command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exists");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission Denide");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }
                else
                {
                    printf("Read operation is successfully\n");
                    printf("Data from file is : %s\n", EmptyBuffer);

                    free(EmptyBuffer);
                }
            }     

            //Marvellous CVFS : > open, __filename, permission (open Demo.txt 3)
            else if (strcmp(command[0], "open") == 0)
            {
                iRet = OpenFile(command[1], atoi(command[2]));
                
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Incorrect parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File is not preset\n");
                }
                else if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Max Files are opens\n");
                }
                else if (iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission Denide\n");
                }
                else if (iRet >= 0)
                {
                    printf("File is successfully opened with file descriptor : %d\n", iRet);
                }
            }

            else
            {
                printf("There is no such command\n");
            }

        } // End of else 3

        else if(iCount == 4)
        {
            //Marvellous CVFS : > lseek, __fd, size,  from (lseek 3 2 0)
            if (strcmp(command[0], "lseek") == 0)
            {
                iRet = LseekFile(atoi(command[1]), atoi(command[2]), atoi(command[3]));

                if (iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exists\n");
                }
                else if (iRet == EXECUTE_SUCCESS)
                {
                    printf("lseek done\n");
                }
            }
            else
            {
                printf("\nError : Command not found !!!");
            } 
        } // End of else 4
        
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        } // End of else

    } // End of while
    
    return 0;

}// End of main

