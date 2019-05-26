#include "joueur.hpp"

using namespace vcl;


void joueur::setup() {
    mesh player_ = mesh_load_file_obj("data/Dragon/Dragon/Dragon.obj");
    player = player_;
    player.uniform_parameter.scaling = 0.02f;
}

void joueur::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe, vec3 position, float angle) {

    glPolygonOffset( 1.0, 1.0 );
    player.uniform_parameter.translation = position;
    player.uniform_parameter.rotation = rotation_from_axis_angle_mat3({1,0,0},-(angle+0.2f))*rotation_from_axis_angle_mat3({0,1,0},3.14f);
    player.draw(shaders["mesh"], scene.camera);

    if(wireframe) {
        glPolygonOffset( 1.0, 1.0 );
        player.draw(shaders["wireframe"], scene.camera);
    }
}