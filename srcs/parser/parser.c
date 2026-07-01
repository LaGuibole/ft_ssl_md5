#include "ft_ssl.h"

/// @brief Parses a single command-line option.
/// Processes all flags contained in the current argument. When the
/// '-s' option is encountered, the following argument is stored as a
/// string input.
/// @param options Structure storing the parsed options.
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line arguments.
/// @param i Pointer to the current argument index. May be incremented
/// when consuming the argument following '-s'.
/// @return 0 on success, 1 if an error occurs.
static int handle_flag(t_options *options, int argc, char **argv, int *i)
{
	int		j;
	t_input	*node;

	j = 1;
	while (argv[*i][j])
	{
		switch (argv[*i][j])
		{
		case 'p':
			options->p = 1;
			break;
		case 'q':
			options->q = 1;
			break;
		case 'r':
			options->r = 1;
			break;
		case 's':
			if (*i + 1 >= argc)
				return error_option_argument('s');
			node = new_input(INPUT_STRING, argv[*i + 1]);
			if (!node)
				return error_malloc();
			add_input(options, node);
			*i += 1;
			return 0;
		default:
			return error_invalid_option(argv[*i][j]);
		}
		j++;
	}
	return 0;
}

/// @brief Parses the command-line arguments.
/// Extracts options and input sources from the command line. Once the
/// first non-option argument is encountered, all remaining arguments
/// are treated as file inputs. If no input is provided, stdin is used.
/// @param options Structure to populate with the parsed options.
/// @param argc Number of command-line arguments.
/// @param argv Array of command-line arguments.
/// @return 0 on success, 1 if an error occurs.
int parse_options(t_options *options, int argc, char **argv)
{
    int		parsing_options = 1;
	int		i = 0;
	t_input	*node;

	while (++i < argc)
	{
		if (parsing_options && argv[i][0] == '-' && argv[i][1] != '\0')
		{
			if (handle_flag(options, argc, argv, &i))
				return 1;
		}
		else
		{
			parsing_options = 0;
			node = new_input(INPUT_FILE, argv[i]);
			if (!node)
				return error_malloc();
			add_input(options, node);
		}
	}
	if (!options->inputs)
		options->use_stdin = 1;
	return 0;
}