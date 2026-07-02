#include "ft_ssl.h"

int	cmd_md5(int argc, char **argv)
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