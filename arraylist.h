#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

/*
 * Safety: For every macro method other than `GENERATE_ARRAYLIST` 
 * and `ARRAYLIST_CREATE`, you must pass a non-null ArrayList.
 */

/*
 * Initial capacity allocated when creating a new ArrayList.
 */
#define INITIAL_CAPACITY 10

/*
 * Generate `struct arraylist_<name>_t`.
 */
#define GENERATE_ARRAYLIST_STRUCT(name, type) \
    typedef struct arraylist_##name_t {       \
        type   *data;                         \
        size_t  count;                        \
        size_t  capacity;                     \
    } ArrayList_##name;

/*
 * Generates `enum arraylist_error_<name>_t`.
 */
#define GENERATE_ARRAYLIST_ERROR_ENUM(name) \
    typedef enum arraylist_error_##name_t { \
        SUCCESS_##name = 0,                 \
        EMPTY_ARRAYLIST_ERROR_##name,       \
        INDEX_OUT_OF_BOUNDS_ERROR_##name,   \
        MEMORY_ERROR_##name,                \
    } ArrayListError_##name;

/*
 * Generates `ArrayList_<name> *arraylist_create_<name>()`.
 */
#define GENERATE_ARRAYLIST_CREATE(name, type)                           \
    static inline ArrayList_##name *arraylist_create_##name() {         \
        ArrayList_##name *arraylist = malloc(sizeof(ArrayList_##name)); \
        if (arraylist == NULL) {                                        \
            return NULL;                                                \
        }                                                               \
        arraylist->data = malloc(sizeof(type) * INITIAL_CAPACITY);      \
        if (arraylist->data == NULL) {                                  \
            free(arraylist);                                            \
            return NULL;                                                \
        }                                                               \
        arraylist->count    = 0;                                        \
        arraylist->capacity = INITIAL_CAPACITY;                         \
        return arraylist;                                               \
    }

/*
 * Generates `ArrayListError_<name> arraylist_destroy_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_DESTROY(name)                                       \
    static inline void arraylist_destroy_##name(ArrayList_##name *arraylist) { \
        free(arraylist->data);                                                 \
        free(arraylist);                                                       \
    }

/*
 * Generates `size_t arraylist_count_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_COUNT(name)                                               \
    static inline size_t arraylist_count_##name(const ArrayList_##name *arraylist) { \
        return arraylist->count;                                                     \
    }

/*
 * Generates `bool arraylist_is_empty_<name>(ArrayList_<name> *arraylist)`.
 */
#define GENERATE_ARRAYLIST_IS_EMPTY(name)                                             \
    static inline bool arraylist_is_empty_##name(const ArrayList_##name *arraylist) { \
        return arraylist->count == 0;                                                 \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_<name>(ArrayList_<name> *arraylist, size_t index, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET(name, type)                            \
    static inline ArrayListError_##name arraylist_get_##name(         \
        const ArrayList_##name *arraylist, size_t index, type *out) { \
        if (arraylist->count == 0) {                                  \
            return EMPTY_ARRAYLIST_ERROR_##name;                      \
        }                                                             \
        if (index >= arraylist->count) {                              \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                  \
        }                                                             \
        if (out != NULL) {                                            \
            *out = arraylist->data[index];                            \
        }                                                             \
        return SUCCESS_##name;                                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_set_<name>(ArrayList_<name> *arraylist, size_t index, type new_element, type *out)`.
 */
#define GENERATE_ARRAYLIST_SET(name, type)                                        \
    static inline ArrayListError_##name arraylist_set_##name(                     \
        ArrayList_##name *arraylist, size_t index, type new_element, type *out) { \
        if (arraylist->count == 0) {                                              \
            return EMPTY_ARRAYLIST_ERROR_##name;                                  \
        }                                                                         \
        if (index >= arraylist->count) {                                          \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                              \
        }                                                                         \
        if (out != NULL) {                                                        \
            *out = arraylist->data[index];                                        \
        }                                                                         \
        arraylist->data[index] = new_element;                                     \
        return SUCCESS_##name;                                                    \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_first_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_FIRST(name, type)                    \
    static inline ArrayListError_##name arraylist_get_first_##name( \
        const ArrayList_##name *arraylist, type *out) {             \
        if (arraylist->count == 0) {                                \
            return EMPTY_ARRAYLIST_ERROR_##name;                    \
        }                                                           \
        if (out != NULL) {                                          \
            *out = arraylist->data[0];                              \
        }                                                           \
        return SUCCESS_##name;                                      \
    }

