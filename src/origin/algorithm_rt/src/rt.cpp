/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "rt.h"

#include <algorithm>
#include <iostream>

using namespace rt;

#define PRINT_VERBOSE_INFO 1

#if PRINT_VERBOSE_INFO > 0
    #define printNode(a,b,c)        printNodeEX(a,b,c)
    #define printMakeThread(a,b,c)  printMakeThreadEX(a,b,c)
    #define print(a)                printEX(a) 
#else
    #define printNode(a,b,c)
    #define printMakeThread(a,b,c)
    #define print(a)
#endif

void printNodeEX(std::string name, Node* node, std::string suffix = {})
{
    std::cout<< name <<" = "<< (node ? (node->label) : "nullptr") << suffix;
}

void printMakeThreadEX(Node* from, std::string leftOrRight, Node* to)
{
    std::cout<<"[make-thread]\n  "
                << from->label
                << "."
                <<leftOrRight
                <<" = "<<
                to->label<<std::endl;
};

template<typename T>
void printEX(T var)
{
    std::cout<<var;
}

/**
 * @brief Separate subtrees.
 * 
 * @param tree      Tree root.
 * 
 * @param rightMost Tree's right-extreme node reference.
 *                  Alias1: tree's right-most-descendant at max level.
 *                  Alias2: tree's right-most-contour at max level.
 *                  Note1: it is always a leaf node.
 *                  Note2: it may become a threaded-node.
 *                  Note3: it is always a most right contour node.
 * 
 * @param leftMost  Tree's left-extreme node reference.
 *                  Alias1: tree's left-most-descendant at max level.
 *                  Alias2: tree's left-most-contour at max level.
 *                  Note1: it is always a leaf node.
 *                  Note2: it may become a threaded-node.
 *                  Note3: it is always a most left contour node.
 * 
 * @param level     Tree start level.
 */
