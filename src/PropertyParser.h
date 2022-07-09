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

namespace cst
{
    /**
     * @brief Parse properties in the C++ raw string of a node label.
     * 
     * The property format is something like this:
     *   key1 = "value1" key2 = "value2" key3 = "value3" ...
     * 
     * The key ought to be a string like C language variable name.
     * The value ought to be a string like C language string.
     */
    class PropertyParser
    {
    public:
        /**
         * @brief Parse properties in the C++ raw string of a node label.
         * 
         * @param[in] text      The text in the C++ raw string: R"(...text...)"
         * @return Property     Key-value pairs.
         */
        static Property toProperty(const std::string& text);
    };
} // namespace cst
