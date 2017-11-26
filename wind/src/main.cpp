#include <GL/glew.h>
#include <GL/freeglut.h>
#include <PerspectiveCamera.h>
#include <StandardRenderer.h>
#include <Scene.h>
#include <GameObject.h>
#include <StdOutLogHandler.h>
#include "ShaderProgram.h"
#include "Window.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "constants.h"

Renderer *renderer;

const int SCREEN_WIDTH = 1920 / 2;
const int SCREEN_HEIGHT = 1080 / 2;

Camera *camera;
std::shared_ptr<GameObject> palm;
Scene scene;

float lastWindChangeTime = 0;
chag::float3 lastWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);
chag::float3 currentWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);
chag::float3 newWindSpeed = chag::make_vector(0.0f, 0.0f, 0.0f);

sf::Clock timer;

void updateWind();

static const chag::float3 UP_VECTOR = chag::make_vector(0.0f, 1.0f, 0.0f);

void setupCamera() {
    camera = new PerspectiveCamera(
            chag::make_vector(10.0f, 15.0f, 15.0f), // Camera position
            chag::make_vector(0.0f, 8.0f, 0.0f),    // Look-at
            UP_VECTOR,                        // Reference vector
            45,                               // Field of view in degrees
            float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), // aspect ratio
            0.1f,                             // Near clip
            50000.0f);                        // Far clip
}

void loadMeshes() {
    /* Shader setup done once for all meshes that use it */
    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");

    /* Load player mesh and attach it to the player GameObject */
    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../meshes/bush.fbx");
    // references are from the the build folder

    palm = std::make_shared<GameObject>(playerMesh);
    palm->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    palm->addRenderComponent(stdrenderer);

    palm->setAffectedByWind(true);
    palm->setLeafAmplitude(0.01f);
    palm->setBranchAmplitude(0.15f);
    palm->setMainBendiness(0.02);

    /* Add the player to the scene */
    scene.addShadowCaster(palm);
}

// Called when not drawing
void idle(float timeSinceStart,float timeSinceLastCall) {
    scene.update(timeSinceLastCall);
    updateWind();
}

void updateWind() {

    float currentTime = timer.getElapsedTime().asSeconds();
    if(currentTime - lastWindChangeTime > 1.0f) {
        lastWindChangeTime += 1;
        lastWindSpeed = newWindSpeed;
        float x = getRand(0, 0.2);
        float y = getRand(0, 0.2);
        float z = getRand(0, 0.2);

        newWindSpeed.x = x * 2.0f - 1.0f;
        newWindSpeed.y = y * 2.0f - 1.0f;
        newWindSpeed.z = z * 2.0f - 1.0f;

    }

    currentWindSpeed = linearSmoothStep(lastWindSpeed, newWindSpeed, currentTime - lastWindChangeTime);
    scene.setWind(std::make_shared<Wind>(currentWindSpeed));

}

// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    renderer->drawScene(camera, &scene, timeSinceStart);
}

// Called when the window gets resized
void resize(int newWidth, int newHeight) {
    renderer->resize(newWidth, newHeight);
}

void loadLights() {
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight->specularColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->ambientColor= chag::make_vector(0.500f,0.500f,0.500f);

    directionalLight->direction= -chag::make_vector(0.0f,-10.0f,10.0f);
    scene.directionalLight = directionalLight;
}

// Called at start of the program
int main(int argc, char *argv[]) {
    Logger::addLogHandler(new StdOutLogHandler());

    Window* win = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Bubba3D Wind example");

    // Setup callbacks for window events
    win->setResizeMethod(resize);
    win->setIdleMethod(idle);
    win->setDisplayMethod(display);

    // Create and initialize the Renderer
    renderer = new Renderer();
    renderer->initRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);

    loadMeshes();
    loadLights();
    setupCamera();

    win->start(60);
    return 0;
}
