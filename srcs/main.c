// static const char	*type_str(t_input_type type)
// {
// 	if (type == INPUT_STDIN)
// 		return ("STDIN");
// 	if (type == INPUT_STRING)
// 		return ("STRING");
// 	return ("FILE");
// }

// void	print_options(t_options *options)
// {
// 	t_input	*current;

// 	printf("p=%d q=%d r=%d use_stdin=%d\n",
// 		options->p, options->q, options->r, options->use_stdin);
// 	current = options->inputs;
// 	while (current)
// 	{
// 		printf("  [%s] \"%s\"\n", type_str(current->type), current->value);
// 		current = current->next;
// 	}
// }

// int	main(int argc, char **argv)
// {
// 	t_options	options;

// 	options.p = 0;
// 	options.q = 0;
// 	options.r = 0;
// 	options.inputs = NULL;
// 	options.use_stdin = 0;
// 	if (parse_options(&options, argc, argv))
// 	{
// 		free_inputs(options.inputs);
// 		return (1);
// 	}
// 	print_options(&options);
// 	free_inputs(options.inputs);
// 	return (0);
// }

#include "ft_ssl.h"
#include <stdio.h>

static const char	*type_str(t_input_type type)
{
	if (type == INPUT_STDIN)
		return ("STDIN");
	if (type == INPUT_STRING)
		return ("STRING");
	return ("FILE");
}

static void	print_options(t_options *options)
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

static void	test_inputs(t_options *options)
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

int	main(int argc, char **argv)
{
	t_options	options;

	options.p = 0;
	options.q = 0;
	options.r = 0;
	options.inputs = NULL;
	options.use_stdin = 0;

	if (parse_options(&options, argc, argv))
	{
		free_inputs(options.inputs);
		return (1);
	}

	print_options(&options);
	test_inputs(&options);

	free_inputs(options.inputs);
	return (0);
}