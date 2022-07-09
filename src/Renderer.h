/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#pragma once

#include "SyntaxTree.h"

#include <string>
#include <memory>

namespace cst
{
    using SyntaxTreePtr = std::shared_ptr<SyntaxTree>;
    class CairoContext;
    using CairoContextPtr = std::shared_ptr<CairoContext>;

    /**
     * @brief Syntax tree renderer.
     */
    class Renderer
    {
    public:
        Renderer(SyntaxTreePtr pSyntaxTree,
                TreeSize treeSize,
                std::string fileName,
                std::string fileType = option::FileType::getFileType(), 
                double fontSize = option::FontSize::getFontSize(),
                double pageMarginW =option::PageMargin::getPageMarginW(),
                double pageMarginH =option::PageMargin::getPageMarginH()
                );
        
        /**
         * @brief Drawing(rendering) the tree.
         * 
         * @return true     Pass.
         * @return false    Fail.
         */
        bool drawTree();

    private:
        SyntaxTreePtr tree_;
        TreeSize treeSize_;
        CairoContextPtr ctx_;
        std::string fileType_;
        std::string fileName_;
        double fontSize_;
        double pageMarginW_;
        double pageMarginH_;

        int init(const std::string &fileType,const std::string &fileName);
        void internalDrawTree();
        void fillPage();
        void drawNode(Node* n);
        void drawBox(Node* n);
        void drawText(Node* n);
        void drawEdge(Node* n);
        void saveFile();
        double cx(Node* n);
        double cy(Node* n);
        Box getPage();

        struct Color
        {
            Color()=default;
            Color(double r, double g, double b):red(r), green(g), blue(b){}
            double red = {};
            double green = {};
            double blue = {};
        };

        static const std::map<std::string, Color> x11ColorMap;
    };
} // namespace cst
