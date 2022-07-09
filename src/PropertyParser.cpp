/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "PropertyParser.h"

#include <set>

using namespace cst;

using CharPtr = std::string::const_iterator;

//
// To be done.
// 
// Only support follow properties at the moment.
//
std::set<std::string> defKeySet = 
{
    "label",
    "color"
};

bool IsSpace(int ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '\f');
}

bool IsEq(int ch)
{
    return ch == '=';
}

std::string EatKey(CharPtr& beg, CharPtr end)
{
   std::string key;

    while(beg < end)
    {
        if(0x20 < *beg && *beg < 0x7F && *beg != '"' && !IsSpace(*beg) && !IsEq(*beg))
        {
            key.push_back(*beg);
        }
        else
        {
            break;
        }
        ++beg;
    }

    return key;
}

std::string EatKeyAndMatch(CharPtr& beg, CharPtr end, const std::set<std::string>& keySet)
{
    std::string key = EatKey(beg, end);
    if(!key.empty() && keySet.find(key) != keySet.end())
    {
        return key;
    }
    return {};
}

bool EatEq(CharPtr& beg, CharPtr end)
{
    if(beg < end && IsEq(*beg))
    {
        ++beg;
        return true;
    }
    return false;
}

void EatSpace(CharPtr& beg, CharPtr end)
{
    while(beg < end)
    {
        if(IsSpace(*beg))
        {
            ++beg;
        }
        else
        {
            break;
        }
    }
}

std::string EatValue(CharPtr& beg, CharPtr end)
{
    std::string value;
    if(beg < end && *beg == '"')
    {
        ++beg;
        while(beg < end)
        {
            if(*beg == '\\' && (beg+1) != end)
            {
                value.push_back(*(beg + 1));
                beg += 2;
                continue;
            }
            else if(*beg == '\\' && (beg + 1) == end)
            {
                value.clear();
                break;
            }
            else if(*beg == '"')
            {
                ++beg;
                break;
            }
            else
            {
                value.push_back(*beg);
                ++beg;
            }
        }
    }
    else
    {
        return EatKey(beg, end);
    }

    return value;
}


Property PropertyParser:: toProperty(const std::string& text)
{
    Property prop;

    if(!text.empty())
    {
        auto beg = text.begin();
        auto end = text.end();
        std::string key;
        bool hasEq;
        std::string value;

        while(beg < end)
        {
            EatSpace(beg, end);
            key = EatKeyAndMatch(beg,end, defKeySet);
            EatSpace(beg, end);
            hasEq  = EatEq(beg, end);
            EatSpace(beg, end);
            value = EatValue(beg,end);
            if(!key.empty() && hasEq && !value.empty())
            {
                prop[key] = value;
                continue;
            }
            else
            {
                break;
            }
        }
    }

    return prop;
}