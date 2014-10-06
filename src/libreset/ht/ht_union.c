#include "ht/ht.h"

int
ht_union(
    struct ht* dest,
    struct ht const* src,
    struct r_set_cfg const* cfg
) {
    size_t dest_b = ht_nbuckets(dest);
    size_t bucket_ration = ht_nbuckets(src) / dest_b;

    // iterate over all the destination buckets
    while (dest_b--) {
        struct avl* avl_dest = &dest->buckets[dest_b].avl;

        // calculate the range of elements going into dest
        r_hash min_hash = dest_b << (BITCOUNT(min_hash) - dest->sizeexp);
        r_hash max_hash;
        max_hash = min_hash +
                   ((~(r_hash) 0) >> (BITCOUNT(min_hash) - dest->sizeexp));

        // iterate over all the source buckets for a destination
        size_t b = bucket_ration;
        size_t off = dest_b * bucket_ration;
        while (b--) {

            // perform the union merge for the bucket
            struct avl* avl_src = &src->buckets[off + b].avl;
            int retval = avl_union(avl_dest, avl_src, min_hash, max_hash, cfg);
            if (retval < 0) {
                return retval;
            }
        }
    }

    return 0;
}
