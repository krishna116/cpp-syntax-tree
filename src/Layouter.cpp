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
#include "Boxy.h"

#include <limits>
#include <algorithm>

#define assert_true(x) assert(x)

using namespace cst;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// A helper to get width and height of the tree.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class Helper
{
public:
    void init()
    {
        xMin = std::numeric_limits<double>::max();
        xMax = std::numeric_limits<double>::min();
        yMax = std::numeric_limits<double>::min();
    }
    void update(const Node* const v)
    {
        auto xmin = v->x();
        auto xmax = v->x() + v->textBox().width;
        auto ymax = v->y();

        if(xmin < xMin)
        {
            xMin = xmin;
            // Patch for align page center when do renderering.
            xMin2 = xmin - v->textBox().width * 0.5;
        }

        if(xmax > xMax) xMax = xmax;
        if(ymax > yMax) yMax = ymax;
    }
    void output(TreeSize& treeSize)
    {
        treeSize.xmin = xMin2;
        treeSize.xmax = xMax;
        treeSize.ymax = yMax;
    }
private:
    double xMin = {};
    double xMin2 = {};
    double xMax = {};
    double yMax = {};
};

Helper gHelper;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Layouter implementation.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Layouter::Layouter(double fontSize, double nodeHSep, double nodeVSep)
    :boxy_{new Boxy(fontSize)}, nodeHSep_{nodeHSep}, nodeVSep_{nodeVSep}
{
}

bool Layouter::layout(Node* t, TreeSize& treeSize)
{
    if(!boxy_->good()) return false;
    gHelper.init();

    FirstWalk(t, *boxy_);
    SecondWalk(t, (-1.0) * t->prelim());

    gHelper.output(treeSize);

    return true;
}

void Layouter::FirstWalk(Node *v, Boxy& boxy, Node *leftSibingOfV)
{
    // Initialize node label's textbox.
    v->textBox(boxy.getTextBox(v->label()));

    if (!v->isLeaf())
    {
        auto dac = v->leftMostChild();

        Node *leftSibingOfChild = nullptr;
        for (auto &child : v->childArray())
        {
            FirstWalk(child, boxy, leftSibingOfChild);
            dac = Apportion(child, leftSibingOfChild, dac);
            leftSibingOfChild = child;
        }

        ExecuteShifts(v);

        auto midpoint = (v->leftMostChild()->prelim() 
                        + v->rightMostChild()->prelim()
                        ) * 0.5;

        if (leftSibingOfV)
        {
            v->prelim(leftSibingOfV->prelim() + nodeHSep_);
            //v->prelim(leftSibingOfV->prelim() + Distance(leftSibingOfV));
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
            v->prelim(leftSibingOfV->prelim() + nodeHSep_);
            //v->prelim(leftSibingOfV->prelim() + Distance(leftSibingOfV));
        }
        else
        {
            // v->prelim(0.0f);
        }
    }
}

void Layouter::SecondWalk(Node *v, double m, size_t level)
{
    v->x(v->prelim() + m);
    v->y(level * nodeVSep_);

    gHelper.update(v);

    for (auto &child : v->childArray())
    {
        SecondWalk(child, m + v->mod(), level + 1);
    }
}

Node* Layouter::Apportion(Node *v, Node *leftSibingOfV, Node *dac)
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
        auto LL = RL->parent()->leftMostChild(); // Left most sibling of RL.

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
                       + nodeHSep_;
                    // + Distance(LR);
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

Node* Layouter::NextLeft(Node *v)
{
    if (!v->childArray().empty())
    {
        return v->leftMostChild();
    }
    else
    {
        return v->thread();
    }
}

Node* Layouter::NextRight(Node *v)
{
    if (!v->childArray().empty())
    {
        return v->rightMostChild();
    }
    else
    {
        return v->thread();
    }
}

void Layouter::MoveSubTree(Node *w0, Node *w1, double shift)
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

        auto end = w0->parent()->childArray().end();
        auto left = end;
        auto right = end;
        for (auto it = w0->parent()->childArray().begin(); it != end; ++it)
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

void Layouter::ExecuteShifts(Node *v)
{
    double shift = {};
    double change = {};

    auto & childArray = v->childArray();
    for (auto it = childArray.rbegin(); it != childArray.rend(); ++it)
    {
        (*it)->prelim((*it)->prelim() + shift);
        (*it)->mod((*it)->mod() + shift);
        change += (*it)->change();
        shift += (*it)->shift() + change;
    }
}

Node* Layouter::Ancestor(Node *vi, Node *v, Node *dac)
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

double Layouter::Distance(Node *v)
{
    return nodeHSep_;
    //return v->textBox().width;
    //return v->textBox().width * 0.5 + 10.0f;
    //return v->textBox().width + config::MinHSep;
}