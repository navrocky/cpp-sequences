#pragma once

#include <string>
#include <utility>

namespace Sequences
{

class JoinToStrStringContinuation
{
public:
    JoinToStrStringContinuation(const std::string& separator)
        : separator(separator)
    {
    }

    JoinToStrStringContinuation(const JoinToStrStringContinuation& src) = delete;

    JoinToStrStringContinuation(JoinToStrStringContinuation&& src)
        : separator(std::move(src.separator))
    {
    }

    template <typename SrcSequence>
    std::string create(SrcSequence&& srcSequence) const
    {
        // TODO made this more optimal if we know source sequence size

        std::string result;
        typename SrcSequence::ValueType val;
        while (srcSequence.getNextValue(val))
        {
            auto s = std::to_string(val);
            if (result.empty())
                result = s;
            else
                result += separator + s;
        }
        return result;
    }

private:
    std::string separator;
};

auto joinToStdString(const std::string& separator = ", ")
{
    return JoinToStrStringContinuation(separator);
}
}
