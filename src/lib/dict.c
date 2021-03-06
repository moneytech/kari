#include "kari.h"
#include "kari_stdlib.h"
#include <gc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

/* dset */
struct dset_state {
    kari_dict_val_t* dict;
    char* key;
};

K_FN(_dset_3)
{
    struct dset_state* st = (struct dset_state*)state;
    kari_dict_set(st->dict->items, st->key, argument);
    return argument;
}

K_FN(_dset_2)
{
    struct dset_state* st = (struct dset_state*)state;
    KASSERT(K_TYPE_OF(argument) == KARI_STRING, "Expected string");
    st->key = ((kari_string_t*)argument)->str;
    return (kari_value_t*)kari_create_native_function(context, K_REF(_dset_3), st);
}

K_FN(dset)
{
    struct dset_state* st = (struct dset_state*)GC_MALLOC(sizeof(struct dset_state));
    KASSERT(K_TYPE_OF(argument) == KARI_DICT, "Expected dictionary");
    st->dict = (kari_dict_val_t*)argument;
    return (kari_value_t*)kari_create_native_function(context, K_REF(_dset_2), st);
}

K_FN(_dget_2)
{
    kari_dict_t* d = (kari_dict_t*)state;
    KASSERT(K_TYPE_OF(argument) == KARI_STRING, "Expected string");
    
    if(kari_dict_exists(d, ((kari_string_t*)argument)->str)) {
        return (kari_value_t*)kari_dict_find_value(d, ((kari_string_t*)argument)->str);
    } else {
        return kari_nil();
    }
}

K_FN(dget)
{
    KASSERT(K_TYPE_OF(argument) == KARI_DICT, "Expected dictionary");
    return (kari_value_t*)kari_create_native_function(context, K_REF(_dget_2), ((kari_dict_val_t*)argument)->items);
}

K_FN(keys)
{
    size_t i;
    kari_dict_t* d;
    kari_vec_t* v = new_kari_vec();
    kari_array_t* a = kari_create_array();
    KASSERT(K_TYPE_OF(argument) == KARI_DICT, "Expected dictionary");
    d = ((kari_dict_val_t*)argument)->items;
    for(i = 0; i < d->keys->count; i++) {
        kari_vec_push(v, kari_create_string((char*)d->keys->entries[i]));
    }
    a->items = v;
    return (kari_value_t*)a;
}