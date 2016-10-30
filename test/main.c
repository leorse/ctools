#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#define __CT_MEMORY_DEBUG__ 1



#include "ctools.h"
#include "linked_list.h"



typedef struct LBL_STR
{
    struct LBL_STR* suivant;
    struct LBL_STR* precedent;
    int i;
    char une_chaine[25];
} TYP_STR;


void monlog(const char* message, ...)
{
    
    va_list args;
    fprintf(stderr, "dans mon log :");
    va_start(args, message);
    vfprintf(stderr, message,args);
    va_end(args);
}

void initElmt(TYP_STR** lst)
{
    unsigned int i = 0;
    TYP_STR* ptr = NULL;

    *lst = (TYP_STR*)__CT_creerElement(sizeof(TYP_STR), NULL);
    strcpy((*lst)->une_chaine, "mon pointeur_1!!");
    (*lst)->i = 1;
    ptr = *lst;
    for(i = 0; i < 20; i++)
    {
        ptr = (TYP_STR*)__CT_creerElement(sizeof(TYP_STR), ptr);
        //sprintf(ptr->une_chaine, "pointeur %10i", i);
        ptr->i = i + 2;
    }
}

/**
 * 
 * 
 */
void testMallocMemoryLeak()
{
    printf("in testMallocMemoryLeak\n");
    __CT_CLEAR_MEMORY_REGISTER();
    //generate a memory leak 
    __CT_MALLOC(10);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMallocMemoryLeak\n");
}

/**
 * 
 * 
 */
void testMallocNoMemoryLeak()
{
    char* memoryToClear = NULL;
    printf("in testMallocNoMemoryLeak\n");
    __CT_CLEAR_MEMORY_REGISTER();
    //don't generate a memory leak 
    memoryToClear = (char*)__CT_MALLOC(25);
    __CT_FREE(memoryToClear);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMallocNoMemoryLeak\n");
}

