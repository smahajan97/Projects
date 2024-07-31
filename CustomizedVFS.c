#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXINODE 50

#define READ 1
#define WRITE 2

#define MAXFILESIZE 2048

#define REGULAR 1
#define SPECIAL 2

#define START 0
#define CURRENT 1
#define END 2

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// structure name       : SUPERBLOCK
// Structure members    : int TotalInodes (Total number of inodes available in the filesystem)
//                        int FreeInode (Number of free inodes available in the filesystem)
//Structure Description : Represents the superblock of the filesystem containing information about total and free inodes.
//                        Used to manage and track inode usage within the filesystem.
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct superblock
{
    int TotalInodes;
    int FreeInode;
} SUPERBLOCK, *PSUPERBLOCK;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structure name        : INODE
// Structure members     : char FileName[50]: Name of the file associated with the inode
//                         int InodeNumber: Unique identifier for the inode
//                         int FileSize: Maximum size of the file
//                         int FileActualSize: Actual size of the file
//                         int FileType: Type of the file (e.g., regular file)
//                         char *Buffer: Buffer to store file contents
//                         int LinkCount: Number of links pointing to this inode
//                         int ReferenceCount: Number of references to this inode
//                         int permission: Permission level for the file (1, 2, 3)
//                         struct inode *next: Pointer to the next inode in the list
// Structure Description : Represents an inode structure used to store metadata and data of a file.
//                         Contains information such as file name, size, type, permissions, and pointers for file management.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct inode
{
    char FileName[50];
    int InodeNumber;
    int FileSize;
    int FileActualSize;
    int FileType;
    char *Buffer;
    int LinkCount;
    int ReferenceCount;
    int permission; // 1 2 3
    struct inode *next;
} INODE, *PINODE, **PPINODE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Structure name        : FILETABLE
// Structure members     : int readoffset: Current read offset in the file
//                         int writeoffset: Current write offset in the file
//                         int count: Count of open instances of this file table entry
//                         int mode: Mode of file access (1 = read, 2 = write, 3 = read/write)
//                         PINODE ptrinode: Pointer to the inode structure representing the file
// Structure Description : Represents a file table entry used to manage open file instances.
//                         Contains information such as read/write offsets, access mode, and a pointer to the associated inode.
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct filetable
{
    int readoffset;
    int writeoffset;
    int count;
    int mode; // 1 2 3
    PINODE ptrinode;
} FILETABLE, *PFILETABLE;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// structure name        : UFDT
// Structure members     : PFILETABLE ptrfiletable: Pointer to a file table entry
// Structure Description : Represents User File Descriptor Table entry used to manage file descriptors.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct ufdt
{
    PFILETABLE ptrfiletable;
} UFDT;

