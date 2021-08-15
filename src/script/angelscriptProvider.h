#pragma once

#include <angelscript.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>
#include "abstractScriptProvider.h"
#include "angelscriptHolder.h"
#include "angelscriptHelpers.h"

class angelscriptHolder;

class angelscriptProvider : public abstractScriptProvider {
public:
    angelscriptProvider();
    void initProvider() override;
    void initScripts() override;
    void render(double delta) override;
    void stop() override;
    void addScript(angelscriptHolder* script);

    template <typename F>
    int registerGlobalFunction(F f, const std::string& name) {
        return registerGlobalFunction<F>(f, name, asTypeString<F>(name)());
    }

    template <typename F>
    int registerGlobalFunction(F f, const std::string& name, const std::string& decl) {
        return this->asEngine->RegisterGlobalFunction(decl.c_str(), asFUNCTION(f), asCALL_CDECL);
    }

    static void print(const std::string& message);
    static void messageCallback(const asSMessageInfo* msg, void* param);

    asIScriptEngine* asEngine = nullptr;
private:
    bool started;
    std::vector<std::unique_ptr<angelscriptHolder>> scripts;
};
