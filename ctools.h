#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef __CTOOLS_H__
#define __CTOOLS_H__

//#ifndef __CT_LOG
//#define __CT_LOG(MESSAGE,...)  fprintf(stderr, MESSAGE, ##__VA_ARGS__)
//#endif 
// 1= mode sans sortie
// 2= mode avec sortie (verbeux)
#ifdef __CT_MEMORY_DEBUG__

#define __CT_MEMORY_TEST_CRUSH 0x4A
#define __CT_SIZE_TYPE_TAILLE__ unsigned long int
#define __CT_MEMORY_TAILLE_FIC 25
typedef struct __CT_LBL_MEMORY_DEBUG
{
    void* ptr;
    __CT_SIZE_TYPE_TAILLE__ taille;
    int    ligne;
    unsigned short checksum;
    char   fichier[__CT_MEMORY_TAILLE_FIC + 1];
} __CT_TYP_MEMORY_DEBUG;
__CT_TYP_MEMORY_DEBUG *__CT_alloc_lst = NULL;
long __CT_alloc_memory = 0L;
long __CT_alloc_memory_max = 0L;

void __CT_log(char const *message, ...);
void (*__CT_LOG)(const char* message, ...) = __CT_log;

#define __CT_MEMORY_CHUNK 5L
#define __CT_CLEAR_MEMORY_REGISTER()   __CT_alloc_memory = 0L; \
                                  if(__CT_alloc_lst != NULL) {free(__CT_alloc_lst);} \
                                  __CT_alloc_lst = NULL; \
                                  __CT_alloc_memory_max = 0; \
                                  __CT_LOG = __CT_log;
#define __CT_SET_LOG_CALLBACK(FUNCPTR)  __CT_LOG = FUNCPTR
/**
 * \brief Default log function
 * \return none
*/
void __CT_log(char const *message, ...)
{
    va_list args;
    
        va_start(args, message);
        vfprintf(stderr, message, args);
        va_end(args);
}
    

/**
 * \brief Get only the filename by looking for the last / or \
 * \return the filename resulting of a strdup, MUST BE FREED
*/
void __CT_basename(char const *path, char* dest)
{
    //looking for the last / (UNIX)
    const char *string = strrchr(path, '/');
    int longFic = 0; //the size of the found path
    
    if(dest == NULL) return;
    
    if (!string)
    {
        //if not found, looking for the last \ (Windows)
        string = strrchr(path, '\\');
        if (!string)
        {
            //if still not found, duplicate the path
            string=path;
            //return strdup(path);
        }
        else
        {
            //if found, s point to '/' or '\', so we do s+1
            string++;
        }
    }
    else
    {
        //if found, s point to '/' or '\', so we do s+1
        string++;
    }
    
    //if found, duplicate the string. +1 to avoid the \ or /
    //return strdup(s + 1);
    longFic = strlen(string);
    memset(dest, '\0', __CT_MEMORY_TAILLE_FIC+1);
    if(longFic>__CT_MEMORY_TAILLE_FIC)
    {
        //size too long for destination string, so we cut it to __MEMORY_TAILLE_FIC
        strncpy(dest, string, __CT_MEMORY_TAILLE_FIC);
    }
    else
    {
        //string fits well into destination string, so we cut it to __MEMORY_TAILLE_FIC
        strncpy(dest, string, longFic);
    }
    
}


/**
 * \brief Empty a cell of the memory array
 */
void __CT_vider_alloc_indice(int indice)
{
    //check if indice is in the top boundary
    if(indice<__CT_alloc_memory_max && indice>=0)
    {
        memset(&__CT_alloc_lst[indice], 0, sizeof(__CT_TYP_MEMORY_DEBUG));
        __CT_alloc_memory--;
    }
}

/**
 * \brief add an element into the main array
 * \param ptr the element to add is pointed by ptr
 * \param taille, the size of the element to add
 * \param ligne, the line in the code where the element is allocated
 * \param fic, the filename (full or short) where the element si allocated
 */
