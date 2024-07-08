#include <iostream>

#include "./System_Scripts/Runtime_Script.h"
#include "./System_Scripts/System_Manager.h"
#include "./Camera.h"

using namespace std;
class CameraMovement: public RuntimeScript
{
public:
    static FunctionEvent<glm::mat4> getTransMatrix;

    SystemManager* sys = SystemManager::GetInstance();
    sf::Vector2i lastMousePos = sf::Mouse::getPosition(*sys->GetMainWindow());
    
    float speed = 0.1f;
	
    //this updates the transformation matrix for any scripts that need it
    void UpdateScripts()
    {
        getTransMatrix.Trigger(camera.GetTransformationMatrix());
    }

    void Start() {}

    void Update()
	{
        //Rotate the camera
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            camera.rotation.x -= 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            camera.rotation.x += 1.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            camera.rotation.y += 1.0f;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            camera.rotation.y -= 1.0f;
        }
        //
        //Camera Debug
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            camera.PrintCameraInfo();
        }
        //
        //Move the camera
        //cout << sf::Mouse::getPosition(*sys->GetMainWindow()).x << " " << sf::Mouse::getPosition(*sys->GetMainWindow()).y << endl;
        //Do Input Calculations
        sf::Vector2i nextMousePos = sf::Mouse::getPosition(*sys->GetMainWindow());
        sf::Vector2i direction = lastMousePos - nextMousePos;

        glm::vec3 directionGLM = glm::vec3(-direction.y, -direction.x, 0.0f);
        cout << directionGLM.x << " " << directionGLM.y << endl;
        if (glm::isnan(directionGLM).x || glm::isnan(directionGLM).y)
        {
            directionGLM = glm::vec3(0.0f);
        }

        camera.rotation += directionGLM;

        //
        lastMousePos = sf::Mouse::getPosition(*sys->GetMainWindow());
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            camera.position += 1.0f * speed * camera.cameraFront;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            camera.position += -1.0f * speed * camera.cameraFront;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            camera.position -= 1.0f * speed * camera.cameraSide;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            camera.position += 1.0f * speed * camera.cameraSide;
        }
        //

        UpdateScripts();
	}

private:
	Camera camera;
};