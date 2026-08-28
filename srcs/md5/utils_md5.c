#include "ft_ssl.h"

/// @brief Initializes the MD5 internal state with the algorithm's
///        predefined initialization constants.
/// @param state Pointer to the MD5 state structure.
void md5_init(t_md5_state *state)
{
    state->a = 0x67452301;
    state->b = 0xefcdab89;
    state->c = 0x98badcfe;
    state->d = 0x10325476;    
}

/// @brief Performs a 32-bit left circular rotation.
/// @param x Value to rotate.
/// @param n Number of bits to rotate.
/// @return The rotated 32-bit value.
uint32_t	left_rotate(uint32_t x, int n)
{
    return ((x << n) | (x >> (32 - n)));
}

/// @brief Reads a 32-bit word from a 64-byte block using little-endian encoding.
/// @param block Pointer to the input block.
/// @param index Index of the 32-bit word to read (0 to 15).
/// @return The reconstructed 32-bit word.
static uint32_t read_word_le(const unsigned char *block, int index)
{
    return ((uint32_t)block[index * 4]
        | ((uint32_t)block[index * 4 + 1] << 8)
        | ((uint32_t)block[index * 4 + 2] << 16)
        | ((uint32_t)block[index * 4 + 3] << 24));
}

/// @brief Computes the MD5 F auxiliary function used in round 1.
/// @param x First operand.
/// @param y Second operand.
/// @param z Third operand.
/// @return Result of the F function.
static uint32_t	f_func(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) | (~x & z));
}

/// @brief Computes the MD5 G auxiliary function used in round 2.
/// @param x First operand.
/// @param y Second operand.
/// @param z Third operand.
/// @return Result of the G function.
static uint32_t	g_func(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & z) | (y & ~z));
}

/// @brief Computes the MD5 H auxiliary function used in round 3.
/// @param x First operand.
/// @param y Second operand.
/// @param z Third operand.
/// @return Result of the H function.
static uint32_t	h_func(uint32_t x, uint32_t y, uint32_t z)
{
    return (x ^ y ^ z);
}

/// @brief Computes the MD5 I auxiliary function used in round 4.
/// @param x First operand.
/// @param y Second operand.
/// @param z Third operand.
/// @return Result of the I function.
static uint32_t	i_func(uint32_t x, uint32_t y, uint32_t z)
{
    return (y ^ (x | ~z));
}

/// @brief Processes a single 512-bit block and updates the MD5 state.
/// @param state Pointer to the current MD5 state.
/// @param block Pointer to the 64-byte input block.
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