int __CT_insert_alloc_lst(void* ptr, __CT_SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic)
{
    long inc = 0;
    int trouve = 0;


    if(__CT_alloc_memory_max == 0)
    {
        __CT_alloc_memory_max += __CT_MEMORY_CHUNK;
        __CT_alloc_memory++;
        __CT_alloc_lst = (__CT_TYP_MEMORY_DEBUG*)malloc(sizeof(__CT_TYP_MEMORY_DEBUG) * __CT_alloc_memory_max);
        if(__CT_alloc_lst == NULL)
        {
            return 1;
        }
        memset(__CT_alloc_lst, 0, sizeof(__CT_TYP_MEMORY_DEBUG)*__CT_alloc_memory_max);

        __CT_alloc_lst[0].ptr = ptr;
        __CT_alloc_lst[0].taille = taille;
        __CT_alloc_lst[0].ligne = ligne;
        __CT_basename(fic, __CT_alloc_lst[0].fichier);
        //strncpy(__alloc_lst[0].fichier, tmpFic, taille_coupe);
        //__alloc_lst[0].fichier[taille_coupe] = '\0';
#if __CT_MEMORY_DEBUG__ == 2
        printf("__MEMORY__ premier init, alloc de %ld*%ld=%ld, taille lst:%ld, ligne:%d, fonc:%s\n", __CT_alloc_memory_max, (__CT_SIZE_TYPE_TAILLE__)sizeof(__CT_TYP_MEMORY_DEBUG),
               sizeof(__CT_TYP_MEMORY_DEBUG)*__CT_alloc_memory_max,
               __CT_MEMORY_CHUNK, __CT_alloc_lst[0].ligne, __CT_alloc_lst[0].fichier);
#endif // __MEMORY_DEBUG__
    }
    else
    {
        trouve = 0;
        while(inc < __CT_alloc_memory_max && trouve == 0)
        {
            if(__CT_alloc_lst[inc].ptr == NULL)
            {
                __CT_alloc_lst[inc].ptr = ptr;
                __CT_alloc_lst[inc].taille = taille;
                __CT_alloc_lst[inc].ligne = ligne;
                __CT_basename(fic, __CT_alloc_lst[inc].fichier);
                //__alloc_lst[inc].fichier[taille_coupe] = '\0';
#if __CT_MEMORY_DEBUG__ == 2
                printf("__MEMORY__ trouve vide elmt:%ld, ajout taille:%ld, ligne:%d, fichier:%s\n", inc, taille, __CT_alloc_lst[inc].ligne, __CT_alloc_lst[inc].fichier);
#endif // __MEMORY_DEBUG__
                __CT_alloc_memory++;
                trouve = 1;
            }
            inc++;
        }
        if(trouve == 0)
        {
            __CT_alloc_lst = (__CT_TYP_MEMORY_DEBUG*)realloc(__CT_alloc_lst, sizeof(__CT_TYP_MEMORY_DEBUG) * (__CT_alloc_memory_max + __CT_MEMORY_CHUNK));
            memset(__CT_alloc_lst + __CT_alloc_memory_max, 0, sizeof(__CT_TYP_MEMORY_DEBUG)*__CT_MEMORY_CHUNK);
            __CT_alloc_lst[__CT_alloc_memory_max].ptr = ptr;
            __CT_alloc_lst[__CT_alloc_memory_max].taille = taille;
            __CT_alloc_lst[__CT_alloc_memory_max].ligne = ligne;
            __CT_basename(fic, __CT_alloc_lst[__CT_alloc_memory_max].fichier);
            //__alloc_lst[__alloc_memory_max].fichier[taille_coupe] = '\0';
            __CT_alloc_memory++;
#if __CT_MEMORY_DEBUG__ == 2
            printf("__MEMORY__ pas trouve vide, augment lst a %ld, raz de %ld a %ld\n", sizeof(__CT_TYP_MEMORY_DEBUG) * (__CT_alloc_memory_max + __CT_MEMORY_CHUNK), __CT_alloc_memory_max, sizeof(__CT_TYP_MEMORY_DEBUG)*__CT_MEMORY_CHUNK);
#endif // __MEMORY_DEBUG__
            __CT_alloc_memory_max += __CT_MEMORY_CHUNK;

        }
    }
    
    return 0;
}

/**
 * @brief 
 * @param ptr
 * @return 
 */
int __CT_avoir_alloc_indice(void* ptr)
{
    long inc = 0;
    
    while(inc < __CT_alloc_memory_max)
    {
        if(__CT_alloc_lst[inc].ptr == ptr)
        {
            return inc;
        }
        inc++;
    }
    return -1;
}