/*
 * Generates `ArrayListError_<name> arraylist_get_last_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_GET_LAST(name, type)                    \
    static inline ArrayListError_##name arraylist_get_last_##name( \
        const ArrayList_##name *arraylist, type *out) {            \
        if (arraylist->count == 0) {                               \
            return EMPTY_ARRAYLIST_ERROR_##name;                   \
        }                                                          \
        if (out != NULL) {                                         \
            *out = arraylist->data[arraylist->count - 1];          \
        }                                                          \
        return SUCCESS_##name;                                     \
    }

/*
 * Generates `ArrayListError_<name> arraylist_grow_<name>(ArrayList_<name> *arraylist, size_t new_capacity)`.
 */
#define GENERATE_ARRAYLIST_GROW(name, type)                               \
    static inline ArrayListError_##name arraylist_grow_##name(            \
        ArrayList_##name *arraylist, size_t new_capacity) {               \
        assert(new_capacity > arraylist->capacity);                       \
        size_t bytes;                                                     \
        if (__builtin_mul_overflow(new_capacity, sizeof(type), &bytes)) { \
            return MEMORY_ERROR_##name;                                   \
        }                                                                 \
        type *new_array = realloc(arraylist->data, bytes);                \
        if (new_array == NULL) {                                          \
            return MEMORY_ERROR_##name;                                   \
        }                                                                 \
        arraylist->data     = new_array;                                  \
        arraylist->capacity = new_capacity;                               \
        return SUCCESS_##name;                                            \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_<name>(ArrayList_<name> *arraylist, size_t index, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD(name, type)                                                                            \
    static inline ArrayListError_##name arraylist_add_##name(                                                         \
        ArrayList_##name *arraylist, size_t index, type element) {                                                    \
        if (index > arraylist->count) {                                                                               \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                                                                  \
        }                                                                                                             \
        if (arraylist->count == arraylist->capacity) {                                                                \
            size_t cap = arraylist->capacity;                                                                         \
            size_t half_of_cap = cap >> 1;                                                                            \
            size_t new_capacity;                                                                                      \
            /* There's a case where `new_capacity == arraylist->capacity` when `arraylist->capacity < 2`, */          \
            /* as `half_of_cap` will be 0, and therefore `new_capacity = cap + half_of_cap = arraylist->capacity`. */ \
            /* However, the initial capacity is always 10, so this case will never happen. */                         \
            if (__builtin_add_overflow(arraylist->capacity, half_of_cap, &new_capacity)) {                            \
                if (cap == SIZE_MAX) {                                                                                \
                    return MEMORY_ERROR_##name;                                                                       \
                }                                                                                                     \
                /* guarantee progress */                                                                              \
                new_capacity = cap + 1;                                                                               \
            }                                                                                                         \
            ArrayListError_##name res = arraylist_grow_##name(                                                        \
                arraylist, new_capacity);                                                                             \
            if (res != SUCCESS_##name) {                                                                              \
                return res;                                                                                           \
            }                                                                                                         \
        }                                                                                                             \
        memmove(&arraylist->data[index + 1], &arraylist->data[index],                                                 \
                (arraylist->count - index) * sizeof(type));                                                           \
        arraylist->data[index] = element;                                                                             \
        arraylist->count += 1;                                                                                        \
        return SUCCESS_##name;                                                                                        \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_first_<name>(ArrayList_<name> *arraylist, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_FIRST(name, type)                    \
    static inline ArrayListError_##name arraylist_add_first_##name( \
        ArrayList_##name *arraylist, type element) {                \
        return arraylist_add_##name(arraylist, 0, element);         \
    }

/*
 * Generates `ArrayListError_<name> arraylist_add_last_<name>(ArrayList_<name> *arraylist, type element)`.
 */
#define GENERATE_ARRAYLIST_ADD_LAST(name, type)                            \
    static inline ArrayListError_##name arraylist_add_last_##name(         \
        ArrayList_##name *arraylist, type element) {                       \
        return arraylist_add_##name(arraylist, arraylist->count, element); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_first_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_FIRST(name, type)                    \
    static inline ArrayListError_##name arraylist_remove_first_##name( \
        ArrayList_##name *arraylist, type *out) {                      \
        return arraylist_remove_##name(arraylist, 0, out);             \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_last_<name>(ArrayList_<name> *arraylist, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE_LAST(name, type)                            \
    static inline ArrayListError_##name arraylist_remove_last_##name(         \
        ArrayList_##name *arraylist, type *out) {                             \
        return arraylist_remove_##name(arraylist, arraylist->count - 1, out); \
    }

