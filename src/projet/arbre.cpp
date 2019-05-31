
#include "arbre.hpp"

using namespace vcl;

void arbre::setup() {
    hierarchy = monopodial::generate(5);
    hierarchy.scaling = 0.5;
    texture_trunk = texture_gpu(image_load_png("data/trunk.png"));
}


void arbre::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, bool wireframe) {
    hierarchy.rotation("0") = rotation;
    hierarchy.translation("0") = position;

    if(!wireframe) {
        glBindTexture(GL_TEXTURE_2D, texture_trunk);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        hierarchy.draw(shaders["mesh"], scene.camera);
    }

    if(wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);
}