__CT_TYP_MEMORY_DEBUG* __CT_avoir_alloc_elem(void* ptr)
{
    __CT_TYP_MEMORY_DEBUG* retour = NULL;
    
    int indice = 0;
    indice = __CT_avoir_alloc_indice(ptr);
    
    if(indice!=-1)
    {
        retour = &__CT_alloc_lst[indice];
    }
    return retour;
}

void __CT_verifier_ecrasement(__CT_TYP_MEMORY_DEBUG* ptr_alloc_lst_element)
{
    void* ptr = ptr_alloc_lst_element->ptr;
    char carControl = '\0';
    __CT_SIZE_TYPE_TAILLE__ taille = ptr_alloc_lst_element->taille;
    int ligne = ptr_alloc_lst_element->ligne;
    char* fichier = ptr_alloc_lst_element->fichier;
    if(ptr != NULL)
    {
        carControl = *(char*)(((char*)ptr)+taille);
        if(carControl != __CT_MEMORY_TEST_CRUSH)
        {
            printf("ECRASEMENT du pointeur alloué ligne %d, fichier %s de taille %ld\n", ligne, fichier, taille);
        }
    }
}

#define __CT_REALLOC(PTR, TAILLE)  __CT_realloc_alloc_lst(PTR, TAILLE, __LINE__, __FILE__)
void* __CT_realloc_alloc_lst(void* oldPtr, __CT_SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic)
{
    void* ptr = NULL;
    int indice = 0;
    
    indice = __CT_avoir_alloc_indice(oldPtr);
    ptr = (void*)realloc(oldPtr, taille+1);
    if(ptr != NULL)
    {
#if __CT_MEMORY_DEBUG__ == 2
        printf("__MEMORY__ realloc indice:%d ligne:%d, fonction:%s\n", indice, ligne, fic);
#endif // __MEMORY_DEBUG__
        if(indice != -1)
        {
            __CT_vider_alloc_indice(indice);
        }
        __CT_insert_alloc_lst(ptr, taille, ligne, fic);
        *((char*)(((char*)ptr)+taille)) = __CT_MEMORY_TEST_CRUSH;
    }
    return ptr;
}

#define __CT_MALLOC(TAILLE)  __CT_malloc_alloc_lst(TAILLE, __LINE__, __FILE__)
void* __CT_malloc_alloc_lst(__CT_SIZE_TYPE_TAILLE__ taille, int ligne, const char* fic)
{
    void* ptr = NULL;
    ptr = (void*)malloc(taille+1);
    if(ptr != NULL)
    {
#if __CT_MEMORY_DEBUG__ == 2
        printf("__MEMORY__ alloc ligne:%d, fonction:%s\n", ligne, fic);
#endif // __MEMORY_DEBUG__
        *((char*)(((char*)ptr)+taille)) = __CT_MEMORY_TEST_CRUSH;
        __CT_insert_alloc_lst(ptr, taille, ligne, fic);
    }
    return ptr;
}

#define __CT_FREE(PTR)  __CT_free_alloc_lst(PTR)
void __CT_free_alloc_lst(void* ptr)
{
    int inc = 0;

    if(ptr == NULL)
    {
        printf("__ERROR_FREE__ element vaut NULL\n");
    }

    inc = __CT_avoir_alloc_indice(ptr);
    if(inc != -1)
    {
        __CT_verifier_ecrasement(&__CT_alloc_lst[inc]);
#if __CT_MEMORY_DEBUG__ == 2
            printf("__MEMORY__ trouve elmt a %d, taille:%ld\n", inc, __CT_alloc_lst[inc].taille);
#endif // __MEMORY_DEBUG__
        __CT_alloc_lst[inc].ptr = NULL;
        __CT_alloc_lst[inc].taille = 0;
        __CT_alloc_memory --;
    }
    else
    {
        printf("__ERROR_FREE__ element non trouvé ou déjà libéré\n");
    }
    free(ptr);
}



