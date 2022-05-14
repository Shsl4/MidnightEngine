#include <Scene/CameraManager.h>
#include <Scene/Scene.h>

CameraManager::CameraManager(Scene* owner) : cameras(15), scene(owner) {


}

bool CameraManager::setActiveCamera(WeakPointer<CameraComponent> & camera) {

    // If the pointer is valid and the camera is registered.
    if (camera.valid() && cameras.contains(camera)) {

        // Set the camera as active.
        activeCamera = camera;
        return true;

    }

    return false;

}

void CameraManager::registerCamera(WeakPointer<CameraComponent> camera) {
    
    // Append the camera to the registered camera array.
    this->cameras.append(camera);

    if (activeCamera.expired()) {

        // Set the current camera.
        activeCamera = camera;

    }

}

void CameraManager::unregisterCamera(CameraComponent* camera) {

    if(!camera) { return; }
    
    size_t i = 0;
    
    for (auto& e : cameras) {
        if (camera == e.raw()) {
            break;
        }
        ++i;
    }
    
    cameras.removeAt(i);

    // If no more cameras are present in the scene.
    if (cameras.getSize() == 0) {

        // Clear the current camera.
        activeCamera = nullptr;
        return;

    }

    // Set the current camera as the first one in the camera array.
    if (this->cameras.getSize() > 0) {
        activeCamera = this->cameras[0];
    }
    else{
        // Or nullptr if the array is empty.
        activeCamera = nullptr;
    }
    

}
