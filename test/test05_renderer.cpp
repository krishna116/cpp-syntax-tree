/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Renderer.h"
#include "Parser.h"
#include "Boxy.h"
#include "Layouter.h"

#include <iostream>

using namespace cst;

int test_renderer()
{
   static const char* buf = R"~(
[S
    [R"(label = "while" color = "red")"]
    [E
        [id a]
        [relop <]
        [id b]
    ]
    [R"(label="do" color = "red")"]
    [S1
        [R"(label = "if" color = "red")"]
        [E
            [id c]
            [relop <]
            [id d]
        ]
        [R"(label = "then" color = "red")"]
        [S1
            [id x]
            [=]
            [E
                [E
                    [id y]
                ]
                [+]
                [E
                    [id z]
                ]
            ]
        ]
        [R"(label = "else" color = "red")"]
        [S2
            [id x]
            [=]
            [E
                [E
                    [id y]
                ]
                [-]
                [E
                    [id z]
                ]
            ]
        ]
    ]
]
    )~";

    auto syntaxTree = Parser::buildSyntaxTree(buf);
    if(syntaxTree != nullptr)
    {
        TreeSize treeSize;
        Layouter layouter;
        if(layouter.layout(syntaxTree->getRoot(), treeSize))
        {
            Renderer renderer(syntaxTree, treeSize, "test.pdf");
            if(renderer.drawTree())
            {
                std::cout << "draw tree pass." << std::endl;
                return 0;
            }
            else
            {
                std::cout << "renderer.drawTree fail." << std::endl;
            }
        }
        else
        {
            std::cout << "layouter.layout fail." << std::endl;
        }
    }
    else
    {
        std::cout << "Parser::buildSyntaxTree fail." << std::endl;
    }

    return 1;
}

int main()
{
    return test_renderer();
}