/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "bjl.h"

namespace bjl
{
    /**
     * @brief Layout a tree using algorithm-bjl
     * 
     * @param[in] t     Input a tree to layout.
     */
    void TreeLayout(Node* t);

    /**
     * @brief Allocate initial space for the tree nodes.
     * 
     * @param v                 A root node.
     * @param leftSibingOfV     The root node's left sibling.
     */
    void FirstWalk(Node* v, Node* leftSibingOfV = {});

    /**
     * @brief Calculate final x and y position of the tree nodes.
     * 
     * @param v         A root node.
     * @param m         Global modifier for all the nodes.
     * @param level     Level will go down.
     */
    void SecondWalk(Node* v, double m = {}, size_t level = {});

    /**
     * @brief Adjust tree node position to make it more beautiful.
     * 
     * @param v                 A node.
     * @param leftSibingOfV     The node's left sibling.
     * @param dac               Default ancestor.
     * 
     * @return node             A new default ancestor.
     */
    Node* Apportion(Node* v, Node* leftSibingOfV, Node* dac);

    /**
     * @brief Calculate node's left contour node at next level.
     * 
     * @param v         A contour node.
     * @return Node*    Next left contour node.
     */
    Node* NextLeft(Node* v);

    /**
     * @brief Calculate node's right contour node at next level.
     * 
     * @param v         A contour node.
     * @return Node*    Next right contour node.
     */
    Node* NextRight(Node* v);

    /**
     * @brief Adjust subtree position.
     * 
     * w0 and w1 are two subtree roots.
     * w0 and w1 are sibling nodes.
     * 
     * @param w0        A sibling node to the left.
     * @param w1        A sibling node to the right.
     * @param shift     Shift size.
     */
    void MoveSubTree(Node* w0, Node* w1, double shift);

    /**
     * @brief Shift current subtree.
     * 
     * @param v         Current subtree root node.
     */
    void ExecuteShifts(Node* v);

    /**
     * @brief Get the dummy ancestor.
     * 
     * @param vi        Left tree node.
     * @param v         Right subtree root node.
     * @param dac       Default ancestor.
     * @return Node*    Dummy ancestor.
     */
    Node* Ancestor(Node* vi, Node* v, Node* dac);
}

using namespace bjl;

#define assert_true(x) assert(x)

void bjl::TreeLayout(Node *t)
{
    FirstWalk(t);
    SecondWalk(t, (-1.0) * t->prelim());
}

void bjl::FirstWalk(Node *v, Node *leftSibingOfV)
{
    if (!v->isLeaf())
    {
        auto dac = v->getLeftMostChild();

        Node *leftSibingOfChild = nullptr;
        for (auto &child : v->getChildArray())
        {
            FirstWalk(child, leftSibingOfChild);
            dac = Apportion(child, leftSibingOfChild, dac);
            leftSibingOfChild = child;
        }

        ExecuteShifts(v);

        auto midpoint = (v->getLeftMostChild()->prelim() + v->getRightMostChild()->prelim()) * 0.5;

        if (leftSibingOfV)
        {
            // This node(v) has left sibling, this node offset to right.
            v->prelim(leftSibingOfV->prelim() + config::MinHSep);
            v->mod(v->prelim() - midpoint);
        }
        else
        {
            v->prelim(midpoint);
        }
    }
    else
    {
        if (leftSibingOfV)
        {
            // This node(v) has left sibling, this node offset to right.
            v->prelim(leftSibingOfV->prelim() + config::MinHSep);
        }
        else
        {
            // v->prelim(0.0f);
        }
    }
}

void bjl::SecondWalk(Node *v, double m, size_t level)
{
    v->x(v->prelim() + m);
    v->y(level * config::MinVSep);
    for (auto &child : v->getChildArray())
    {
        SecondWalk(child, m + v->mod(), level + 1);
    }
}

