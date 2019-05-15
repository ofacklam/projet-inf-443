#include "obstacle.hpp"

using namespace vcl;

obstacle::obstacle() {
    
}

void obstacle::setup() {
    object = mesh_primitive_parallelepiped();
}

void obstacle::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe) {
    object.uniform_parameter.translation = position;
    object.uniform_parameter.rotation = rotation;
    object.uniform_parameter.scaling_axis = scaling;

    glPolygonOffset( 1.0, 1.0 );
    object.draw(shaders["mesh"], scene.camera);

    if(wireframe) {
        glPolygonOffset( 1.0, 1.0 );
        object.draw(shaders["wireframe"], scene.camera);
    }
}