UFDT UFDTArr[50];
SUPERBLOCK SUPERBLOCKobj;
PINODE head = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : man
//Input parameters     : char *name (The name of the command for which the manual entry is requested)
//Return value         : void
//Function Description : This function provides descriptions for various commands used in the project.
//                     : It prints the description and usage of that command.
//                     : If the command name does not match, it prints an error message.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void man(char *name)
{
    if (name == NULL) return;

    if (strcmp(name, "create") == 0)
    {
        printf("Description: Used to create new regular file\n");
        printf("Usage: create File_name Permission\n");
    }
    else if (strcmp(name, "read") == 0)
    {
        printf("Description: Used to read data from regular file\n");
        printf("Usage: read File_name No_Of_Bytes_To_Read\n");
    }
    else if (strcmp(name, "write") == 0)
    {
        printf("Description: Used to write data into regular file\n");
        printf("Usage: write File_name\nAfter this enter the data that we want to write\n");
    }
    else if (strcmp(name, "ls") == 0)
    {
        printf("Description: Used to list all information of files\n");
        printf("Usage: ls\n");
    }
    else if (strcmp(name, "stat") == 0)
    {
        printf("Description: Used to display information of file\n");
        printf("Usage: stat File_name\n");
    }
    else if (strcmp(name, "fstat") == 0)
    {
        printf("Description: Used to display information of file\n");
        printf("Usage: fstat File_Descriptor\n");
    }
    else if (strcmp(name, "truncate") == 0)
    {
        printf("Description: Used to remove data from the file\n");
        printf("Usage: truncate File_name\n");
    }
    else if (strcmp(name, "open") == 0)
    {
        printf("Description: Used to open existing file\n");
        printf("Usage: open File_name mode\n");
    }
    else if (strcmp(name, "close") == 0)
    {
        printf("Description: Used to close opened file\n");
        printf("Usage: close File_name\n");
    }
    else if (strcmp(name, "closeall") == 0)
    {
        printf("Description: Used to close all opened files\n");
        printf("Usage: closeall\n");
    }
    else if (strcmp(name, "lseek") == 0)
    {
        printf("Description: Used to change file offset\n");
        printf("Usage: lseek File_Descriptor ChangeInOffset StartPoint\n");
    }
    else if (strcmp(name, "rm") == 0)
    {
        printf("Description: Used to delete the file\n");
        printf("Usage: rm File_Name\n");
    }
    else
    {
        printf("ERROR: No manual entry available.\n");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : DisplayHelp
//Input parameters     : -
//Return value         : void
//Function Description : This function prints a list of available commands along with their descriptions.
//                     : It provides a quick reference for users to understand the functionalities offered by the file system.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("ls: To List out all files\n");
    printf("clear: To clear console\n");
    printf("open: To open the file\n");
    printf("close: To close the file\n");
    printf("closeall: To close all opened files\n");
    printf("read: To Read the contents from file\n");
    printf("write: To Write contents into file\n");
    printf("exit: To Terminate file system\n");
    printf("stat: To Display information of file using name\n");
    printf("fstat: To Display information of file using file descriptor\n");
    printf("truncate: To Remove all data from file\n");
    printf("rm: To Delete the file\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : GetFDFromName
//Input parameters     : char *name: Name of the file whose file descriptor is to be retrieved
//Return value         : Returns the file descriptor (index in UFDTArr) if the file is found and open
//                       Returns -1 if the file with the given name is not found or not open
//Function Description : Searches through the UFDTArr array to find an open file with the given `name`.
//                       Returns the file descriptor (index) if found, which can be used to access file information and operations.
//                       If no file with the given `name` is found among the open files, returns -1.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int GetFDFromName(char *name)
{
    int i = 0;

    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
            if (strcmp((UFDTArr[i].ptrfiletable->ptrinode->FileName), name) == 0)
                break;
        i++;
    }

    if (i == 50) return -1;
    else return i;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : Get_Inode
//Input parameters     : char *name( Name of the file to find its corresponding inode)
//Return value         : Returns a pointer to the inode structure (`PINODE`) if the file with the given name is found
//                       Returns NULL if the file with the given name is not found in the linked list of inodes
//Function Description : Searches the linked list of inodes starting from `head` to find an inode with the specified `name`.
//                       If found, returns a pointer to the inode structure (`PINODE`) representing the file metadata.
//                       If not found, returns NULL indicating that no inode exists for the specified file name.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PINODE Get_Inode(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
        return NULL;

    while (temp != NULL)
    {
        if (strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }
    return temp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : createDILB
//Input parameters     :  - 
//Return value         : void
//Function Description : This function creates the Disk Inode List Block (DILB).
//                       It initializes the inodes and links them in a singly linked list.
//                       This is a fundamental step for setting up the file system structure.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    while (i <= MAXINODE)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        newn->LinkCount = 0;
        newn->ReferenceCount = 0;
        newn->FileType = 0;
        newn->FileSize = 0;

        newn->Buffer = NULL;
        newn->next = NULL;

        newn->InodeNumber = i;

        if (temp == NULL)
        {
            head = newn;
            temp = head;
        }
        else
        {
            temp->next = newn;
            temp = temp->next;
        }
        i++;
    }
    printf("DILB created successfully\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : InitialiseSuperBlock
//Input parameters     : -
//Return value         : void
//Function Description : This function initializes the superblock.
//                     : It sets up the array of file descriptors (UFDT) and initializes the superblock object.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    int i = 0;
    while (i < MAXINODE)
    {
        UFDTArr[i].ptrfiletable = NULL;
        i++;
    }

    SUPERBLOCKobj.TotalInodes = MAXINODE;
    SUPERBLOCKobj.FreeInode = MAXINODE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : CreateFile
//Input parameters     : 1) char *name: The name of the file to be created.
//                     : 2) int permission(0, 1, 2, or 3)
//Return value         : int - Returns a file descriptor index if the file is created successfully.
//                             Returns -1 if the file name is NULL, or permission is 0 or greater than 3.
//                             Returns -2 if no inodes are free.
//                             Returns -3 if the file already exists.
//Function Description : This function creates a new regular file with the specified name and permission.
//                       It checks for valid input parameters, ensures that a free inode is available, and verifies that the file does not already exist.
//                       It allocates memory for a new file table entry and sets up the inode with the provided file name and permission.
//                       This function is crucial for adding new files to the file system.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(char *name, int permission)
{
    int i = 0;
    PINODE temp = head;

    if ((name == NULL) || (permission == 0) || (permission > 3))
        return -1;

    if (SUPERBLOCKobj.FreeInode == 0)
        return -2;

    (SUPERBLOCKobj.FreeInode)--;

    if (Get_Inode(name) != NULL)
        return -3;

    while (temp != NULL)
    {
        if (temp->FileType == 0)
            break;
        temp = temp->next;
    }

    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable == NULL)
            break;
        i++;
    }

    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));

    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = permission;
    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;

    UFDTArr[i].ptrfiletable->ptrinode = temp;

    strcpy(UFDTArr[i].ptrfiletable->ptrinode->FileName, name);
    UFDTArr[i].ptrfiletable->ptrinode->FileType = REGULAR;
    UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount = 1;
    UFDTArr[i].ptrfiletable->ptrinode->LinkCount = 1;

    UFDTArr[i].ptrfiletable->ptrinode->FileSize = MAXFILESIZE;
    UFDTArr[i].ptrfiletable->ptrinode->FileActualSize = 0;
    UFDTArr[i].ptrfiletable->ptrinode->permission = permission;
    UFDTArr[i].ptrfiletable->ptrinode->Buffer = (char *)malloc(MAXFILESIZE);

    return i;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name    : rm_File
//Input parameters : char *name(The name of the file to be removed)
//Return value     : int - Returns 0 if the file is removed successfully.
//                         Returns -1 if the file does not exist.
//Function Description :  -This function removes the specified file from the file system.
//                       - It first retrieves the file descriptor using the file name.
//                        -If the file is found, it decreases the link count of the inode associated with the file.
//                        -If the link count reaches zero, the function deallocates the memory used by the file and it's
//                         buffer, and updates the file type to indicate that it is no longer in use.
//                        -Finally, it frees the file descriptor table entry and increments the count of free inodes in the superblock.
//                        -This function is essential for managing the deletion of files in the file system.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int rm_File(char *name)
{
    int fd = 0;

    fd = GetFDFromName(name);
    if (fd == -1)
        return -1;

    (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount)--;

    if (UFDTArr[fd].ptrfiletable->ptrinode->LinkCount == 0)
    {
        UFDTArr[fd].ptrfiletable->ptrinode->FileType = 0;
        free(UFDTArr[fd].ptrfiletable->ptrinode->Buffer);
        free(UFDTArr[fd].ptrfiletable);
    }

    UFDTArr[fd].ptrfiletable = NULL;
    (SUPERBLOCKobj.FreeInode)++;
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : ReadFile
//Input parameters     : Int fd (File descriptor indicating the file to read from)
//                       char *arr (Buffer to store the read data)
//                       int isize (Number of bytes to read from the file)
//Return value         : int(Returns isize if the read operation is successful.
//                       Returns -1 if the file descriptor is invalid (file not open).
//                       Returns -2 if the file is not opened in read mode.
//                       Returns -3 if the read offset is at the end of the file.
//                       Returns -4 if the file type is not regular)
//Function Description : This function reads data from a regular file identified by the file descriptor fd.
//  - It checks various conditions such as whether the file is open, whether it's in read mode, and whether it's a regular file.
//   - If all conditions are met, it reads isize bytes from the file's buffer starting from the current read offset.
//   - The read offset is then updated accordingly.
//   - This function is crucial for reading data from files managed by the file system.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int ReadFile(int fd, char *arr, int isize)
{
    int read_size = 0;

    if (UFDTArr[fd].ptrfiletable == NULL)
        return -1;

    if (UFDTArr[fd].ptrfiletable->mode != READ && UFDTArr[fd].ptrfiletable->mode != READ + WRITE)
        return -2;

    if (UFDTArr[fd].ptrfiletable->ptrinode->permission != READ && UFDTArr[fd].ptrfiletable->ptrinode->permission != READ + WRITE)
        return -2;

    if (UFDTArr[fd].ptrfiletable->readoffset == UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
        return -3;

    if (UFDTArr[fd].ptrfiletable->ptrinode->FileType != REGULAR)
        return -4;

    read_size = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) - (UFDTArr[fd].ptrfiletable->readoffset);
    if (read_size < isize)
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), read_size);

        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + read_size;
    }
    else
    {
        strncpy(arr, (UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->readoffset), isize);

        UFDTArr[fd].ptrfiletable->readoffset = UFDTArr[fd].ptrfiletable->readoffset + isize;
    }

    return isize;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : WriteFile
