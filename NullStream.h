#pragma once

#include <ostream>
#include <functional>
#include <memory>

namespace
{
    class NullStreambuf : public std::streambuf
    {
    protected:
        int overflow(int c) override
        {
            return c;
        }
    };
    class NullOStream : private NullStreambuf, public std::ostream
    {
    public:
        NullOStream() : std::ostream( this ) {}
    };

    class Provider
    {
    public:
        std::ostream & operator () ()
        {
            return *stream;
        }
    private:
        std::shared_ptr<NullOStream> stream = std::make_shared<NullOStream>();
    };
};

namespace NullStream
{
    std::function<std::ostream &()> provider()
    {
        return Provider();
    }
}