void testSprintfOK()
{
    char* memoryToClear = NULL;
    const short size = 25;
    printf("in testSprintfOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    //don't generate a memory leak 
    memoryToClear = (char*)__CT_MALLOC(size);
    //insert string: size < 25
    __CT_SPRINTF(memoryToClear, "A string of %d car", size);
    printf ("  - String: %s\n", memoryToClear);
    __CT_FREE(memoryToClear);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testSprintfOK\n");
}

void testSprintfKO()
{
    char* memoryToClear = NULL;
    const short size = 25;
    printf("in testSprintfKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    //don't generate a memory leak 
    memoryToClear = (char*)__CT_MALLOC(size);
    //insert string: size > 25
    __CT_SPRINTF(memoryToClear, "A huge big string of %d car", size);
    printf ("  - String: %s\n", memoryToClear);
    __CT_FREE(memoryToClear);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testSprintfKO\n");
}

void testMemcpyOK()
{
    char* memorySrc = NULL;
    char* memoryDest = NULL;
    const short sizeSrc = 25;
    const short sizeDest = 25;
    const short sizeTrf = 25;
    printf("in testMemcpyOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    memoryDest = (char*)__CT_MALLOC(sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_FREE(memorySrc);
    __CT_FREE(memoryDest);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMemcpyOK\n");
}

void testMemcpySrcKO()
{
    char* memorySrc = NULL;
    char* memoryDest = NULL;
    const short sizeSrc = 25;
    const short sizeDest = 30;
    const short sizeTrf = 27;
    printf("in testMemcpySrcKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    memoryDest = (char*)__CT_MALLOC(sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_FREE(memorySrc);
    __CT_FREE(memoryDest);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMemcpySrcKO\n");
}

void testMemcpyDestKO()
{
    char* memorySrc = NULL;
    char* memoryDest = NULL;
    const short sizeSrc = 30;
    const short sizeDest = 25;
    const short sizeTrf = 27;
    printf("in testMemcpyDestKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    memoryDest = (char*)__CT_MALLOC(sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_FREE(memorySrc);
    __CT_FREE(memoryDest);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMemcpyDestKO\n");
}

void testStrcpyOK()
{
    char* memorySrc = NULL;
    const short sizeSrc = 30;
    printf("in testStrcpyOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    __CT_STRCPY(memorySrc, "a simple copy");
    __CT_FREE(memorySrc);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStrcpyOK\n");
}

void testStrcpyKO()
{
    char* memorySrc = NULL;
    const short sizeSrc = 15;
    printf("in testStrcpyKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    __CT_STRCPY(memorySrc, "a too much long copy");
    __CT_FREE(memorySrc);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStrcpyKO\n");
}

void testMemsetOK()
{
    char* memorySrc = NULL;
    const short sizeSrc = 30;
    printf("in testMemsetOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    __CT_MEMSET(memorySrc, '\0', sizeSrc);
    __CT_FREE(memorySrc);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMemsetOK\n");
}

void testMemsetKO()
{
    char* memorySrc = NULL;
    const short sizeSrc = 15;
    printf("in testMemsetKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    memorySrc = (char*)__CT_MALLOC(sizeSrc);
    __CT_MEMSET(memorySrc, '\0', sizeSrc+5);
    __CT_FREE(memorySrc);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testMemsetKO\n");
}

void testLog()
{
    printf("in testLog\n");
    __CT_CLEAR_MEMORY_REGISTER();
    printf("   default log\n");
    __CT_TRACE_ALLOC();
    printf("   local log\n");
    __CT_SET_LOG_CALLBACK(monlog);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testLog\n");
}

/*****************************************************
 * STATIC TEST
*****************************************************/

//test that we don't see le static element in the trace result
void testAddStatic()
{
    char tab[50];
    char* memory1 = NULL;
    char* memory2 = NULL;
    char* memory3 = NULL;
    const short size = 30;
    
    printf("in testAddStatic\n");
    __CT_CLEAR_MEMORY_REGISTER();
    memory1 = (char*)__CT_MALLOC(size);
    __CT_MEMSET(memory1, '\0', size);
    __CT_ADD_STATIC(tab, 50);
    memory2 = (char*)__CT_MALLOC(size);
    __CT_MEMSET(memory2, '\0', size);
    memory3 = (char*)__CT_MALLOC(size);
    __CT_MEMSET(memory3, '\0', size);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testAddStatic\n");
}

void testStaticSprintfOK()
{
    const short size = 25;
    char memoryToClear[size];
    
    printf("in testStaticSprintfOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    __CT_ADD_STATIC(memoryToClear, size);
    //insert string: size < 25
    __CT_SPRINTF(memoryToClear, "A string of %d car", size);
    printf ("  - String: %s\n", memoryToClear);
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticSprintfOK\n");
}

void testStaticSprintfKO()
{
    const short size = 25;
    char memoryToClear[size];
    
    printf("in testStaticSprintfKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    __CT_ADD_STATIC(memoryToClear, size);
    //insert string: size > 25
    __CT_SPRINTF(memoryToClear, "A huge big string of %d car", size);
    printf ("  - String: %s\n", memoryToClear);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticSprintfKO\n");
}

void testStaticMemcpyOK()
{
    
    const short sizeSrc = 25;
    const short sizeDest = 25;
    char memorySrc[sizeSrc];
    char memoryDest[sizeDest];
    const short sizeTrf = 25;
    printf("in testStaticMemcpyOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_ADD_STATIC(memoryDest, sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticMemcpyOK\n");
}

void testStaticMemcpySrcKO()
{
    const short sizeSrc = 25;
    const short sizeDest = 30;
    char memorySrc[sizeSrc];
    char memoryDest[sizeDest];
    
    const short sizeTrf = 27;
    printf("in testStaticMemcpySrcKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_ADD_STATIC(memoryDest, sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticMemcpySrcKO\n");
}

void testStaticMemcpyDestKO()
{
    const short sizeSrc = 25;
    const short sizeDest = 30;
    char memorySrc[sizeSrc];
    char memoryDest[sizeDest];
    const short sizeTrf = 27;
    printf("in testStaticMemcpyDestKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_ADD_STATIC(memoryDest, sizeDest);
    __CT_MEMCPY(memoryDest, memorySrc, sizeTrf);
    
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticMemcpyDestKO\n");
}

void testStaticStrcpyOK()
{
    const short sizeSrc = 30;
    char memorySrc[sizeSrc];
    
    printf("in testStaticStrcpyOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_STRCPY(memorySrc, "a simple copy");

    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticStrcpyOK\n");
}

void testStaticStrcpyKO()
{
    const short sizeSrc = 15;
    char memorySrc[sizeSrc];
    
    printf("in testStaticStrcpyKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_STRCPY(memorySrc, "a too much long copy");
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticStrcpyKO\n");
}

void testStaticMemsetOK()
{
    const short sizeSrc = 30;
    char memorySrc[sizeSrc];
    
    printf("in testStaticMemsetOK\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_MEMSET(memorySrc, '\0', sizeSrc);

    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticMemsetOK\n");
}

void testStaticMemsetKO()
{
    const short sizeSrc = 15;
    char* memorySrc[sizeSrc];
    
    printf("in testStaticMemsetKO\n");
    __CT_CLEAR_MEMORY_REGISTER();
    
    __CT_ADD_STATIC(memorySrc, sizeSrc);
    __CT_MEMSET(memorySrc, '\0', sizeSrc+5);
    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();
    printf("out testStaticMemsetKO\n");
}

int main(int argc, char **argv)
{
    TYP_STR* mon_premier;
    
    testMallocMemoryLeak();
    testMallocNoMemoryLeak();
    
    testSprintfOK();
    testSprintfKO();
    
    testMemcpyOK();
    testMemcpySrcKO();
    testMemcpyDestKO();
    
    testStrcpyOK();
    testStrcpyKO();
    
    testMemsetOK();
    testMemsetKO();
    
    testLog();
    
    /********************************************
     * STATIQUE
    **********************************************/
    testAddStatic();
    
    testStaticSprintfOK();
    testStaticSprintfKO();
    
    testStaticMemcpyOK();
    testStaticMemcpySrcKO();
    testStaticMemcpyDestKO();
    
    testStaticStrcpyOK();
    testStaticStrcpyKO();
    
    testStaticMemsetOK();
    testStaticMemsetKO();
    
    __CT_CLEAR_MEMORY_REGISTER();
    initElmt(&mon_premier);

    __CT_libererElements(mon_premier, sizeof(TYP_STR));

    __CT_TRACE_ALLOC();
    __CT_CLEAR_MEMORY_REGISTER();

    return 0;
}
