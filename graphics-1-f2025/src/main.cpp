#include "Window.h"
#include <glad/glad.h>

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    /* Loop until the user closes the window */
    while (!WindowShouldClose())
    {
        // Colors are represented as fractions between 0.0 and 1.0, so convert using a colour-picker tool accordingly!
        float r = 239.0f / 255.0f;
        float g = 136.0f / 255.0f;
        float b = 190.0f / 255.0f;
        float a = 1.0f;

        /* Render here */
        glClearColor(r, g, b, a);
        glClear(GL_COLOR_BUFFER_BIT);

        Loop();
    }

    DestroyWindow();
    return 0;
}
