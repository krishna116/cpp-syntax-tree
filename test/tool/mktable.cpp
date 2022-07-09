/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @file mktable.cpp
 *
 * @brief Make table for the first 256 UTF8 characters.
 *
 */

#include <cstdio>
#include <string>
#include <vector>

void mktable(const std::string&  table, const std::vector<uint8_t>& removed_chars = {})
{
    std::vector<char>vec(256, 1);
    const char* indent = "    ";

    for(auto ch : removed_chars) vec[ch] = 0;

    printf("static const bool %s[256] = {\n", table.c_str());

    int i = 0;
    for(auto ch : vec)
    {
        if(i%16 == 0) printf("%s", indent);
        printf("%d", ch);
        if(i%16 < 15) printf(", ");
        if(i%16 == 15)
        {
            if(i != 255)
            {
                printf(",  // %3d -> %3d\n", i-15, i);
            }
            else
            {
                printf("   // %3d -> %3d\n};\n", i-15, i);
            }
        }
        ++i;
    }
}

int main(int argc, char* argv[])
{
    if(argc >= 2)
    {
        std::vector<uint8_t> removed_chars;
        for(auto i = 2 ; i < argc; ++i)
        {
            removed_chars.push_back((uint8_t)std::atoi(argv[i]));
        }
        mktable(argv[1], removed_chars);
    }
    else
    {
        static const char* help = R"(mktable <table-name> [remove-char-list]

Make table for the first 256 UTF8 characters.

Example:
  mktable foo 1 3 5
)";

        printf("%s", help);
    }

    return 0;
}