void setup(Node *tree, Extreme &rightMost, Extreme &leftMost, size_t level = 0)
{
    //
    // At first, L will be the tree's left-child and R will be the tree's
    // right-child, but it is not always is this, it may be changed later.
    //
    Node *L;
    Node *R;

    // Extreme for left-subtree.
    Extreme extremeLR;
    Extreme extremeLL;
    // Extreme for right-subtree.
    Extreme extremeRR;
    Extreme extremeRL;

    if (tree == nullptr)
    {
        // Mark null-node-level on top of root-node-level(0),
        // so it cannot be selected as extreme.
        rightMost.level = -1;
        leftMost.level = -1;
    }
    else // Leaf-node or non-leaf-node.
    {
        // Allocate y coordinate.
        tree->yCoord = level * config::MinVSep;

        //
        // Run into subtrees.
        L = tree->leftLink;
        R = tree->rightLink;
        setup(L, extremeLR, extremeLL, level + 1);
        setup(R, extremeRR, extremeRL, level + 1);

        //
        // Now the setup backtracking to here.
        // extremeLR, extremeLL, extremeRR, extremeRL are available.
        //
        if (R == nullptr && L == nullptr) // Node is leaf.
        {
            rightMost.addr = tree;
            leftMost.addr = tree;
            leftMost.level = level;
            rightMost.level = level;
            rightMost.offset = 0;
            leftMost.offset = 0;
            tree->offset = 0;
        }
        else // Node is not leaf.
        {
            double curContourSep = config::MinHSep;
            double rootSep = config::MinHSep;
            double leftOffSum = 0.0;
            double rightOffsum = 0.0;

            // Calculate max separation between L and R.
            while (L != nullptr && R != nullptr)
            {
                if (curContourSep < config::MinHSep)
                {
                    rootSep = rootSep + (config::MinHSep - curContourSep);
                    curContourSep = config::MinHSep;
                }

                if (L->rightLink != nullptr)
                {
                    leftOffSum += L->offset;
                    // Eat space.
                    curContourSep -= L->offset;
                    // Go down to right contour.
                    L = L->rightLink;
                }
                else
                {
                    // Change contour link direction.
                    leftOffSum -= L->offset;
                    curContourSep = curContourSep + L->offset;
                    L = L->leftLink;
                }

                if (R->leftLink != nullptr)
                {
                    rightOffsum -= R->offset;
                    // Eat space.
                    curContourSep -= R->offset;
                    // Go down to left contour.
                    R = R->leftLink;
                }
                else
                {
                    // Change contour link direction.
                    rightOffsum += R->offset;
                    curContourSep = curContourSep + R->offset;
                    R = R->rightLink;
                }
            } // while-end

            // Put tree root at the middle.
            tree->offset = (rootSep) / 2;

            // (Tree's left-hand-side offset to left): size.
            leftOffSum = leftOffSum - tree->offset;

            // (Tree's right-hand-side offset to right): size.
            rightOffsum = rightOffsum + tree->offset;

            //
            // Update extreme. The tree model:
            //
            //                      T
            //                    /   \
            //                 L        R
            //                / \      / \
            //              LL--LR    RL--RR
            //
            // Note that either LR or RL may be the lowerest exist,
            // because the above while loop will make one nullptr or both nullptr.
            //
            if (extremeRL.level > extremeLL.level || tree->leftLink == nullptr)
            {
                //
                //                                T
                //                              /   \
                //                           L        R
                //                          / \      / \
                //                         ?--?     *--?
                //                                  |
                // Looking-from-left-to-right-->   RL
                //
                // Note:
                //   If RR exist:
                //      if(level(RL)>level(RR)) it is RL;
                //      if(level(RL)<=level(RR)) it is still RL, as RL equal RR;
                //
                leftMost = extremeRL;
                leftMost.offset += tree->offset;
            }
            else
            {
                //
                //                                         T
                //                                       /   \
                //                                    L        R
                //                                   / \      / \
                //                                  *--?     ?--?
                //                                  |
                // Looking-from-left-to-right-->   LL
                //
                // Note:
                //   If LR exist:
                //      if(level(LL)>level(LR)) it is LL;
                //      if(level(LL)<=level(LR)) it is still LL, as LL equal LR;
                //
                leftMost = extremeLL;
                leftMost.offset -= tree->offset;
            }

            if (extremeLR.level > extremeRR.level || tree->rightLink == nullptr)
            {
                //
                //         t
                //       /   \
                //    L        R
                //   / \      / \
                //  ?--*     ?--?
                //     |
                //     LR      <--Looking-from-right-to-Left.
                //
                rightMost = extremeLR;
                rightMost.offset -= tree->offset;
            }
            else
            {
                //
                //         t
                //       /   \
                //    L        R
                //   / \      / \
                //  ?--?     ?--*
                //              |
                //              RR    <--Looking-from-right-to-Left.
                //
                rightMost = extremeRR;
                rightMost.offset += tree->offset;
            }

            print("[node-info]\n");
            printNode("  T", tree, ", ");
            printNode("  L", L, ", ");
            printNode("  R", R, ", ");
            printNode("  LL", extremeLL.addr, ", ");
            printNode("  LR", extremeLR.addr, ", ");
            printNode("  RL", extremeRL.addr, ", ");
            printNode("  RR", extremeRR.addr, "\n");

            //
            // Update extrme.
            //
            // Note:
            //   The L and R may be changed after above while loop,
            //   so it may be not the tree's left-child or right-child.
            //
            if (L != nullptr && L != tree->leftLink)
            {
                extremeRR.addr->thread = true;
                extremeRR.addr->offset = std::abs(extremeRR.offset 
                                         + tree->offset 
                                         - leftOffSum);

                if ((leftOffSum - tree->offset) <= extremeRR.offset)
                {
                    // L is at extremeRR's left hand side.
                    extremeRR.addr->leftLink = L;
                    printMakeThread(extremeRR.addr, "leftLink", L);
                }
                else
                {
                    // L is at extremeRR's right hand side.
                    extremeRR.addr->rightLink = L;
                    printMakeThread(extremeRR.addr, "rightLink", L);
                }
            }
            else if (R != nullptr && R != tree->rightLink)
            {
                extremeLL.addr->thread = true;
                extremeLL.addr->offset = std::abs(extremeLL.offset 
                                         - tree->offset 
                                         - rightOffsum);
                if ((rightOffsum + tree->offset) >= extremeLL.offset)
                {
                    // R is at extremeRR's right hand side.
                    extremeLL.addr->rightLink = R;
                    printMakeThread(extremeLL.addr, "rightLink", R);
                }
                else
                {
                    // R is at extremeRR's left hand side.
                    extremeLL.addr->leftLink = R;
                    printMakeThread(extremeLL.addr, "leftLink", R);
                }
            }
        } // Node is not a leaf end.
    } // Node is not null end.
} // setup-end

/**
 * @brief Calculate total offset get the final x position.
 * 
 * @param tree      Tree root.
 * @param xpos      Any start position.
 */
void petrify(Node *tree, double xpos = 0.0f)
{
    if (tree != nullptr)
    {
        tree->xCoord = xpos;

        if (tree->thread)
        {
            // The thread should be removed before iterate nodes.
            tree->thread = false;
            tree->leftLink = nullptr;
            tree->rightLink = nullptr;
        }

        // Update totol offset for subtrees and go down.
        petrify(tree->leftLink, xpos - tree->offset);
        petrify(tree->rightLink, xpos + tree->offset);
    }
}

void rt::layout(Node *tree)
{
    Extreme rightMost;
    Extreme leftMost;

    setup(tree, rightMost, leftMost);
    petrify(tree);
}
