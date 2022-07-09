/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "BaseType.h"

using namespace cst;
using namespace cst::option;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// NodeSep
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double NodeSep::hSep_ = NodeSep::defHSep;
double NodeSep::vSep_ = NodeSep::defVSep;
bool NodeSep::isValid(double nodeSep)
{
    return ValueBetween(nodeSep, valueMin, valueMax);
}

double NodeSep::getHSep()
{
    return hSep_;
}

void NodeSep::setHSep(double value)
{
    if(isValid(value)) hSep_ = value;
}

double NodeSep::getVSep()
{
    return vSep_;
}

void NodeSep::setVSep(double value)
{
    if(isValid(value)) vSep_ = value;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FontSize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double FontSize::fontSize_ = FontSize::defValue;
bool FontSize::isValid(double fontSize)
{
    return ValueBetween(fontSize, valueMin, valueMax);
}

double FontSize::getFontSize()
{
    return fontSize_;
}

void FontSize::setFontSize(double value)
{
    if(isValid(value)) fontSize_ = value;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PageMargin
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double PageMargin::pageMarginW_ = PageMargin::defValue;
double PageMargin::pageMarginH_ = PageMargin::defValue;
bool PageMargin::isValid(double pageMarginValue)
{
    return ValueBetween(pageMarginValue, valueMin, valueMax);
}

double PageMargin::getPageMarginH()
{
    return pageMarginH_;
}

double PageMargin::getPageMarginW()
{
    return pageMarginW_;
}

void PageMargin::setPageMarginW(double value)
{
    if(isValid(value)) pageMarginW_ = value;
}

void PageMargin::setPageMarginH(double value)
{
    if(isValid(value)) pageMarginH_ = value;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// FileType
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
std::string FileType::fileType_ = FileType::getDefFileType();
std::string FileType::getDefFileType()
{
    return "pdf";
}

const std::vector<std::string>& FileType::getAll()
{
    static const std::vector<std::string> types =
    {
        "pdf", "svg", "png", "dot"
    };
    return types;
}

bool FileType::isValid(const std::string& fileType)
{
    for(auto& type : getAll())
    {
        if(type == fileType) return true;
    }
    return false;
}

std::string FileType::getFileType()
{
    return fileType_;
}   

void FileType::setFileType(std::string fileType)
{
    if(isValid(fileType)) fileType_ = fileType;
}
