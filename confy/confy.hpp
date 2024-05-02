#if 0 // Do not delete this block! License Check!
/**
 * MIT License
 * 
 * Copyright (c) 2024 mauro 🤙
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
 * Source Code At: https://github.com/mauro-balades/Confy 
 * Have fun! 🤙
 */
/**
 * Notices: We require the c++ standard 17 or higher. 
 *   We make use of smart pointers and other modern c++ features.
 * You can define the following macros to add some custom behavior:
 *   - CONFY_NO_EXCEPTIONS: If you define this macro, we will not use exceptions.
 *   - CONFY_NO_USING_NAMESPACE: If you define this macro, we will not use the using namespace confy. (not recommended)
 *   - CONFY_JUST_DECLARATIONS: If you define this macro, we will not define the implementation of the functions.
 */

#endif // Finish License Check!

#ifndef __CONFY_MAIN_H__
#define __CONFY_MAIN_H__

#include <string>
#include <vector>
#include <memory>

#ifndef CONFY_NO_EXCEPTIONS
#include <stdexcept>
#endif

#ifndef CONFY_NO_USING_NAMESPACE
using namespace confy;
#endif

namespace confy {

/**
 * @brief The main class to manage the configuration root interface.
 *
 * This class is the main class to manage the configuration root interface.
 * Root interfaces are used to create a schema for the configuration.
 * If the parser finds a key that is not in the schema, it will throw an exception.
 *
 * Example:
 * @code
 * // Create a root interface
 * auto root = confy::RootInterface::create({
 *     {"key1", confy::Type::String},
 * });
 */
class Interface {
};

} // namespace confy

#endif // __CONFY_MAIN_H__
