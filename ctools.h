#include <stdio.h>
#include <stdlib.h>

#ifndef __CTOOLS_H__
#define __CTOOLS_H__


#define __MEMORY_DEBUG__ 1


// 1= mode sans sortie
// 2= mode avec sortie (verbeux)
#ifdef __MEMORY_DEBUG__

#define __SIZE_TYPE_TAILLE__ unsigned long int
#define __MEMORY_TAILLE_FIC 25
typedef struct LBL___MEMORY_DEBUG
{
    void* ptr;
    __SIZE_TYPE_TAILLE__ taille;
    int    ligne;
    char   fichier[__MEMORY_TAILLE_FIC + 1];
} TYP___MEMORY_DEBUG;
TYP___MEMORY_DEBUG *__alloc_lst = NULL;
long __alloc_memory = 0L;
long __alloc_memory_max = 0L;

#define __MEMORY_CHUNK 5L
#define __CLEAR_MEMORY_REGISTER   __alloc_memory = 0L; \
                                  if(__alloc_lst != NULL) {free(__alloc_lst);} \
                                  __alloc_lst = NULL;

char *__basename(char const *path)
{
    char *s = strrchr(path, '/');
    if (!s)
    {
        s = strrchr(path, '\\');
        if (!s)
        {
            return strdup(path);
        }
    }
    return strdup(s + 1);
}

int __insert_alloc_lst(void* ptr, __SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic)
{
    long inc = 0;
    int trouve = 0;
    int long_fic = 0;
    int taille_coupe = 0;
    char *tmpFic = NULL;

    if(fic != NULL)
    {
        tmpFic = __basename(fic);
        long_fic = strlen(tmpFic);
        taille_coupe = long_fic > __MEMORY_TAILLE_FIC ? __MEMORY_TAILLE_FIC : long_fic;
    }

    if(__alloc_memory_max == 0)
    {
        __alloc_memory_max += __MEMORY_CHUNK;
        __alloc_memory++;
        __alloc_lst = (TYP___MEMORY_DEBUG*)malloc(sizeof(TYP___MEMORY_DEBUG) * __alloc_memory_max);
        if(__alloc_lst == NULL)
        {
            return 1;
        }
        memset(__alloc_lst, 0, sizeof(TYP___MEMORY_DEBUG)*__alloc_memory_max);

        __alloc_lst[0].ptr = ptr;
        __alloc_lst[0].taille = taille;
        __alloc_lst[0].ligne = ligne;
        strncpy(__alloc_lst[0].fichier, tmpFic, taille_coupe);
        __alloc_lst[0].fichier[taille_coupe] = '\0';
#if __MEMORY_DEBUG__ == 2
        printf("__MEMORY__ premier init, alloc de %ld*%ld=%ld, taille lst:%ld, ligne:%d, fonc:%s\n", __alloc_memory_max, (__SIZE_TYPE_TAILLE__)sizeof(TYP___MEMORY_DEBUG),
               sizeof(TYP___MEMORY_DEBUG)*__alloc_memory_max,
               __MEMORY_CHUNK, __alloc_lst[0].ligne, __alloc_lst[0].fichier);
#endif // __MEMORY_DEBUG__
    }
    else
    {
        trouve = 0;
        while(inc < __alloc_memory_max && trouve == 0)
        {
            if(__alloc_lst[inc].ptr == NULL)
            {
                __alloc_lst[inc].ptr = ptr;
                __alloc_lst[inc].taille = taille;
                __alloc_lst[inc].ligne = ligne;
                strncpy(__alloc_lst[inc].fichier, tmpFic, taille_coupe);
                __alloc_lst[inc].fichier[taille_coupe] = '\0';
#if __MEMORY_DEBUG__ == 2
                printf("__MEMORY__ trouve vide elmt:%ld, ajout taille:%ld, ligne:%d, fichier:%s\n", inc, taille, __alloc_lst[inc].ligne, __alloc_lst[inc].fichier);
#endif // __MEMORY_DEBUG__
                __alloc_memory++;
                trouve = 1;
            }
            inc++;
        }
        if(trouve == 0)
        {
            __alloc_lst = (TYP___MEMORY_DEBUG*)realloc(__alloc_lst, sizeof(TYP___MEMORY_DEBUG) * (__alloc_memory_max + __MEMORY_CHUNK));
            memset(__alloc_lst + __alloc_memory_max, 0, sizeof(TYP___MEMORY_DEBUG)*__MEMORY_CHUNK);
            __alloc_lst[__alloc_memory_max].ptr = ptr;
            __alloc_lst[__alloc_memory_max].taille = taille;
            __alloc_lst[__alloc_memory_max].ligne = ligne;
            strncpy(__alloc_lst[__alloc_memory_max].fichier, tmpFic, taille_coupe);
            __alloc_lst[__alloc_memory_max].fichier[taille_coupe] = '\0';
            __alloc_memory++;
#if __MEMORY_DEBUG__ == 2
            printf("__MEMORY__ pas trouve vide, augment lst a %ld, raz de %ld a %ld\n", sizeof(TYP___MEMORY_DEBUG) * (__alloc_memory_max + __MEMORY_CHUNK), __alloc_memory_max, sizeof(TYP___MEMORY_DEBUG)*__MEMORY_CHUNK);
#endif // __MEMORY_DEBUG__
            __alloc_memory_max += __MEMORY_CHUNK;

        }
    }
    if(tmpFic != NULL)
    {
        free(tmpFic);
    }
    return 0;
}

