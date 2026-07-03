#include "ft_ssl.h"
#include <stdio.h>

const char	*type_str(t_input_type type)
{
	if (type == INPUT_STDIN)
		return ("STDIN");
	if (type == INPUT_STRING)
		return ("STRING");
	return ("FILE");
}

void	print_options(t_options *options)
{
	t_input	*current;

	printf("p=%d q=%d r=%d use_stdin=%d\n",
		options->p, options->q, options->r, options->use_stdin);
	current = options->inputs;
	while (current)
	{
		printf("  [%s] \"%s\"\n", type_str(current->type), current->value);
		current = current->next;
	}
}

void	test_inputs(t_options *options)
{
	t_input		*current;
	t_buffer	buffer;

	current = options->inputs;
	while (current)
	{
		printf("\n=== %s ===\n", type_str(current->type));

		if (read_input(current, &buffer))
		{
			printf("Read failed.\n");
			current = current->next;
			continue ;
		}

		printf("Length : %zu bytes\n", buffer.len);

		if (buffer.len > 0)
			printf("Content: \"%.*s\"\n",
				(int)buffer.len, (char *)buffer.data);

		free_buffer(&buffer);
		current = current->next;
	}

	if (options->use_stdin)
	{
		printf("\n=== STDIN ===\n");

		if (read_fd(STDIN_FILENO, &buffer))
		{
			printf("Read failed.\n");
			return ;
		}

		printf("Length : %zu bytes\n", buffer.len);

		if (buffer.len > 0)
			printf("Content: \"%.*s\"\n",
				(int)buffer.len, (char *)buffer.data);

		free_buffer(&buffer);
	}
}

void	print_digest_le(t_md5_state *state)
{
	uint32_t	words[4];
	int			i;
	int			j;

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
			printf("%02x", (words[i] >> (8 * j)) & 0xff);
			j++;
		}
		i++;
	}
	printf("\n");
}