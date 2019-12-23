#include"imgSaver.hpp"

vector<HSLColor> ImgSaver::colorPalette;

vector<HSLColor> ImgSaver::getColorPalette(int n) {
    pdd hue, lightness;
    double sat=0.6;
    hue.first = 0;
    if(n <= 10) {
        hue.second = hue.first + 1.0;
        lightness = {0.6, 0.6};
    } else {
        hue.second = hue.first + 0.1*n;
        lightness = {0.85, 0.15};
    }
    vector<HSLColor> res;
    double m = n;
    double k = n+1;
    double in=0;
    for(int i=0; i < n; i++) {
        res.push_back(HSLColor(modf((hue.first*(m-i)+hue.second*i)/m, &in), sat, (lightness.first*(k-i)+lightness.second*i)/k));
    }
    return res;
}

void ImgSaver::init(int n) {
    // Magick::InitializeMagick({});
    // colorPalette = getColorPalette(n);
}

void ImgSaver::saveImg(string name, ll width, ll height, vector<Rectangle> &rect, vector<pll> &placement) {
    // Magick::Image img (Magick::Geometry(width, height), "white");
    // for(int i=0; i < (int)rect.size(); i++) {
    //     Rectangle &r = rect[i];
    //     HSLColor &c = colorPalette[r.id];
    //     pll &pos = placement[r.id];
    //     img.fillColor(Magick::ColorHSL(c.h, c.s, c.l));
    //     img.draw(Magick::DrawableRectangle(pos.first, pos.second, pos.first+r.width-1, pos.second+r.height-1));
    // }
    // img.flip();
    // img.magick("png");
    // img.write("images/"+name);
}