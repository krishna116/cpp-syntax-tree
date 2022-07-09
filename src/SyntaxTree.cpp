/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "SyntaxTree.h"

#include <cassert>
#include <iostream>

using namespace cst;

SyntaxTree::SyntaxTree(Node *root) 
    : root_(root)
{
}

SyntaxTree::~SyntaxTree()
{
    freeTree(root_);
}

Node *SyntaxTree::getRoot()const
{
    return root_;
}

void SyntaxTree::setRoot(Node *root)
{
    freeTree(root_);
    root_ = root;
}

void internalDumpTree(Node *tree, std::size_t depth = 0)
{
    if (tree)
    {
        std::cout << std::string(depth * 2, ' ')
                  << tree->label() << "["
                  << "x=" << tree->x() <<", "
                  << "y=" << tree->y() 
                  <<"]\n";

        for (auto &child : tree->childArray())
        {
            internalDumpTree(child, depth + 1);
        }
    }
}

void SyntaxTree::dumpTree()const
{
    internalDumpTree(root_);
}

Node *SyntaxTree::newNode(const std::string& label)
{
    static std::size_t nextId = 0;
    
    auto node = new Node(label);
    node->id(nextId++);
    return node;
}

void internalFreeTree(Node *treeRoot)
{
    if (treeRoot)
    {
        for (auto &child : treeRoot->childArray())
        {
            internalFreeTree(child);
        }
        delete treeRoot;
    }
}

void SyntaxTree::freeTree(Node *&treeRoot)
{
    if (treeRoot)
    {
        internalFreeTree(treeRoot);
        treeRoot = nullptr;
    }
}


