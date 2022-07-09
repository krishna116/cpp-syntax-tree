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
#include <memory>

namespace cst
{
    class SyntaxTree;
    using SyntaxTreePtr = std::shared_ptr<SyntaxTree>;

    /**
     * @brief Parse stream and return a syntax tree.
     */
    class Parser
    {
    public:
        /**
         * @brief Parse stream and return a syntax tree.
         * 
         * @param[in] stream        Stream to be parsed.
         * 
         * @return SyntaxTreePtr    A syntax tree.
         */
        static SyntaxTreePtr buildSyntaxTree(const std::string &stream);
    };
}//namespace cst
