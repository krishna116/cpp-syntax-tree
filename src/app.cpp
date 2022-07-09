/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "config.h"
#include "Parser.h"
#include "Layouter.h"
#include "Renderer.h"

#include <iostream>
#include <stdexcept>
#include <fstream>
#include <string>

using namespace cst;

/**
 * @brief Get dot file stream from tree node.
 *
 * @param[in] node      A tree node.
 * @param[in] stream    Output stream.
 */
void GetDotStreamHelper(Node *node, std::string &stream)
{
    static const std::string indent(4, ' ');
    static const std::string indent2(8, ' ');

    std::string nodeId = std::to_string(node->id());
    stream += indent + nodeId;
    if (node->cppRawStr().empty())
    {
        stream += "[label = \"" + node->label() + "\"];\n";
    }
    else
    {
        stream += "[" + node->cppRawStr() + "];\n";
    }

    for (auto const &child : node->childArray())
    {
        stream += indent2 + nodeId + " -> " + std::to_string(child->id()) + ";\n";
    }

    for (auto const &child : node->childArray())
    {
        GetDotStreamHelper(child, stream);
    }
}

/**
 * @brief Get the dot file stream from tree.
 *
 * @param[in] tree      Tree root.
 * @return std::string  File stream.
 */
std::string GetDotStream(Node *tree)
{
    std::string s = R"(//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Please use dot(https://www.graphviz.org/) to draw this file to image.
// 
// The drawing command:
//   dot -Tpdf <file.dot> -o <file.pdf>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

digraph cpp_syntax_tree {
    node[ordering = out];

)";

    GetDotStreamHelper(tree, s);
    s += "}\n";

    return s;
}

/**
 * @brief Save stream to file.
 *
 * @param[in] stream    Stream to be saved.
 * @param[in] fileName  The output file name.
 */
void SaveStreamToFile(const std::string &stream, const std::string &fileName)
{
    std::ofstream ofs(fileName);
    if (!ofs)
        throw std::runtime_error("Cannot write file => " + fileName);
    ofs << stream;
    ofs.flush();
    ofs.close();
}

/**
 * @brief Draw tree from iFile and save result to oFile.
 *
 * @param[in] iFile     Specify input file name.
 * @param[in] oFile     Speicfy output file name.
 *
 * @return 0            Work pass.
 * @return other        Work fail.
 */
