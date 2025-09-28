#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <ctime>

struct Vertex
{
    Vector2 pos;   // offset 0
    Vector3 col;   // offset 8
};

// Assignment 1 object 1 -- WHITE triangle (vertex colours white)
static const Vertex vertices_white[3] =
{
    { { -0.6f, -0.4f }, { 1.0f, 1.0f, 1.0f } },
    { {  0.6f, -0.4f }, { 1.0f, 1.0f, 1.0f } },
    { {   0.f,  0.6f }, { 1.0f, 1.0f, 1.0f } }
};

// Assignment 1 object 2 -- RAINBOW triangle (separate buffers for pos & color)
static const Vector2 vertex_positions[3] =
{
    { -0.6f, -0.4f },
    {  0.6f, -0.4f },
    {  0.0f,  0.6f }
};

static const Vector3 vertex_colors[3] =
{
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f }
};

int main()
{
    CreateWindow(800, 800, "Graphics 1");

    // Shaders
    GLuint a1_tri_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a1_triangle.vert");
    GLuint a1_tri_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a1_triangle.frag");
    GLuint a1_tri_shader = CreateProgram(a1_tri_vert, a1_tri_frag);

    // --- Buffers for rainbow triangle (positions & colors in separate VBOs)
    GLuint vertex_buffer_rainbow_positions = 0;
    GLuint vertex_buffer_rainbow_colors = 0;
    glGenBuffers(1, &vertex_buffer_rainbow_positions);
    glGenBuffers(1, &vertex_buffer_rainbow_colors);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_colors), vertex_colors, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // --- Buffer for white triangle (interleaved pos+col)
    GLuint vertex_buffer_white = 0;
    glGenBuffers(1, &vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    // --- VAO for rainbow triangle
    GLuint vertex_array_rainbow = 0;
    glGenVertexArrays(1, &vertex_array_rainbow);
    glBindVertexArray(vertex_array_rainbow);
    {
        glEnableVertexAttribArray(0); // a_pos
        glEnableVertexAttribArray(1); // a_col

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_positions);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void*)0);

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_colors);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (void*)0);
    }
    glBindVertexArray(GL_NONE);

    // --- VAO for white triangle (interleaved)
    GLuint vertex_array_white = 0;
    glGenVertexArrays(1, &vertex_array_white);
    glBindVertexArray(vertex_array_white);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
        glEnableVertexAttribArray(0); // a_pos
        glEnableVertexAttribArray(1); // a_col
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));
    }
    glBindVertexArray(GL_NONE);

    // Uniform locations
    GLint u_color = glGetUniformLocation(a1_tri_shader, "u_color");
    GLint u_world = glGetUniformLocation(a1_tri_shader, "u_world");

    int object_index = 0;

    // Main loop
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Time since init (seconds)
        float tt = Time();

        // Background
        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

       
        if (IsKeyPressed(KEY_SPACE))
        {
            object_index = (object_index + 1) % 5;
        }


        switch (object_index)
        {
        case 0: 
        {
            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } break;

        case 1: 
        {
            glUseProgram(a1_tri_shader);
            
            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } break;

        case 2: // 3) Color changes over time (uniform)
        {
            float r = (sinf(tt * 1.0f) + 1.0f) * 0.5f;
            float g = (cosf(tt * 1.3f) + 1.0f) * 0.5f;
            float b = (sinf(tt * 0.7f) + 1.0f) * 0.5f;

            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, r, g, b);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(MatrixIdentity()));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } break;

        case 3: 
        {
            float x = sinf(tt); 
            Matrix world = MatrixTranslate(x, 0.0f, 0.0f);

            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 0.9f, 0.8f, 0.3f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(world));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } break;

        case 4: 
        {
            Matrix world = MatrixRotateZ(tt); 

            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 0.3f, 0.85f, 0.85f);
            glUniformMatrix4fv(u_world, 1, GL_FALSE, MatrixToFloat(world));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        } break;
        }

       
        Loop();
    }

    
    glDeleteVertexArrays(1, &vertex_array_rainbow);
    glDeleteVertexArrays(1, &vertex_array_white);
    glDeleteBuffers(1, &vertex_buffer_rainbow_positions);
    glDeleteBuffers(1, &vertex_buffer_rainbow_colors);
    glDeleteBuffers(1, &vertex_buffer_white);
    glDeleteProgram(a1_tri_shader);
    glDeleteShader(a1_tri_frag);
    glDeleteShader(a1_tri_vert);

    DestroyWindow();
    return 0;
}
