#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    //----Variables----
    glm::vec3 position = glm::vec3(0.0f);

    //In degrees
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f);
    glm::vec3 cameraSide = glm::vec3(0.0f);

    //----Methods----
    //returns a view transformation matrix
    glm::mat4 GetTransformationMatrix()
    {
        glm::mat4 viewTrans = glm::mat4(1.0f);

        viewTrans = glm::rotate(viewTrans, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        viewTrans = glm::rotate(viewTrans, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        viewTrans = glm::rotate(viewTrans, glm::radians(rotation.z), glm::vec3(0, 0, 1));

        viewTrans = glm::translate(viewTrans, position);

        //update cameraFront
        cameraFront = glm::vec3(-glm::sin(glm::radians(rotation.y)), glm::sin(glm::radians(rotation.x)), glm::cos(glm::radians(rotation.x)) * glm::cos(glm::radians(rotation.y)));
        cameraFront = glm::normalize(cameraFront);
        //update cameraSide
        cameraSide = glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

        return viewTrans;
    }

    void PrintCameraInfo()
    {
        std::cout << "---Camera Info----" << std::endl;
        std::cout << "Camera rot: " << this->rotation.x << " " << this->rotation.y << " " << this->rotation.z << std::endl;
        std::cout << "Camera pos: " << this->position.x << " " << this->position.y << " " << this->position.z << std::endl;
        std::cout << "Camera Front: " << this->cameraFront.x << " " << this->cameraFront.y << " " << this->cameraFront.z << std::endl;
        std::cout << std::endl;
    }

    //I was thinking of adding local cordinates and here's how you would have them:
    /*
    1. have 2 separate transformation matrixs; a local and a world one.
    2. to convert to local cordinates, times the input vector with the local transformation matrix
    3. to convert to world cordinates, times the input vector with the world transformation matrix

    The problem: How would I edit the get transformation matrix function?
        -First I have to figure out how a local and world space interact with each other
        -I was thinking: viewTrans = worldTrans * localTrans; and then rotate and translate each one. But the results were wonky to deal with

    glm::vec3 ToLocalCords(glm::vec3 worldCords)
    {
        glm::vec4 test = glm::vec4(worldCords.x, worldCords.y, worldCords.z, 0);
        test = localTrans * test;
        return glm::vec3(test.x, test.y, test.z);
    }

    //This returns the global coordinates of where the local coordinates would be at
    glm::vec3 ToGlobalCords(glm::vec3 localCords)
    {
        glm::vec4 test = glm::vec4(localCords.x, localCords.y, localCords.z, 0);
        test = worldTrans * test;
        return glm::vec3(test.x, test.y, test.z);
    }
    */
private:



};