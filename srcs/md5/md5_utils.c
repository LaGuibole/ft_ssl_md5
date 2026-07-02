#include "ft_ssl.h"

void md5_init(t_md5_state *state)
{
    state->a = 0x67452301;
    state->b = 0xefcdab89;
    state->c = 0x98badcfe;
    state->d = 0x10325476;    
}

uint32_t	left_rotate(uint32_t x, int n)
{
    return ((x << n) | (x >> (32 - n)));
}

static uint32_t read_word_le(const unsigned char *block, int index)
{
    return ((uint32_t)block[index * 4]
        | ((uint32_t)block[index * 4 + 1] << 8)
        | ((uint32_t)block[index * 4 + 2] << 16)
        | ((uint32_t)block[index * 4 + 3] << 24));
}

static uint32_t	f_func(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) | (~x & z));
}

static uint32_t	g_func(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & z) | (y & ~z));
}

static uint32_t	h_func(uint32_t x, uint32_t y, uint32_t z)
{
    return (x ^ y ^ z);
}

static uint32_t	i_func(uint32_t x, uint32_t y, uint32_t z)
{
    return (y ^ (x | ~z));
}

void md5_process_block(t_md5_state *state, const unsigned char *block)
{
    uint32_t    m[16];
    uint32_t    a, b, c, d;
    uint32_t    f;
    int         g;
    int         i;

    i = 0;
    while (i < 16)
    {
        m[i] = read_word_le(block, i);
        i++;
    }
    a = state->a;
    b = state->b;
    c = state->c;
    d = state->d;
    i = 0;
    while (i < 64)
    {
       if (i < 16)
       {
            f = f_func(b, c, d);
            g = i;
       }
       else if (i < 32)
       {
            f = g_func(b, c, d);
            g = (5 * i + 1) % 16;
       }
       else if (i < 48)
       {
            f = h_func(b, c, d);
            g = (3 * i + 5) % 16;
       }
       else
       {
            f = i_func(b, c, d);
            g = (7 * i) % 16;
       }

       f = f + a + g_md5_k[i] + m[g];

       a = d;
       d = c;
       c = b;
       b = b + left_rotate(f, g_md5_s[i]);
       
       i++;
    }
    state->a += a;
    state->b += b;
    state->c += c;
    state->d += d;
}