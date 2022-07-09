/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include "BaseType.h"

#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace cst
{
    /**
     * @brief The syntax tree.
     * 
     * Manage a syntax tree's life time.
     */
    class SyntaxTree
    {
    public:
        SyntaxTree(Node* root = nullptr);
        ~SyntaxTree();

        /**
         * @brief Set the tree root.
         * 
         * @param[in] root The tree root.
         */
        void setRoot(Node* root);

        /**
         * @brief Get the tree root.
         * 
         * @return Node*    The tree root.
         */
        Node *getRoot()const;

        /**
         * @brief Dump tree.
         * 
         */
        void dumpTree()const;

        /**
         * @brief Create new tree node.
         * 
         * @param[in] label     The node label.
         * @return Node*        The new node.
         */
        static Node* newNode(const std::string& label);
        
        /**
         * @brief Free the tree.
         * 
         * @param treeRoot  The tree root.
         */
        static void freeTree(Node*& treeRoot);
    private:
        Node *root_ = nullptr;
    }; // SyntaxTree end.
} // namespace cst
