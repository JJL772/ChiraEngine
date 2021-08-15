#include "freecam.h"

freecam::freecam(engine* engine, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp, float newZoom, float newSpeed, float newSensitivity, bool controls) :
                 perspectiveCamera(engine, newYaw, newPitch, newPosition, currentUp, newZoom), movementSpeed(newSpeed), mouseSensitivity(newSensitivity) {
    if (controls) {
        this->setupKeybinds(engine);
    }
}

void freecam::init(engine* engine) {
    perspectiveCamera::init(engine);
    this->setActive(engine->isMouseCaptured());
}

void freecam::translate(glm::vec3 offset, double delta) {
    perspectiveCamera::translate(offset, this->movementSpeed, delta);
}

void freecam::translateLocal(signedAxis axis, double delta) {
    perspectiveCamera::translateLocal(axis, this->movementSpeed, delta);
}

void freecam::setActive(bool newActive) {
    perspectiveCamera::setActive(newActive);
    this->capturedMouse = !this->capturedMouse;
}

void freecam::setupKeybinds(engine* engine) const {
    // todo: get primary camera
    engine->addKeybind(keybind(GLFW_KEY_W, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZP, e->getDeltaTime());
    }));
    engine->addKeybind(keybind(GLFW_KEY_S, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZN, e->getDeltaTime());
    }));
    engine->addKeybind(keybind(GLFW_KEY_A, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XP, e->getDeltaTime());
    }));
    engine->addKeybind(keybind(GLFW_KEY_D, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XN, e->getDeltaTime());
    }));
    engine->addKeybind(keybind(GLFW_KEY_SPACE, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YP, e->getDeltaTime());
    }));
    engine->addKeybind(keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YN, e->getDeltaTime());
    }));
    engine->addMousebind(mousebind(MOVE, [this](class engine* e, double xOffset, double yOffset) {
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->isCurrent() && cam->isActive()) {
            xOffset *= this->mouseSensitivity;
            yOffset *= this->mouseSensitivity;
            cam->yaw += (float) xOffset;
            bool invertYAxis = false;
            e->getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
            if (invertYAxis) {
                cam->pitch += (float) yOffset;
            } else {
                cam->pitch -= (float) yOffset;
            }
            if (cam->pitch > 89.5f)
                cam->pitch = 89.5f;
            if (cam->pitch < -89.5f)
                cam->pitch = -89.5f;
        }
    }));
    engine->addKeybind(keybind(GLFW_KEY_TAB, GLFW_PRESS, [](class engine* e) {
        // todo: get primary camera
        auto* cam = (freecam*) e->getWorld("")->getCamera();
        if (cam->capturedMouse) {
            e->captureMouse(false);
            cam->setActive(false);
        } else {
            e->captureMouse(true);
            cam->setActive(true);
        }
    }));
}
