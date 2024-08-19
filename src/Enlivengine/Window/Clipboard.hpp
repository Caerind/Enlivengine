#pragma once

#include <string>

namespace en
{

class Clipboard
{
public:
    static std::string GetString();
    static void SetString(const std::string& str);

private:
    Clipboard() = delete;
};

} // namespace en
