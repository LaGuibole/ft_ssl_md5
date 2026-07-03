#include "ft_ssl.h"

void sha256_init(t_sha256_state *state)
{
    state->h[0] = 0x6a09e667;
    state->h[1] = 0xbb67ae85;
    state->h[2] = 0x3c6ef372;
    state->h[3] = 0xa54ff53a;
    state->h[4] = 0x510e527f;
    state->h[5] = 0x9b05688c;
    state->h[6] = 0x1f83d9ab;
    state->h[7] = 0x5be0cd19;
}

uint32_t right_rotate(u_int32_t x, int n)
{
    return ((x >> n) | (x << (32 - n)));
}

static u_int32_t read_word_be(const unsigned char *block, int index)
{
    return (((u_int32_t)block[index * 4] << 24)
        | ((u_int32_t)block[index * 4 + 1] << 16)
        | ((u_int32_t)block[index * 4 + 2] << 8)
        | ((u_int32_t)block[index * 4 + 3]));
}

static uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) ^ (~x & z));
}

static uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x ^ y) ^ (x & z) ^ (y & z));
}

static uint32_t big_sigma0(uint32_t x)
{
    return (right_rotate(x, 2)
        ^ right_rotate(x, 13)
        ^ right_rotate(x, 22));
}

static uint32_t big_sigma1(uint32_t x)
{
    return (right_rotate(x, 6)
        ^ right_rotate(x, 11)
        ^ right_rotate(x, 25));
}

static uint32_t small_sigma0(uint32_t x)
{
    return (right_rotate(x, 7)
        ^ right_rotate(x, 18)
        ^ (x >> 3));
}

static uint32_t small_sigma1(uint32_t x)
{
    return (right_rotate(x, 17)
        ^ right_rotate(x, 19)
        ^ (x >> 10));
}

void sha256_process_block(t_sha256_state *state, const unsigned char *block)
{
    uint32_t    w[64];
    uint32_t    a, b, c, d, e, f, g, h, t1, t2;
    int         i;
    
    i = 0;
    while (i < 16)
    {
        w[i] = read_word_be(block, i);
        i++;
    }
    while (i < 64)
    {
        w[i] = small_sigma1(w[i - 2]) 
            + w[i - 7] 
            + small_sigma0(w[i - 15]) 
            + w[i - 16];
        i++;
    }
    a = state->h[0];
    b = state->h[1];
    c = state->h[2];
    d = state->h[3];
    e = state->h[4];
    f = state->h[5];
    g = state->h[6];
    h = state->h[7];
    i = 0;
    while (i < 64)
    {
        t1 = h
            + big_sigma1(e)
            + ch(e, f, g)
            + g_sha256_k[i]
            + w[i];
        t2 = big_sigma0(a)
            + maj(a, b, c);
         h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;

        i++;
    }
    state->h[0] += a;
    state->h[1] += b;
    state->h[2] += c;
    state->h[3] += d;
    state->h[4] += e;
    state->h[5] += f;
    state->h[6] += g;
    state->h[7] += h;
}
