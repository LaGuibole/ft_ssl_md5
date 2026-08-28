#include "ft_ssl.h"

int	main(int argc, char **argv)
{
	return dispatch_commands(argc, argv);
	
	// (void)argc;
	// (void)argv;
	// unsigned char	*padded;
	// size_t			padded_len;
	// size_t			nb_blocks;
	// t_md5_state		state;
	// size_t			i;

	// padded = md5_pad((const unsigned char *)"abc", 3, &padded_len);
	// if (!padded)
	// 	return (1);
	// md5_init(&state);
	// nb_blocks = padded_len / 64;
	// i = 0;
	// while (i < nb_blocks)
	// {
	// 	md5_process_block(&state, padded + (i * 64));
	// 	i++;
	// }
	// printf("got:      ");
	// print_digest_le(&state);
	// printf("expected: 900150983cd24fb0d6963f7d28e17f72\n");
	// free(padded);
	// return (0);
}