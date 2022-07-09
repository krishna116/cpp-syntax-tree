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

#include <cassert>
#include <string>

#include <cairo.h>
// #include <cairo-ft.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-svg.h>

namespace cst
{
    /**
     * @brief A cairo drawing context.
     */
    class CairoContext
    {
    public:
        /**
         * @brief Construct a new Cairo Context object
         * 
         * @param[in] width     Page width.
         * @param[in] height    Page height.
         * @param[in] fileType  Specify output file type.
         * @param[in] fileName  Specify output file name.
         * @param[in] fontSize  Font size.
         */
        CairoContext(double width, 
                     double height,
                     std::string fileType, 
                     std::string fileName,
                     double fontSize = option::FontSize::getFontSize());

        /**
         * @brief Construct a new Cairo Context object
         * 
         * @param[in] fontSize  Font size.
         */
        CairoContext(double fontSize = option::FontSize::getFontSize());

        /**
         * @brief Destroy the Cairo Context object
         * 
         */
        ~CairoContext();

        /**
         * @brief Check state is good or not.
         * 
         * @return true     Ok.
         * @return false    Not ok.
         */
        bool good()const;

        /**
         * @brief Get cairo surface used for drawing.
         * 
         * @return Cairo surface.
         */
        cairo_surface_t* const& cs()const;

        /**
         * @brief Get cairo instance used for drawing.
         * 
         * @return Cairo instance.
         */
        cairo_t* const& cr()const;

    private:
        cairo_t* cr_ = nullptr;
        cairo_surface_t* cs_ = nullptr;
        double width_ = {};
        double height_ = {};
        std::string fileType_;
        std::string fileName_;
        double fontSize_ = {};
        bool good_ = false;
        bool init();
    }; // class RenderContext;
} // namespace cst
