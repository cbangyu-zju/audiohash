#include <cstdlib>

extern "C" {
#include <execinfo.h>
#include <sys/types.h>
}

#include "error.h"

AudioHashError::AudioHashError()
{
    void *error_backtrace[16];
    int error_backtrace_size;

    error_backtrace_size = backtrace(error_backtrace, 16);

    char **strings = backtrace_symbols(error_backtrace, error_backtrace_size);

    for (int i = 0; i < error_backtrace_size; i++)
    {
        callstack += strings[i];
        callstack += "\n";
    }

    free(strings);
}

AudioHashError::~AudioHashError() throw()
{
}

const char *AudioHashError::what() const throw()
{
    return callstack.c_str();
}
