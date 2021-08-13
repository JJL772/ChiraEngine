#include "phongMaterial.h"

void phongMaterial::updateLighting(const std::vector<std::unique_ptr<abstractLight>>& lights) {
    unsigned short pl = 0, dl = 0, sl = 0;
    for (const auto& light : lights) {
        engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].ambient", ((pointLight*)(light.get()))->ambient.x, ((pointLight*)(light.get()))->ambient.y, ((pointLight*)(light.get()))->ambient.z);
        engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].diffuse", ((pointLight*)(light.get()))->diffuse.x, ((pointLight*)(light.get()))->diffuse.y, ((pointLight*)(light.get()))->diffuse.z);
        engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].specular", ((pointLight*)(light.get()))->specular.x, ((pointLight*)(light.get()))->specular.y, ((pointLight*)(light.get()))->specular.z);
        if (light->type == POINT_LIGHT) {
            engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].position", ((pointLight*)(light.get()))->position.x, ((pointLight*)(light.get()))->position.y, ((pointLight*)(light.get()))->position.z);
            engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].constant", ((pointLight*)(light.get()))->constant);
            engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].linear", ((pointLight*)(light.get()))->linear);
            engine::getShader(this->shaderName)->setUniform("pointLight[" + std::to_string(pl) + "].quadratic", ((pointLight*)(light.get()))->quadratic);
            pl++;
        } else if (light->type == DIRECTIONAL_LIGHT) {
            engine::getShader(this->shaderName)->setUniform("directionalLight[" + std::to_string(dl) + "].direction", ((directionalLight*)(light.get()))->direction.x, ((directionalLight*)(light.get()))->direction.y, ((directionalLight*)(light.get()))->direction.z);
            dl++;
        } else if (light->type == SPOT_LIGHT) {
            engine::getShader(this->shaderName)->setUniform("spotLight[" + std::to_string(sl) + "].position", ((spotLight*)(light.get()))->position.x, ((spotLight*)(light.get()))->position.y, ((spotLight*)(light.get()))->position.z);
            engine::getShader(this->shaderName)->setUniform("spotLight[" + std::to_string(sl) + "].constant", ((spotLight*)(light.get()))->constant);
            engine::getShader(this->shaderName)->setUniform("spotLight[" + std::to_string(sl) + "].linear", ((spotLight*)(light.get()))->linear);
            engine::getShader(this->shaderName)->setUniform("spotLight[" + std::to_string(sl) + "].quadratic", ((spotLight*)(light.get()))->quadratic);
            engine::getShader(this->shaderName)->setUniform("spotLight[" + std::to_string(sl) + "].direction", ((spotLight*)(light.get()))->direction.x, ((spotLight*)(light.get()))->direction.y, ((spotLight*)(light.get()))->direction.z);
            sl++;
        }
    }
}
