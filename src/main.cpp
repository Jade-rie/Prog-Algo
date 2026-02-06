#include <sil/sil.hpp>
#include "random.hpp"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <complex>
#include <glm/gtx/matrix_transform_2d.hpp>

void keep_green_only(sil::Image &image) // Prend l'image par référence pour pouvoir la modifier
{
    for (glm::vec3 &color : image.pixels())
    {
        color.r = 0.f;
        color.b = 0.f;
    }
}

void chanel_swap(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        std::swap(color.r, color.b);
    }
}

void blackandwhite(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        float Y = 0.30 * color.r + 0.59 * color.g + 0.11 * color.b;
        color = {Y, Y, Y};
    }
}

void negatif(sil::Image &image)
{
    for (glm::vec3 &color : image.pixels())
    {
        color = glm::vec3(1 - color.r, 1 - color.g, 1 - color.b);
    }
}

void degrade(sil::Image &image)
{
    for (float x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            float color = x / image.width();
            image.pixel(x, y) = glm::vec3(color, color, color);
        }
    }
}

void miroir(sil::Image &image)
{
    for (float x{0}; x < image.width() / 2.f; x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            int x_miroir = image.width() - 1 - x;
            std::swap(image.pixel(x, y), image.pixel(x_miroir, y));
        }
    }
}

void bruit(sil::Image &image)
{

    for (int i = 0; i < 10000; i++)
    {
        float r = random_float(0, 1);
        float g = random_float(0, 1);
        float b = random_float(0, 1);
        int x = random_int(0, image.width());
        int y = random_int(0, image.height());
        image.pixel(x, y) = glm::vec3(r, g, b);
    }
}

sil::Image rotation(sil::Image &image)
{
    sil::Image new_image{image.height(), image.width()};
    for (float x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            new_image.pixel(image.height() - y - 1, x) = image.pixel(x, y);
        }
    }
    return new_image;
}

sil::Image RGB_split(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            if (x + 30 < image.width() && x - 30 >= 0)
            {
                new_image.pixel(x + 30, y).r = image.pixel(x, y).r;
                new_image.pixel(x, y).g = image.pixel(x, y).g;
                new_image.pixel(x - 30, y).b = image.pixel(x, y).b;
            }
        }
    }
    return new_image;
}

sil::Image luminosite(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {

            new_image.pixel(x, y).r = std::pow(image.pixel(x, y).r, 0.5);
            new_image.pixel(x, y).g = std::pow(image.pixel(x, y).g, 0.5);
            new_image.pixel(x, y).b = std::pow(image.pixel(x, y).b, 0.5);
        }
    }
    return new_image;
}

void cercle(sil::Image &image, float cx, float cy, float r1, float thickness)
{
    float r2 = r1 - thickness;
    // float r1 = 60;
    // float thickness = 5;
    // float cx = image.width()/2.0f;
    // float cy = image.height() / 2.0f;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {

            if (std::pow(x - cx, 2) + std::pow(y - cy, 2) < std::pow(r1, 2) && std::pow(x - cx, 2) + std::pow(y - cy, 2) > std::pow(r2, 2))
            {
                image.pixel(x, y) = {1.0f, 1.0f, 1.0f};
            }
        }
    }
}

void rosace(sil::Image &image)
{
    float r = 50;
    float theta = M_PI;
    float x = image.width() / 2.0f + r * std::cos(theta);
    float y = image.height() / 2.0f + r * std::sin(theta);

    for (float theta = 0; theta < 2 * M_PI; theta += M_PI / 3.0f)
    {
        float x = image.width() / 2.0f + r * std::cos(theta);
        float y = image.height() / 2.0f + r * std::sin(theta);
        cercle(image, x, y, r, 3);
    }
}

sil::Image mosaique(sil::Image &image)
{
    sil::Image new_image{image.width() * 5, image.height() * 5};

    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            new_image.pixel(x, y) = image.pixel(x % image.width(), y % image.height());
        }
    }
    return new_image;
}

sil::Image miroir_mosaique(sil::Image &image)
{
    sil::Image new_image{image.width() * 5, image.height() * 5};

    for (int x{0}; x < new_image.width(); x++)
    {
        for (int y{0}; y < new_image.height(); y++)
        {
            int pos_x = x % image.width();
            int pos_y = y % image.height();

            if ((x / image.width()) % 2 == 1)
            {
                pos_x = image.width() - 1 - pos_x;
            }

            if ((y / image.height()) % 2 == 1)
            {
                pos_y = image.height() - 1 - pos_y;
            }

            new_image.pixel(x, y) = image.pixel(pos_x, pos_y);
        }
    }
    return new_image;
}