int DoWork(std::string iFile, std::string oFile)
{
    if (iFile.empty())
        return 0;
    if (oFile.empty())
        oFile = iFile + "." + option::FileType::getFileType();

    try
    {
        std::ifstream ifs(iFile);
        if (!ifs)
            throw std::runtime_error("Cannot read file => " + iFile);

        std::istreambuf_iterator<char> begin(ifs);
        std::istreambuf_iterator<char> end;
        std::string stream(begin, end);
        if (stream.empty())
        {
            throw std::runtime_error("Read empty file => " + iFile);
        }

        auto tree = Parser::buildSyntaxTree(stream);
        if (tree == nullptr)
        {
            throw std::runtime_error("Parser::buildSyntaxTree failed");
        }

        Layouter layouter;
        TreeSize treeSize;
        if (!layouter.layout(tree->getRoot(), treeSize))
        {
            throw std::runtime_error("layouter.layout tree failed.");
        }

        if (option::FileType::getFileType() == "dot")
        {
            SaveStreamToFile(GetDotStream(tree->getRoot()), oFile);
        }
        else
        {
            Renderer renderer(tree, treeSize, oFile);
            if (!renderer.drawTree())
            {
                throw std::runtime_error("renderer.drawTree failed.");
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cout << "[cpp-syntax-tree]\n";
        std::cout << "[error] " << e.what() << std::endl;
        return 1;
    }

    std::cout << "[cpp-syntax-tree]\n";
    std::cout << "[input ] " << iFile << std::endl;
    std::cout << "[output] " << oFile << std::endl;

    return 0;
}

/**
 * @brief Show application help.
 *
 * @return 0
 */
int ShowHelp()
{
    std::cout << AppInfo::HelpStr << std::endl;
    return 0;
}

/**
 * @brief Show application version.
 *
 * @return 0
 */
int ShowVersion()
{
    std::cout << AppInfo::VersionStr << std::endl;
    return 0;
}

/**
 * @brief Parse user input args and do the work.
 *
 * @param argc  Argument count.
 * @param argv  Argument array.
 *
 * @return 0        Pass.
 * @return other    Fail.
 */
int ParseArgs(int argc, char *argv[])
{
    if (argc < 2)
        return ShowHelp();

    std::string in;
    std::string out;
    int i = 1;
    bool good = true;
    std::cout.precision(2);
    std::cout << std::fixed;
    while (i < argc)
    {
        if ((std::string("-t") == argv[i] 
            || std::string("--type") == argv[i]) 
            && (i + 1) < argc)
        {
            good = option::FileType::isValid(argv[i + 1]);
            if (!good)
            {
                std::cout << "Invalid file type"
                          << ", the valid file type is [";
                bool first = true;
                for(auto& type: option::FileType::getAll())
                {
                    if(first)
                    {
                        std::cout << type;
                        first = false;
                    }
                    else
                    {
                        std::cout << ", ";
                        std::cout << type;
                    }
                }
                std::cout << "]\n";

                return 1;
            }
            option::FileType::setFileType(argv[i + 1]);
            i += 2;
        }
        else if ((std::string("-o") == argv[i] 
                || std::string("--output") == argv[i]) 
                && (i + 1) < argc)
        {
            out = argv[i + 1];
            i += 2;
        }
        else if (std::string("--hns") == argv[i] && (i + 1) < argc)
        {
            auto number = std::atof(argv[i + 1]);
            good = option::NodeSep::isValid(number);
            if (!good)
            {
                std::cout << "Invalid horizontal node separation"
                          << ", the valid value range is ["
                          << option::NodeSep::valueMin
                          << ", "
                          << option::NodeSep::valueMax
                          <<"]\n";
                return 1;
            }
            option::NodeSep::setHSep(number);
            i += 2;
        }
        else if (std::string("--vns") == argv[i] && (i + 1) < argc)
        {
            auto number = std::atof(argv[i + 1]);
            good = option::NodeSep::isValid(number);
            if (!good)
            {
                std::cout << "Invalid vertical node separation"
                          << ", the valid value range is ["
                          << option::NodeSep::valueMin
                          << ", "
                          << option::NodeSep::valueMax
                          <<"]\n";
                return 1;
            }
            option::NodeSep::setVSep(number);
            i += 2;
        }
        else if (std::string("--pmw") == argv[i] && (i + 1) < argc)
        {
            auto number = std::atof(argv[i + 1]);
            good = option::PageMargin::isValid(number);
            if (!good)
            {
                std::cout << "Invalid page margin width"
                          << ", the valid value range is ["
                          << option::PageMargin::valueMin
                          << ", "
                          << option::PageMargin::valueMax
                          <<"]\n";
                return 1;
            }
            option::PageMargin::setPageMarginW(number);
            i += 2;
        }
        else if (std::string("--pmh") == argv[i] && (i + 1) < argc)
        {
            auto number = std::atof(argv[i + 1]);
            good = option::PageMargin::isValid(number);
            if (!good)
            {
                std::cout << "Invalid page margin height"
                          << ", the valid value range is ["
                          << option::PageMargin::valueMin
                          << ", "
                          << option::PageMargin::valueMax
                          <<"]\n";
                return 1;
            }
            option::PageMargin::setPageMarginH(number);
            i += 2;
        }
        else if (std::string("--fts") == argv[i] && (i + 1) < argc)
        {
            auto number = std::atof(argv[i + 1]);
            good = option::FontSize::isValid(number);
            if (!good)
            {
                std::cout << "Invalid font size"
                          << ", the valid value range is ["
                          << option::FontSize::valueMin
                          << ", "
                          << option::FontSize::valueMax
                          <<"]\n";
                return 1;
            }
            option::FontSize::setFontSize(number);
            i += 2;
        }
        else if (std::string("-h") == argv[i] 
                || std::string("--help") == argv[i])
        {
            return ShowHelp();
        }
        else if (std::string("-v") == argv[i] 
                || std::string("--version") == argv[i])
        {
            return ShowVersion();
        }
        else if (argv[i][0] != '-' && in.empty())
        {
            in = argv[i];
            i += 1;
        }
        else
        {
            good = false;
            break;
        }
    } // while end

    if (!good || in.empty())
    {
        std::cout << "Invalid parameter.\n";
        return 1;
    }

    return DoWork(in, out);
}

int main(int argc, char *argv[])
{
    return ParseArgs(argc, argv);
}
