#include "ft_ssl.h"

/// @brief 
/// @param data buffer->data 
/// @param len buffer->len
/// @param padded_len 
/// @return 
unsigned char *apply_padding(const unsigned char *data, size_t len, size_t *padded_len, t_endian endian_length)
{
    size_t          padding_zeros;
    unsigned char   *buffer;
    uint64_t        bit_len;
    size_t          i = 0;

    padding_zeros = (56 - (len + 1) % 64 + 64) % 64;
    *padded_len = len + 1 + padding_zeros + 8;

    buffer = malloc(*padded_len);
    if (!buffer)
        return NULL;
    
    ft_memset(buffer, 0, *padded_len);
    ft_memcpy(buffer, data, len);

    buffer[len] = 0x80;
    bit_len = (uint64_t)len * 8;

    while (i < 8)
    {
        if (endian_length == ENDIAN_LITTLE)
            buffer[*padded_len - 8 + i] = (unsigned char)(bit_len >> (8 * i));
        else
            buffer[*padded_len - 1 - i] = (unsigned char)(bit_len >> (8 * i));
        i++;
    }

    return buffer;
}

// void state_to_digest(t_md5_state *state, t_digest *digest)
// {
//     uint32_t    words[4]; // words = 32bits
//     int         i;
//     int         j;

//     words[0] = state->a;
//     words[1] = state->b;
//     words[2] = state->c;
//     words[3] = state->d;

//     i = 0;
//     while (i < 4)
//     {
//         j = 0;
//         while (j < 4)
//         {
//             digest->bytes[i * 4 + j] = (words[i] >> (8 * j)) & 0xFF;
//             j++;
//         }
//         i++;
//     }
// }

void    words_to_digest(const uint32_t *words, int nb_words, t_endian byte_order, t_digest *digest)
{
	int	i = 0;
	int	j;

	while (i < nb_words)
	{
		j = 0;
		while (j < 4)
		{
			if (byte_order == ENDIAN_LITTLE)
				digest->bytes[i * 4 + j] = (words[i] >> (8 * j)) & 0xff;
			else
				digest->bytes[i * 4 + j] = (words[i] >> (24 - 8 * j)) & 0xff;
			j++;
		}
		i++;
	}
}

void md5_state_to_digest(t_md5_state *state, t_digest *digest)
{
	uint32_t	words[4];

	words[0] = state->a;
	words[1] = state->b;
	words[2] = state->c;
	words[3] = state->d;
	words_to_digest(words, 4, ENDIAN_LITTLE, digest);
}

void sha256_state_to_digest(t_sha256_state *state, t_digest *digest)
{
	words_to_digest(state->h, 8, ENDIAN_BIG, digest);
}


void    print_digest_hex(t_digest *digest)
{
    static const char   hex[] = "0123456789abcdef";
    size_t              i;

    i = 0;
    // while (i < MD5_DIGEST_SIZE)
    // {
    //     fd_printf(1, "%c", &hex[digest->bytes[i] >> 4]);
    //     fd_printf(1, "%c", &hex[digest->bytes[i] & 0xf]);
    //     i++;
    // }
    while (i < digest->size)
    {
        write(1, &hex[digest->bytes[i] >> 4], 1);
		write(1, &hex[digest->bytes[i] & 0xf], 1);
		i++;
    }
}

void print_result(t_input *input, t_digest *digest, t_options *options, const char *label)
{
    if (options->q)
    {
        print_digest_hex(digest);
        fd_printf(1, "\n");
        return ;
    }
    if (options->r)
    {
        print_digest_hex(digest);
        if (input->type == INPUT_FILE)
            fd_printf(1, " %s", input->value);
        else if (input->type == INPUT_STRING)
            fd_printf(1, " \"%s\"", input->value);
        fd_printf(1, "\n");
        return ;
    }
    if (input->type == INPUT_STDIN)
    {
		fd_printf(1, "(stdin)= ");
        print_digest_hex(digest);
        fd_printf(1, "\n");
        return ;
    }
    fd_printf(1, "%s (", label);
    if (input->type == INPUT_FILE)
        fd_printf(1, "%s", input->value);
    else
        fd_printf(1, "\"%s\"", input->value);
    fd_printf(1, ") = ");
    print_digest_hex(digest);
    fd_printf(1, "\n");
}