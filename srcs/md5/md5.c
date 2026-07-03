#include "ft_ssl.h"

int	cmd_md5(int argc, char **argv)
{
	t_options	options;
	t_input		*current;
	t_buffer	buffer;
	t_digest	digest;

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
	if (options.p)
		handle_p_flag(&options);
	current = options.inputs;
	while (current)
	{
		if (read_input(current, &buffer))
		{
			current = current->next;
			continue ;
		}
		if (md5_hash(buffer.data, buffer.len, &digest))
		{
			free_buffer(&buffer);
			free_inputs(options.inputs);
			return (1);
		}
		print_result(current, &digest, &options, "MD5");
		free_buffer(&buffer);
		current = current->next;
	}
	free_inputs(options.inputs);
	return (0);
}