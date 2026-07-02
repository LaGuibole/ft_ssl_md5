#include "ft_ssl.h"

static void state_to_digest(t_md5_state *state, t_digest *digest)
{
    uint32_t    words[4]; // words = 32bits
    int         i;
    int         j;

    words[0] = state->a;
    words[1] = state->b;
    words[2] = state->c;
    words[3] = state->d;

    i = 0;
    while (i < 4)
    {
        j = 0;
        while (j < 4)
        {
            digest->bytes[i * 4 + j] = (words[i] >> (8 * j)) & 0xFF;
            j++;
        }
        i++;
    }
}

int    md5_hash(const unsigned char *data, size_t len, t_digest *digest)
{
    unsigned char   *padded;
    size_t          padded_len;
    size_t          nb_blocks;
    size_t          i;
    t_md5_state     state;

    padded = md5_pad(data, len, &padded_len);
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
    state_to_digest(&state, digest);
    free(padded);
    return 0;
}

void    print_digest_hex(t_digest *digest)
{
    static const char   hex[] = "0123456789abcdef";
    int                 i;

    i = 0;
    // while (i < MD5_DIGEST_SIZE)
    // {
    //     fd_printf(1, "%c", &hex[digest->bytes[i] >> 4]);
    //     fd_printf(1, "%c", &hex[digest->bytes[i] & 0xf]);
    //     i++;
    // }
    while (i < MD5_DIGEST_SIZE)
    {
        write(1, &hex[digest->bytes[i] >> 4], 1);
		write(1, &hex[digest->bytes[i] & 0xf], 1);
		i++;
    }
}