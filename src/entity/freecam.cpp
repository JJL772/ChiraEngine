#include "freecam.h"

freecam::freecam(engine* e, float newYaw, float newPitch, glm::vec3 newPosition, glm::vec3 currentUp, float newZoom, float newSpeed, float newSensitivity, bool controls) :
                 perspectiveCamera(e, newYaw, newPitch, newPosition, currentUp, newZoom), movementSpeed(newSpeed), mouseSensitivity(newSensitivity) {
    if (controls) {
        this->setupKeybinds(e);
    }
}

void freecam::init(engine* e) {
    perspectiveCamera::init(e);
    this->setActive(((engine*) e)->isMouseCaptured());
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

void freecam::setupKeybinds(engine* e) const {
    // todo: get primary camera
    /*
    e->addKeybind(keybind(GLFW_KEY_W, GLFW_REPEAT,[](engine* e) {
        auto* cam = (freecam*) e->getWorldEntityManager()->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZP, e->getDeltaTime());
    }));
    e->addKeybind(keybind(GLFW_KEY_S, GLFW_REPEAT,[](engine* e) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(ZN, e->getDeltaTime());
    }));
    e->addKeybind(keybind(GLFW_KEY_A, GLFW_REPEAT,[](engine* e) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XP, e->getDeltaTime());
    }));
    e->addKeybind(keybind(GLFW_KEY_D, GLFW_REPEAT,[](engine* e) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(XN, e->getDeltaTime());
    }));
    e->addKeybind(keybind(GLFW_KEY_SPACE, GLFW_REPEAT,[](engine* e) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YP, e->getDeltaTime());
    }));
    e->addKeybind(keybind(GLFW_KEY_LEFT_SHIFT, GLFW_REPEAT,[](class engine* e) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) cam->translateLocal(YN, e->getDeltaTime());
    }));
    e->addMousebind(mousebind(MOVE, [this](engine* e, double xOffset, double yOffset) {
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->isCurrent() && cam->isActive()) {
            xOffset *= this->mouseSensitivity;
            yOffset *= this->mouseSensitivity;
            cam->yaw += (float) xOffset;
            bool invertYAxis = false;
            engine::getSettingsLoader()->getValue("input", "invertYAxis", &invertYAxis);
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
    e->addKeybind(keybind(GLFW_KEY_TAB, GLFW_PRESS, [](class engine* e) {
        // todo: get primary camera
        auto* cam = (freecam*) e->getWorld(0)->getPrimaryCamera();
        if (cam->capturedMouse) {
            e->captureMouse(false);
            cam->setActive(false);
        } else {
            e->captureMouse(true);
            cam->setActive(true);
        }
    }));
    */
}
