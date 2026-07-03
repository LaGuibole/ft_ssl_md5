#include "ft_ssl.h"

static void print_quoted_stripped(const unsigned char *data, size_t len)
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

void print_result(t_input *input, t_digest *digest, t_options *options, const char *label)
{
    if (options->q)
    {
        print_digest_hex(digest);
        fd_printf(1, "\n");
        return ;
    }
    if (options->r)
    {
        print_digest_hex(digest);
        if (input->type == INPUT_FILE)
            fd_printf(1, " %s", input->value);
        else if (input->type == INPUT_STRING)
            fd_printf(1, " \"%s\"", input->value);
        fd_printf(1, "\n");
        return ;
    }
    if (input->type == INPUT_STDIN)
    {
        fd_printf(1, "MD5(stdin)= ");
        print_digest_hex(digest);
        fd_printf(1, "\n");
        return ;
    }
    fd_printf(1, "%s (", label);
    if (input->type == INPUT_FILE)
        fd_printf(1, "%s", input->value);
    else
        fd_printf(1, "\"%s\"", input->value);
    fd_printf(1, ") = ");
    print_digest_hex(digest);
    fd_printf(1, "\n");
}

int handle_p_flag(t_options *options)
{
    t_buffer buffer;
    t_digest digest;

    if (read_fd(0, &buffer))
        return error_read("stdin");
    if (md5_hash(buffer.data, buffer.len, &digest))
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
        print_quoted_stripped(buffer.data, buffer.len - 1);
        fd_printf(1, "\")= ");
        print_digest_hex(&digest);
        fd_printf(1, "\n");
    }
    free_buffer(&buffer);
    return 0;
}