/*
 * Spectra PS5 native homebrew test screen.
 * This is a development UI built against the public native-app boilerplate.
 * It does not launch the system Browser or PlayStation Store.
 */

#include "demo_renderer.hpp"

namespace
{
void draw_scene(ps5::demo::Canvas &canvas) noexcept
{
    using ps5::demo::Color;

    canvas.clear(Color::background);

    // Header and status strip.
    canvas.rectangle(70, 72, 1780, 3, Color::cyan);
    canvas.text(90, 115, "SPECTRA", 14, Color::white);
    canvas.text(92, 185, "PS5 NATIVE APP - FIRST BOOT TEST", 5, Color::cyan);
    canvas.text(92, 245, "BUILD CHECK: RENDERER ACTIVE", 4, Color::white);

    // Three non-functional test panels establish the first native UI layout.
    canvas.rectangle(90, 340, 540, 390, Color::panel);
    canvas.rectangle(690, 340, 540, 390, Color::panel);
    canvas.rectangle(1290, 340, 540, 390, Color::panel);

    canvas.rectangle(90, 340, 540, 8, Color::cyan);
    canvas.rectangle(690, 340, 540, 8, Color::yellow);
    canvas.rectangle(1290, 340, 540, 8, Color::magenta);

    canvas.text(130, 405, "01 / HOME", 6, Color::cyan);
    canvas.text(130, 485, "NATIVE RENDER", 5, Color::white);
    canvas.text(130, 550, "Panel and text test", 4, Color::white);
    canvas.circle(180, 640, 28, Color::cyan);

    canvas.text(730, 405, "02 / BROWSER", 6, Color::yellow);
    canvas.text(730, 485, "PLACEHOLDER", 5, Color::white);
    canvas.text(730, 550, "System browser not", 4, Color::white);
    canvas.text(730, 595, "connected in this build", 4, Color::white);
    canvas.rectangle(735, 635, 80, 12, Color::yellow);

    canvas.text(1330, 405, "03 / STORE", 6, Color::magenta);
    canvas.text(1330, 485, "PLACEHOLDER", 5, Color::white);
    canvas.text(1330, 550, "No Store API or", 4, Color::white);
    canvas.text(1330, 595, "account integration", 4, Color::white);
    canvas.triangle(1385, 625, 34, 55, Color::magenta);

    canvas.rectangle(90, 790, 1740, 2, Color::white);
    canvas.text(90, 835, "SPECTRA DEV BUILD", 5, Color::white);
    canvas.text(90, 895, "NATIVE TITLE FOLDER - NOT A RETAIL PKG", 4, Color::yellow);
    canvas.text(90, 970, "FIRST MILESTONE: DISPLAY AND RENDER LOOP", 4, Color::cyan);
}
} // namespace

int main()
{
    ps5::demo::run(draw_scene, "SPECTRA PS5 NATIVE TEST");
}
