#include "engine.h"

#if __has_include(<windows.h>)
#include <windows.h>
#endif

#include <iostream>

#define IMGUI_USER_CONFIG "../../src/config/imguiConfig.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "../loader/jsonSettingsLoader.h"
#include "../loader/image.h"
#include "../sound/alSoundManager.h"
#include "../implementation/discordRichPresence.h"

engine::engine(const std::string& configPath) {
#ifdef WIN32
#if DEBUG
    system(("chcp " + std::to_string(CP_UTF8) + " > nul").c_str());
#else
    FreeConsole();
#endif
#endif
    virtualFileSystem::addResourceDirectory(ENGINE_FILESYSTEM_PREFIX);
    engine::setSettingsLoader(new jsonSettingsLoader(configPath));
    this->lastTime = 0;
    this->currentTime = 0;
    this->lastMouseX = -1;
    this->lastMouseY = -1;
}

void engine::init() {
    this->started = true;

    chiraLogger::addCallback([this](const loggerType type, const std::string& source, const std::string& message) {
        this->getConsole()->engineLoggingHook(type, source, message);
    });

    bool angelscriptEnabled = true;
    engine::getSettingsLoader()->getValue("scripting", "angelscript", &angelscriptEnabled);
    if (angelscriptEnabled) {
        this->addScriptProvider("angelscript", new angelscriptProvider{});
    }

    if (!glfwInit()) {
        chiraLogger::log(ERR, "GLFW", "GLFW not defined");
        exit(EXIT_FAILURE);
    }
    glfwSetErrorCallback(engine::errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    int windowWidth = 1600;
    engine::getSettingsLoader()->getValue("graphics", "windowWidth", &windowWidth);
    int windowHeight = 900;
    engine::getSettingsLoader()->getValue("graphics", "windowHeight", &windowHeight);
    bool fullscreen = false;
    engine::getSettingsLoader()->getValue("graphics", "fullscreen", &fullscreen);
    std::string title = "Basic Game Engine";
    engine::getSettingsLoader()->getValue("engine", "title", &title);
    this->window = glfwCreateWindow(windowWidth,
                                    windowHeight,
                                    title.c_str(),
                                    fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                                    nullptr);
    if (!this->window) {
        chiraLogger::log(ERR, "GLFW", "Window creation failed");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->window);
    glfwSetWindowUserPointer(this->window, this);

    if (engine::getSettingsLoader()->hasValue("engine", "iconPath")) {
        std::string path{};
        engine::getSettingsLoader()->getValue("engine", "iconPath", &path);
        this->setIcon(path);
    } else {
        chiraLogger::log(WARN, "ChiraEngine", "You should not unset the iconPath property unless you are a trained professional!");
    }

#if DEBUG
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    chiraLogger::log(INFO, "GLFW", "Using GLFW v" + std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(rev));
#endif

    if (!gladLoadGL(glfwGetProcAddress)) {
        chiraLogger::log(ERR, "OpenGL", "OpenGL 3.3 Core must be available to run this program");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

#if DEBUG
int vertexAttributes, textureUnits;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &vertexAttributes);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureUnits);
    chiraLogger::log(INFO, "OpenGL", "Maximum number of vertex attributes is " + std::to_string(vertexAttributes));
    chiraLogger::log(INFO, "OpenGL", "Maximum number of texture units is " + std::to_string(textureUnits));
#endif

    int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(this->window, engine::framebufferSizeCallback);
    engine::setBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(1);

    glfwSetInputMode(this->window, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
    bool rawMouseMotion = false;
    engine::getSettingsLoader()->getValue("input", "rawMouseMotion", &rawMouseMotion);
    if (glfwRawMouseMotionSupported() && rawMouseMotion) {
        glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }
    glfwSetKeyCallback(this->window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(this->window, mouseMovementCallback);
    glfwSetScrollCallback(this->window, mouseScrollCallback);

#if DEBUG
    IMGUI_CHECKVERSION();
#endif
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    chiraLogger::log(INFO, "ImGUI", "ImGUI loaded successfully");

    bool openalEnabled = true;
    engine::getSettingsLoader()->getValue("scripting", "angelscript", &openalEnabled);
    if (openalEnabled) {
        this->setSoundManager(new alSoundManager{});
    }
    this->soundManager->init();

    if (engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxPointLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_POINT_LIGHTS", std::to_string(maxLights));
    }
    if (engine::getSettingsLoader()->hasValue("engine", "maxPointLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxDirectionalLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_DIRECTIONAL_LIGHTS", std::to_string(maxLights));
    }
    if (engine::getSettingsLoader()->hasValue("engine", "maxSpotLights")) {
        int maxLights;
        engine::getSettingsLoader()->getValue("engine", "maxSpotLights", &maxLights);
        shaderFile::addPreprocessorSymbol("MAX_SPOT_LIGHTS", std::to_string(maxLights));
    }

    this->callRegisteredFunctions(&(this->initFunctions));

    for (const auto& shaderPair : engine::shaders) {
        shaderPair.second->compile();
    }
    for (const auto& texturePair : engine::textures) {
        texturePair.second->compile();
    }
    for (const auto& materialPair : engine::materials) {
        materialPair.second->compile();
    }
    for (const auto& [name, scriptProvider] : this->scriptProviders) {
        scriptProvider->initProvider();

        if (name == "angelscript") {
            auto* angel = (angelscriptProvider*) scriptProvider.get();

            angel->registerGlobalFunction(engine::setBackgroundColor, "setBackgroundColor");

            angel->asEngine->RegisterGlobalFunction("void captureMouse(bool)", asMETHOD(engine, captureMouse), asCALL_THISCALL_ASGLOBAL, this);
            angel->asEngine->RegisterGlobalFunction("bool isMouseCaptured()", asMETHOD(engine, isMouseCaptured), asCALL_THISCALL_ASGLOBAL, this);
            angel->asEngine->RegisterGlobalFunction("void showConsole(bool)", asMETHOD(engine, showConsole), asCALL_THISCALL_ASGLOBAL, this);
        }

        scriptProvider->initScripts();
    }
}

void engine::run() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();

    while (!glfwWindowShouldClose(this->window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        this->render();
        // todo: get primary camera
        this->soundManager->setListenerPosition(this->getWorld("")->getPrimaryCamera()->getPosition());
        this->soundManager->setListenerRotation(this->getWorld("")->getPrimaryCamera()->getRotation(), this->getWorld("")->getPrimaryCamera()->getUpVector());
        this->soundManager->update();
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        this->keyboardRepeatingCallback();
        this->mouseButtonRepeatingCallback();
        if (discordRichPresence::initialized()) {
            discordRichPresence::updatePresence();
        }
    }

    this->stop();
}

void engine::render() {
    this->lastTime = this->currentTime;
    this->currentTime = glfwGetTime();
    for (const auto& shaderPair : engine::shaders) {
        // todo: get primary camera
        shaderPair.second->setUniform("p", this->getWorld("")->getPrimaryCamera()->getProjectionMatrix());
        shaderPair.second->setUniform("v", this->getWorld("")->getPrimaryCamera()->getViewMatrix());
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    callRegisteredFunctions(&(this->renderFunctions));

#if DEBUG
    if (this->getConsole()->getEnabled()) {
        this->getConsole()->render();
    }
#endif

    for (const auto& scriptProviderPair : this->scriptProviders) {
        scriptProviderPair.second->render(this->getDeltaTime());
    }

    // todo: this
    //this->worldPtr->render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void engine::stop() {
    chiraLogger::log(INFO_IMPORTANT, "ChiraEngine", "Gracefully exiting...");

    for (const auto& scriptProviderPair : this->scriptProviders) {
        scriptProviderPair.second->stop();
    }

    if (discordRichPresence::initialized()) {
        discordRichPresence::shutdown();
    }

    for (const auto& texturePair : engine::textures) {
        texturePair.second->discard();
    }
    for (const auto& shaderPair : engine::shaders) {
        shaderPair.second->discard();
    }

    callRegisteredFunctions(&(this->stopFunctions));

    // todo: this
    //this->worldPtr->discard();
    this->soundManager->stop();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void engine::addInitFunction(const std::function<void(engine*)>& init) {
    this->initFunctions.push_back(init);
}

void engine::addRenderFunction(const std::function<void(engine*)>& render) {
    this->renderFunctions.push_back(render);
}

void engine::addStopFunction(const std::function<void(engine*)>& stop) {
    this->stopFunctions.push_back(stop);
}

void engine::addKeybind(const keybind& keybind) {
    this->keybinds.push_back(keybind);
}

std::vector<keybind>* engine::getKeybinds() {
    return &(this->keybinds);
}

void engine::addMousebind(const mousebind& mousebind) {
    this->mousebinds.push_back(mousebind);
}

std::vector<mousebind>* engine::getMousebinds() {
    return &(this->mousebinds);
}

void engine::addScriptProvider(const std::string& name, abstractScriptProvider* scriptProvider) {
    this->scriptProviders.insert(std::make_pair(name, scriptProvider));
}

abstractScriptProvider* engine::getScriptProvider(const std::string& name) {
    if (this->scriptProviders.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getScriptProvider", "Script provider " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return this->scriptProviders.at(name).get();
}

void engine::setSoundManager(abstractSoundManager* newSoundManager) {
    this->soundManager.reset(newSoundManager);
}

abstractSoundManager* engine::getSoundManager() {
    if (!this->soundManager) {
        chiraLogger::log(WARN, "engine::getSoundManager", "Must set sound manager in engine::setSoundManager for this call to function");
        return nullptr;
    }
    return this->soundManager.get();
}

void engine::setSettingsLoader(abstractSettingsLoader* newSettingsLoader) {
    engine::settingsLoader.reset(newSettingsLoader);
    engine::setSettingsLoaderDefaults();
}

abstractSettingsLoader* engine::getSettingsLoader() {
    if (!engine::settingsLoader) {
        chiraLogger::log(WARN, "engine::getSettingsLoader", "Must set settings loader in engine::setSettingsLoader for this call to function");
        return nullptr;
    }
    return engine::settingsLoader.get();
}

void engine::addShader(const std::string& name, shader* s) {
    engine::shaders.insert(std::make_pair(name, s));
}

shader* engine::getShader(const std::string& name) {
    if (engine::shaders.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getShader", "Shader " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return engine::shaders.at(name).get();
}

void engine::addTexture(const std::string& name, texture* t) {
    engine::textures.insert(std::make_pair(name, t));
}

texture* engine::getTexture(const std::string& name) {
    if (engine::textures.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getTexture", "Texture " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return engine::textures.at(name).get();
}

void engine::addMesh(const std::string& name, mesh* m) {
    engine::meshes.insert(std::make_pair(name, m));
}

mesh* engine::getMesh(const std::string& name) {
    if (engine::meshes.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getMesh", "Mesh " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return engine::meshes.at(name).get();
}

void engine::addMaterial(const std::string& name, abstractMaterial* m) {
    engine::materials.insert(std::make_pair(name, m));
}

abstractMaterial* engine::getMaterial(const std::string& name) {
    if (engine::materials.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getMaterial", "Material " + name + " is not recognized, check that you registered it properly");
        return nullptr;
    }
    return engine::materials.at(name).get();
}

void engine::addGenericResource(const std::string& name, abstractResource* r) {
    engine::resources.insert(std::make_pair(name, r));
}

abstractResource* engine::getGenericResource(const std::string& name) {
    if (engine::resources.count(name) == 0) {
        chiraLogger::log(ERR, "engine::getMaterial", "Material " + name + " is not recognized, check that you registered it properly");
    }
    return engine::resources.at(name).get();
}

void engine::addWorld(const std::string& name, world* newWorld) {
    // todo: add world
}

world* engine::getWorld(const std::string& name) {
    // todo: get world
    return nullptr;
}

void engine::addEntity(const std::string& world, const std::string& name, entity* newEntity) {
    // todo: add entity
}

void engine::addEntity(const std::string& name, entity* newEntity) {
    // todo: add entity
}

entity* engine::getEntity(const std::string& name) {
    // todo: get entity
    return nullptr;
}

void engine::setBackgroundColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

bool engine::isStarted() const {
    return this->started;
}

double engine::getDeltaTime() const {
    return this->currentTime - this->lastTime;
}

void engine::captureMouse(bool capture) {
    this->mouseCaptured = capture;
    if (capture) {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
    } else {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
    }
}

bool engine::isMouseCaptured() const {
    return this->mouseCaptured;
}

void engine::showConsole(bool shouldShow) {
    this->getConsole()->setEnabled(shouldShow);
}

console* engine::getConsole() {
    return &(this->consoleUI);
}

void engine::callRegisteredFunctions(const std::vector<std::function<void(engine*)>>* list) {
    for (const std::function<void(engine*)>& func : *list) {
        func(this);
    }
}

void engine::setSettingsLoaderDefaults() {
    engine::settingsLoader->load();
    engine::settingsLoader->addCategory("engine");
    engine::settingsLoader->setValue("engine", "iconPath", std::string("ui/icon.png"), false, false);
    engine::settingsLoader->setValue("engine", "title", std::string("ChiraEngine"), false, false);
    engine::settingsLoader->setValue("engine", "consoleColoredText", true, false, false);
    engine::settingsLoader->setValue("engine", "maxPointLights", 64, false, false);
    engine::settingsLoader->setValue("engine", "maxDirectionalLights", 4, false, false);
    engine::settingsLoader->setValue("engine", "maxSpotLights", 4, false, false);
    engine::settingsLoader->addCategory("audio");
    engine::settingsLoader->setValue("audio", "openal", true, false, false);
    engine::settingsLoader->addCategory("input");
    engine::settingsLoader->setValue("input", "rawMouseMotion", true, false, false);
    engine::settingsLoader->setValue("input", "invertYAxis", false, false, false);
    engine::settingsLoader->addCategory("graphics");
    engine::settingsLoader->setValue("graphics", "windowWidth", 1600, false, false);
    engine::settingsLoader->setValue("graphics", "windowHeight", 900, false, false);
    engine::settingsLoader->setValue("graphics", "fullscreen", false, false, false);
    engine::settingsLoader->addCategory("scripting");
    engine::settingsLoader->setValue("scripting", "angelscript", true, false, false);
    engine::settingsLoader->save();
}

void engine::setIcon(const std::string& iconPath) {
#if DEBUG
    assert(this->started);
#endif
    GLFWimage images[1];
    int width, height, bitsPerPixel;
    image icon(virtualFileSystem::getTexturePath(iconPath), &width, &height, &bitsPerPixel, 4, false);
#if DEBUG
    assert(icon.getData());
#endif
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = icon.getData();
    glfwSetWindowIcon(this->window, 1, images);
}

void engine::errorCallback(int error, const char* description) {
#if DEBUG
    std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
#endif
}

void engine::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void engine::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_REPEAT) return;
    for (keybind& k : *e->getKeybinds()) {
        if (k.getButton() == key && k.getAction() == action) {
            k.run(e);
        }
    }
}

void engine::keyboardRepeatingCallback() {
    for (keybind& k : this->keybinds) {
        if (glfwGetKey(this->window, k.getButton()) && k.getAction() == GLFW_REPEAT) {
            k.run(this);
        }
    }
}

void engine::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    for (keybind& k : *e->getKeybinds()) {
        if (k.getButton() == button && k.getAction() == action) {
            k.run(e);
        }
    }
}

void engine::mouseButtonRepeatingCallback() {
    for (keybind& k : this->keybinds) {
        if (k.isMouse() && (glfwGetMouseButton(this->window, k.getButton()) && k.getAction() == GLFW_REPEAT)) {
            k.run(this);
        }
    }
}

void engine::mouseMovementCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));

    if (e->lastMouseX == -1) e->lastMouseX = xPos;
    if (e->lastMouseY == -1) e->lastMouseY = yPos;

    int width, height;
    glfwGetWindowSize(e->window, &width, &height);
    double xOffset = xPos - e->lastMouseX;
    double yOffset = yPos - e->lastMouseY;

    for (mousebind& bind : *e->getMousebinds()) {
        if (bind.getType() == MOVE) {
            bind.run(e, xOffset, yOffset);
        }
    }

    e->lastMouseX = xPos;
    e->lastMouseY = yPos;
}

void engine::mouseScrollCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* e = static_cast<engine*>(glfwGetWindowUserPointer(window));
    for (mousebind& bind : *e->getMousebinds()) {
        if (bind.getType() == SCROLL) {
            bind.run(e, xPos, yPos);
        }
    }
}
