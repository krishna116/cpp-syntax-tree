/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Boxy.h"
#include "CairoContext.h"
#include "SyntaxTree.h"

using namespace cst;

Boxy::Boxy(double fontSize)
{
    ctx_ = std::make_shared<CairoContext>(fontSize);
}

bool Boxy::good()const
{
     return ctx_->good();
}

TextBox Boxy::getTextBox(std::string text)
{
    if(!ctx_->good() || text.empty()) return {};

    TextBox textBox;
    cairo_text_extents_t extents;

    cairo_text_extents(ctx_->cr(), text.c_str(), &extents);

    textBox.width = extents.width;
    textBox.height = extents.height;
    textBox.xBearing = extents.x_bearing;
    textBox.yBearing = extents.y_bearing;

    return textBox;
}

bool Boxy::initTextBox(Node* t)
{
    if(!good()) return false;

    return internalInitTextBox(t);
}

bool Boxy::internalInitTextBox(Node* t)
{
    if(t != nullptr && !t->label().empty())
    {
        for(auto& child: t->childArray())
        {
            if(!internalInitTextBox(child))
            {
                return false;
            }
        }
        t->textBox(getTextBox(t->label()));
        return true;
    }

    return false;
}
