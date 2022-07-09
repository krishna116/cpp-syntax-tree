/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "PropertyParser.h"

#include <iostream>

using namespace cst;

int test1()
{
    std::cout<<"==test1=====================================\n";
    std::string text = R"(label = "a node label" color = "red")";
    auto prop = PropertyParser::toProperty(text);
    for(auto& pair: prop)
    {
        std::cout<<"["<<pair.first<< "] = ["<<pair.second<<"]"<<std::endl;
    }

    if(prop.size() == 2) return 0;

    return 1;
}

int test2()
{
    std::cout<<"==test2=====================================\n";
    std::string text = R"( label = "lab\"el" color = "dark slate grey" )";
    auto prop = PropertyParser::toProperty(text);
    for(auto& pair: prop)
    {
        std::cout<<"["<<pair.first<< "] = ["<<pair.second<<"]"<<std::endl;
    }

    if(prop.size() == 2) return 0;
    
    return 1;
}

int test3()
{
    std::cout<<"==test3=====================================\n";
    std::string text = R"(label = a-node-label color = "green" other properties omitted...)";
    auto prop = PropertyParser::toProperty(text);
    for(auto& pair: prop)
    {
        std::cout<<"["<<pair.first<< "] = ["<<pair.second<<"]"<<std::endl;
    }

    if(prop.size() == 2) return 0;
    
    return 1;
}

int main()
{
    int i = 0;

    i += test1();
    i += test2();
    i += test3();

    return i;
}