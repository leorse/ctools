#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

#include "../ctools.h"
#include "../linked_list.h"

typedef struct LBL_STR
{
    struct LBL_STR* suivant;
    struct LBL_STR* precedent;
    int i;
    char une_chaine[25];
} TYP_STR;


void initElmt(TYP_STR** lst)
{
    unsigned int i = 0;
    TYP_STR* ptr = NULL;

    *lst = creerElement(sizeof(TYP_STR), NULL);
    strcpy((*lst)->une_chaine, "mon pointeur_1!!");
    (*lst)->i = 1;
    ptr = *lst;
    for(i = 0; i < 200; i++)
    {
        ptr = creerElement(sizeof(TYP_STR), ptr);
        //sprintf(ptr->une_chaine, "pointeur %10i", i);
        ptr->i = i + 2;
    }
}

int main(int argc, char **argv)
{
    TYP_STR* mon_premier;
    char* liste = NULL;
    // TYP_STR* ptr, *ptr2;

    //initElmt(&mon_premier);

    /*printf("Premier sens\n");
    ptr = mon_premier;
    while(ptr != NULL)
    {
        printf("      str:%d\n", ptr->i);
        ptr2 = ptr;
        ptr = ptr->suivant;
    }
    printf("Sens inverse:\n");
    while(ptr2 != NULL)
    {
        printf("      str:%d\n", ptr2->i);
        ptr2 = ptr2->precedent;
    }*/
   /* __MALLOC(15);
    __MALLOC(55);
    __MALLOC(65);
    __MALLOC(25);
    __MALLOC(1555);__MALLOC(69);__MALLOC(125);
    __MALLOC(125);

    //libererElements(mon_premier, sizeof(TYP_STR));

    __MALLOC(200);
    __MALLOC(200);
    __MALLOC(200);
    __MALLOC(16);
    __MALLOC(200);__MALLOC(168);__MALLOC(150);
    __MALLOC(15);
*/
    liste = (char*)__MALLOC(10);
    liste = (char*)__REALLOC(liste, 20);
    sprintf(liste, "AZERTYUIOP1234567890");
    __FREE(liste);
    __TRACE_ALLOC()
    __CLEAR_MEMORY_REGISTER

    return 0;
}
