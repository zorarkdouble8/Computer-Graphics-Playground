#pragma once

#include<glad/glad.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

#include<iostream>

using namespace std;

const char* vertexShaderCode = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderCode = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void render()
{
    float vertices[] = {
        -0.8f, 0.0f, 0.0f,
        0.8f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    //getting the vertex buffer to store in our vertice
    unsigned int VertexBuffer;
    glGenBuffers(1, &VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);

    //transfering the buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    //Fragment Sgader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);


    //link shaders together to create a program pipeline
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //check for compilation errors
    int isSuccesful;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isSuccesful);

    if (!isSuccesful)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR: Vertex shader compilation failed \n" << infoLog << endl << "----End of error log----" << endl;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isSuccesful);
    if (!isSuccesful)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR: Vertex shader compilation failed \n" << infoLog << endl << "----End of error log----" << endl;
    }

    //specifying how OpenGl should connect vertex data to the vertex shader:
    unsigned int vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    

    //render the program
    glUseProgram(shaderProgram);
    glBindVertexArray(vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
}

int main()
{
    sf::Context context;

    //Initialize glad
    if (!gladLoadGLLoader((GLADloadproc)(context.getFunction)))
    {
        cout << "Could not initialize GLAD!!!!" << endl;
        return -1;
    }

    // create the window
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

    // activate the window
    window.setActive(true);

    // load resources, initialize the OpenGL states, ...

    // run the main loop
    bool running = true;
    while (running)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
        }

        // clear the buffers
        glClearColor(155, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        // draw...
        render();

        // end the current frame (internally swaps the front and back buffers)
        window.display();
    }

    // release resources...

    return 0;
}