#define __MALLOC(TAILLE)  __malloc_alloc_lst(TAILLE, __LINE__, __FILE__)
void* __malloc_alloc_lst(__SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic)
{
    void* ptr = NULL;
    ptr = (void*)malloc(taille);
    if(ptr != NULL)
    {
#if __MEMORY_DEBUG__ == 2
        printf("__MEMORY__ alloc ligne:%d, fonction:%s\n", ligne, fic);
#endif // __MEMORY_DEBUG__
        __insert_alloc_lst(ptr, taille, ligne, fic);
    }
    return ptr;
}

#define __FREE(PTR)  __free_alloc_lst(PTR)
void __free_alloc_lst(void* ptr)
{
    int inc = 0;
    int trouve = 0;

    if(ptr == NULL)
    {
        return;
    }

    while(inc < __alloc_memory_max && trouve == 0)
    {
        if(__alloc_lst[inc].ptr == ptr)
        {
#if __MEMORY_DEBUG__ == 2
            printf("__MEMORY__ trouve elmt a %d, taille:%ld\n", inc, __alloc_lst[inc].taille);
#endif // __MEMORY_DEBUG__
            __alloc_lst[inc].ptr = NULL;
            __alloc_lst[inc].taille = 0;
            __alloc_memory --;
            trouve = 1;
        }
        inc++;
    }
    free(ptr);
}



int __tri_callback_alloc_lst(const void *element1, const void *element2)
{
    const TYP___MEMORY_DEBUG *elem1 = (TYP___MEMORY_DEBUG*)element1;
    const TYP___MEMORY_DEBUG *elem2 = (TYP___MEMORY_DEBUG*)element2;
    int retStrcmp = 0;

    if(elem1->ptr == NULL && elem2->ptr != NULL)
    {
        return 1;
    }
    if(elem1->ptr != NULL && elem2->ptr == NULL)
    {
        return -1;
    }
    if(elem1->ptr == NULL && elem2->ptr == NULL)
    {
        return 0;
    }
    retStrcmp = strcmp(elem1->fichier, elem2->fichier);
    //si les fichiers et les lignes sont identiques, alors c'est une allocation au même endroit
    if(retStrcmp < 0)
    {
        return 1;
    }
    if(retStrcmp > 0)
    {
        return -1;
    }
    else
    {
        if(elem1->ligne == elem2->ligne)
        {
            return 0;
        }
        if(elem1->ligne != elem2->ligne)
        {
            //on renvoie 1 si ligne1 est supérieur à ligne2
            return elem1->ligne > elem2->ligne ? 1 : -1;
        }
    }
    return 0;
}


/** \brief cette fonction tri la liste d'allocation
 *
 * \return void
 *
 */
void __tri_alloc_lst()
{
    qsort(__alloc_lst, __alloc_memory_max, sizeof(TYP___MEMORY_DEBUG), __tri_callback_alloc_lst);
}

