/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Lexer.h"

using namespace cst;

Lexer::Lexer(uint8_t* buf, std::size_t bufSize)
    :buf(buf),
    end(buf+bufSize),
    cursor(buf),
    marker(buf),
    cppRawBegin(nullptr),
    cppRawEnd(nullptr),
    currentTokenType(TokenType::Eof)
{
}

Lexer::TokenType Lexer::getNextTokenType()
{
    marker = cursor;

    while(cursor<end)
    {
        if(isLeftSquare(*cursor))
        {
            ++cursor;
            currentTokenType = TokenType::LeftSquare;
            return currentTokenType;
        }
        else if(isRightSquare(*cursor))
        {
            ++cursor;
            currentTokenType = TokenType::RightSquare;
            return currentTokenType;
        }
        else if(isSpace(*cursor))
        {
            ++cursor;
            while(cursor<end && isSpace(*cursor))
            {
                ++cursor;
            }
            marker = cursor;
            continue;
        }
        else if(isNonControlSpaceSquare(*cursor))
        {
            auto backup = cursor;

            // Try to eat C++ raw string.
            if(*cursor == 'R' && (cursor + 1) < end && *(cursor + 1) == '"')
            {
                if(eatCppRawString())
                {
                    currentTokenType = TokenType::CppRawString;
                    return currentTokenType;
                }
                else
                {
                    currentTokenType = TokenType::Error;
                    return currentTokenType;
                }
            }
            
            // Fall back to eat basic string.
            cursor = backup;
            ++cursor;
            while(cursor<end && isNonControlSpaceSquare(*cursor))
            {
                ++cursor;
            }
            currentTokenType = TokenType::BasicString;
            return currentTokenType;
        }
        else
        {
            currentTokenType = TokenType::Error;
            return currentTokenType;
        }
    }

    currentTokenType = TokenType::Eof;
    return currentTokenType;
}

Lexer::TokenType Lexer::getCurrentTokenType()
{
    return currentTokenType;
}

std::string Lexer::getCurrentToken()
{
    if(currentTokenType == TokenType::CppRawString)
    {
        return std::string((char*)cppRawBegin, cppRawEnd - cppRawBegin);
    }
    else
    {
        return std::string((char*)marker, cursor - marker);
    }
}

std::size_t Lexer::getCursorOffset()
{
    return this->cursor - this->buf;
}

bool Lexer::isLeftSquare(uint8_t ch)
{
    return ch == '[';
}

bool Lexer::isRightSquare(uint8_t ch)
{
    return ch == ']';
}

bool Lexer::isNonControlSpaceSquare(uint8_t ch)
{
    static const bool nonControlSpaceSquare[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0
    };
    return nonControlSpaceSquare[ch];
}

bool Lexer::isSpace(uint8_t ch)
{
    static const bool spaceCharArray[256] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    return spaceCharArray[ch];
}

bool Lexer::eatCppRawString()
{
    // 
    // R"~~~~~~~~~~~~(............)~~~~~~~~~~~~"
    //   ^           ^             ^           ^
    //   1           2             3           4
    //
    uint8_t* prefixBegin = nullptr;     // 1
    uint8_t* prefixEnd = nullptr;       // 2
    uint8_t* suffixBegin = nullptr;     // 3
    uint8_t* suffixEnd = nullptr;       // 4

    auto isSamePrefixAndSuffix = [&]()
    {
        if(prefixEnd-prefixBegin == suffixEnd - suffixBegin)
        {
            auto it1 = prefixBegin;
            auto it2 = suffixBegin;
            while(it1 < prefixEnd && *it1 == *it2 && *it1 == *prefixBegin)
            {
                ++it1;
                ++it2;
            }
            if (it1 == prefixEnd && it2 == suffixEnd)
            {
                // Snapshot the raw string data.
                cppRawBegin = prefixEnd + 1;
                cppRawEnd = suffixBegin - 1;
                return true;
            }
        }

        return false;
    };

    auto findPrefix = [&]()
    {
        cursor += 2;    // Eat [R"]
        prefixBegin = cursor;
        while(cursor < end && *cursor != '(')
        {
            ++cursor;
        }
        if(cursor < end && *cursor == '(')
        {
            prefixEnd = cursor++;
            return true;
        }

        return false;
    };

    auto findSuffix = [&]()
    {
        while(cursor < end && *cursor != ')')
        {
            ++cursor;
        }
        if(cursor < end && *cursor == ')')
        {
            suffixBegin = ++cursor;
        }
        else
        {
            return false;
        }

        while(cursor < end && *cursor != '"')
        {
            ++cursor;
        }
        if(cursor < end && *cursor == '"')
        {
            suffixEnd = cursor++;
            return true;
        }

        return false;
    };

    //
    // Working start here.
    //
    if(findPrefix())
    {
        while(cursor<end)
        {
            if(findSuffix() && isSamePrefixAndSuffix())
            {
                return true;
            }
        }
    }

    return false;
}
