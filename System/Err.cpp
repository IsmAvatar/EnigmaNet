////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "./Err.hpp"
#include <streambuf>
#include <cstdio>


////////////////////////////////////////////////////////////
// Private data
////////////////////////////////////////////////////////////
namespace
{
// This class will be used as the default streambuf of sf::Err,
// it outputs to stderr by default (to keep the default beaviour)
class DefaultErrStreamBuf : public std::streambuf
{
public :

    DefaultErrStreamBuf()
    {
        // Allocate the write buffer
        static const int size = 64;
        char* buffer = new char[size];
        setp(buffer, buffer + size);
    }

    ~DefaultErrStreamBuf()
    {
        // Synchronize
        sync();

        // Delete the write buffer
        delete[] pbase();
    }

private :

    virtual int overflow(int character)
    {
        if ((character != EOF) && (pptr() != epptr()))
        {
            // Valid character
            return sputc(static_cast<char>(character));
        }
        else
        {
            // Invalid character, or not enough space in the buffer: synchronize output
            return sync();
        }
    }

    virtual int sync()
    {
        // Check if there is something into the write buffer
        if (pbase() != pptr())
        {
            // Retrieve the contents of the write buffer
            int size = static_cast<int>(pptr() - pbase());
            std::string buffer(pbase(), size);

            // Write it into the standard error output
            fprintf(stderr, "%s", buffer.c_str());

            // Reset the pointer position to the beginning of the write buffer
            setp(pbase(), epptr());
        }

        return 0;
    }
};
}

namespace sf
{
////////////////////////////////////////////////////////////
std::ostream& Err()
{
    static DefaultErrStreamBuf buffer;
    static std::ostream stream(&buffer);

    return stream;
}


} // namespace sf