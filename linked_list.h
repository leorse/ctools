#ifndef LINKED_LIST_H_INCLUDED
#define LINKED_LIST_H_INCLUDED

typedef struct LBL_VOID
{
    struct LBL_VOID* suivant;
    struct LBL_VOID* precedent;
} TYP_VOID;


void* creerElement(size_t tailleElt, void* lst)
{
    TYP_VOID* elt = NULL;
    TYP_VOID* ptr = NULL;

    elt = (TYP_VOID*)__MALLOC(tailleElt);
    if(elt == NULL)
    {
        return NULL;
    }
    //elt->suivant
    ptr = (TYP_VOID*)elt;
    ptr->suivant = NULL;
    if(lst != NULL)
    {
        //lst->suivant
        ptr = (TYP_VOID*)lst;
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


void libererElements(void* premierElmt, size_t tailleElt)
{
    TYP_VOID* ptr = NULL;
    TYP_VOID* suiv = NULL;

    if(premierElmt == NULL)
    {
        return;
    }

    ptr = (TYP_VOID*)premierElmt;

    do
    {
        suiv = ptr->suivant;
        __FREE(ptr);
        ptr = suiv;
    }
    while(suiv != NULL);

}

#endif // LINKED_LIST_H_INCLUDED
