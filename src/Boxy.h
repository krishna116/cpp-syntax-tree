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
#include <memory>

namespace cst
{
    class CairoContext;
    using CairoContextPtr = std::shared_ptr<CairoContext>;
    class Node;

    /**
     * @brief Used to calculate string's textbox for layouting.
     */
    class Boxy
    {
    public:
        /**
         * @brief Construct a new Boxy object
         * 
         * @param[in] fontSize  Font size used to calculate textbox.
         */
        Boxy(double fontSize = option::FontSize::getFontSize());
        /**
         * @brief Check this object state is ok or not.
         * 
         * It can be used before do the calculating.
         * 
         * @return true     Ok.
         * @return false    Not ok.
         */
        bool good()const;
        /**
         * @brief Calculate text's textbox.
         * 
         * @param[in] text      Input string.
         * 
         * @return TextBox      Output the string's textbox.
         */
        TextBox getTextBox(std::string text);
        /**
         * @brief Init a three's all node's label's textbox.
         * 
         * @param[in] t     Input a tree.
         * @return true     Work pass.
         * @return false    Work fail.
         */
        bool initTextBox(Node* t);

    private:
        CairoContextPtr ctx_;
        bool internalInitTextBox(Node* t);
    };
}