#define __TRACE_ALLOC() __trace_alloc_lst();
/** \brief fonction qui affiche le contenu de la liste d'allocation
 *  sous forme d'un tableau
 *  chaque ligne du tableau est la somme de toutes les allocations d'une même ligne de code
 *
 * \return void
 *
 */
void __trace_alloc_lst()
{
    long cumul = 0L;
    unsigned long totalCumul = 0L;
    int ligne_prec = 0;
    int inc = 0;
    unsigned long nbElem = 0L;
    unsigned long totalNbElem = 0L;
    int STOP = 0;
    char fichier_prec[__MEMORY_TAILLE_FIC + 1] = "";
#if __MEMORY_DEBUG__ == 2
    for(inc = 0; inc < __alloc_memory_max; inc++)
    {
        if(__alloc_lst[inc].ptr == NULL)
        {
            printf("__MEMORY__ N:%3d;NULL\n", inc);
        }
        else
        {
            printf("__MEMORY__ N:%3d;T:%10ld;L:%5d;F:%40s\n", inc, __alloc_lst[inc].taille,
                   __alloc_lst[inc].ligne, __alloc_lst[inc].fichier);
        }
    }
#endif // __MEMORY_DEBUG__
    __tri_alloc_lst();
#if __MEMORY_DEBUG__ == 2
    for(inc = 0; inc < __alloc_memory_max; inc++)
    {
        if(__alloc_lst[inc].ptr == NULL)
        {
            printf("__MEMORY__ N:%3d;NULL\n", inc);
        }
        else
        {
            printf("__MEMORY__ N:%3d;T:%10ld;L:%5d;F:%40s\n", inc, __alloc_lst[inc].taille,
                   __alloc_lst[inc].ligne, __alloc_lst[inc].fichier);
        }
    }
#endif // __MEMORY_DEBUG__
    if(__alloc_memory == 0)
    {
        printf("/----------------------------------------------------\\\n");
        printf("| AUCUNE ALLOCATION RESTANTE                         |\n");
        printf("\\----------------------------------------------------/\n");
    }
    else
    {
        ligne_prec = __alloc_lst[0].ligne;
        strcpy(fichier_prec, __alloc_lst[0].fichier);
        printf("/------------------------------------------------------------------\\\n");
        printf("|NB ALLOC|   CUMUL  |LIGNE|               FICHIER                  |\n");
        printf("|------------------------------------------------------------------|\n");

        while(inc < __alloc_memory_max && STOP == 0)
        {
            nbElem ++;
            totalNbElem++;
            cumul += __alloc_lst[inc].taille;
            totalCumul += __alloc_lst[inc].taille;
            //si le prochain element est au dela de la taille
            //si la ligne suivante est different de la ligne en cours
            //si le prochain element est NULL
            //si le fichier suivant est different du fichier en cours
            //alors RUPTURE, on affiche le cumul
            if( (inc + 1) == __alloc_memory_max || __alloc_lst[inc + 1].ligne != ligne_prec
                    || __alloc_lst[inc + 1].ptr == NULL || strcmp(__alloc_lst[inc + 1].fichier, fichier_prec))
            {
                printf("|%8ld|%10ld|%5d|%40s|\n", nbElem, cumul, __alloc_lst[inc].ligne, __alloc_lst[inc].fichier);
                cumul = 0;
                nbElem = 0;
                if((inc + 1) != __alloc_memory_max && __alloc_lst[inc + 1].ptr != NULL)
                {
                    ligne_prec = __alloc_lst[inc + 1].ligne;
                    strcpy(fichier_prec, __alloc_lst[inc + 1].fichier);
                }
                else
                {
                    STOP = 1;
                }
            }
            inc++;
        }
        printf("|------------------------------------------------------------------|\n");
        printf("|%8ld|%10ld|                                              |\n", totalNbElem, totalCumul);
        printf("\\------------------------------------------------------------------/\n");
        //printf("__MEMORY__ %ld elements pour %ld oct alloues\n", __alloc_memory, cumul);
    }
}
#else

#define __CLEAR_MEMORY_REGISTER
#define __TRACE_ALLOC(MODE)
#define __FREE(PTR) free(PTR)
#define __MALLOC(TAILLE) malloc(TAILLE)

#endif // __MEMORY_DEBUG__

#endif // __CTOOLS_H__
