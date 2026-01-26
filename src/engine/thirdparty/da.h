/*
------------------------------------------------------------------------------
DYNAMIC ARRAY LIB FOR C - ASTOLFI Vincent 2025

<!> WARNING : this lib is greatly inspired by the work of tsoding (github.com/tsoding) during his different project
<!> WARNING : My goal on this lib is just to train myself and to understand the different concept behind those da

Usage:

// define your dynamic array like so :

typedef struct {
    Type* items;
    size_t count; 
    size_t capacity;
} my_dynamic_array;

// This mean that this lib can work with any type of stored items
// Your struct just has to follow the three same variables

// then include the lib
#define DA_LIB_IMPLEMENTATION
#include <da.h>

API:

    da_append(*da, Item);
    da_free(*da);
    da_foreach(Type, Iterator, *da);
    da_append_amount(*da, Item, amount) -> append the same item 'amount' times
    da_remove(*da, Place);

ERRORS: 
    
    Those functions does not returns anything
    If an error occurs, it will at memory management so the app will close
    An error message will be printed for debugging

EXAMPLE: 

    ```c
    #define DA_LIB_IMPLEMENTATION
    #include "../da.h"

    #include <stdio.h>

    typedef struct
    {
        int* items;
        size_t count;
        size_t capacity;
    } Numbers;

    int main(void) 
    {
        Numbers numbers = {0};    

        da_append(&numbers, 1);
        da_append(&numbers, 2);
        da_append(&numbers, 3);

        da_foreach(int, it, &numbers) {
            size_t index = it - numbers.items;
            printf("%zu: %d\n", index, *it); 
        }

        da_free(&numbers);
    }    
    ```

    see (https://github.com/viastolfi/clibs/tree/main/dynamic_array/example) for more example

------------------------------------------------------------------------------
*/
#ifndef DA_H
#define DA_H

#ifndef DA_REALLOC
#include    <stdlib.h>
#define     DA_REALLOC realloc
#endif //DA_REALLOC

#ifndef DA_FREE
#include    <stdlib.h>
#define     DA_FREE free
#endif // DA_FREE

#ifndef DA_ASSERT
#include    <assert.h>
#define     DA_ASSERT assert
#endif // DA_ASSSERT

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#ifndef DA_LIB
#define     DA_LIB static inline
#endif // DA_LIB

#ifdef DA_LIB_IMPLEMENTATION

#ifndef DA_INIT_CAP 
#define     DA_INIT_CAP 256
#endif

#define da_append(da, item)                                                                 \
    do {                                                                                    \
        if((da)->count + 1 > (da)->capacity) {                                              \
            if((da)->capacity == 0) {                                                       \
                (da)->capacity = DA_INIT_CAP;                                               \
            } else {                                                                        \
                (da)->capacity *= 2;                                                        \
            }                                                                               \
            (da)->items = DA_REALLOC((da)->items, (da)->capacity * sizeof(*(da)->items));   \
            DA_ASSERT((da)->items != NULL && "realloc failed");                             \
        }                                                                                   \
        (da)->items[(da)->count++] = item;                                                  \
    } while(0)


#define da_append_amount(da, item, amount)                \
    do {                                                  \
        for(size_t i = 0; i < (size_t) amount; ++i) {     \
            da_append(da, item);                          \
        }                                                 \
    }while(0)


#define da_free(da)                \
    do {                           \
        DA_FREE((da)->items);      \
        (da)->items = NULL;        \
        (da)->count = 0;           \
        (da)->capacity = 0;        \
} while(0)

#define da_foreach(Type, it, da) for(Type* it = (da)->items; it < (da)->items + (da)->count; ++it)

#define da_remove(da, i)                                                \
    do {                                                                \
        size_t j = (i);                                                 \
        DA_ASSERT(j < (da)->count && "remove : index out of bound");    \
        (da)->items[j] = (da)->items[--(da)->count];                    \
    } while(0)

#endif // DA_LIB_IMPLEMENTATION

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // DA_H
