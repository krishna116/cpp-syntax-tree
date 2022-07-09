/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "bjl.h"

#include <iostream>
#include <string>

using namespace bjl;

void freeTree(Node* t)
{
    if(t)
    {
        for(auto& child: t->getChildArray())
        {
            freeTree(child);
        }
        delete t;
    }
}

void dumpTree(Node* t, size_t depth = {})
{
    if(t)
    {
        std::cout << std::string(depth*2, ' ')
                  << t->label()
                  << "[x=" << t->x() << "]"
                  << "[y=" << t->y() << "]"
                //<< "[mod=" << t->mod() << "]"
                //<< "[shift=" << t->shift() << "]"
                //<< "[change=" << t->change() << "]"
                <<std::endl;

        for(auto& child: t->getChildArray())
        {
            dumpTree(child, depth + 1);
        }
    }
}

void test1()
{
    auto t = new Node("a",{ new Node("b"), 
                            new Node("c")
                          });

    std::cout<<"--before-layout----------------[algorithm-bjl]\n";
    dumpTree(t);
    TreeLayout(t);
    std::cout<<"--after-layout-----------------[algorithm-bjl]\n";
    dumpTree(t);
    freeTree(t);
}

void test2()
{
    auto t = new Node("a",{ new Node("b",{
                                            new Node("g"),
                                            new Node("h"),
                                            new Node("i"),
                                            new Node("j"),
                                            new Node("k"),
                                            new Node("l"),
                                            new Node("m", {new Node("p")})
                                         }
                                    ), 
                            new Node("c"), 
                            new Node("d"), 
                            new Node("e", {new Node("n")}), 
                            new Node("f", {new Node("o",{
                                                          new Node("q"),
                                                          new Node("r"),
                                                          new Node("s"),
                                                          new Node("t"),
                                                          new Node("u"),
                                                          new Node("v"),
                                                          new Node("w")
                            })})
    });

    std::cout<<"--before-layout----------------[algorithm-bjl]\n";
    dumpTree(t);
    TreeLayout(t);
    std::cout<<"--after-layout-----------------[algorithm-bjl]\n";
    dumpTree(t);
    freeTree(t);
}

int main()
{
    std::cout << "==[test1]============================================\n";
    test1();

    std::cout << "\n==[test2]============================================\n";
    test2();

    return 0;
}