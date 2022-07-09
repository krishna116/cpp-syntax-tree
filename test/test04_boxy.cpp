/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Boxy.h"

#include <iostream>

using namespace cst;

int test_boxy()
{
    std::cout << "test boxy..." << std::endl;

    Boxy boxy;
    if (boxy.good())
    {
        std::string text = "hello world";
        auto textBox = boxy.getTextBox(text);
        if (textBox.width > 0.0f && textBox.height > 0.0f)
        {
            std::cout << "text   = " << text << std::endl;
            std::cout << "width  = " << textBox.width << std::endl;
            std::cout << "height = " << textBox.height << std::endl;
            std::cout << "test boxy pass." << std::endl;
            return 0;
        }
    }
    std::cout << "test boxy fail." << std::endl;
    return 1;
}

int main()
{
    return test_boxy();
}