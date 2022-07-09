/**
 * The MIT License
 *
 * Copyright 2022 Krishna sssky307@163.com
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

#include "Renderer.h"
#include "CairoContext.h"
#include "config.h"

#include <cassert>

using namespace cst;

Renderer::Renderer(SyntaxTreePtr pSyntaxTree, 
                    TreeSize treeSize,
                    std::string fileName,
                    std::string fileType, 
                    double fontSize,
                    double pageMarginW,
                    double pageMarginH)
    :tree_(pSyntaxTree),
    treeSize_(treeSize),
    fileType_(fileType),
    fileName_(fileName),
    fontSize_(fontSize),
    pageMarginW_(pageMarginW),
    pageMarginH_(pageMarginH)
{
}

bool Renderer::drawTree()
{
    assert(tree_ != nullptr);

    auto page = getPage();

    ctx_ = std::make_shared<CairoContext>(page.width,
                                           page.height,
                                           fileType_,
                                           fileName_,
                                           fontSize_);
    if(!ctx_->good()) return false;

    internalDrawTree();
    saveFile();

    return true;
}

void Renderer::internalDrawTree()
{
    fillPage();
    drawNode(tree_->getRoot());
}

void Renderer::fillPage()
{
    // To be done.
}

void Renderer::drawNode(Node *n)
{
    drawEdge(n);
    drawBox(n);
    drawText(n);

    for (auto child : n->childArray())
    {
        drawNode(child);
    }
}

void Renderer::drawEdge(Node *n)
{
    // Edge is from this node to parent node.
    if (n->parent() == nullptr)
    {
        return;
    }
    cairo_move_to(ctx_->cr(), 
                    cx(n), 
                    cy(n) - fontSize_ * 0.45
                    );
    cairo_line_to(ctx_->cr(), 
                    cx(n->parent()), 
                    cy(n->parent()) + fontSize_ * 0.45
                    );

    cairo_set_source_rgba(ctx_->cr(), 0.0, 0.0, 0.0, 0.85);
    cairo_stroke(ctx_->cr());
}

void Renderer::drawBox(Node *n)
{   
    //cairo_rectangle(ctx_->cr(), cx(n), cy(n), n->textBox().width, n->textBox().height);
}

void Renderer::drawText(Node *n)
{
    auto getTextColor = [&]()->Color
    {
        if(!n->property().empty())
        {
            auto it = n->property().find("color");
            if(it != n->property().end())
            {
                auto it2 = Renderer::x11ColorMap.find((*it).second);
                if(it2 !=  Renderer::x11ColorMap.end())
                {
                    return (*it2).second;
                }
            }
        }
        return {};
    };

    Color c = getTextColor();
    cairo_set_source_rgba(ctx_->cr(), c.red, c.green, c.blue, 1.0);
    cairo_move_to(ctx_->cr(), 
                cx(n) - n->textBox().width * 0.5 + n->textBox().xBearing, 
                cy(n) - n->textBox().height * 0.5 - n->textBox().yBearing
                );
    cairo_show_text(ctx_->cr(), n->label().c_str());
}

void Renderer::saveFile()
{
    cairo_show_page(ctx_->cr());
    if (fileType_ == "png" && !fileName_.empty())
    {
        cairo_surface_write_to_png(ctx_->cs(), fileName_.c_str());
    }
}

double Renderer::cx(Node* n)
{
    return n->x() + pageMarginW_ - treeSize_.xmin;
}

double Renderer::cy(Node* n)
{
    return n->y() + pageMarginH_;
}

Box Renderer::getPage()
{
    assert(tree_ != nullptr);

    Box box;
    box.width = pageMarginW_ * 2.0 + treeSize_.xmax - treeSize_.xmin;
    box.height = pageMarginH_ *2.0 + treeSize_.ymax;

    return box;
}

const std::map<std::string, Renderer::Color> Renderer::x11ColorMap = {
    {"AliceBlue", {0.941176, 0.972549, 1.000000}},
    {"AntiqueWhite", {0.980392, 0.921569, 0.843137}},
    {"AntiqueWhite1", {1.000000, 0.937255, 0.858824}},
    {"AntiqueWhite2", {0.933333, 0.874510, 0.800000}},
    {"AntiqueWhite3", {0.803922, 0.752941, 0.690196}},
    {"AntiqueWhite4", {0.545098, 0.513726, 0.470588}},
    {"BlanchedAlmond", {1.000000, 0.921569, 0.803922}},
    {"BlueViolet", {0.541176, 0.168627, 0.886275}},
    {"CadetBlue1", {0.596078, 0.960784, 1.000000}},
    {"CadetBlue2", {0.556863, 0.898039, 0.933333}},
    {"CadetBlue3", {0.478431, 0.772549, 0.803922}},
    {"CornflowerBlue", {0.392157, 0.584314, 0.929412}},
    {"DarkBlue", {0.000000, 0.000000, 0.545098}},
    {"DarkCyan", {0.000000, 0.545098, 0.545098}},
    {"DarkGoldenrod", {0.721569, 0.525490, 0.043137}},
    {"DarkGoldenrod1", {1.000000, 0.725490, 0.058824}},
    {"DarkGoldenrod2", {0.933333, 0.678431, 0.054902}},
    {"DarkGoldenrod3", {0.803922, 0.584314, 0.047059}},
    {"DarkGoldenrod4", {0.545098, 0.396078, 0.031373}},
    {"DarkGray", {0.662745, 0.662745, 0.662745}},
    {"DarkGrey", {0.662745, 0.662745, 0.662745}},
    {"DarkKhaki", {0.741176, 0.717647, 0.419608}},
    {"DarkMagenta", {0.545098, 0.000000, 0.545098}},
    {"DarkOliveGreen1", {0.792157, 1.000000, 0.439216}},
    {"DarkOliveGreen2", {0.737255, 0.933333, 0.407843}},
    {"DarkOliveGreen3", {0.635294, 0.803922, 0.352941}},
    {"DarkOliveGreen4", {0.431373, 0.545098, 0.239216}},
    {"DarkOrange", {1.000000, 0.549020, 0.000000}},
    {"DarkOrange1", {1.000000, 0.498039, 0.000000}},
    {"DarkOrange2", {0.933333, 0.462745, 0.000000}},
    {"DarkOrange3", {0.803922, 0.400000, 0.000000}},
    {"DarkOrange4", {0.545098, 0.270588, 0.000000}},
    {"DarkOrchid", {0.600000, 0.196078, 0.800000}},
    {"DarkOrchid1", {0.749020, 0.243137, 1.000000}},
    {"DarkOrchid2", {0.698039, 0.227451, 0.933333}},
    {"DarkOrchid3", {0.603922, 0.196078, 0.803922}},
    {"DarkOrchid4", {0.407843, 0.133333, 0.545098}},
    {"DarkRed", {0.545098, 0.000000, 0.000000}},
    {"DarkSalmon", {0.913725, 0.588235, 0.478431}},
    {"DarkSeaGreen", {0.560784, 0.737255, 0.560784}},
    {"DarkSeaGreen1", {0.756863, 1.000000, 0.756863}},
    {"DarkSeaGreen2", {0.705882, 0.933333, 0.705882}},
    {"DarkSeaGreen3", {0.607843, 0.803922, 0.607843}},
    {"DarkSeaGreen4", {0.411765, 0.545098, 0.411765}},
    {"DarkSlateGray1", {0.592157, 1.000000, 1.000000}},
    {"DarkSlateGray2", {0.552941, 0.933333, 0.933333}},
    {"DarkSlateGray3", {0.474510, 0.803922, 0.803922}},
    {"DarkViolet", {0.580392, 0.000000, 0.827451}},
    {"DeepPink", {1.000000, 0.078431, 0.576471}},
    {"DeepPink1", {1.000000, 0.078431, 0.576471}},
    {"DeepPink2", {0.933333, 0.070588, 0.537255}},
    {"DeepPink3", {0.803922, 0.062745, 0.462745}},
    {"DeepPink4", {0.545098, 0.039216, 0.313726}},
    {"DimGray", {0.411765, 0.411765, 0.411765}},
    {"DimGrey", {0.411765, 0.411765, 0.411765}},
    {"FloralWhite", {1.000000, 0.980392, 0.941176}},
    {"GhostWhite", {0.972549, 0.972549, 1.000000}},
    {"GreenYellow", {0.678431, 1.000000, 0.184314}},
    {"HotPink", {1.000000, 0.411765, 0.705882}},
    {"HotPink1", {1.000000, 0.431373, 0.705882}},
    {"HotPink2", {0.933333, 0.415686, 0.654902}},
    {"HotPink3", {0.803922, 0.376471, 0.564706}},
    {"HotPink4", {0.545098, 0.227451, 0.384314}},
    {"IndianRed", {0.803922, 0.360784, 0.360784}},
    {"IndianRed1", {1.000000, 0.415686, 0.415686}},
    {"IndianRed2", {0.933333, 0.388235, 0.388235}},
    {"IndianRed3", {0.803922, 0.333333, 0.333333}},
    {"IndianRed4", {0.545098, 0.227451, 0.227451}},
    {"LavenderBlush", {1.000000, 0.941176, 0.960784}},
    {"LavenderBlush1", {1.000000, 0.941176, 0.960784}},
    {"LavenderBlush2", {0.933333, 0.878431, 0.898039}},
    {"LavenderBlush3", {0.803922, 0.756863, 0.772549}},
    {"LavenderBlush4", {0.545098, 0.513726, 0.525490}},
    {"LawnGreen", {0.486275, 0.988235, 0.000000}},
    {"LemonChiffon", {1.000000, 0.980392, 0.803922}},
    {"LemonChiffon1", {1.000000, 0.980392, 0.803922}},
    {"LemonChiffon2", {0.933333, 0.913725, 0.749020}},
    {"LemonChiffon3", {0.803922, 0.788235, 0.647059}},
    {"LemonChiffon4", {0.545098, 0.537255, 0.439216}},
    {"LightBlue", {0.678431, 0.847059, 0.901961}},
    {"LightBlue1", {0.749020, 0.937255, 1.000000}},
    {"LightBlue2", {0.698039, 0.874510, 0.933333}},
    {"LightBlue3", {0.603922, 0.752941, 0.803922}},
    {"LightBlue4", {0.407843, 0.513726, 0.545098}},
    {"LightCoral", {0.941176, 0.501961, 0.501961}},
    {"LightCyan", {0.878431, 1.000000, 1.000000}},
    {"LightCyan1", {0.878431, 1.000000, 1.000000}},
    {"LightCyan2", {0.819608, 0.933333, 0.933333}},
    {"LightCyan3", {0.705882, 0.803922, 0.803922}},
    {"LightCyan4", {0.478431, 0.545098, 0.545098}},
    {"LightGoldenrod", {0.933333, 0.866667, 0.509804}},
    {"LightGoldenrod1", {1.000000, 0.925490, 0.545098}},
    {"LightGoldenrod2", {0.933333, 0.862745, 0.509804}},
    {"LightGoldenrod3", {0.803922, 0.745098, 0.439216}},
    {"LightGoldenrod4", {0.545098, 0.505882, 0.298039}},
    {"LightGoldenrodYellow", {0.980392, 0.980392, 0.823529}},
    {"LightGray", {0.827451, 0.827451, 0.827451}},
    {"LightGreen", {0.564706, 0.933333, 0.564706}},
    {"LightGrey", {0.827451, 0.827451, 0.827451}},
    {"LightPink", {1.000000, 0.713726, 0.756863}},
    {"LightPink1", {1.000000, 0.682353, 0.725490}},
    {"LightPink2", {0.933333, 0.635294, 0.678431}},
    {"LightPink3", {0.803922, 0.549020, 0.584314}},
    {"LightPink4", {0.545098, 0.372549, 0.396078}},
    {"LightSalmon", {1.000000, 0.627451, 0.478431}},
    {"LightSalmon1", {1.000000, 0.627451, 0.478431}},
    {"LightSalmon2", {0.933333, 0.584314, 0.447059}},
    {"LightSalmon3", {0.803922, 0.505882, 0.384314}},
    {"LightSalmon4", {0.545098, 0.341176, 0.258824}},
    {"LightSkyBlue", {0.529412, 0.807843, 0.980392}},
    {"LightSkyBlue1", {0.690196, 0.886275, 1.000000}},
    {"LightSkyBlue2", {0.643137, 0.827451, 0.933333}},
    {"LightSkyBlue3", {0.552941, 0.713726, 0.803922}},
    {"LightSlateBlue", {0.517647, 0.439216, 1.000000}},
    {"LightSlateGray", {0.466667, 0.533333, 0.600000}},
    {"LightSlateGrey", {0.466667, 0.533333, 0.600000}},
    {"LightSteelBlue", {0.690196, 0.768627, 0.870588}},
    {"LightSteelBlue1", {0.792157, 0.882353, 1.000000}},
    {"LightSteelBlue2", {0.737255, 0.823529, 0.933333}},
    {"LightSteelBlue3", {0.635294, 0.709804, 0.803922}},
    {"LightSteelBlue4", {0.431373, 0.482353, 0.545098}},
    {"LightYellow", {1.000000, 1.000000, 0.878431}},
    {"LightYellow1", {1.000000, 1.000000, 0.878431}},
    {"LightYellow2", {0.933333, 0.933333, 0.819608}},
    {"LightYellow3", {0.803922, 0.803922, 0.705882}},
    {"LightYellow4", {0.545098, 0.545098, 0.478431}},
    {"MediumAquamarine", {0.400000, 0.803922, 0.666667}},
    {"MediumOrchid", {0.729412, 0.333333, 0.827451}},
    {"MediumOrchid1", {0.878431, 0.400000, 1.000000}},
    {"MediumOrchid2", {0.819608, 0.372549, 0.933333}},
    {"MediumOrchid3", {0.705882, 0.321569, 0.803922}},
    {"MediumOrchid4", {0.478431, 0.215686, 0.545098}},
    {"MediumPurple", {0.576471, 0.439216, 0.858824}},
    {"MediumPurple1", {0.670588, 0.509804, 1.000000}},
    {"MediumPurple2", {0.623529, 0.474510, 0.933333}},
    {"MediumPurple3", {0.537255, 0.407843, 0.803922}},
    {"MediumSlateBlue", {0.482353, 0.407843, 0.933333}},
    {"MediumVioletRed", {0.780392, 0.082353, 0.521569}},
    {"MintCream", {0.960784, 1.000000, 0.980392}},
    {"MistyRose", {1.000000, 0.894118, 0.882353}},
    {"MistyRose1", {1.000000, 0.894118, 0.882353}},
    {"MistyRose2", {0.933333, 0.835294, 0.823529}},
    {"MistyRose3", {0.803922, 0.717647, 0.709804}},
    {"MistyRose4", {0.545098, 0.490196, 0.482353}},
    {"NavajoWhite", {1.000000, 0.870588, 0.678431}},
    {"NavajoWhite1", {1.000000, 0.870588, 0.678431}},
    {"NavajoWhite2", {0.933333, 0.811765, 0.631373}},
    {"NavajoWhite3", {0.803922, 0.701961, 0.545098}},
    {"NavajoWhite4", {0.545098, 0.474510, 0.368627}},
    {"OldLace", {0.992157, 0.960784, 0.901961}},
    {"OliveDrab", {0.419608, 0.556863, 0.137255}},
    {"OliveDrab1", {0.752941, 1.000000, 0.243137}},
    {"OliveDrab2", {0.701961, 0.933333, 0.227451}},
    {"OliveDrab3", {0.603922, 0.803922, 0.196078}},
    {"OliveDrab4", {0.411765, 0.545098, 0.133333}},
    {"OrangeRed", {1.000000, 0.270588, 0.000000}},
    {"OrangeRed1", {1.000000, 0.270588, 0.000000}},
    {"OrangeRed2", {0.933333, 0.250980, 0.000000}},
    {"OrangeRed3", {0.803922, 0.215686, 0.000000}},
    {"OrangeRed4", {0.545098, 0.145098, 0.000000}},
    {"PaleGoldenrod", {0.933333, 0.909804, 0.666667}},
    {"PaleGreen", {0.596078, 0.984314, 0.596078}},
    {"PaleGreen1", {0.603922, 1.000000, 0.603922}},
    {"PaleGreen2", {0.564706, 0.933333, 0.564706}},
    {"PaleGreen3", {0.486275, 0.803922, 0.486275}},
    {"PaleTurquoise", {0.686275, 0.933333, 0.933333}},
    {"PaleTurquoise1", {0.733333, 1.000000, 1.000000}},
    {"PaleTurquoise2", {0.682353, 0.933333, 0.933333}},
    {"PaleTurquoise3", {0.588235, 0.803922, 0.803922}},
    {"PaleTurquoise4", {0.400000, 0.545098, 0.545098}},
    {"PaleVioletRed", {0.858824, 0.439216, 0.576471}},
    {"PaleVioletRed1", {1.000000, 0.509804, 0.670588}},
    {"PaleVioletRed2", {0.933333, 0.474510, 0.623529}},
    {"PaleVioletRed3", {0.803922, 0.407843, 0.537255}},
    {"PaleVioletRed4", {0.545098, 0.278431, 0.364706}},
    {"PapayaWhip", {1.000000, 0.937255, 0.835294}},
    {"PeachPuff", {1.000000, 0.854902, 0.725490}},
    {"PeachPuff1", {1.000000, 0.854902, 0.725490}},
    {"PeachPuff2", {0.933333, 0.796078, 0.678431}},
    {"PeachPuff3", {0.803922, 0.686275, 0.584314}},
    {"PeachPuff4", {0.545098, 0.466667, 0.396078}},
    {"PowderBlue", {0.690196, 0.878431, 0.901961}},
    {"RebeccaPurple", {0.400000, 0.200000, 0.600000}},
    {"RosyBrown", {0.737255, 0.560784, 0.560784}},
    {"RosyBrown1", {1.000000, 0.756863, 0.756863}},
    {"RosyBrown2", {0.933333, 0.705882, 0.705882}},
    {"RosyBrown3", {0.803922, 0.607843, 0.607843}},
    {"RosyBrown4", {0.545098, 0.411765, 0.411765}},
    {"SaddleBrown", {0.545098, 0.270588, 0.074510}},
    {"SandyBrown", {0.956863, 0.643137, 0.376471}},
    {"SkyBlue", {0.529412, 0.807843, 0.921569}},
    {"SkyBlue1", {0.529412, 0.807843, 1.000000}},
    {"SkyBlue2", {0.494118, 0.752941, 0.933333}},
    {"SkyBlue3", {0.423529, 0.650980, 0.803922}},
    {"SlateBlue", {0.415686, 0.352941, 0.803922}},
    {"SlateBlue1", {0.513726, 0.435294, 1.000000}},
    {"SlateBlue2", {0.478431, 0.403922, 0.933333}},
    {"SlateBlue3", {0.411765, 0.349020, 0.803922}},
    {"SlateGray", {0.439216, 0.501961, 0.564706}},
    {"SlateGray1", {0.776471, 0.886275, 1.000000}},
    {"SlateGray2", {0.725490, 0.827451, 0.933333}},
    {"SlateGray3", {0.623529, 0.713726, 0.803922}},
    {"SlateGray4", {0.423529, 0.482353, 0.545098}},
    {"SlateGrey", {0.439216, 0.501961, 0.564706}},
    {"VioletRed", {0.815686, 0.125490, 0.564706}},
    {"VioletRed1", {1.000000, 0.243137, 0.588235}},
    {"VioletRed2", {0.933333, 0.227451, 0.549020}},
    {"VioletRed3", {0.803922, 0.196078, 0.470588}},
    {"VioletRed4", {0.545098, 0.133333, 0.321569}},
    {"WebGray", {0.501961, 0.501961, 0.501961}},
    {"WebGrey", {0.501961, 0.501961, 0.501961}},
    {"WebMaroon", {0.501961, 0.000000, 0.000000}},
    {"WebPurple", {0.501961, 0.000000, 0.501961}},
    {"WhiteSmoke", {0.960784, 0.960784, 0.960784}},
    {"X11Gray", {0.745098, 0.745098, 0.745098}},
    {"X11Grey", {0.745098, 0.745098, 0.745098}},
    {"X11Maroon", {0.690196, 0.188235, 0.376471}},
    {"X11Purple", {0.627451, 0.125490, 0.941176}},
    {"YellowGreen", {0.603922, 0.803922, 0.196078}},
    {"alice blue", {0.941176, 0.972549, 1.000000}},
    {"antique white", {0.980392, 0.921569, 0.843137}},
    {"aquamarine", {0.498039, 1.000000, 0.831373}},
    {"aquamarine1", {0.498039, 1.000000, 0.831373}},
    {"aquamarine2", {0.462745, 0.933333, 0.776471}},
    {"aquamarine3", {0.400000, 0.803922, 0.666667}},
    {"azure", {0.941176, 1.000000, 1.000000}},
    {"azure1", {0.941176, 1.000000, 1.000000}},
    {"azure2", {0.878431, 0.933333, 0.933333}},
    {"azure3", {0.756863, 0.803922, 0.803922}},
    {"azure4", {0.513726, 0.545098, 0.545098}},
    {"beige", {0.960784, 0.960784, 0.862745}},
    {"bisque", {1.000000, 0.894118, 0.768627}},
    {"bisque1", {1.000000, 0.894118, 0.768627}},
    {"bisque2", {0.933333, 0.835294, 0.717647}},
    {"bisque3", {0.803922, 0.717647, 0.619608}},
    {"bisque4", {0.545098, 0.490196, 0.419608}},
    {"blanched almond", {1.000000, 0.921569, 0.803922}},
    {"blue violet", {0.541176, 0.168627, 0.886275}},
    {"brown", {0.647059, 0.164706, 0.164706}},
    {"brown1", {1.000000, 0.250980, 0.250980}},
    {"brown2", {0.933333, 0.231373, 0.231373}},
    {"brown3", {0.803922, 0.200000, 0.200000}},
    {"brown4", {0.545098, 0.137255, 0.137255}},
    {"burlywood", {0.870588, 0.721569, 0.529412}},
    {"burlywood1", {1.000000, 0.827451, 0.607843}},
    {"burlywood2", {0.933333, 0.772549, 0.568627}},
    {"burlywood3", {0.803922, 0.666667, 0.490196}},
    {"burlywood4", {0.545098, 0.450980, 0.333333}},
    {"chartreuse", {0.498039, 1.000000, 0.000000}},
    {"chartreuse1", {0.498039, 1.000000, 0.000000}},
    {"chartreuse2", {0.462745, 0.933333, 0.000000}},
    {"chartreuse3", {0.400000, 0.803922, 0.000000}},
    {"chocolate", {0.823529, 0.411765, 0.117647}},
    {"chocolate1", {1.000000, 0.498039, 0.141176}},
    {"chocolate2", {0.933333, 0.462745, 0.129412}},
    {"chocolate3", {0.803922, 0.400000, 0.113725}},
    {"chocolate4", {0.545098, 0.270588, 0.074510}},
    {"coral", {1.000000, 0.498039, 0.313726}},
    {"coral1", {1.000000, 0.447059, 0.337255}},
    {"coral2", {0.933333, 0.415686, 0.313726}},
    {"coral3", {0.803922, 0.356863, 0.270588}},
    {"coral4", {0.545098, 0.243137, 0.184314}},
    {"cornflower blue", {0.392157, 0.584314, 0.929412}},
    {"cornsilk", {1.000000, 0.972549, 0.862745}},
    {"cornsilk1", {1.000000, 0.972549, 0.862745}},
    {"cornsilk2", {0.933333, 0.909804, 0.803922}},
    {"cornsilk3", {0.803922, 0.784314, 0.694118}},
    {"cornsilk4", {0.545098, 0.533333, 0.470588}},
    {"crimson", {0.862745, 0.078431, 0.235294}},
    {"dark blue", {0.000000, 0.000000, 0.545098}},
    {"dark cyan", {0.000000, 0.545098, 0.545098}},
    {"dark goldenrod", {0.721569, 0.525490, 0.043137}},
    {"dark gray", {0.662745, 0.662745, 0.662745}},
    {"dark grey", {0.662745, 0.662745, 0.662745}},
    {"dark khaki", {0.741176, 0.717647, 0.419608}},
    {"dark magenta", {0.545098, 0.000000, 0.545098}},
    {"dark orange", {1.000000, 0.549020, 0.000000}},
    {"dark orchid", {0.600000, 0.196078, 0.800000}},
    {"dark red", {0.545098, 0.000000, 0.000000}},
    {"dark salmon", {0.913725, 0.588235, 0.478431}},
    {"dark violet", {0.580392, 0.000000, 0.827451}},
    {"deep pink", {1.000000, 0.078431, 0.576471}},
    {"dim gray", {0.411765, 0.411765, 0.411765}},
    {"dim grey", {0.411765, 0.411765, 0.411765}},
    {"firebrick", {0.698039, 0.133333, 0.133333}},
    {"firebrick1", {1.000000, 0.188235, 0.188235}},
    {"firebrick2", {0.933333, 0.172549, 0.172549}},
    {"firebrick3", {0.803922, 0.149020, 0.149020}},
    {"firebrick4", {0.545098, 0.101961, 0.101961}},
    {"floral white", {1.000000, 0.980392, 0.941176}},
    {"fuchsia", {1.000000, 0.000000, 1.000000}},
    {"gainsboro", {0.862745, 0.862745, 0.862745}},
    {"ghost white", {0.972549, 0.972549, 1.000000}},
    {"gold", {1.000000, 0.843137, 0.000000}},
    {"gold1", {1.000000, 0.843137, 0.000000}},
    {"gold2", {0.933333, 0.788235, 0.000000}},
    {"gold3", {0.803922, 0.678431, 0.000000}},
    {"gold4", {0.545098, 0.458824, 0.000000}},
    {"goldenrod", {0.854902, 0.647059, 0.125490}},
    {"goldenrod1", {1.000000, 0.756863, 0.145098}},
    {"goldenrod2", {0.933333, 0.705882, 0.133333}},
    {"goldenrod3", {0.803922, 0.607843, 0.113725}},
    {"goldenrod4", {0.545098, 0.411765, 0.078431}},
    {"gray", {0.745098, 0.745098, 0.745098}},
    {"gray100", {1.000000, 1.000000, 1.000000}},
    {"gray40", {0.400000, 0.400000, 0.400000}},
    {"gray41", {0.411765, 0.411765, 0.411765}},
    {"gray42", {0.419608, 0.419608, 0.419608}},
    {"gray43", {0.431373, 0.431373, 0.431373}},
    {"gray44", {0.439216, 0.439216, 0.439216}},
    {"gray45", {0.450980, 0.450980, 0.450980}},
    {"gray46", {0.458824, 0.458824, 0.458824}},
    {"gray47", {0.470588, 0.470588, 0.470588}},
    {"gray48", {0.478431, 0.478431, 0.478431}},
    {"gray49", {0.490196, 0.490196, 0.490196}},
    {"gray50", {0.498039, 0.498039, 0.498039}},
    {"gray51", {0.509804, 0.509804, 0.509804}},
    {"gray52", {0.521569, 0.521569, 0.521569}},
    {"gray53", {0.529412, 0.529412, 0.529412}},
    {"gray54", {0.541176, 0.541176, 0.541176}},
    {"gray55", {0.549020, 0.549020, 0.549020}},
    {"gray56", {0.560784, 0.560784, 0.560784}},
    {"gray57", {0.568627, 0.568627, 0.568627}},
    {"gray58", {0.580392, 0.580392, 0.580392}},
    {"gray59", {0.588235, 0.588235, 0.588235}},
    {"gray60", {0.600000, 0.600000, 0.600000}},
    {"gray61", {0.611765, 0.611765, 0.611765}},
    {"gray62", {0.619608, 0.619608, 0.619608}},
    {"gray63", {0.631373, 0.631373, 0.631373}},
    {"gray64", {0.639216, 0.639216, 0.639216}},
    {"gray65", {0.650980, 0.650980, 0.650980}},
    {"gray66", {0.658824, 0.658824, 0.658824}},
    {"gray67", {0.670588, 0.670588, 0.670588}},
    {"gray68", {0.678431, 0.678431, 0.678431}},
    {"gray69", {0.690196, 0.690196, 0.690196}},
    {"gray70", {0.701961, 0.701961, 0.701961}},
    {"gray71", {0.709804, 0.709804, 0.709804}},
    {"gray72", {0.721569, 0.721569, 0.721569}},
    {"gray73", {0.729412, 0.729412, 0.729412}},
    {"gray74", {0.741176, 0.741176, 0.741176}},
    {"gray75", {0.749020, 0.749020, 0.749020}},
    {"gray76", {0.760784, 0.760784, 0.760784}},
    {"gray77", {0.768627, 0.768627, 0.768627}},
    {"gray78", {0.780392, 0.780392, 0.780392}},
    {"gray79", {0.788235, 0.788235, 0.788235}},
    {"gray80", {0.800000, 0.800000, 0.800000}},
    {"gray81", {0.811765, 0.811765, 0.811765}},
    {"gray82", {0.819608, 0.819608, 0.819608}},
    {"gray83", {0.831373, 0.831373, 0.831373}},
    {"gray84", {0.839216, 0.839216, 0.839216}},
    {"gray85", {0.850980, 0.850980, 0.850980}},
    {"gray86", {0.858824, 0.858824, 0.858824}},
    {"gray87", {0.870588, 0.870588, 0.870588}},
    {"gray88", {0.878431, 0.878431, 0.878431}},
    {"gray89", {0.890196, 0.890196, 0.890196}},
    {"gray90", {0.898039, 0.898039, 0.898039}},
    {"gray91", {0.909804, 0.909804, 0.909804}},
    {"gray92", {0.921569, 0.921569, 0.921569}},
    {"gray93", {0.929412, 0.929412, 0.929412}},
    {"gray94", {0.941176, 0.941176, 0.941176}},
    {"gray95", {0.949020, 0.949020, 0.949020}},
    {"gray96", {0.960784, 0.960784, 0.960784}},
    {"gray97", {0.968627, 0.968627, 0.968627}},
    {"gray98", {0.980392, 0.980392, 0.980392}},
    {"gray99", {0.988235, 0.988235, 0.988235}},
    {"green yellow", {0.678431, 1.000000, 0.184314}},
    {"green", {0.000000, 1.000000, 0.000000}},
    {"grey", {0.745098, 0.745098, 0.745098}},
    {"grey100", {1.000000, 1.000000, 1.000000}},
    {"grey40", {0.400000, 0.400000, 0.400000}},
    {"grey41", {0.411765, 0.411765, 0.411765}},
    {"grey42", {0.419608, 0.419608, 0.419608}},
    {"grey43", {0.431373, 0.431373, 0.431373}},
    {"grey44", {0.439216, 0.439216, 0.439216}},
    {"grey45", {0.450980, 0.450980, 0.450980}},
    {"grey46", {0.458824, 0.458824, 0.458824}},
    {"grey47", {0.470588, 0.470588, 0.470588}},
    {"grey48", {0.478431, 0.478431, 0.478431}},
    {"grey49", {0.490196, 0.490196, 0.490196}},
    {"grey50", {0.498039, 0.498039, 0.498039}},
    {"grey51", {0.509804, 0.509804, 0.509804}},
    {"grey52", {0.521569, 0.521569, 0.521569}},
    {"grey53", {0.529412, 0.529412, 0.529412}},
    {"grey54", {0.541176, 0.541176, 0.541176}},
    {"grey55", {0.549020, 0.549020, 0.549020}},
    {"grey56", {0.560784, 0.560784, 0.560784}},
    {"grey57", {0.568627, 0.568627, 0.568627}},
    {"grey58", {0.580392, 0.580392, 0.580392}},
    {"grey59", {0.588235, 0.588235, 0.588235}},
    {"grey60", {0.600000, 0.600000, 0.600000}},
    {"grey61", {0.611765, 0.611765, 0.611765}},
    {"grey62", {0.619608, 0.619608, 0.619608}},
    {"grey63", {0.631373, 0.631373, 0.631373}},
    {"grey64", {0.639216, 0.639216, 0.639216}},
    {"grey65", {0.650980, 0.650980, 0.650980}},
    {"grey66", {0.658824, 0.658824, 0.658824}},
    {"grey67", {0.670588, 0.670588, 0.670588}},
    {"grey68", {0.678431, 0.678431, 0.678431}},
    {"grey69", {0.690196, 0.690196, 0.690196}},
    {"grey70", {0.701961, 0.701961, 0.701961}},
    {"grey71", {0.709804, 0.709804, 0.709804}},
    {"grey72", {0.721569, 0.721569, 0.721569}},
    {"grey73", {0.729412, 0.729412, 0.729412}},
    {"grey74", {0.741176, 0.741176, 0.741176}},
    {"grey75", {0.749020, 0.749020, 0.749020}},
    {"grey76", {0.760784, 0.760784, 0.760784}},
    {"grey77", {0.768627, 0.768627, 0.768627}},
    {"grey78", {0.780392, 0.780392, 0.780392}},
    {"grey79", {0.788235, 0.788235, 0.788235}},
    {"grey80", {0.800000, 0.800000, 0.800000}},
    {"grey81", {0.811765, 0.811765, 0.811765}},
    {"grey82", {0.819608, 0.819608, 0.819608}},
    {"grey83", {0.831373, 0.831373, 0.831373}},
    {"grey84", {0.839216, 0.839216, 0.839216}},
    {"grey85", {0.850980, 0.850980, 0.850980}},
    {"grey86", {0.858824, 0.858824, 0.858824}},
    {"grey87", {0.870588, 0.870588, 0.870588}},
    {"grey88", {0.878431, 0.878431, 0.878431}},
    {"grey89", {0.890196, 0.890196, 0.890196}},
    {"grey90", {0.898039, 0.898039, 0.898039}},
    {"grey91", {0.909804, 0.909804, 0.909804}},
    {"grey92", {0.921569, 0.921569, 0.921569}},
    {"grey93", {0.929412, 0.929412, 0.929412}},
    {"grey94", {0.941176, 0.941176, 0.941176}},
    {"grey95", {0.949020, 0.949020, 0.949020}},
    {"grey96", {0.960784, 0.960784, 0.960784}},
    {"grey97", {0.968627, 0.968627, 0.968627}},
    {"grey98", {0.980392, 0.980392, 0.980392}},
    {"grey99", {0.988235, 0.988235, 0.988235}},
    {"honeydew", {0.941176, 1.000000, 0.941176}},
    {"honeydew1", {0.941176, 1.000000, 0.941176}},
    {"honeydew2", {0.878431, 0.933333, 0.878431}},
    {"honeydew3", {0.756863, 0.803922, 0.756863}},
    {"honeydew4", {0.513726, 0.545098, 0.513726}},
    {"hot pink", {1.000000, 0.411765, 0.705882}},
    {"indian red", {0.803922, 0.360784, 0.360784}},
    {"ivory", {1.000000, 1.000000, 0.941176}},
    {"ivory1", {1.000000, 1.000000, 0.941176}},
    {"ivory2", {0.933333, 0.933333, 0.878431}},
    {"ivory3", {0.803922, 0.803922, 0.756863}},
    {"ivory4", {0.545098, 0.545098, 0.513726}},
    {"khaki", {0.941176, 0.901961, 0.549020}},
    {"khaki1", {1.000000, 0.964706, 0.560784}},
    {"khaki2", {0.933333, 0.901961, 0.521569}},
    {"khaki3", {0.803922, 0.776471, 0.450980}},
    {"khaki4", {0.545098, 0.525490, 0.305882}},
    {"lavender", {0.901961, 0.901961, 0.980392}},
    {"lavender blush", {1.000000, 0.941176, 0.960784}},
    {"lawn green", {0.486275, 0.988235, 0.000000}},
    {"lemon chiffon", {1.000000, 0.980392, 0.803922}},
    {"blue", {0.000000, 0.000000, 1.000000}},
    {"light blue", {0.678431, 0.847059, 0.901961}},
    {"light coral", {0.941176, 0.501961, 0.501961}},
    {"light cyan", {0.878431, 1.000000, 1.000000}},
    {"light goldenrod", {0.933333, 0.866667, 0.509804}},
    {"light gray", {0.827451, 0.827451, 0.827451}},
    {"light green", {0.564706, 0.933333, 0.564706}},
    {"light grey", {0.827451, 0.827451, 0.827451}},
    {"light pink", {1.000000, 0.713726, 0.756863}},
    {"light salmon", {1.000000, 0.627451, 0.478431}},
    {"light yellow", {1.000000, 1.000000, 0.878431}},
    {"linen", {0.980392, 0.941176, 0.901961}},
    {"magenta", {1.000000, 0.000000, 1.000000}},
    {"magenta1", {1.000000, 0.000000, 1.000000}},
    {"magenta2", {0.933333, 0.000000, 0.933333}},
    {"magenta3", {0.803922, 0.000000, 0.803922}},
    {"magenta4", {0.545098, 0.000000, 0.545098}},
    {"maroon", {0.690196, 0.188235, 0.376471}},
    {"maroon1", {1.000000, 0.203922, 0.701961}},
    {"maroon2", {0.933333, 0.188235, 0.654902}},
    {"maroon3", {0.803922, 0.160784, 0.564706}},
    {"maroon4", {0.545098, 0.109804, 0.384314}},
    {"medium aquamarine", {0.400000, 0.803922, 0.666667}},
    {"medium orchid", {0.729412, 0.333333, 0.827451}},
    {"medium purple", {0.576471, 0.439216, 0.858824}},
    {"mint cream", {0.960784, 1.000000, 0.980392}},
    {"misty rose", {1.000000, 0.894118, 0.882353}},
    {"moccasin", {1.000000, 0.894118, 0.709804}},
    {"navajo white", {1.000000, 0.870588, 0.678431}},
    {"old lace", {0.992157, 0.960784, 0.901961}},
    {"olive", {0.501961, 0.501961, 0.000000}},
    {"olive drab", {0.419608, 0.556863, 0.137255}},
    {"orange", {1.000000, 0.647059, 0.000000}},
    {"orange red", {1.000000, 0.270588, 0.000000}},
    {"orange1", {1.000000, 0.647059, 0.000000}},
    {"orange2", {0.933333, 0.603922, 0.000000}},
    {"orange3", {0.803922, 0.521569, 0.000000}},
    {"orange4", {0.545098, 0.352941, 0.000000}},
    {"orchid", {0.854902, 0.439216, 0.839216}},
    {"orchid1", {1.000000, 0.513726, 0.980392}},
    {"orchid2", {0.933333, 0.478431, 0.913725}},
    {"orchid3", {0.803922, 0.411765, 0.788235}},
    {"orchid4", {0.545098, 0.278431, 0.537255}},
    {"pale goldenrod", {0.933333, 0.909804, 0.666667}},
    {"pale green", {0.596078, 0.984314, 0.596078}},
    {"pale turquoise", {0.686275, 0.933333, 0.933333}},
    {"papaya whip", {1.000000, 0.937255, 0.835294}},
    {"peach puff", {1.000000, 0.854902, 0.725490}},
    {"peru", {0.803922, 0.521569, 0.247059}},
    {"pink", {1.000000, 0.752941, 0.796078}},
    {"pink1", {1.000000, 0.709804, 0.772549}},
    {"pink2", {0.933333, 0.662745, 0.721569}},
    {"pink3", {0.803922, 0.568627, 0.619608}},
    {"pink4", {0.545098, 0.388235, 0.423529}},
    {"plum", {0.866667, 0.627451, 0.866667}},
    {"plum1", {1.000000, 0.733333, 1.000000}},
    {"plum2", {0.933333, 0.682353, 0.933333}},
    {"plum3", {0.803922, 0.588235, 0.803922}},
    {"plum4", {0.545098, 0.400000, 0.545098}},
    {"powder blue", {0.690196, 0.878431, 0.901961}},
    {"purple", {0.627451, 0.125490, 0.941176}},
    {"purple1", {0.607843, 0.188235, 1.000000}},
    {"purple2", {0.568627, 0.172549, 0.933333}},
    {"purple3", {0.490196, 0.149020, 0.803922}},
    {"rebecca purple", {0.400000, 0.200000, 0.600000}},
    {"red", {1.000000, 0.000000, 0.000000}},
    {"red1", {1.000000, 0.000000, 0.000000}},
    {"red2", {0.933333, 0.000000, 0.000000}},
    {"red3", {0.803922, 0.000000, 0.000000}},
    {"red4", {0.545098, 0.000000, 0.000000}},
    {"rosy brown", {0.737255, 0.560784, 0.560784}},
    {"saddle brown", {0.545098, 0.270588, 0.074510}},
    {"salmon", {0.980392, 0.501961, 0.447059}},
    {"salmon1", {1.000000, 0.549020, 0.411765}},
    {"salmon2", {0.933333, 0.509804, 0.384314}},
    {"salmon3", {0.803922, 0.439216, 0.329412}},
    {"salmon4", {0.545098, 0.298039, 0.223529}},
    {"sandy brown", {0.956863, 0.643137, 0.376471}},
    {"seashell", {1.000000, 0.960784, 0.933333}},
    {"seashell1", {1.000000, 0.960784, 0.933333}},
    {"seashell2", {0.933333, 0.898039, 0.870588}},
    {"seashell3", {0.803922, 0.772549, 0.749020}},
    {"seashell4", {0.545098, 0.525490, 0.509804}},
    {"sienna", {0.627451, 0.321569, 0.176471}},
    {"sienna1", {1.000000, 0.509804, 0.278431}},
    {"sienna2", {0.933333, 0.474510, 0.258824}},
    {"sienna3", {0.803922, 0.407843, 0.223529}},
    {"sienna4", {0.545098, 0.278431, 0.149020}},
    {"silver", {0.752941, 0.752941, 0.752941}},
    {"sky blue", {0.529412, 0.807843, 0.921569}},
    {"slate blue", {0.415686, 0.352941, 0.803922}},
    {"slate gray", {0.439216, 0.501961, 0.564706}},
    {"slate grey", {0.439216, 0.501961, 0.564706}},
    {"snow", {1.000000, 0.980392, 0.980392}},
    {"snow1", {1.000000, 0.980392, 0.980392}},
    {"snow2", {0.933333, 0.913725, 0.913725}},
    {"snow3", {0.803922, 0.788235, 0.788235}},
    {"snow4", {0.545098, 0.537255, 0.537255}},
    {"tan", {0.823529, 0.705882, 0.549020}},
    {"tan1", {1.000000, 0.647059, 0.309804}},
    {"tan2", {0.933333, 0.603922, 0.286275}},
    {"tan3", {0.803922, 0.521569, 0.247059}},
    {"tan4", {0.545098, 0.352941, 0.168627}},
    {"thistle", {0.847059, 0.749020, 0.847059}},
    {"thistle1", {1.000000, 0.882353, 1.000000}},
    {"thistle2", {0.933333, 0.823529, 0.933333}},
    {"thistle3", {0.803922, 0.709804, 0.803922}},
    {"thistle4", {0.545098, 0.482353, 0.545098}},
    {"tomato", {1.000000, 0.388235, 0.278431}},
    {"tomato1", {1.000000, 0.388235, 0.278431}},
    {"tomato2", {0.933333, 0.360784, 0.258824}},
    {"tomato3", {0.803922, 0.309804, 0.223529}},
    {"tomato4", {0.545098, 0.211765, 0.149020}},
    {"violet", {0.933333, 0.509804, 0.933333}},
    {"violet red", {0.815686, 0.125490, 0.564706}},
    {"web gray", {0.501961, 0.501961, 0.501961}},
    {"web grey", {0.501961, 0.501961, 0.501961}},
    {"web maroon", {0.501961, 0.000000, 0.000000}},
    {"web purple", {0.501961, 0.000000, 0.501961}},
    {"wheat", {0.960784, 0.870588, 0.701961}},
    {"wheat1", {1.000000, 0.905882, 0.729412}},
    {"wheat2", {0.933333, 0.847059, 0.682353}},
    {"wheat3", {0.803922, 0.729412, 0.588235}},
    {"wheat4", {0.545098, 0.494118, 0.400000}},
    {"white", {1.000000, 1.000000, 1.000000}},
    {"white smoke", {0.960784, 0.960784, 0.960784}},
    {"x11 gray", {0.745098, 0.745098, 0.745098}},
    {"x11 grey", {0.745098, 0.745098, 0.745098}},
    {"x11 maroon", {0.690196, 0.188235, 0.376471}},
    {"x11 purple", {0.627451, 0.125490, 0.941176}},
    {"yellow", {1.000000, 1.000000, 0.000000}},
    {"yellow green", {0.603922, 0.803922, 0.196078}},
    {"yellow1", {1.000000, 1.000000, 0.000000}},
    {"yellow2", {0.933333, 0.933333, 0.000000}},
    {"yellow3", {0.803922, 0.803922, 0.000000}},
    {"yellow4", {0.545098, 0.545098, 0.000000}}
};