//Input parameters     : int fd: File descriptor (index in UFDTArr) of the file to write to
//                       char *arr: Pointer to the array containing data to be written
//                       int isize: Size of data to write
//Return value         : On success, returns isize (number of bytes written)
//                       On failure: Returns -1 if the file is not opened in WRITE or READ + WRITE mode
//                                   Returns -1 if the file's inode does not have WRITE or READ + WRITE permission
//                                   Returns -2 if the write offset reaches MAXFILESIZE
//                                   Returns -3 if the file type is not REGULAR
//Function Description : Writes data from the array `arr` to the file associated with file descriptor `fd`.
//                       Updates the write offset and actual file size in the file table and inode.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int WriteFile(int fd, char *arr, int isize)
{
    if (((UFDTArr[fd].ptrfiletable->mode) != WRITE) && ((UFDTArr[fd].ptrfiletable->mode) != READ + WRITE))
        return -1;

    if (((UFDTArr[fd].ptrfiletable->ptrinode->permission) != WRITE) && ((UFDTArr[fd].ptrfiletable->ptrinode->permission) != READ + WRITE))
        return -1;

    if ((UFDTArr[fd].ptrfiletable->writeoffset) == MAXFILESIZE)
        return -2;

    if ((UFDTArr[fd].ptrfiletable->ptrinode->FileType) != REGULAR)
        return -3;

    strncpy((UFDTArr[fd].ptrfiletable->ptrinode->Buffer) + (UFDTArr[fd].ptrfiletable->writeoffset), arr, isize);

    (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + isize;

    (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + isize;

    return isize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : OpenFile
//Input parameters     : char *name: Name of the file to open
//                       int mode: Mode in which to open the file (READ, WRITE, or READ + WRITE)
//Return value         : Returns the file descriptor on success (index in UFDTArr)
//                       On failure: Returns -1 if invalid parameters are passed (name is NULL or mode is <= 0)
//                               Returns -2 if the inode for the file does not exist
//                               Returns -3 if the requested mode exceeds the file's permission
//Function Description : Opens a file specified by its name with the given mode.
//                       Allocates a file table entry in UFDTArr and initializes it.
//                       Updates read and write offsets based on the mode.
//                       Increments the reference count of the file inode.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int OpenFile(char *name, int mode)
{
    int i = 0;
    PINODE temp = NULL;

    if (name == NULL || mode <= 0)
        return -1;

    temp = Get_Inode(name);
    if (temp == NULL)
        return -2;

    if (temp->permission < mode)
        return -3;

    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable == NULL)
            break;
        i++;
    }

    UFDTArr[i].ptrfiletable = (PFILETABLE)malloc(sizeof(FILETABLE));
    if (UFDTArr[i].ptrfiletable == NULL)
        return -1;
    UFDTArr[i].ptrfiletable->count = 1;
    UFDTArr[i].ptrfiletable->mode = mode;

    if (mode == READ + WRITE)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    else if (mode == READ)
    {
        UFDTArr[i].ptrfiletable->readoffset = 0;
    }
    else if (mode == WRITE)
    {
        UFDTArr[i].ptrfiletable->writeoffset = 0;
    }
    UFDTArr[i].ptrfiletable->ptrinode = temp;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)++;

    return i;
}

