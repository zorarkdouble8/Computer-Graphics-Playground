#pragma once
#include "glm/glm.hpp"

float vertexs[] = {
    //positions          //colors          //Texture cordinates
    //Top
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //BL
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //BR
    -0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //TL
    0.5f, 0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TR

    //Bottom (starting at 4)
    -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, //BL
    0.5f, -0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //BR
    -0.5f, 0.5f, 0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //TL
    0.5f, 0.5f, 0.5f,   1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //TR
};

unsigned int indices[] = {
    //top plane
    0, 1, 2,
    3, 2, 1,

    //bottom plane
    4, 5, 6,
    7, 6, 5,

    //z plane (it's normal to)
    4, 1, 5,
    4, 1, 0,

    //-z plane
    6, 7, 3,
    6, 2, 3,

    //x plane
    5, 3, 1,
    5, 3, 7,

    //-x plane
    4, 2, 0,
    4, 2, 6,

};

glm::vec3 shapePositions[10] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};