int __CT_tri_callback_alloc_lst(const void *element1, const void *element2)
{
    const __CT_TYP_MEMORY_DEBUG *elem1 = (__CT_TYP_MEMORY_DEBUG*)element1;
    const __CT_TYP_MEMORY_DEBUG *elem2 = (__CT_TYP_MEMORY_DEBUG*)element2;
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
void __CT_tri_alloc_lst()
{
    qsort(__CT_alloc_lst, __CT_alloc_memory_max, sizeof(__CT_TYP_MEMORY_DEBUG), __CT_tri_callback_alloc_lst);
}

#define __CT_TRACE_ALLOC() __CT_trace_alloc_lst()
/** \brief fonction qui affiche le contenu de la liste d'allocation
 *  sous forme d'un tableau
 *  chaque ligne du tableau est la somme de toutes les allocations d'une même ligne de code
 *
 * \return void
 *
 */
void __CT_trace_alloc_lst()
{
    long cumul = 0L;
    unsigned long totalCumul = 0L;
    int ligne_prec = 0;
    int inc = 0;
    unsigned long nbElem = 0L;
    unsigned long totalNbElem = 0L;
    int STOP = 0;
    char fichier_prec[__CT_MEMORY_TAILLE_FIC + 1] = "";
#if __CT_MEMORY_DEBUG__ == 2
    for(inc = 0; inc < __CT_alloc_memory_max; inc++)
    {
        if(__CT_alloc_lst[inc].ptr == NULL)
        {
            printf("__MEMORY__ N:%3d;NULL\n", inc);
        }
        else
        {
            printf("__MEMORY__ N:%3d;T:%10ld;L:%5d;F:%40s\n", inc, __CT_alloc_lst[inc].taille,
                   __CT_alloc_lst[inc].ligne, __CT_alloc_lst[inc].fichier);
        }
    }
#endif // __MEMORY_DEBUG__
    __CT_tri_alloc_lst();
#if __CT_MEMORY_DEBUG__ == 2
    for(inc = 0; inc < __CT_alloc_memory_max; inc++)
    {
        if(__CT_alloc_lst[inc].ptr == NULL)
        {
            printf("__MEMORY__ N:%3d;NULL\n", inc);
        }
        else
        {
            printf("__MEMORY__ N:%3d;T:%10ld;L:%5d;F:%40s\n", inc, __CT_alloc_lst[inc].taille,
                   __CT_alloc_lst[inc].ligne, __CT_alloc_lst[inc].fichier);
        }
    }
#endif // __MEMORY_DEBUG__
    if(__CT_alloc_memory == 0)
    {
        __CT_LOG("/----------------------------------------------------\\\n");
        __CT_LOG("| AUCUNE ALLOCATION RESTANTE                         |\n");
        __CT_LOG("\\----------------------------------------------------/\n");
        //printf("/----------------------------------------------------\\\n");
        //printf("| AUCUNE ALLOCATION RESTANTE                         |\n");
        //printf("\\----------------------------------------------------/\n");
    }
    else
    {
        ligne_prec = __CT_alloc_lst[0].ligne;
        strcpy(fichier_prec, __CT_alloc_lst[0].fichier);
        printf("/------------------------------------------------------------------\\\n");
        printf("|NB ALLOC|   CUMUL  |LIGNE|               FICHIER                  |\n");
        printf("|------------------------------------------------------------------|\n");
        inc=0;
        STOP=0;
        while(inc < __CT_alloc_memory_max && STOP == 0)
        {
            nbElem ++;
            totalNbElem++;
            cumul += __CT_alloc_lst[inc].taille;
            totalCumul += __CT_alloc_lst[inc].taille;
            //si le prochain element est au dela de la taille
            //si la ligne suivante est different de la ligne en cours
            //si le prochain element est NULL
            //si le fichier suivant est different du fichier en cours
            //alors RUPTURE, on affiche le cumul
            if( (inc + 1) == __CT_alloc_memory_max || __CT_alloc_lst[inc + 1].ligne != ligne_prec
                    || __CT_alloc_lst[inc + 1].ptr == NULL || strcmp(__CT_alloc_lst[inc + 1].fichier, fichier_prec))
            {
                printf("|%8ld|%10ld|%5d|%40s|\n", nbElem, cumul, __CT_alloc_lst[inc].ligne, __CT_alloc_lst[inc].fichier);
                cumul = 0;
                nbElem = 0;
                if((inc + 1) != __CT_alloc_memory_max && __CT_alloc_lst[inc + 1].ptr != NULL)
                {
                    ligne_prec = __CT_alloc_lst[inc + 1].ligne;
                    strcpy(fichier_prec, __CT_alloc_lst[inc + 1].fichier);
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

#define __CT_SPRINTF(PTR,FORMAT,...)  __CT_sprintf_verif_ecrasement(PTR, FORMAT, ##__VA_ARGS__)

/**
 * @brief Verifie que le contenu du sprintf ne depasse pas la taille du tableau
 */
void __CT_sprintf_verif_ecrasement(char* tab, char* format, ...)
{
    int retour = 0;
    va_list args;

    __CT_TYP_MEMORY_DEBUG* element =  __CT_avoir_alloc_elem(tab);
    
        
    if(element != NULL)
    {
        va_start(args, format);
        retour = vsnprintf(tab, element->taille, format, args);
        retour++;
        if(retour>element->taille)
        {
            printf("DEPASSEMENT/sprintf/%s:%d/size:%ld/sprintf size:%d\n", element->fichier, element->ligne, element->taille,retour);
        }
        va_end(args);        
    }
    va_start(args, format);
    vsprintf(tab, format,args);
    va_end(args);
}

#define __CT_STRCPY(PTR,SRC)  __CT_strcpy_verif_ecrasement(PTR, SRC)

/**
 * @brief Verifie que le contenu du sprintf ne depasse pas la taille du tableau
 */
void __CT_strcpy_verif_ecrasement(char* tab, const char* src)
{
    __CT_SIZE_TYPE_TAILLE__ size = 0;

    __CT_TYP_MEMORY_DEBUG* element =  __CT_avoir_alloc_elem(tab);
    size=strlen(src);
        
    if(element != NULL)
    {
        if(size>element->taille)
        {
            printf("DEPASSEMENT/strcpy/%s:%d/size:%ld/source size:%ld\n", element->fichier, element->ligne, element->taille, size);
        }       
    }
    strcpy(tab, src);
}

#define __CT_MEMCPY(PTR,SRC,N)  __CT_memcpy_verif_ecrasement(PTR, SRC, N)

/**
 * @brief Verifie que le contenu du sprintf ne depasse pas la taille du tableau
 */
void __CT_memcpy_verif_ecrasement(void* tab, void* src, size_t size)
{

    __CT_TYP_MEMORY_DEBUG* element =  __CT_avoir_alloc_elem(tab);
        
    if(element != NULL)
    {
        if(size>element->taille)
        {
            printf("DEPASSEMENT dest/memcpy/%s:%d/size ptr:%ld/size:%d\n", element->fichier, element->ligne, element->taille, size);
        }       
    }
    
    element =  __CT_avoir_alloc_elem(src);
        
    if(element != NULL)
    {
        if(size>element->taille)
        {
            printf("DEPASSEMENT src/memcpy/%s:%d/size ptr:%ld/size:%ld\n", element->fichier, element->ligne, element->taille, size);
        }       
    }
    memcpy(tab, src, size);
}

#define __CT_MEMSET(PTR,CAR,N)  __CT_memset_verif_ecrasement(PTR, CAR, N)

/**
 * @brief Verifie que le taille demandee ne depasse pas la taille allouee au pointeur
 */
void __CT_memset_verif_ecrasement(void* tab, char car, size_t size)
{

    __CT_TYP_MEMORY_DEBUG* element =  __CT_avoir_alloc_elem(tab);
        
    if(element != NULL)
    {
        if(size>element->taille)
        {
            printf("DEPASSEMENT dest/memset/%s:%d/size ptr:%ld/size:%d\n", element->fichier, element->ligne, element->taille, size);
        }       
    }
    memset(tab, car, size);
}

#else

#define __CT_CLEAR_MEMORY_REGISTER()
#define __CT_TRACE_ALLOC(MODE)
#define __CT_FREE(PTR) free(PTR)
#define __CT_MALLOC(TAILLE) malloc(TAILLE)
#define __CT_REALLOC(PTR, TAILLE)  realloc(PTR, TAILLE)
#define __CT_SPRINTF(PTR,FORMAT,...) sprintf(PTR,FORMAT, ##__VA_ARGS__)
#define __CT_STRCPY(PTR,SRC) strcpy(PTR,SRC)
#define __CT_MEMCPY(PTR,SRC,N) memcpy(PTR,SRC,N)
#define __CT_MEMSET(PTR,CAR,N) memset(PTR,CAR,N)
#define __CT_SET_LOG_CALLBACK(FUNCPTR)

#endif // __MEMORY_DEBUG__

#endif // __CTOOLS_H__
