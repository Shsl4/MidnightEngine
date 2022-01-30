#include <Scene/CameraManager.h>
#include <Scene/Scene.h>

CameraManager::CameraManager(Scene *owner) : cameras(15), scene(owner) {


}

bool CameraManager::setActiveCamera(CameraComponent *camera) {

    // If the pointer is valid and the camera is registered.
    if (camera && cameras.contains(camera)) {

        // Set the camera as active.
        activeCamera = camera;
        return true;

    }

    return false;

}

void CameraManager::registerCamera(CameraComponent *camera) {

    if(!camera) { return; }
    
    // Append the camera to the registered camera array.
    this->cameras.append(camera);

    if (!activeCamera) {

        // Set the current camera.
        activeCamera = camera;

    }

}

void CameraManager::unregisterCamera(CameraComponent *camera) {

    if(!camera) { return; }
    
    // Remove the camera from the registered camera array.
    this->cameras.removeFirstOf(camera);

    // If no more cameras are present in the scene.
    if (cameras.getSize() == 0) {

        // Clear the current camera.
        activeCamera = nullptr;
        return;

    }

    // Set the current camera as the first one in the camera array.
    activeCamera = this->cameras[0];

}
