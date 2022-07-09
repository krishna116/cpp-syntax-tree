/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

/**
 * @file ctpp.cpp
 * 
 * @brief Convert x11-color-names to cpp data structure.
 * 
 * The input should be a dictionary file like this:
 * 
 * 255 250 250 snow
 * 248 248 255 ghost white
 *
 */

#include <iostream>
#include <regex>
#include <string>
#include <sstream>
#include <fstream>
#include <set>

extern const char *x11_colors;

struct ColorInfo
{
    std::string name;
    std::string red;
    std::string green;
    std::string blue;

    bool operator<(const ColorInfo &other) const
    {
        return name < other.name;
    }
};

void PrintColorInfoSet(const std::set<ColorInfo> &colorInfoSet, std::string &result, bool usingUint8 = true)
{
    static const char *prefixUint8 = R"(struct Color
{
    Color()=default;
    Color(uint8_t r, uint8_t g, uint8_t b):red(r), green(g), blue(b){}
    uint8_t red = {};
    uint8_t green = {};
    uint8_t blue = {};
};

)";
    static const char *prefixDouble = R"(struct Color
{
    Color()=default;
    Color(double r, double g, double b):red(r), green(g), blue(b){}
    double red = {};
    double green = {};
    double blue = {};
};
)";

    auto data = [&](const std::string &c)
    {
        if (!usingUint8)
        {
            return std::to_string(std::atoi(c.c_str()) / 255.0f);
        }
        return c;
    };

    usingUint8 ? (result += prefixUint8) : (result += prefixDouble);
    result += "std::map<std::string, Color> x11ColorMap = {";

    for (auto &cf : colorInfoSet)
    {
        result += "    {\"" + cf.name + "\", {";
        result += data(cf.red) + ", ";
        result += data(cf.green) + ", ";
        result += data(cf.blue) + "}},\n";
    }

    if (!result.empty())
        result.pop_back(); // Pop last '\n'
    if (!result.empty())
        result.pop_back(); // Pop last ','

    result += "\n};\n";
}

void GetColorInfoSet(const std::regex &re, const std::string &line, std::set<ColorInfo> &colorInfoSet)
{
    std::smatch match;
    if (std::regex_match(line, match, re))
    {
        ColorInfo cf;
        cf.name = match[4].str();
        cf.red = match[1].str();
        cf.green = match[2].str();
        cf.blue = match[3].str();
        colorInfoSet.insert(cf);
    }
}

/*
Convert x11 color to cpp data structure file.
*/
void ctpp(bool usingUint8, std::string fileName)
{
    //std::stringstream ss(x11_colors);
    std::string result;
    std::string line;
    std::set<ColorInfo> colorInfoSet;
    
    std::fstream ss(fileName);
    if(!ss)
    {
        std::cout << "Cannot read file => " << fileName << std::endl;
        return;
    }

    std::regex re("([0-9]+)[ ]+([0-9]+)[ ]+([0-9]+)[ ]+([a-zA-Z0-9]+[ ]*[a-zA-Z0-9]+)");
    while (std::getline(ss, line))
    {
        GetColorInfoSet(re, line, colorInfoSet);
    }

    PrintColorInfoSet(colorInfoSet, result, usingUint8);

    std::cout << result;
}

int main(int argc, char *argv[])
{
    if(argc == 3 && std::string("--double") == argv[1])
    {
        ctpp(false, argv[2]);    
    }
    else if(argc == 3 && std::string("--uint8") == argv[1])
    {
        ctpp(true, argv[2]);
    }
    else
    {
        std::cout << R"(ctpp <option> <file>

Convert x11-color-names to cpp data structure.

Options:
  --double  using double data type.
  --uint8   using uint8_t data type.
)";
    }

    return 0;
}
