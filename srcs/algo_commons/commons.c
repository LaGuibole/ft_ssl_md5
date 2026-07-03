#include "ft_ssl.h"

/// @brief 
/// @param data buffer->data 
/// @param len buffer->len
/// @param padded_len 
/// @return 
unsigned char *apply_padding(const unsigned char *data, size_t len, size_t *padded_len, char *algo)
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

    if (ft_strcmp(algo, "md5") == 0)
    {
        while (i < 8)
        {
            buffer[*padded_len - 8 + i] = (unsigned char)(bit_len >> (8 * i));
            i++;
        }
    }
    else if (ft_strcmp(algo, "sha256") == 0)
    {
        while (i < 8)
        {
            buffer[*padded_len - 1 + i] = (unsigned char)(bit_len >> (8 * i));
            i++;
        }
    }
    return buffer;
}