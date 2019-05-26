
#include "arbre.hpp"

using namespace vcl;

void arbre::setup() {
    hierarchy = monopodial::generate(6);
}


void arbre::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe) {
    hierarchy.rotation("0") = rotation;
    hierarchy.translation("0") = position;

    hierarchy.draw(shaders["mesh"], scene.camera);

    if(wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);
}