/*
 * Generates `ArrayListError_<name> arraylist_remove_<name>(ArrayList_<name> *arraylist, size_t index, type *out)`.
 */
#define GENERATE_ARRAYLIST_REMOVE(name, type)                         \
    static inline ArrayListError_##name arraylist_remove_##name(      \
        ArrayList_##name *arraylist, size_t index, type *out) {       \
        if (arraylist->count == 0) {                                  \
            return EMPTY_ARRAYLIST_ERROR_##name;                      \
        }                                                             \
        if (index >= arraylist->count) {                              \
            return INDEX_OUT_OF_BOUNDS_ERROR_##name;                  \
        }                                                             \
        if (out != NULL) {                                            \
            *out = arraylist->data[index];                            \
        }                                                             \
        memmove(&arraylist->data[index], &arraylist->data[index + 1], \
                (arraylist->count - index - 1) * sizeof(type));       \
        arraylist->count -= 1;                                        \
        return SUCCESS_##name;                                        \
    }

/*
 * Generates the full implementation of an ArrayList suffixed by `name` for a given `type`.
 */
#define GENERATE_ARRAYLIST(name, type)          \
    GENERATE_ARRAYLIST_STRUCT(name, type)       \
    GENERATE_ARRAYLIST_ERROR_ENUM(name)         \
    GENERATE_ARRAYLIST_CREATE(name, type)       \
    GENERATE_ARRAYLIST_DESTROY(name)            \
    GENERATE_ARRAYLIST_COUNT(name)              \
    GENERATE_ARRAYLIST_IS_EMPTY(name)           \
    GENERATE_ARRAYLIST_GET(name, type)          \
    GENERATE_ARRAYLIST_GET_FIRST(name, type)    \
    GENERATE_ARRAYLIST_GET_LAST(name, type)     \
    GENERATE_ARRAYLIST_SET(name, type)          \
    GENERATE_ARRAYLIST_GROW(name, type)         \
    GENERATE_ARRAYLIST_ADD(name, type)          \
    GENERATE_ARRAYLIST_ADD_FIRST(name, type)    \
    GENERATE_ARRAYLIST_ADD_LAST(name, type)     \
    GENERATE_ARRAYLIST_REMOVE(name, type)       \
    GENERATE_ARRAYLIST_REMOVE_FIRST(name, type) \
    GENERATE_ARRAYLIST_REMOVE_LAST(name, type)

/*
 * User-facing macros.
 */
#define ARRAYLIST_CREATE(name) \
    arraylist_create_##name()

#define ARRAYLIST_DESTROY(name, arraylist) \
    arraylist_destroy_##name(arraylist)

#define ARRAYLIST_COUNT(name, arraylist) \
    arraylist_count_##name(arraylist)

#define ARRAYLIST_IS_EMPTY(name, arraylist) \
    arraylist_is_empty_##name(arraylist)

#define ARRAYLIST_GET(name, arraylist, index, out) \
    arraylist_get_##name(arraylist, index, out)

#define ARRAYLIST_GET_FIRST(name, arraylist, out) \
    arraylist_get_first_##name(arraylist, out)

#define ARRAYLIST_GET_LAST(name, arraylist, out) \
    arraylist_get_last_##name(arraylist, out)

#define ARRAYLIST_SET(name, arraylist, index, new_element, out) \
    arraylist_set_##name(arraylist, index, new_element, out)

#define ARRAYLIST_ADD(name, arraylist, index, element) \
    arraylist_add_##name(arraylist, index, element)

#define ARRAYLIST_ADD_FIRST(name, arraylist, element) \
    arraylist_add_first_##name(arraylist, element)

#define ARRAYLIST_ADD_LAST(name, arraylist, element) \
    arraylist_add_last_##name(arraylist, element)

#define ARRAYLIST_REMOVE(name, arraylist, index, out) \
    arraylist_remove_##name(arraylist, index, out)

#define ARRAYLIST_REMOVE_FIRST(name, arraylist, out) \
    arraylist_remove_first_##name(arraylist, out)

#define ARRAYLIST_REMOVE_LAST(name, arraylist, out) \
    arraylist_remove_last_##name(arraylist, out)

#endif // ARRAYLIST_H
