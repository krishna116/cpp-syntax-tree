/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Layouter.h"
#include "SyntaxTree.h"

#include <iostream>
#include <string>

using namespace cst;

int work(Node* t)
{
    SyntaxTree tree(t);

    std::cout<<"--before-layout----------------[algorithm-bjl]\n";
    tree.dumpTree();

    TreeSize treeSize;
    Layouter layouter;
    if(layouter.layout(tree.getRoot(), treeSize))
    {
        std::cout<<"--after-layout-----------------[algorithm-bjl]\n";
        tree.dumpTree();
        std::cout<<"tree width  = "<< treeSize.xmax - treeSize.xmin <<std::endl;
        std::cout<<"tree height = "<< treeSize.ymax <<std::endl;

        return 0;
    }

    return 1;
}

int test1()
{
    auto t = new Node("a",{ new Node("b"), 
                            new Node("c")
                          });

    std::cout << "\n==[test1]============================================\n";
    return work(t);
}

int test2()
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

    std::cout << "==[test2]============================================\n";
    return work(t);
}

int main()
{
    int i = 0;

    i += test1();
    i += test2();

    return i;
}