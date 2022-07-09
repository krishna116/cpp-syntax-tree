/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Lexer.h"

#include <iostream>

using namespace cst;

int test_lexer()
{
    std::string text = R"--(R"(abc)" def var123 R"~(def)~" R"(cat and fish)")--";
    Lexer lexer((uint8_t *)text.data(), text.size());

    int i = 0;
    int ret = 1;
    while ((ret = static_cast<int>(lexer.getNextTokenType())) > 0)
    {
        std::cout << "[id=" << ++i << "]"
                  << "[" << lexer.getCurrentToken() << "]"
                  << std::endl;

        if (i > 36)
        {
            break;
        }
    }

    std::cout << "--lexer return " << ((ret == 0) ? ("ok") : ("error")) << std::endl;
    return ret;
}


int main()
{
    return test_lexer();
}