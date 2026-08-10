#include "ft_ssl.h"

int    md5_hash(const unsigned char *data, size_t len, t_digest *digest)
{
    unsigned char   *padded;
    size_t          padded_len;
    size_t          nb_blocks;
    size_t          i;
    t_md5_state     state;

    padded = apply_padding(data, len, &padded_len, ENDIAN_LITTLE);
    if (!padded)
        return error_malloc();
    md5_init(&state);
    nb_blocks = padded_len / 64;
    i = 0;
    while (i < nb_blocks)
    {
        md5_process_block(&state, padded + (i * 64));
        i++;
    }
    md5_state_to_digest(&state, digest);
    digest->size = MD5_DIGEST_SIZE;
    free(padded);
    return 0;
}

