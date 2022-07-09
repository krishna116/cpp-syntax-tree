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
//   Improving Walker's Algorithm to Run in Linear Time
// [Paper-Author]
//   Christoph Buhheim, Michael Jünger, and Sebastian Leipert
//

#pragma once

#include <vector>
#include <string>
#include <cassert>

namespace bjl
{
    namespace config
    {
        // Horizontal separation between two nodes.
        auto constexpr MinHSep = 10.0f;

        // Vertical separation between two nodes.
        auto constexpr MinVSep = 10.0f;
    }

    // Node of tree front declaration.
    class Node;

    /**
     * @brief Layout a tree using algorithm-bjl
     * 
     * @param[in] t     Input a tree to layout.
     */
    void TreeLayout(Node* t);
    
    /**
     * @brief Node of tree definition.
     */
    class Node
    {
    public:
        Node(std::string label, std::vector<Node*> listOfChild = {})
            :label_{label},
            parent_{},
            ancestor_{this},
            thread_{},
            prelim_{},
            mod_{},
            x_{},
            y_{},
            shift_{},
            change_{}
        {
            append(listOfChild);
        }

        bool isLeaf() const
        {
            return childArray_.empty();
        }

        Node* getLeftMostChild() const
        {
            if(!childArray_.empty())
            {
               return childArray_.front();
            }
            
            return {};
        }

        Node* getRightMostChild() const
        {
            if(!childArray_.empty())
            {
                return childArray_.back();
            }

            return {};
        }

        const std::vector<Node*>& getChildArray() const
        {
            return childArray_;
        }

        void append(Node* child)
        {
            assert(child != nullptr);
            child->parent(this);
            childArray_.push_back(child);
        }

        void append(const std::vector<Node*>& listOfChild)
        {
            for(auto& child: listOfChild)
            {
                append(child);
            }
        }

        double prelim() const
        {
            return prelim_;
        }

        void prelim(double val)
        {
            prelim_ = val;
        }

        double mod() const
        {
            return mod_;
        }

        void mod(double val)
        {
            mod_ = val;
        }

        Node* thread() const
        {
            return thread_;
        }

        void thread(Node* v)
        {
            thread_ = v;
        }

        void ancestor(Node* v)
        {
            ancestor_ = v;
        }
        Node* ancestor() const
        {
            return ancestor_;
        }

        void change(double val)
        {
            change_ = val;
        }

        double change() const
        {
            return change_;
        }

        void shift(double val)
        {
            shift_ = val;
        }

        double shift() const
        {
            return shift_;
        }

        void parent(Node* v)
        {
            parent_ = v;
        }
        Node* parent() const
        {
            return parent_;
        }

        void x(double val)
        {
            x_ = val;
        }

        double x() const
        {
            return x_;
        }

        void y(double val)
        {
            y_ = val;
        }

        double y() const
        {
            return y_;
        }

        void label(std::string text)
        {
            label_ = text;
        }

        std::string label()const
        {
            return label_;
        }

    private:
        std::string label_; ///< Node label, used to debug.

        Node* parent_;      ///< Node's parent.
        Node* ancestor_;    ///< Node's ancestor, used by the algorithm.
        Node* thread_;      ///< Node's thread, used by the algorithm.
        std::vector<Node*> childArray_; ///< Node's child array.

        double prelim_;     ///< Prelim x position.
        double mod_;        ///< Modifier or offset of this tree.
        double x_;          ///< Final x position.
        double y_;          ///< Final y position.

        double shift_;      ///< Used by the algorithm.
        double change_;     ///< Used by the algorithm.
    }; // class Node end
} // namespace bjl end
