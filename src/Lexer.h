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

namespace cst
{
    /**
     * @brief Parse stream to tokens.
     */
    class Lexer
    {
    public:
        /**
         * @brief Lexer token type.
         */
        enum class TokenType : int
        {
            Error = -1,
            Eof = 0,        ///< End of stream.
            LeftSquare,     ///< "["
            RightSquare,    ///< "]"
            BasicString,    ///< Node label.
            CppRawString    ///< Node label of C++ raw string like R"(...)"
        };

        /**
         * @brief Init a lexer with a stream buffer.
         * 
         * @param buf       ///< Buffer begin.
         * @param bufSize   ///< Buffer size.
         */
        Lexer(uint8_t *buf, std::size_t bufSize);

        /**
         * @brief Parse the stream and return next token type.
         * 
         * @return TokenType    Next token type.
         */
        TokenType getNextTokenType();

        /**
         * @brief Get the Current Token type.
         * 
         * @return TokenType    Current token type.
         */
        TokenType getCurrentTokenType();
        
        /**
         * @brief Get the Current Token.
         * 
         * @return std::string  Current token.
         */
        std::string getCurrentToken();

        /**
         * @brief Get the cursor offset from stream begin.
         * 
         * It is used to debug.
         * 
         * @return std::size_t  Cursor offset.
         */
        std::size_t getCursorOffset();

    private:
        uint8_t *buf;    ///< Buffer iterator Begin.
        uint8_t *end;    ///< Buffer iterator end.
        uint8_t *marker; ///< Current token begin.
        uint8_t *cursor; ///< Next Token begin.

        uint8_t *cppRawBegin; ///< Current cpp raw string begin.
        uint8_t *cppRawEnd;   ///< Current cpp raw string end.

        TokenType currentTokenType; ///< Current token type.

        /**
         * @brief Checks whether ch is a '[' character.
         * 
         * @param ch        Character to be checked.
         * @return true     It is left square.
         * @return false    It is not left square.
         */
        inline bool isLeftSquare(uint8_t ch);

        /**
         * @brief Checks whether ch is a ']' character.
         * 
         * @param ch        Character to be checked.
         * @return true     It is right square.
         * @return false    It is not right square.
         */
        inline bool isRightSquare(uint8_t ch);

        /**
         * @brief Checks whether ch is a control/space/square character.
         * 
         * @param ch        Character to be checked.
         * @return true     It is the type of char.
         * @return false    It is not the type of char.
         */
        inline bool isNonControlSpaceSquare(uint8_t ch);

        /**
         * @brief Checks whether ch is a space character.
         * 
         * @param ch        Character to be checked.
         * @return true     It is space character.
         * @return false    It is not space character.
         */
        inline bool isSpace(uint8_t ch);

        /**
         * @brief Eat the C++ raw string from cursor position.
         * 
         * If the token is not C++ raw string, return false.
         * 
         * @return true     Eat pass.
         * @return false    Eat fail.
         */
        inline bool eatCppRawString();
    };
}