//////////////////////////////////////////////////////////////////////////////////////
//Function Name        : CloseFileByFD
//Input parameters     : int fd (File descriptor of the file to close)
//Return value         : -
//Function Description : Closes the file specified by its file descriptor.
//                       Resets read and write offsets of the file.
//                       Decrements the reference count of the file inode.
//////////////////////////////////////////////////////////////////////////////////////

void CloseFileByFD(int fd)
{
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    (UFDTArr[fd].ptrfiletable->ptrinode->ReferenceCount)--;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : CloseFileByName
//Input parameters     : char *name ( Name of the file to close)
//Return value         : int Returns 0, if successful
//                       Returns -1, if the file descriptor is not found
//Function Description : Closes the file specified by its name.
//                       Resets read and write offsets of the file.
//                       Decrements the reference count of the file inode.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CloseFileByName(char *name)
{
    int i = 0;
    i = GetFDFromName(name);
    if (i == -1)
        return -1;

    UFDTArr[i].ptrfiletable->readoffset = 0;
    UFDTArr[i].ptrfiletable->writeoffset = 0;
    (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : CloseAllFile
//Input parameters     : -
//Return value         : void
//Function Description : Closes all opened files by resetting their read and write offsets to zero.
//                       Decrements the reference count of each file inode.
//                       Stops execution after the first file is closed to avoid further unnecessary iterations.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CloseAllFile()
{
    int i = 0;
    while (i < 50)
    {
        if (UFDTArr[i].ptrfiletable != NULL)
        {
            UFDTArr[i].ptrfiletable->readoffset = 0;
            UFDTArr[i].ptrfiletable->writeoffset = 0;
            (UFDTArr[i].ptrfiletable->ptrinode->ReferenceCount)--;
            break;
        }
        i++;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : LseekFile
//Input parameters     : int fd (File descriptor pointing to the file in UFDTArr)
//                       int size (Size to move the offset by)
//                       int from (Reference position to seek from (START, CURRENT, END))
//Return value         : Returns 0 on success
//                       Returns -1 on failure
//Function Description : This function moves the read or write offset of the file specified by 'fd'.
//                       It supports seeking relative to the current position, the start of the file, or the end of the file.
//                       For read operations, it checks bounds and updates the read offset.
//                       For write operations, it updates the write offset and may adjust the file size.
//                       Returns appropriate error codes (-1) if the operation fails due to invalid parameters or bounds.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int LseekFile(int fd, int size, int from)
{
    if ((fd < 0) || (from > 2))
        return -1;
    if (UFDTArr[fd].ptrfiletable == NULL)
        return -1;

    if ((UFDTArr[fd].ptrfiletable->mode == READ) || (UFDTArr[fd].ptrfiletable->mode == READ + WRITE))
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) > UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->readoffset) + size;
        }
        else if (from == START)
        {
            if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                return -1;
            if (size < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = size;
        }
        else if (from == END)
        {
            if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->readoffset) + size) < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->readoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    else if (UFDTArr[fd].ptrfiletable->mode == WRITE)
    {
        if (from == CURRENT)
        {
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > MAXFILESIZE)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->writeoffset) + size;
        }
        else if (from == START)
        {
            if (size > MAXFILESIZE)
                return -1;
            if (size < 0)
                return -1;
            if (size > (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize))
                (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) = size;
            (UFDTArr[fd].ptrfiletable->writeoffset) = size;
        }
        else if (from == END)
        {
            if ((UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size > MAXFILESIZE)
                return -1;
            if (((UFDTArr[fd].ptrfiletable->writeoffset) + size) < 0)
                return -1;
            (UFDTArr[fd].ptrfiletable->writeoffset) = (UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize) + size;
        }
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : ls_file
//Input parameters     : -
//Return value         : void
//Function Description : This function lists all files present in the file system.
//                       It traverses through the linked list of inodes (head) to print details of each file.
//                       Prints file name, inode number, file size, and link count for each valid file.
//                       If there are no files (FreeInode equals MAXINODE), it prints an error message.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ls_file()
{
    int i = 0;
    PINODE temp = head;

    if (SUPERBLOCKobj.FreeInode == MAXINODE)
    {
        printf("ERROR : There are no files\n");
        return;
    }

    printf("\nFile Name\tInode Number\tFile Size\tLink Count\n");
    printf("---------------------------------------------------------------\n");
    while (temp != NULL)
    {
        if (temp->FileType != 0)
        {
            printf("%s\t\t%d\t\t%d\t\t%d\n", temp->FileName, temp->InodeNumber, temp->FileActualSize, temp->LinkCount);
        }
        temp = temp->next;
    }
    printf("---------------------------------------------------------------\n");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name        : fstat_file
//Input parameters     : int fd: File descriptor of the file to retrieve statistical information.
//Return value         : int Returns 0 upon successful retrieval of file statistics.
//                           Returns -1 if the file descriptor is invalid (negative).
//                           Returns -2 if the file descriptor points to a NULL file table entry.
//Function Description : This function retrieves and displays statistical information about a file specified by its file descriptor 'fd'.
//                     : It checks if 'fd' is valid and points to a valid file table entry in UFDTArr.
//                     : Prints various details such as file name, inode number, file size, link count, reference count, and file permissions.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int fstat_file(int fd)
{
    PINODE temp = head;
    int i = 0;

    if (fd < 0)
        return -1;

    if (UFDTArr[fd].ptrfiletable == NULL)
        return -2;

    temp = UFDTArr[fd].ptrfiletable->ptrinode;

    printf("\n---------Statistical Information about file---------\n");
    printf("File name : %s\n", temp->FileName);
    printf("Inode Number %d\n", temp->InodeNumber);
    printf("File size : %d\n", temp->FileActualSize);
    printf("Link count : %d\n", temp->LinkCount);
    printf("Reference count : %d\n", temp->ReferenceCount);

    if (temp->permission == 1)
        printf("File Permission : Read only\n");
    else if (temp->permission == 2)
        printf("File Permission : Write\n");
    else if (temp->permission == 3)
        printf("File Permission : Read & Write\n");
    printf("----------------------------------------------------\n\n");

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name        : stat_file
// Input parameters     : char *name - Name of the file
// Return value         : int - Returns 0 on success, -1 for NULL parameter, -2 if file not found
// Function Description : Retrieves and displays statistical information about a file identified by its name.
//                      : Prints details such as file name, inode number, file size, link count, reference count,
//                        and file permissions (read-only, write, or read-write).
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int stat_file(char *name)
{
    PINODE temp = head;
    int i = 0;

    if (name == NULL)
        return -1;

    while (temp != NULL)
    {
        if (strcmp(name, temp->FileName) == 0)
            break;
        temp = temp->next;
    }

    if (temp == NULL)
        return -2;

    printf("\n---------Statistical Information about file---------\n");
    printf("File name : %s\n", temp->FileName);
    printf("Inode Number %d\n", temp->InodeNumber);
    printf("File size : %d\n", temp->FileActualSize);
    printf("Link count : %d\n", temp->LinkCount);
    printf("Reference count : %d\n", temp->ReferenceCount);

    if (temp->permission == 1)
        printf("File Permission : Read only\n");
    else if (temp->permission == 2)
        printf("File Permission : Write\n");
    else if (temp->permission == 3)
        printf("File Permission : Read & Write\n");
    printf("----------------------------------------------------\n\n");

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Function Name    : truncate_File
//Input parameters : char *name: Name of the file to truncate.
//Return value     : int  Returns 0 upon successful truncation.
//                        Returns -1 if the file descriptor is invalid (file not open).
//Function Description : This function truncates (clears) the contents of the file specified by 'name' to zero bytes.
//                     : It locates the file descriptor associated with 'name' using GetFDFromName function.
//                     : If the file descriptor is valid, it sets the file's buffer to zero, resets read and write offsets to zero,
//                       and sets the file's actual size to zero.
//                     : This operation effectively removes all data from the file, preparing it for reuse or deletion.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int truncate_File(char *name)
{
    int fd = GetFDFromName(name);
    if (fd == -1)
        return -1;

    memset(UFDTArr[fd].ptrfiletable->ptrinode->Buffer, 0, 1024);
    UFDTArr[fd].ptrfiletable->readoffset = 0;
    UFDTArr[fd].ptrfiletable->writeoffset = 0;
    UFDTArr[fd].ptrfiletable->ptrinode->FileActualSize = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name        : main
// Input parameters     : -
// Return value         : int - Returns 0 upon successful execution of the program
// Function Description : Initializes the superblock and creates the Data Inode Linked Block (DILB).
//                      : Provides a command-line interface for interacting with the Marvellous Virtual File System.
//                      : Supports commands such as ls, closeall, clear, help, exit, stat, fstat, close, rm,
//                        man, write, truncate, create, open, read, and lseek, each with appropriate error handling.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char *ptr = NULL;
    int ret = 0, fd = 0, count = 0;
    char command[4][80], str[80], arr[1024];

    InitialiseSuperBlock();
    CreateDILB();

    while (1)
    {
        fflush(stdin);
        strcpy(str, "");

        printf("\nMarvellous VFS : > ");

        fgets(str, 80, stdin);

        count = sscanf(str, "%s %s %s %s", command[0], command[1], command[2], command[3]);

        if (count == 1)
        {
            if (strcmp(command[0], "ls") == 0)
            {
                ls_file();
                continue;
            }
            else if (strcmp(command[0], "closeall") == 0)
            {
                CloseAllFile();
                printf("All files closed successfully\n");
                continue;
            }
            else if (strcmp(command[0], "clear") == 0)
            {
                system("cls");
                continue;
            }
            else if (strcmp(command[0], "help") == 0)
            {
                DisplayHelp();
                continue;
            }
            else if (strcmp(command[0], "exit") == 0)
            {
                printf("Terminating the Customized Virtual File System\n");
                break;
            }
            else
            {
                printf("\nERROR : Command not found\n");
                continue;
            }
        }
        else if (count == 2)
        {
            if (strcmp(command[0], "stat") == 0)
            {
                ret = stat_file(command[1]);
                if (ret == -1)
                    printf("ERROR : Incorrect parameters\n");
                if (ret == -2)
                    printf("ERROR : There is no such file\n");
                continue;
            }
            else if (strcmp(command[0], "fstat") == 0)
            {
                ret = fstat_file(atoi(command[1]));
                if (ret == -1)
                    printf("ERROR : Incorrect parameters\n");
                if (ret == -2)
                    printf("ERROR : There is no such file\n");
                continue;
            }
            else if (strcmp(command[0], "close") == 0)
            {
                ret = CloseFileByName(command[1]);
                if (ret == -1)
                    printf("ERROR : There is no such file\n");
                continue;
            }
            else if (strcmp(command[0], "rm") == 0)
            {
                ret = rm_File(command[1]);
                if (ret == -1)
                    printf("ERROR : There is no such file\n");
                continue;
            }
            else if (strcmp(command[0], "man") == 0)
            {
                man(command[1]);
            }
            else if (strcmp(command[0], "write") == 0)
            {
                fd = GetFDFromName(command[1]);
                if (fd == -1)
                {
                    printf("Error : Incorrect parameter\n");
                    continue;
                }
                printf("Enter the data : \n");
                scanf("%[^\n]", arr);

                ret = strlen(arr);
                if (ret == 0)
                {
                    printf("Error : Incorrect parameter\n");
                    continue;
                }
                ret = WriteFile(fd, arr, ret);
                if (ret == -1)
                    printf("ERROR : Permission denied\n");
                if (ret == -2)
                    printf("ERROR : There is no sufficient memory to write\n");
                if (ret == -3)
                    printf("ERROR : It is not regular file\n");
            }
            else if (strcmp(command[0], "truncate") == 0)
            {
                ret = truncate_File(command[1]);
                if (ret == -1)
                    printf("Error : Incorrect parameter\n");
            }
            else
            {
                printf("\nERROR : Command not found\n");
                continue;
            }
        }
        else if (count == 3)
        {
            if (strcmp(command[0], "create") == 0)
            {
                ret = CreateFile(command[1], atoi(command[2]));
                if (ret >= 0)
                    printf("File is successfully created with file descriptor : %d\n", ret);
                if (ret == -1)
                    printf("ERROR : Incorrect parameters\n");
                if (ret == -2)
                    printf("ERROR : There are no inodes\n");
                if (ret == -3)
                    printf("ERROR : File already exists\n");
                if (ret == -4)
                    printf("ERROR : Memory allocation failure\n");
                continue;
            }
            else if (strcmp(command[0], "open") == 0)
            {
                ret = OpenFile(command[1], atoi(command[2]));
                if (ret >= 0)
                    printf("File is successfully opened with file descriptor : %d\n", ret);
                if (ret == -1)
                    printf("ERROR : Incorrect parameters\n");
                if (ret == -2)
                    printf("ERROR : File not present\n");
                if (ret == -3)
                    printf("ERROR : Permission denied\n");
                continue;
            }
            else if (strcmp(command[0], "read") == 0)
            {
                fd = GetFDFromName(command[1]);
                if (fd == -1)
                {
                    printf("Error : Incorrect parameter\n");
                    continue;
                }
                ptr = (char *)malloc(sizeof(atoi(command[2])) + 1);
                if (ptr == NULL)
                {
                    printf("Error : Memory allocation failure\n");
                    continue;
                }
                ret = ReadFile(fd, ptr, atoi(command[2]));
                if (ret == -1)
                    printf("ERROR : File not existing\n");
                if (ret == -2)
                    printf("ERROR : Permission denied\n");
                if (ret == -3)
                    printf("ERROR : Reached at end of file\n");
                if (ret == -4)
                    printf("ERROR : It is not regular file\n");
                if (ret == 0)
                    printf("ERROR : File empty\n");
                if (ret > 0)
                {
                    write(2, ptr, ret);
                }
                continue;
            }
            else
            {
                printf("\nERROR : Command not found\n");
                continue;
            }
        }
        else if (count == 4)
        {
            if (strcmp(command[0], "lseek") == 0)
            {
                fd = GetFDFromName(command[1]);
                if (fd == -1)
                {
                    printf("ERROR : Incorrect parameter\n");
                    continue;
                }
                ret = LseekFile(fd, atoi(command[2]), atoi(command[3]));
                if (ret == -1)
                {
                    printf("ERROR : Unable to perform lseek\n");
                }
            }
            else
            {
                printf("\nERROR : Command not found\n");
                continue;
            }
        }
        else
        {
            printf("\nERROR : Command not found\n");
            continue;
        }
    }
    return 0;
}
