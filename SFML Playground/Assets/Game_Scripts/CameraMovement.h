#pragma once

#include <iostream>

#include "../../System_Scripts/Runtime_Script.h"
#include "../../System_Scripts/System_Manager.h"
#include "../../System_Scripts/Camera.h"

using namespace std;
class CameraMovement: public RuntimeScript
{
public:
    static FunctionEvent<glm::mat4> getTransMatrix;

    SystemManager* sys = SystemManager::GetInstance();
    glm::vec3 lastMousePos;
    
    float speed = 0.1f;
	
    //this updates the transformation matrix for any scripts that need it
    void UpdateScripts()
    {
        getTransMatrix.Trigger(_camera.GetTransformationMatrix());
    }

    void RotateCameraWithKeys()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            _camera.rotation.x -= 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            _camera.rotation.x += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            _camera.rotation.y += 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            _camera.rotation.y -= 1.0f;
        }
    }

    void RotateCameraWithMouse()
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*sys->GetMainWindow());

        glm::vec3 newMousePos = glm::vec3(mousePos.y, mousePos.x, 0.0f);
        glm::vec3 direction = newMousePos - lastMousePos;

        //Check if the result is a number
        if (glm::isnan(direction).x || glm::isnan(direction).y)
        {
            direction = glm::vec3(0.0f);
        }

        _camera.rotation += direction;

        lastMousePos = newMousePos;
    }

    void MoveCamera()
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            _camera.position += 1.0f * speed * _camera.cameraFront;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            _camera.position += -1.0f * speed * _camera.cameraFront;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            _camera.position -= 1.0f * speed * _camera.cameraSide;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            _camera.position += 1.0f * speed * _camera.cameraSide;
        }
    }

    void Update()
    {
        //Camera Debug
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            cout << "Mouse Position: " << sf::Mouse::getPosition(*sys->GetMainWindow()).x << " " << sf::Mouse::getPosition(*sys->GetMainWindow()).y << endl;
            _camera.PrintCameraInfo();
        }

        RotateCameraWithKeys();
        RotateCameraWithMouse();
        MoveCamera();

        UpdateScripts();
	}

    void Start() 
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(*sys->GetMainWindow());
        lastMousePos = glm::vec3(mousePos.x, mousePos.y, 0.0f);
    }

private:
	Camera _camera;
};