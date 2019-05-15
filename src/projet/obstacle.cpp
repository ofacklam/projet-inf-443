#include "obstacle.hpp"

using namespace vcl;

obstacle::obstacle() {
    object = mesh_primitive_parallelepiped();
}

void obstacle::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui, vcl::vec3 position, vcl::mat3 rotation) {
    //object.draw(shaders["mesh"], scene.camera);
}