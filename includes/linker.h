#ifndef FT_SSL_H
# define FT_SSL_H

#include "libft.h"

typedef enum e_input_type 
{
    INPUT_STDIN,
    INPUT_STRING,
    INPUT_FILE
} t_input_type;

typedef struct s_input
{
    t_input_type    type;
    char            *value; // file name or the string given with -s flag
    struct s_input  *next; // linked-list in chronologic order
} t_input;

typedef struct s_options
{
    int         p; // echo std + checksum append
    int         q; // quit mode
    int         r; // reverse format
    t_input     *inputs; // linked list of object to be hashed
    int         use_stdin; // true if no input is found
} t_options;


#endif