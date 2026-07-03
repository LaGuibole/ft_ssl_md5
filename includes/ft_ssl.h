#ifndef FT_SSL_H
# define FT_SSL_H

#include "libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

# define CHUNK_SIZE	4096
# define MD5_DIGEST_SIZE 16
# define SHA256_DIGEST_SIZE 32
# define MAX_DIGEST_SIZE 32

typedef enum e_input_type 
{
	INPUT_STDIN,
	INPUT_STRING,
	INPUT_FILE
} t_input_type;

typedef struct s_input
{
	t_input_type    type;
	char      		*value; // file name or the string given with -s flag
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

typedef struct s_md5_state
{
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
} t_md5_state;

typedef struct s_sha256_state
{
	uint32_t	h[8];
} t_sha256_state;

extern const uint32_t	g_md5_k[64];
extern const int		g_md5_s[64];
extern const uint32_t	g_sha256_k[64];
extern const int		g_sha256_s[64];

typedef struct s_digest 
{
	unsigned char 	bytes[MAX_DIGEST_SIZE];
	size_t			size;
} t_digest;

// parser
int				parse_options(t_options *options, int argc, char **argv);
t_input			*new_input(t_input_type type, const char *value);
void			add_input(t_options *options, t_input *new);
void			free_inputs(t_input *inputs);

// cmd handling
int 			cmd_md5(int argc, char **argv);
int				cmd_sha256(int argc, char **argv);
int				dispatch_commands(int argc, char **argv);

// read inputs, files, string
int				read_input(t_input *input, t_buffer *buffer);
int				read_fd(int fd, t_buffer *buffer);
void			free_buffer(t_buffer *buffer);

// md5 & sha 256
unsigned char 	*md5_pad(const unsigned char *data, size_t len, size_t *padded_len);
void			md5_init(t_md5_state *state);
void			md5_process_block(t_md5_state *state, const unsigned char *block);
uint32_t		left_rotate(uint32_t x, int n);
// uint32_t		read_word_le(const unsigned char *block, int index);
int				md5_hash(const unsigned char *data, size_t len, t_digest *digest);
void    		print_digest_hex(t_digest *digest);
void 			print_result(t_input *input, t_digest *digest, t_options *options, const char *label);
int 			handle_p_flag(t_options *options);
// sha 256
uint32_t right_rotate(u_int32_t x, int n);


// errors
int				error_invalid_option(char options);
int				error_option_argument(char options);
int				error_malloc(void);
int 			error_open(const char *name);
int 			error_read(const char *name);
int				error_invalid_command(const char *name);
//debug
void			print_options(t_options *options);
void			test_inputs(t_options *options);
const char		*type_str(t_input_type type);
void			print_digest_le(t_md5_state *state);

//test
unsigned char *apply_padding(const unsigned char *data, size_t len, size_t *padded_len, char *algo);

#endif	