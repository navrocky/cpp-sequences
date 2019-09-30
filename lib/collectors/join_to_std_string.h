#pragma once

#include "../common/sequence.h"
#include <string>

namespace std
{
std::string to_string(const std::string& s)
{
    return s;
}
}

namespace Sequences
{

class JoinToStrStringCollector
{
public:
    JoinToStrStringCollector(const std::string& separator)
        : separator(separator)
    {
    }

    JoinToStrStringCollector(const JoinToStrStringCollector& src) = delete;

    JoinToStrStringCollector(JoinToStrStringCollector&& src)
        : separator(std::move(src.separator))
    {
    }

    template <typename SrcSequence>
    std::string create(SrcSequence&& srcSequence) const
    {
        // TODO made this more optimal if we know source sequence size

        std::string result;
        typename SrcSequence::ValueType val;
        while (Internal::getNextSequenceValue(
            srcSequence, [&result, this](const typename SrcSequence::ValueType& val) {
                auto s = std::to_string(val);
                if (result.empty())
                    result = s;
                else
                    result += separator + s;
            }))
        {
        }
        return result;
    }

private:
    std::string separator;
};

auto joinToStdString(const std::string& separator = ", ")
{
    return JoinToStrStringCollector(separator);
}
}
