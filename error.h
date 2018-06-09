#ifndef AUDIOHASH_ERROR_H
#define AUDIOHASH_ERROR_H

#include <exception>
#include <string>

class AudioHashError : public std::exception
{
    std::string callstack;

public:
    AudioHashError();
    ~AudioHashError() throw();
    virtual const char *what() const throw();
};

class FileOpenError: public AudioHashError
{
};

class BadFormatError: public AudioHashError
{
};

class ResampleError: public AudioHashError
{
};

class InvalidInputError: public AudioHashError
{
};

#endif