Node* bjl::Apportion(Node *v, Node *leftSibingOfV, Node *dac)
{
    //
    //     [-]     [+]      o:  Outside contour.
    //      .       .       i:  Inside contour.
    //     / \     / \      LL: Left-tree(s) Left-most-contour.
    //    /___\   /___\     LR: Left-tree(s) Right-most-contour.
    //    o   i   i   o     RL: Right-tree Left-contour.
    //    LL  LR  RL  RR    RR: Right-tree Right-contour.
    //
    if (leftSibingOfV)
    {
        auto RL = v;
        auto RR = v;
        auto LR = leftSibingOfV;
        auto LL = RL->parent()->getLeftMostChild(); // Left most sibling of RL.

        auto LLMod = LL->mod();
        auto LRMod = LR->mod();
        auto RLMod = RL->mod();
        auto RRMod = RR->mod();

        //
        // Compare Left tree(s) Right most contour and Right tree Left contour
        // at same level.
        //
        while (NextRight(LR) && NextLeft(RL))
        {
            LL = NextLeft(LL);
            LR = NextRight(LR);
            RL = NextLeft(RL);
            RR = NextRight(RR);
            RR->ancestor(v);

            auto shift = (LR->prelim() + LRMod) 
                       - (RL->prelim() + RLMod) 
                       + config::MinHSep;
            if (shift > 0.0f)
            {
                MoveSubTree(Ancestor(LR, v, dac), v, shift);
                RLMod += shift;
                RRMod += shift;
            }

            LLMod += LL->mod();
            LRMod += LR->mod();
            RLMod += RL->mod();
            RRMod += RR->mod();
        } // while-end

        if (NextRight(LR) != nullptr && NextRight(RR) == nullptr)
        {
            // RR thread point to LR's right-contour at next level.
            RR->thread(NextRight(LR));
            RR->mod(RR->mod() + LRMod - RRMod);
        }

        if (NextLeft(RL) != nullptr && NextLeft(LL) == nullptr)
        {
            // LL thread point to RL's left-contour at next level.
            LL->thread(NextLeft(RL));
            LL->mod(LL->mod() + RLMod - LLMod);
            dac = v;
        }
    } // if-leftSibingOfV-end

    return dac;
}

Node *bjl::NextLeft(Node *v)
{
    if (!v->getChildArray().empty())
    {
        return v->getLeftMostChild();
    }
    else
    {
        return v->thread();
    }
}

Node *bjl::NextRight(Node *v)
{
    if (!v->getChildArray().empty())
    {
        return v->getRightMostChild();
    }
    else
    {
        return v->thread();
    }
}

void bjl::MoveSubTree(Node *w0, Node *w1, double shift)
{
    //
    // Count the number of subtrees from w0 to w1.
    //
    auto getNumber = [&]()
    {
        assert_true(w0 != nullptr);
        assert_true(w0->parent() != nullptr);
        assert_true(w1 != nullptr);
        assert_true(w1->parent() == w0->parent());

        auto end = w0->parent()->getChildArray().end();
        auto left = end;
        auto right = end;
        for (auto it = w0->parent()->getChildArray().begin(); it != end; ++it)
        {
            if (*it == w0)
            {
                left = it;
                for (it = it + 1; it != end; ++it)
                {
                    if (*it == w1)
                    {
                        right = it; // left < right.
                        break;
                    }
                }
                break;
            }
        }

        assert_true(left != end);
        assert_true(right != end);
        // assert_true(left < right);

        return right - left;
    };

    auto subtrees = getNumber();
    w1->change(w1->change() - shift / subtrees);
    w1->shift(w1->shift() + shift);
    w0->change(w0->change() + shift / subtrees);
    w1->prelim(w1->prelim() + shift);
    w1->mod(w1->mod() + shift);
}

void bjl::ExecuteShifts(Node *v)
{
    double shift = {};
    double change = {};

    auto & childArray = v->getChildArray();
    for (auto it = childArray.rbegin(); it != childArray.rend(); ++it)
    {
        (*it)->prelim((*it)->prelim() + shift);
        (*it)->mod((*it)->mod() + shift);
        change += (*it)->change();
        shift += (*it)->shift() + change;
    }
}

Node *bjl::Ancestor(Node *vi, Node *v, Node *dac)
{
    assert_true(vi != nullptr);
    assert_true(vi->ancestor() != nullptr);
    assert_true(v != nullptr);

    if (vi->ancestor()->parent() == v->parent())
    {
        return vi->ancestor();
    }
    else
    {
        return dac;
    }
}