#ifndef FT_SSL_H
# define FT_SSL_H

#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

# define CHUNK_SIZE	4096

typedef enum e_input_type 
{
	INPUT_STDIN,
	INPUT_STRING,
	INPUT_FILE
} t_input_type;

typedef struct s_input
{
	t_input_type    type;
	char      *value; // file name or the string given with -s flag
	struct s_input  *next; // pointer to next input
} t_input;

typedef struct s_options
{
	int         p; // echo std + checksum append
	int         q; // quit mode
	int         r; // reverse format
	t_input     *inputs; // linked list of object to be hashed
	int         use_stdin; // true if no input is found
} t_options;

typedef struct s_command 
{
	const char  *command_name;
	int         (*run)(int argc, char** argv);
} t_command;

typedef struct s_buffer
{
	unsigned char	*data;
	size_t			len;
	size_t			capacity;
} t_buffer;

int			parse_options(t_options *options, int argc, char **argv);
t_input		*new_input(t_input_type type, const char *value);
void		add_input(t_options *options, t_input *new);
void		free_inputs(t_input *inputs);
int			error_invalid_option(char options);
int			error_option_argument(char options);
int			error_malloc(void);
int 		error_open(const char *name);
int 		error_read(const char *name);
// void		print_options(t_options *options);
int			read_input(t_input *input, t_buffer *buffer);
int			read_fd(int fd, t_buffer *buffer);
void		free_buffer(t_buffer *buffer);

#endif