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

#include <memory>

namespace cst
{
    class Node;
    class Boxy;
    using BoxyPtr = std::shared_ptr<Boxy>;

    /**
     * @brief Calculate x and y position of a tree's all nodes.
     */
    class Layouter
    {
    public:
        /**
         * @brief Construct a new Layouter object
         * 
         * @param[in] fontSize  It determines the textbox of node label.
         * @param[in] nodeHSep  Node horizontal separation.
         * @param[in] nodeVSep  Node vertical separation.
         * 
         */
        Layouter(double fontSize = option::FontSize::getFontSize(),
                 double nodeHSep = option::NodeSep::getHSep(),
                 double nodeVSep = option::NodeSep::getVSep()
        );
        /**
         * @brief Layout a tree using BJL's algorithm.
         * 
         * @param[in] t             A tree to layout.
         * @param[out] TreeSize     The tree size.
         */
        bool layout(Node* t, TreeSize& treeSize);

    private:
        BoxyPtr boxy_;
        double nodeHSep_;
        double nodeVSep_;

        //~~~~~~~~~~~~~~~~~~~Layout algorithm~~~~~~~~~~~~~~~~~~~~~~~~~
        // [Paper]
        //   Improving Walker's Algorithm to Run in Linear Time
        // [Paper-Author]
        //   Christoph Buhheim, Michael Jünger, and Sebastian Leipert
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        void FirstWalk(Node *v, Boxy& boxy, Node *leftSibingOfV = nullptr);
        void SecondWalk(Node *v, double m, size_t level = 0);
        Node *Apportion(Node *v, Node *leftSibingOfV, Node *dac);
        Node *NextLeft(Node *v);
        Node *NextRight(Node *v);
        void MoveSubTree(Node *w0, Node *w1, double shift);
        void ExecuteShifts(Node *v);
        Node *Ancestor(Node *vi, Node *v, Node *dac);
        double Distance(Node *v);
    };
} 
