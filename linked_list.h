#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

typedef struct __CT_LBL_VOID
{
    struct __CT_LBL_VOID* suivant;
    struct __CT_LBL_VOID* precedent;
} __CT_TYP_VOID;


void* __CT_creerElement(size_t tailleElt, void* lst)
{
    __CT_TYP_VOID* elt = NULL;
    __CT_TYP_VOID* ptr = NULL;

    elt = (__CT_TYP_VOID*)__CT_MALLOC(tailleElt);
    if(elt == NULL)
    {
        return NULL;
    }
    //elt->suivant
    ptr = (__CT_TYP_VOID*)elt;
    ptr->suivant = NULL;
    if(lst != NULL)
    {
        //lst->suivant
        ptr = (__CT_TYP_VOID*)lst;
        ptr->suivant = elt;
        //elt->precedent
        elt->precedent = lst;
    }
    else
    {
        //elt->precedent
        elt->precedent = NULL;
    }

    return elt;
}


void __CT_libererElements(void* premierElmt, size_t tailleElt)
{
    __CT_TYP_VOID* ptr = NULL;
    __CT_TYP_VOID* suiv = NULL;

    if(premierElmt == NULL)
    {
        return;
    }

    ptr = (__CT_TYP_VOID*)premierElmt;

    do
    {
        suiv = ptr->suivant;
        __CT_FREE(ptr);
        ptr = suiv;
    }
    while(suiv != NULL);

}

#endif // LINKED_LIST_H_INCLUDED