void glitch(sil::Image &image)
{
    int r{0};
    int g{0};
    int b{0};
    for (int i = 0; i < 100; i++)
    {

        int x1 = random_int(0, image.width());
        int y1 = random_int(0, image.height());
        int x2 = random_int(0, image.width());
        int y2 = random_int(0, image.height());

        int largeur = random_int(1, 30);
        int hauteur = random_int(1, 10);
        for (int h = 0; h < hauteur; h++)
        {
            for (int l = 0; l < largeur; l++)
            {
                if (x1 + l < image.width() && y1 + h < image.height() && x2 + l < image.width() && y2 + h < image.height())
                {
                    r = image.pixel(x1 + l, y1 + h).r;
                    g = image.pixel(x1 + l, y1 + h).g;
                    b = image.pixel(x1 + l, y1 + h).b;

                    image.pixel(x1 + l, y1 + h).r = image.pixel(x2 + l, y2 + h).r;
                    image.pixel(x1 + l, y1 + h).g = image.pixel(x2 + l, y2 + h).g;
                    image.pixel(x1 + l, y1 + h).b = image.pixel(x2 + l, y2 + h).b;

                    image.pixel(x2 + l, y2 + h).r = r;
                    image.pixel(x2 + l, y2 + h).g = g;
                    image.pixel(x2 + l, y2 + h).b = b;
                }
            }
        }
    }
}

void tri(sil::Image &image)
{
    for (int i = 0; i < 800; i++)
    {
        int x = random_int(0, image.width());
        int y = random_int(0, image.height());
        int largeur = random_int(10, 70);
        std::vector<glm::vec3> v;

        for (int l = 0; l < largeur; l++)
        {
            if (x + l < image.width())
            {
                v.push_back(image.pixel(x + l, y));
            }
            std::sort(v.begin(), v.end(), [](glm::vec3 const &color1, glm::vec3 const &color2)
                      {
            float moy1 = (color1.r + color1.g + color1.b) / 3.0f;  
            float moy2 = (color2.r + color2.g + color2.b) / 3.0f;
            return moy1 < moy2; });

            for (int l = 0; l < v.size(); l++)
            {
                if (x + l < image.width())
                {
                    image.pixel(x + l, y) = v[l];
                }
            }
        }
    }
}

void fractale(sil::Image &image)
{
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            float x2 = 4.0f * x / image.width() - 2.0f;
            float y2 = 4.0f * y / image.height() - 2.0f;

            std::complex<float> c{x2, y2};
            std::complex<float> z{0.0f, 0.0f};

            int compteur = 0;

            while (std::abs(z) < 2.0f && compteur < 30)
            {
                z = z * z + c;
                compteur++;
            }

            if (compteur == 30)
            {
                image.pixel(x, y) = {1.0f, 1.0f, 1.0f};
            }
            else
            {
                float brightness = compteur / 30.0f;
                image.pixel(x, y) = {brightness, brightness, brightness};
            }
        }
    }
}

glm::vec3 sRGB_to_linear(glm::vec3 color)
{
    glm::vec3 L{0};
    if (color.r <= 0.04045)
    {
        L.r = color.r / 12.92;
    }
    else
    {
        L.r = std::pow(((color.r + 0.055) / 1.055), 2.4);
    }

    if (color.g <= 0.04045)
    {
        L.g = color.g / 12.92;
    }
    else
    {
        L.g = std::pow(((color.g + 0.055) / 1.055), 2.4);
    }

    if (color.b <= 0.04045)
    {
        L.b = color.b / 12.92;
    }
    else
    {
        L.b = std::pow(((color.b + 0.055) / 1.055), 2.4);
    }
    return L;
}

glm::vec3 linear_srgb_to_oklab(glm::vec3 c)
{
    float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
    float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
    float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    return {
        0.2104542553f * l_ + 0.7936177850f * m_ - 0.0040720468f * s_,
        1.9779984951f * l_ - 2.4285922050f * m_ + 0.4505937099f * s_,
        0.0259040371f * l_ + 0.7827717662f * m_ - 0.8086757660f * s_,
    };
}

