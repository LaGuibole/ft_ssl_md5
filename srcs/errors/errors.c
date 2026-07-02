#include "ft_ssl.h"

int error_invalid_option(char options)
{
    fd_printf(2, "ft_ssl: invalid option -- '%s'\n", &options);
    return 1;
}

int error_option_argument(char options)
{
    fd_printf(2, "ft_ssl: option requires an argument -- '%s'\n", &options);
    return 1;
}

int error_malloc(void)
{
    fd_printf(2, "ft_ssl: malloc: Cannot allocate memory\n");
    return 1;
}

int error_open(const char *name)
{
    fd_printf(2, "ft_ssl: %s: No such file or directory\n", &name);
    return 1;
}

int error_read(const char *name)
{
    fd_printf(2, "ft_ssl: %s: Error reading file\n", &name);
    return 1;
}

int error_unexpected_argument(const char *name)
{
    fd_printf(2, "ft_ssl: unexpected argument -- '%s'\n", name);
    return 1;
}

int error_invalid_command(const char *name)
{
    fd_printf(2, "Error: %s invalid command\n", name);
    return 1;
}