/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Parser.h"
#include "SyntaxTree.h"

#include <iostream>

using namespace cst;

int test_parser()
{
    std::string treeStr = R"~(
[S
    [NP a]
    [VP
        [V b1]
        [NP
            [Adj c1]  
            [NP c2]
        ]
        [V R"(cpp raw text)"]
    ]
]
)~";

    auto syntaxTree = Parser::buildSyntaxTree(treeStr);
    if (syntaxTree)
    {
        syntaxTree->dumpTree();
        std::cout << "\n--parser is ok." << std::endl;
        return 0;
    }

    return 1;
}

int main()
{
    return test_parser();
}