glm::vec3 oklab_to_linear_srgb(glm::vec3 c)
{
    float l_ = c.r + 0.3963377774f * c.g + 0.2158037573f * c.b;
    float m_ = c.r - 0.1055613458f * c.g - 0.0638541728f * c.b;
    float s_ = c.r - 0.0894841775f * c.g - 1.2914855480f * c.b;

    float l = l_ * l_ * l_;
    float m = m_ * m_ * m_;
    float s = s_ * s_ * s_;

    return {
        +4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
        -1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
        -0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

glm::vec3 linear_to_sRGB(glm::vec3 color)
{
    glm::vec3 L{0};
    if (color.r <= 0.0031308)
    {
        L.r = color.r * 12.92;
    }
    else
    {
        L.r = (std::pow(1.055 * color.r, 1 / 2.4)) - 0.055;
    }

    if (color.g <= 0.0031308)
    {
        L.g = color.g * 12.92;
    }
    else
    {
        L.g = (std::pow(1.055 * color.g, 1 / 2.4)) - 0.055;
    }

    if (color.b <= 0.0031308)
    {
        L.b = color.b * 12.92;
    }
    else
    {
        L.b = (std::pow(1.055 * color.b, 1 / 2.4)) - 0.055;
    }
    return L;
}

void degrade_couleur(sil::Image &image)
{
    for (float x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            glm::vec3 color1 = {1.0f, 0.0f, 0.0f};
            glm::vec3 color2 = {0.0f, 1.0f, 0.0f};
            glm::vec3 color1_linear = sRGB_to_linear(color1);
            glm::vec3 color2_linear = sRGB_to_linear(color2);
            glm::vec3 color1_oklab = linear_srgb_to_oklab(color1_linear);
            glm::vec3 color2_oklab = linear_srgb_to_oklab(color2_linear);
            float pourcentage_gauche = (image.width() - x) / image.width();
            float pourcentage_droite = x / image.width();
            glm::vec3 color = pourcentage_gauche * color1_oklab + pourcentage_droite * color2_oklab;
            glm::vec3 color_linear_srgb = oklab_to_linear_srgb(color);
            glm::vec3 color_RGB = linear_to_sRGB(color_linear_srgb);
            image.pixel(x, y) = color_RGB;
        }
    }
}

sil::Image convolution(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    float k = 10;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            glm::vec3 sum_color{};
            int count{0};
            for (int i = -k; i <= k; i++)
            {
                for (int j = -k; j <= k; j++)
                {
                    if (x + i < image.width() && x + i >= 0 && y + j < image.height() && y + j >= 0)
                    {
                        glm::vec3 color = image.pixel(x + i, y + j);
                        sum_color += color;
                        count++;
                    }
                    else
                    {

                        count++;
                    }
                }
            }
            glm::vec3 moy = sum_color / static_cast<float>(count);
            new_image.pixel(x, y) = moy;
        }
    }
    return new_image;
}

void normalisation(sil::Image &image)
{
    float min_brightness = 1.0f;
    float max_brightness = 0.0f;
    float brightness{0};
    glm::vec3 color{0};
    std::vector<glm::vec3> v{0};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            brightness = (image.pixel(x, y).r + image.pixel(x, y).g + image.pixel(x, y).b) / 3.0f;
            max_brightness = std::max(brightness, max_brightness);
            min_brightness = std::min(brightness, min_brightness);
        }
    }

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            color = (image.pixel(x, y) - min_brightness) / (max_brightness - min_brightness);
            image.pixel(x, y) = color;
        }
    }
}

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

sil::Image vortex(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    glm::vec2 center_of_rotation = {image.width() / 2.0f, image.height() / 2.0f};
    float angle = M_PI / 6.0f;
    glm::vec2 point{0};
    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {

            glm::vec2 current_point = {x, y};
            float distance = glm::distance(center_of_rotation, current_point);
            angle = distance / 10.0f;
            glm::vec2 new_point = rotated(current_point, center_of_rotation, angle);
            if (new_point.x < image.width() && new_point.y < image.height() && new_point.x >= 0 && new_point.y >= 0)
            {
                new_image.pixel(x, y) = image.pixel(new_point.x, new_point.y);
            }
            else
            {
                new_image.pixel(x, y) = {0.0f, 0.0f, 0.0f};
            }
        }
    }
    return new_image;
}


double calculateStandardDeviation(const std::vector<glm::vec3> &arr)
{
    glm::vec3 sum{0};
    glm::vec3 mean{0};
    float standardDeviation{0};

    float size = arr.size();

   
    for (int i = 0; i < size; ++i)
    {
        sum += arr[i];
    }

    mean = sum / size;
    for (int i = 0; i < size; ++i)
    {
        standardDeviation += std::pow(glm::distance(arr[i], mean), 2);
    }

    return std::sqrt(standardDeviation / size);
}

