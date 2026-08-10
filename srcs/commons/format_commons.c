#include "ft_ssl.h"

void print_quoted_stripped(const unsigned char *data, size_t len)
{
    size_t i = 0;

    if (len > 0 && data[len - 1] == '\n')
        len--;
    while (i < len)
    {
        write(1, &data[i], 1);
        i++;
    }
    // fd_printf(1, "\"%s\"", data);
}

int handle_p_flag(t_options *options, t_hash_fn hash_fn)
{
    t_buffer buffer;
    t_digest digest;

    if (read_fd(0, &buffer))
        return error_read("stdin");
    if (hash_fn(buffer.data, buffer.len, &digest))
    {
        free_buffer(&buffer);
        return 1;
    }
    if (options->q)
    {
        fd_printf(1, "%s", buffer.data);
        print_digest_hex(&digest);
        fd_printf(1, "\n");
    }
    else
    {
        fd_printf(1, "(\"");
        print_quoted_stripped(buffer.data, buffer.len);
        fd_printf(1, "\")= ");
        print_digest_hex(&digest);
        fd_printf(1, "\n");
    }
    free_buffer(&buffer);
    return 0;
}