/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "CairoContext.h"

using namespace cst;

CairoContext::CairoContext(double width,
                           double height,
                           std::string fileType,
                           std::string fileName,
                           double fontSize)
    : width_{width},
      height_{height},
      fileType_{fileType},
      fileName_{fileName},
      fontSize_{fontSize}
{
    good_ = init();
}

CairoContext::CairoContext(double fontSize)
    : width_{10.0f},
      height_{10.0f},
      fileType_{"png"},
      fontSize_(fontSize)
{
    good_ = init();
}

bool CairoContext::good() const
{
    return good_;
}

cairo_surface_t *const &CairoContext::cs() const
{
    return cs_;
}

cairo_t *const &CairoContext::cr() const
{
    return cr_;
}

CairoContext::~CairoContext()
{
    if (cr_)
    {
        cairo_destroy(cr_);
    }

    if (cs_)
    {
        cairo_surface_destroy(cs_);
    }
}

bool CairoContext::init()
{
    if (width_ <= 1.0f || height_ <= 1.0f)
    {
        return false;
    }

    if (fileType_ == "pdf")
    {
        if (fileName_.empty())
        {
            return false;
        }
        cs_ = cairo_pdf_surface_create(fileName_.c_str(), width_, height_);
    }
    else if (fileType_ == "svg")
    {
        if (fileName_.empty())
        {
            return false;
        }
        cs_ = cairo_svg_surface_create(fileName_.c_str(), width_, height_);
    }
    else if (fileType_ == "png")
    {
        // May not need fileName_.
        cs_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width_, height_);
    }
    else
    {
        return false;
    }

    if (cs_ == nullptr)
    {
        return false;
    }

    cr_ = cairo_create(cs_);
    if (cr_ == nullptr)
    {
        return false;
    }

    cairo_set_font_size(cr_, fontSize_);
    cairo_set_line_width(cr_, 0.5f);
    return true;
}