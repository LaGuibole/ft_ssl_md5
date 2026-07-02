#include "ft_ssl.h"

static const t_command g_commands[] = {
    { "md5", cmd_md5 },
    { "sha256", cmd_sha256 },
    { NULL, NULL }
};

int dispatch_commands(int argc, char **argv)
{
    int i = 0;

    if (argc < 2)
    {
        fd_printf(2, "Usage: ft_ssl command [flags] [file/string]\n");
        return 1;
    }
    while (g_commands[i].command_name)
    {
        if (ft_strcmp(argv[1], (char *)g_commands[i].command_name) == 0)
            return g_commands[i].run(argc - 1, argv + 1);
        i++;
    }
    return error_invalid_command(argv[1]);
}