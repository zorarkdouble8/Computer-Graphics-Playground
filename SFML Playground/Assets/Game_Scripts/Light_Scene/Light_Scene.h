#pragma once

#include <iostream>
#include <functional>

#include <glad/glad.h>
#include <SFML/System.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Runtime_Script.h"
#include "System_Manager.h"
#include "Shader.h"

#include "../CameraMovement.h"
#include "./Shape_Infomation.h"

using namespace std;
class LightScene: public RuntimeScript
{
private:
    glm::mat4x4 modelTrans = glm::mat4(1.0f);
    glm::mat4x4 worldTrans = glm::mat4(1.0f);
    glm::mat4x4 viewTrans = glm::mat4(1.0f);
    glm::mat4x4 projTrans = glm::mat4(1.0f);

    void CheckErrors()
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
            }

            cout << error << endl;
        }
    }

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
        Shader vertexShader("./Assets/Shaders/Light_Scene/Vertex_Shader.glsl", GL_VERTEX_SHADER);
        Shader fragmentShader("./Assets/Shaders/Light_Scene/Fragment_Shader.glsl", GL_FRAGMENT_SHADER);

        unsigned int shaderProgram = CreateShaderProgram(fragmentShader, vertexShader);

        shaderId = shaderProgram;

        glUseProgram(shaderProgram);
    }

    int CreateVertexArray()
    {
        unsigned int tri2VertexArray;
        glGenVertexArrays(1, &tri2VertexArray);
        glBindVertexArray(tri2VertexArray);

        //pos attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        return tri2VertexArray;
    }

    void Render()
    {


        

        //Import all transformation matrixs into the shader
        unsigned int modelTransLoc = glGetUniformLocation(shaderId, "modelTrans");

        unsigned int worldTransLoc = glGetUniformLocation(shaderId, "worldTrans");
        glUniformMatrix4fv(worldTransLoc, 1, GL_FALSE, glm::value_ptr(worldTrans));

        unsigned int viewTransLoc = glGetUniformLocation(shaderId, "viewTrans");
        glUniformMatrix4fv(viewTransLoc, 1, GL_FALSE, glm::value_ptr(viewTrans));

        unsigned int projectTransLoc = glGetUniformLocation(shaderId, "projTrans");
        glUniformMatrix4fv(projectTransLoc, 1, GL_FALSE, glm::value_ptr(projTrans));
        //
        // 
        //Randomize all cube positions and rotations
        for (int x = 0; x < shapePositions.size(); x++)
        {
            modelTrans = glm::mat4x4(1.0f);
            modelTrans = glm::translate(modelTrans, shapePositions[x]);
            //modelTrans = glm::rotate(modelTrans, (float) (3.14 / 8) * x, glm::vec3(0, 1, 0));

            glUniformMatrix4fv(modelTransLoc, 1, GL_FALSE, glm::value_ptr(modelTrans));

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
        }        
        //
    }

    void InitializeRender()
    {
        glEnable(GL_DEPTH_TEST);

        CreateShaders();





        unsigned int tri2VertexBuffer = CreateBuffer(vertexs, GL_ARRAY_BUFFER);
        unsigned int indiceElementBuffer = CreateBuffer(indices, GL_ELEMENT_ARRAY_BUFFER);

        int vertexArray = CreateVertexArray();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indiceElementBuffer);
        
        Render();
    }

public:
    LightScene* handle;

    void UpdateViewMatrix(glm::mat4 cameraTransMatrix)
    {
        viewTrans = cameraTransMatrix;
    }

	void Start()
	{
        projTrans = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        if (handle != nullptr)
        {
            CameraMovement::getTransMatrix.AddFunction(std::bind(&LightScene::UpdateViewMatrix, handle, std::placeholders::_1));
        }

        InitializeRender();
	}
    
	void Update()
	{
        Render();

        CheckErrors();
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