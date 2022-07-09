/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "rt.h"
#include <iostream>

using namespace rt;

void dump(Node *tree, size_t depth = 0)
{
    if (tree)
    {
        printf("%s",std::string(depth * 2, ' ').c_str());
        printf("%s", tree->label.c_str());
        printf("[x = %.2f, y = %.2f]\n", tree->xCoord, tree->yCoord);

        dump(tree->leftLink, depth + 1);
        dump(tree->rightLink, depth + 1);
    }
}

void clear(Node *tree)
{
    if (tree)
    {
        clear(tree->leftLink);
        clear(tree->rightLink);
        delete tree;
    }
}

void work(Node *n)
{
    std::cout << "--Before-layout-------------------------\n";
    dump(n);
    layout(n);
    std::cout << "--After-layout--------------------------\n";
    dump(n);
    std::cout << std::endl;

    clear(n);
}

void test1()
{
    auto n = new Node("a",
                        new Node("b"),
                        new Node("c")
                    );
    work(n);
}

void test2()
{
    auto n = new Node("a",
                        new Node("b",
                                new Node("d"),
                                new Node("e")),
                        new Node("c",
                                nullptr,
                                new Node("f")));
    work(n);
}

int main()
{
    test1();
    test2();
    
    return 0;
}