/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

namespace cst
{
    template<typename T1, typename T2, typename T3>
    bool ValueBetween(T1 value, T2 min, T3 max)
    {
        return min <= value && value <= max;
    }

    namespace option
    {
        class NodeSep{
        public:
            static constexpr double defHSep = 25.0f; // Default horizontal separation.
            static constexpr double defVSep = 25.0f; // Default vertical separation.
            static constexpr double valueMin = 1.0f;
            static constexpr double valueMax = 300.0f;
            static bool isValid(double nodeSep);
            static double getHSep();
            static void setHSep(double value);
            static double getVSep();
            static void setVSep(double value);
        private:
            static double hSep_;
            static double vSep_;
        };

        class FontSize{
        public:
            static constexpr double defValue = 12.0f; // Default value.
            static constexpr double valueMin = 1.0f;
            static constexpr double valueMax = 100.0f;
            static bool isValid(double fontSize);
            static double getFontSize();
            static void setFontSize(double value);
        private:
            static double fontSize_;
        };
    
        auto constexpr defEmptyLabel = "<empty>";

        class PageMargin{
        public:
            static constexpr double defValue = 20.0f; // Default value.
            static constexpr double valueMin = 1.0f;
            static constexpr double valueMax = 300.0f;
            static bool isValid(double pageMarginValue);
            static double getPageMarginW();
            static double getPageMarginH();
            static void setPageMarginW(double value);
            static void setPageMarginH(double value);
        private:
            static double pageMarginW_;
            static double pageMarginH_;
        };

        class FileType{
        public:
            static std::string getDefFileType();
            static const std::vector<std::string>& getAll();
            static bool isValid(const std::string& fileType);
            static std::string getFileType();
            static void setFileType(std::string fileType);
        private:
            static std::string fileType_;
        };
    } // Common options end.

    class Node;
    using NodeArray = std::vector<Node*>;
    
    //
    // A rectangle.
    //
    struct Box
    {
        double width = {};
        double height = {};
    };

    //
    // Tree size.
    //
    struct TreeSize
    {
        double xmin = {};   ///< Most left x position.
        double xmax = {};   ///< Most right x position.
        double ymax = {};   ///< Most bottom y position.
    };
    
    //
    // A string's shape infomation for layouter and renderer.
    //
    struct TextBox
    {
        double width = {};
        double height = {};
        double xBearing = {};
        double yBearing = {};
    };

    //
    // Used by layout algorithm.
    // 
    struct Layout
    {
        Node* ancestor = {};
        Node* thread = {};
        double prelim = {};
        double mod = {};
        double x = {};
        double y = {};
        double shift = {};
        double change = {};
    };

    //
    // Used by renderer.
    //
    using Property = std::map<std::string, std::string>;

    //
    // Node data.
    //
    struct NodeData
    {
        std::size_t id = {};
        std::string label;
        std::string cppRawStr;
        TextBox textBox;
        Layout layout;
        Property prop;
    };

    /**
     * @brief Syntax tree node definition.
     */
    class Node
    {
    public:
        Node(const std::string& label, const NodeArray& childArray = {})
        {
            initNodeLabel(label);
            parent(nullptr);
            ancestor(this);
            append(childArray);
        }

        void parent(Node* value)
        {
            parent_ = value;
        }

        Node* parent() const
        {
            return parent_;
        }

        void append(Node* child)
        {
            //assert(child);
            child->parent(this);
            childArray_.push_back(child);
        }

        void append(const NodeArray& childArray)
        {
            for(auto& child : childArray)
            {
                append(child);
            }
        }

        const NodeArray& childArray() const
        {
            return childArray_;
        }

        bool isLeaf()const
        {
            return childArray_.empty();
        }

        void id(std::size_t value)
        {
            data_.id = value;
        }

        std::size_t id() const
        {
            return data_.id;
        }

        const std::string& label()const
        {
            return data_.label;
        }

        void label(std::string str)
        {
            data_.label = std::move(str);
        }

        const std::string& cppRawStr()const
        {
            return data_.cppRawStr;
        }

        void cppRawStr(std::string str)
        {
            data_.cppRawStr = std::move(str);
        }

        const Property& property() const
        {
            return data_.prop;
        }

        void property(Property prop)
        {
            data_.prop = std::move(prop);
        }

        void textBox(TextBox textBox)
        {
            data_.textBox = std::move(textBox);
        }

        const TextBox& textBox()const
        {
            return data_.textBox;
        }

        Node* leftMostChild() const
        {
            if(!childArray_.empty())
            {
               return childArray_.front();
            }
            
            return {};
        }

        Node* rightMostChild() const
        {
            if(!childArray_.empty())
            {
                return childArray_.back();
            }

            return {};
        }

        void prelim(double val)
        {
            data_.layout.prelim = val;
        }

        double prelim() const
        {
            return data_.layout.prelim;
        }

        void mod(double val)
        {
            data_.layout.mod = val;
        }

        double mod() const
        {
            return data_.layout.mod;
        }

        void thread(Node* v)
        {
            data_.layout.thread = v;
        }

        Node* thread() const
        {
            return data_.layout.thread;
        }

        void ancestor(Node* v)
        {
            data_.layout.ancestor = v;
        }

        Node* ancestor() const
        {
            return data_.layout.ancestor;
        }

        void change(double val)
        {
            data_.layout.change = val;
        }

        double change() const
        {
            return data_.layout.change;
        }

        void shift(double val)
        {
            data_.layout.shift = val;
        }

        double shift() const
        {
            return data_.layout.shift;
        }

        void x(double val)
        {
            data_.layout.x = val;
        }

        double x() const
        {
            return data_.layout.x;
        }

        void y(double val)
        {
            data_.layout.y = val;
        }

        double y() const
        {
            return data_.layout.y;
        }

    private:
        Node* parent_;
        NodeArray childArray_;  
        NodeData data_;
        void initNodeLabel(const std::string& label)
        {
            if(label.empty())
            {
                data_.label = option::defEmptyLabel;
            }
            else
            {
                data_.label = label;
            }
        }
    };

} // namespace cst
