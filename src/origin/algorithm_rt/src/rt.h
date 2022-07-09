/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

// 
// [Paper]
//   Tidier Drawings of Trees.
// [Paper-Author]
//   Reingold and Tilford
//

#pragma once

#include <string>

namespace rt
{
    namespace config
    {
        // Horizontal separation between two nodes.
        auto constexpr MinHSep = 10.0f;
        // Vertical separation between two nodes.
        auto constexpr MinVSep = 10.0f;
    }

    /**
     * @brief Node of tree definition.
     */
    struct Node
    {
        Node(std::string label_, Node* left, Node* right)
            :label(label_), leftLink(left), rightLink(right)
        {}
        Node(std::string label_)
            :label(label_), leftLink(nullptr), rightLink(nullptr)
        {}

        Node* leftLink = nullptr;   ///< Left child.
        Node* rightLink = nullptr;  ///< Right child.
        double xCoord = 0.0f;       ///< x position.
        double yCoord = 0.0f;       ///< y position.
        double offset = 0.0f;       ///< Offset for subtree.
        bool thread = false;        ///< Is thread or not.

        std::string label;          ///< Node label used for debug.
    };

    /**
     * @brief Extreme used to record subtree's contour node.
     */
    struct Extreme
    {
        Node* addr = nullptr;   ///< Node address.
        double offset = 0;      ///< Offset from root of subtree.
        int level = 0;          ///< Tree level.
    };

    /**
     * @brief Layout a tree.
     * 
     * @param tree  Input a tree.
     */
    void layout(Node* tree);
}
