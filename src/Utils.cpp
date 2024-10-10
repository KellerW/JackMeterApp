#include "Utils.hpp"
#include <cstdio>
#include <poll.h>

bool stdin_available()
{
    struct pollfd pfd[1] = {};
    pfd[0].fd            = fileno(stdin);
    pfd[0].events        = POLLIN;
    int ret              = poll(pfd, 1, 0);
    if (ret > 0)
    {
        if (getc(stdin) == EOF)
            return false;
    }
    return ret > 0;
}
