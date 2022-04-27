#include <UI/CharacterInfoWindow.h>
#include <Scene/FlyingCharacter.h>

CharacterInfoWindow::CharacterInfoWindow(FlyingCharacter *character) : character(character) {

}

void CharacterInfoWindow::render(const char *_errorText) {

    Transform transform = character->getRootComponent()->getWorldTransform();

    CameraComponent* camera = character->getScene()->getCameraManager()->getActiveCamera();

    ImGui::SetNextWindowPos(
            ImVec2(10.0f, 165.0f), ImGuiCond_FirstUseEver
    );
    ImGui::SetNextWindowSize(
            ImVec2(250.0f, 105.0f), ImGuiCond_FirstUseEver
    );

    ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);

    ImGui::Begin("FlyingCharacter", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    ImGui::PopStyleColor();

    ImGui::Text("Position: %.1f %.1f %.1f", transform.position.x, transform.position.y, transform.position.z);
    ImGui::Text("Rotation: %.1f %.1f %.1f", fmod(transform.rotation.x, 360), fmod(transform.rotation.y, 360), fmod(transform.rotation.z, 360));

    if (camera){
        ImGui::Text("Camera FOV: %.1f", camera->getFieldOfView());
    }

    ImGui::End();

}
