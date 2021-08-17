#include "../src/core/engine.h"

#include "../src/resource/texture2d.h"
#include "../src/loader/objMeshLoader.h"
#include "../src/entity/freecam.h"
#include "../src/sound/oggFileSound.h"
#include "../src/resource/phongMaterial.h"
#include "../src/implementation/discordRichPresence.h"

int main() {
    engine e;
    virtualFileSystem::addResourceDirectory("resources/demo/");

    objMeshLoader objMeshLoader;
    engine::getSettingsLoader()->setValue("engine", "title", std::string("Demo Window"), true, true);

    e.addKeybind(keybind(GLFW_KEY_ESCAPE, GLFW_PRESS, [](engine* e) {
        e->stop();
    }));
    e.addKeybind(keybind(GLFW_KEY_1, GLFW_PRESS, [](engine* e) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }));
    e.addKeybind(keybind(GLFW_KEY_2, GLFW_PRESS, [](engine* e) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }));
    e.addKeybind(keybind(GLFW_KEY_GRAVE_ACCENT, GLFW_PRESS, [](engine* e) {
        e->showConsole(!e->getConsole()->getEnabled());
    }));
    e.addKeybind(keybind(GLFW_KEY_M, GLFW_PRESS, [](engine* e) {
        e->getSoundManager()->getSound("helloWorld")->play();
    }));

    e.addInitFunction([](engine* e) {
        const uuids::uuid phonglit_shader = resourceManager::addResource(new phongMaterial{
            resourceManager::addResource(new shader{"phonglit.vsh", "phonglit.fsh"}),
            resourceManager::addResource(new texture2d{"container_diffuse.png", GL_RGBA}),
            resourceManager::addResource(new texture2d{"container_specular.png", GL_RGBA})
        });

        discordRichPresence::init("875778280899358720");
        discordRichPresence::setLargeImage("main_logo");
        discordRichPresence::setDetails("Demo App Running");
        discordRichPresence::setState("https://discord.gg/ASgHFkX");

        e->captureMouse(true);
        //e->getEntity(e->addEntity(new entity{}))->addComponent(e, new meshComponent{});
        //e->getWorld(e->addWorld(new world{}))->addEntity(0);

        bool angelscriptEnabled = true;
        engine::getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
        if (angelscriptEnabled) {
            ((angelscriptProvider*) e->getScriptProvider("angelscript"))->addScript(new angelscriptHolder{"testScript.as"});
        }

        auto* sound = new oggFileSound();
        sound->init("helloWorldCutMono.ogg");
        e->getSoundManager()->addSound("helloWorld", sound);

        /*
        auto* cube_material = (phongMaterial*) resourceManager::getMaterial(resourceManager::getMesh(cube_mesh)->getMaterial());
        cube_material->setShininess();
        cube_material->setLambertFactor();
        auto* phong_shader = resourceManager::getShader(phonglit_shader);
        phong_shader->use();
        phong_shader->setUniform("light.ambient", 0.1f, 0.1f, 0.1f);
        phong_shader->setUniform("light.diffuse", 1.0f, 1.0f, 1.0f);
        phong_shader->setUniform("light.specular", 1.0f, 1.0f, 1.0f);
        phong_shader->setUniform("light.position", 0.0f, 5.0f, 0.0f);
        */

#if DEBUG
        engine::setBackgroundColor(0.0f, 0.0f, 0.3f, 1.0f);
#endif
    });

    e.init();
    e.run();
}
