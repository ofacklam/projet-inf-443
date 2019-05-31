#include "obstacle.hpp"

using namespace vcl;

obstacle::obstacle() {
    
}

void obstacle::setup() {
    object = mesh_primitive_parallelepiped();
}

void obstacle::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, bool wireframe) {
    object.uniform_parameter.translation = position;
    object.uniform_parameter.rotation = rotation;

    glPolygonOffset( 1.0, 1.0 );
    if(!wireframe)
        object.draw(shaders["mesh"], scene.camera);

    if(wireframe) {
        object.draw(shaders["wireframe"], scene.camera);
    }
}