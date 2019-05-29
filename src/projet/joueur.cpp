#include "joueur.hpp"

using namespace vcl;


void joueur::setup() {
    mesh_drawable corps = mesh_load_file_obj("data/Dragon/Dragon_corps.obj");
    mesh_drawable tete  = mesh_load_file_obj("data/Dragon/Dragon_tete.obj");
    mesh_drawable ag1 = mesh_load_file_obj("data/Dragon/Dragon_aileg1.obj");
    mesh_drawable ag2 = mesh_load_file_obj("data/Dragon/Dragon_aileg2.obj");
    mesh_drawable ad1 = mesh_load_file_obj("data/Dragon/Dragon_ailed1.obj");
    mesh_drawable ad2 = mesh_load_file_obj("data/Dragon/Dragon_ailed2.obj");

    player.add_element(corps,"corps","root");
    player.add_element(tete,"tete", "corps", {0.0f,0.0f,0.0f});
    player.add_element(ag1,"ag1","corps",{0,0,0});
    player.add_element(ag2,"ag2","ag1",{0,0,0});
    player.add_element(ad1,"ad1","corps",{0,0,0});
    player.add_element(ad2,"ad2","ad1",{0,0,0});
    player.scaling = 0.02f;
    timer.scale = 0.5f;
}

void joueur::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe, vec3 position, float angle) {
    timer.update();
    const float t = timer.t;

    glPolygonOffset( 1.0, 1.0 );
    player.translation("corps") = position;
    player.rotation("corps") = rotation_from_axis_angle_mat3({1,0,0},-(angle+0.2f))*rotation_from_axis_angle_mat3({0,1,0},3.14f);
    
    /*
    player.rotation("ag1") = rotation_from_axis_angle_mat3({0,0,1},  std::sin(2*3.14f*(t-0.4f)));
    player.rotation("ag2") = rotation_from_axis_angle_mat3({0,0,1},  std::sin(2*3.14f*(t-0.6f)));
    player.rotation("ad1") = rotation_from_axis_angle_mat3({0,0,-1}, std::sin(2*3.14f*(t-0.4f)));
    player.rotation("ad2") = rotation_from_axis_angle_mat3({0,0,-1}, std::sin(2*3.14f*(t-0.6f)));
    */
    
    player.draw(shaders["mesh"], scene.camera);

    if(wireframe) {
        glPolygonOffset( 1.0, 1.0 );
        player.draw(shaders["wireframe"], scene.camera);
    }
}