/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Parser.h"
#include "PropertyParser.h"
#include "Lexer.h"
#include "SyntaxTree.h"

#include <iostream>
#include <stdexcept>

using namespace cst;

using TokenType = Lexer::TokenType;

//
//            +-----------------+
//            |                 |
//            V                 |
// (begin)------>( [ )----->( Label )--------------->( ] )---->(finished)
//                              |                      ^
//                              |                      |
//                              +------>( Label )------+
//
Node *buildSubStree(Lexer &lexer, Node *parent)
{
    Node *pNode;
    auto tokenType = lexer.getCurrentTokenType();

    auto collectProperty = [](Node *pNode, const TokenType& tokenType, const std::string& text)
    {
        if(tokenType == TokenType::CppRawString)
        {
            auto prop = PropertyParser::toProperty(text);
            auto labelFromProp = prop["label"];
            if(!labelFromProp.empty())
            {
                pNode->label(labelFromProp);
            }
            else
            {
                pNode->label(option::defEmptyLabel);
            }
            pNode->cppRawStr(text);
            pNode->property(prop);
        }
        return pNode;
    };

    // Begin.
    if (tokenType == TokenType::LeftSquare)
    {
        tokenType = lexer.getNextTokenType();
        if (tokenType == TokenType::CppRawString || tokenType == TokenType::BasicString)
        {
            // Current node label
            pNode = SyntaxTree::newNode(lexer.getCurrentToken());
            collectProperty(pNode, tokenType, lexer.getCurrentToken());

            // Zero or more child list.
            while (true)
            {
                tokenType = lexer.getNextTokenType();
                if (tokenType == TokenType::BasicString || tokenType == TokenType::CppRawString)
                {
                    // One child label.
                    auto child = SyntaxTree::newNode(lexer.getCurrentToken());
                    collectProperty(child, tokenType, lexer.getCurrentToken());
                    pNode->append(child);
                }
                else if (tokenType == TokenType::LeftSquare)
                {
                    // One child node.
                    pNode->append(buildSubStree(lexer, pNode));
                }
                else if (tokenType == TokenType::RightSquare)
                {
                    // Fininshed.
                    return pNode;
                }
                else
                {
                    // Any error.
                    break;
                }
            }
        }
    }

    std::string msg;
    msg += "Parser::buildSubStree failed.\n";
    msg += "lexer::cursor offset = ";
    msg += std::to_string(lexer.getCursorOffset()) + ".\n";
    throw std::runtime_error(msg.c_str());

    return {};
}

SyntaxTreePtr Parser::buildSyntaxTree(const std::string &stream)
{
    if (stream.empty())
        return {};

    try
    {
        Lexer lexer((uint8_t *)stream.data(), stream.size());

        lexer.getNextTokenType();
        auto rootNodeLabel = lexer.getCurrentToken();

        auto treeRoot = buildSubStree(lexer, nullptr);

        if (lexer.getNextTokenType() == Lexer::TokenType::Eof)
        {
            auto syntaxTree = std::make_shared<SyntaxTree>();
            syntaxTree->setRoot(treeRoot);
            return syntaxTree;
        }
        else
        {
            std::string msg;
            msg += "Parser::buildSyntaxTree failed.\n";
            msg += "lexer::cursor offset = ";
            msg += std::to_string(lexer.getCursorOffset()) + ".\n";
            throw std::runtime_error(msg.c_str());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return {};
}
