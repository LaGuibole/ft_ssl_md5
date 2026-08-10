#include "ft_ssl.h"

int     sha256_hash(const unsigned char *data, size_t len, t_digest *digest)
{
	unsigned char   *padded;
	size_t			padded_len;
	size_t			nb_blocks;
	size_t			i;
	t_sha256_state	state;

	padded = apply_padding(data, len, &padded_len, ENDIAN_BIG);
	if (!padded)
		return error_malloc();
	sha256_init(&state);
	nb_blocks = padded_len / 64;
	i = 0;
	while (i < nb_blocks)
	{
		sha256_process_block(&state, padded + (i * 64));
		i++;
	}
	sha256_state_to_digest(&state, digest);
	digest->size = SHA256_DIGEST_SIZE;
	free(padded);
	return 0;
}