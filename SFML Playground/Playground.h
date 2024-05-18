#pragma once

#include <glad/glad.h>

#include "./System_Scripts/Runtime_Script.h"

class Playground: public RuntimeScript
{
public:
    //Temporary global variables
    sf::Clock clock1;
    int id = 0;

    template<typename FLOAT, size_t SZ>
    unsigned int CreateBuffer(FLOAT(&bufferData)[SZ], GLenum&& bufferType)
    {
        unsigned int bufferId;

        glGenBuffers(1, &bufferId);
        glBindBuffer(bufferType, bufferId);
        glBufferData(bufferType, sizeof(bufferData), bufferData, GL_STATIC_DRAW); //add data to the buffer

        return bufferId;
    }

    unsigned int CreateShaderProgram(Shader vertexShader, Shader fragmentShader)
    {
        //link shaders together to create a program pipeline
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();

        glAttachShader(shaderProgram, vertexShader.GetId());
        glAttachShader(shaderProgram, fragmentShader.GetId());
        glLinkProgram(shaderProgram);

        return shaderProgram;
    }

    int shaderId = 0;
    void CreateShaders()
    {
        Shader vertexShader("./Assets/Shaders/Texture_Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("./Assets/Shaders/Texture_Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);

        unsigned int shaderProgram = CreateShaderProgram(fragmentShader, vertexShader);
        int uniformColorLocation = glGetUniformLocation(shaderProgram, "time");

        id = uniformColorLocation;
        shaderId = shaderProgram;

        glUseProgram(shaderProgram);
    }

    void CreateVertexArray()
    {
        unsigned int tri2VertexArray;
        glGenVertexArrays(1, &tri2VertexArray);
        glBindVertexArray(tri2VertexArray);

        //pos attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        //texture attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(tri2VertexArray);
    }

    struct Image
    {
        int width;
        int height;
        int channels;

        unsigned char* imageData;

        ~Image()
        {
            stbi_image_free(imageData);
        }
    };

    Image LoadAImage(const char* imagePath)
    {
        Image image;

        image.imageData = stbi_load(imagePath, &image.width, &image.height, &image.channels, 0);

        return image;
    }

    void AddTextures()
    {
        unsigned int textures[2];
        glGenTextures(2, textures);

        cout << textures[0] << " " << textures[1] << endl;

        Image wallImage = LoadAImage("./Assets/Textures/wall.jpg");
        Image containerImage = LoadAImage("./Assets/Textures/container.jpg");

        if (wallImage.imageData != nullptr && containerImage.imageData != nullptr)
        {
            glActiveTexture(GL_TEXTURE0 + textures[0]);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallImage.width, wallImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallImage.imageData);
            glGenerateMipmap(GL_TEXTURE_2D);

            glActiveTexture(GL_TEXTURE0 + textures[1]);
            glBindTexture(GL_TEXTURE_2D, textures[1]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, containerImage.width, containerImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerImage.imageData);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            cout << "Failed to generate textures!" << endl;
            return;
        }
    }

    void Render()
    {
        sf::Time time = clock1.getElapsedTime();
        glUniform1f(id, time.asSeconds());


        unsigned int texture1Loc = glGetUniformLocation(shaderId, "texture1");
        if ((int)time.asSeconds() % 2 == 0)
        {
            glUniform1i(texture1Loc, 1);
        }
        else
        {
            glUniform1i(texture1Loc, 2);
        }

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void InitializeRender()
    {
        CreateShaders();

        float vertexs[] = {
            //positions         //colors          //Texture cordinates
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //BL
            0.0f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 0.5f, 1.0f, //T
            0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //BR
        };

        float textureCordinates[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.5f, 1.0f
        };

        unsigned int tri2VertexBuffer = CreateBuffer(vertexs, GL_ARRAY_BUFFER);

        CreateVertexArray();

        AddTextures();

        Render();
    }

	void Start()
	{
        InitializeRender();
	}

	void Update()
	{
        Render();
	}
};


// A useful Example
//void InitializeRender()
//{
//    const char* vertexShaderCode = "#version 330 core\n"
//        "layout (location = 0) in vec3 aPos;\n"
//        "void main()\n"
//        "{\n"
//        " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//        "}\0";
//
//    const char* fragmentShaderCode = "#version 330 core\n"
//        "out vec4 FragColor;\n"
//        "void main()\n"
//        "{\n"
//        " FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//        "}\0";
//
//    float vertices[] = {
//        0.0f, 0.0f, 0.0f, //BL
//        0.0f, 0.5f, 0.0f, //TL
//        0.5f, 0.0f, 0.0f, //BR
//        0.5f, 0.5f, 0.0f, //TR
//    };
//
//    unsigned int indices[] = {
//        0, 1, 2,
//        1, 2, 3
//    };
//
//    unsigned int vertexBuffer = CreateVerticeBuffer(vertices);
//    unsigned int indiceBuffer = CreateIndiceBuffer(indices);GL_ELEMENT_ARRAY_BUFFER
//    
//    //specifying how OpenGl should connect vertex data to the vertex shader:
//    unsigned int vertexArray;
//    glGenVertexArrays(1, &vertexArray);
//    glBindVertexArray(vertexArray);
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //specify data input for Vertex array pointer
//    glEnableVertexAttribArray(0);
//
//    //create shaders
//    unsigned int shaderProgram = CreateShaderProgram(fragmentShaderCode, vertexShaderCode);
//    glUseProgram(shaderProgram);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceBuffer);
//    //glBindVertexArray(GL_VERTEX_ARRAY, vertexArray);
// 
// glBindVertexArray(tri1VertexArray);
// glDrawArrays(GL_TRIANGLES, 0, 3); 
// 
//}