
#include "arbre.hpp"

using namespace vcl;

void arbre::setup() {
    hierarchy = monopodial::generate(5);
    hierarchy.scaling = 0.5;
}


void arbre::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe) {
    hierarchy.rotation("0") = rotation;
    hierarchy.translation("0") = position;

    if(!wireframe)
        hierarchy.draw(shaders["mesh"], scene.camera);

    if(wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);
}