sil::Image kuwahara(sil::Image &image)
{
    sil::Image new_image{image.width(), image.height()};
    int k = 3;

    for (int x{0}; x < image.width(); x++)
    {
        for (int y{0}; y < image.height(); y++)
        {
            std::vector<glm::vec3> up_right{};
            std::vector<glm::vec3> up_left{};
            std::vector<glm::vec3> down_right{};
            std::vector<glm::vec3> down_left{};
            for (int i = -k; i <= k; i++)
            {
                for (int j = -k; j <= k; j++)
                {
                    if (x + i < image.width() && x + i >= 0 && y + j < image.height() && y + j >= 0)
                    {
                        glm::vec3 color = image.pixel(x + i, y + j);
                        if (i < 0 && j < 0)
                        {
                            down_left.push_back(image.pixel(x + i, y + j));
                        }
                        if (i > 0 && j < 0)
                        {
                            down_right.push_back(image.pixel(x + i, y + j));
                        }
                        if (i < 0 && j > 0)
                        {
                            up_left.push_back(image.pixel(x + i, y + j));
                        }
                        if (i > 0 && j > 0)
                        {
                            up_right.push_back(image.pixel(x + i, y + j));
                        }
                    }
                }
            }
            std::vector<std::vector<glm::vec3>> gros_pixels = {down_left, up_left, down_right, up_right};
            std::vector<double> StandardDeviation = {calculateStandardDeviation(down_left), calculateStandardDeviation(up_left), calculateStandardDeviation(down_right), calculateStandardDeviation(up_right)};
            size_t min_idx = std::distance(StandardDeviation.begin(), std::min_element(StandardDeviation.begin(), StandardDeviation.end()));
            glm::vec3 sum_color{0.0f, 0.0f, 0.0f};
            int count{0};

            for (size_t i = 0; i < gros_pixels[min_idx].size(); i++)
            {
                sum_color += gros_pixels[min_idx][i];
                count++;
            }

            glm::vec3 moy = sum_color / static_cast<float>(count);
            new_image.pixel(x, y) = moy;
        }
    }
    return new_image;
}

int main()
{

    {
        //---------keep_green_only---------//

        // sil::Image image{"images/logo.png"};
        // keep_green_only(image);
        // image.save("output/keep_green_only.png");

        //---------chanel_swap---------//

        // sil::Image image{"images/logo.png"};
        // chanel_swap(image);
        // image.save("output/chanel_swap.png");

        //---------blackandwhite---------//

        // sil::Image image{"images/logo.png"};
        // blackandwhite(image);
        // image.save("output/blackandwhite.png");

        //---------negatif---------//

        // sil::Image image{"images/logo.png"};
        // negatif(image);
        // image.save("output/negatif.png");

        //---------degrade---------//

        // sil::Image image{"images/logo.png"};
        // degrade(image);
        // image.save("output/degrade.png");

        //---------miroir---------//

        // sil::Image image{"images/logo.png"};
        // miroir(image);
        // image.save("output/miroir.png");

        //---------bruit---------//

        // sil::Image image{"images/logo.png"};
        // bruit(image);
        // image.save("output/bruit.png");

        //---------rotation---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = rotation(image);
        // new_image.save("output/rotation.png");

        //---------RGB_split---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = RGB_split(image);
        // new_image.save("output/RGB_split.png");

        //---------luminosite---------//

        // sil::Image image{"images/photo.jpg"};
        // sil::Image new_image = luminosite(image);
        // new_image.save("output/luminosite.png");

        //---------cercle---------//

        // sil::Image image{300 /*width*/, 200 /*height*/};
        // cercle(image, image.width()/2.0f, image.height() / 2.0f, 60, 5);
        // image.save("output/cercle.png");

        //---------rosace---------//

        // sil::Image image{300 /*width*/, 200 /*height*/};
        // rosace(image);
        // image.save("output/rosace.png");

        //---------mosaique---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = mosaique(image);
        // new_image.save("output/mosaique.png");

        // ---------miroir_mosaique---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = miroir_mosaique(image);
        // new_image.save("output/miroir_mosaique.png");

        //---------glitch---------//

        // sil::Image image{"images/logo.png"};
        // glitch(image);
        // image.save("output/glitch.png");

        //---------tri---------//

        // sil::Image image{"images/logo.png"};
        // tri(image);
        // image.save("output/tri.png");

        //---------fractale---------//

        // sil::Image image{"images/logo.png"};
        // fractale(image);
        // image.save("output/fractale.png");

        //---------degrade_couleur---------//

        // sil::Image image{300 /*width*/, 200 /*height*/};
        // degrade_couleur(image);
        // image.save("output/degrade_couleur.png");

        // ---------convolution---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = convolution(image);
        // new_image.save("output/convolution.png");

        // ---------normalisation---------//

        // sil::Image image{"images/photo_faible_contraste.jpg"};
        // normalisation(image);
        // image.save("output/normalisation.png");

        //---------vortex---------//

        // sil::Image image{"images/logo.png"};
        // sil::Image new_image = vortex(image);
        // new_image.save("output/vortex.png");

        //---------kuwahara---------//

        sil::Image image{"images/photo.jpg"};
        sil::Image new_image = kuwahara(image);
        new_image.save("output/kuwahara.png");
    }
}
