#include "joueur.hpp"

using namespace vcl;


void joueur::setup() {
    mesh_drawable corps = mesh_load_file_obj("data/Dragon/Dragon_corps.obj");
    mesh_drawable tete  = mesh_load_file_obj("data/Dragon/Dragon_tete.obj");
    mesh_drawable ag1 = mesh_load_file_obj("data/Dragon/Dragon_aileg.obj");
    mesh_drawable ad1 = mesh_load_file_obj("data/Dragon/Dragon_ailed.obj");

    player.add_element(corps,"corps","root");
    player.add_element(tete,"tete", "corps", {0.0f,0.0f,0.0f});
    player.add_element(ag1,"ag1","corps",{3.5,-0.5,-10});
    player.add_element(ad1,"ad1","corps",{-3,-0.5,-10});
    player.scaling = 0.02f;
    timer.t_max = 5.0f; 
    timer.scale = 1.0f;

    srand(time(NULL));
    char c = '1' + rand()%3;
    std::string texture = "data/Dragon/Dragon/Texture";
    texture += c;
    texture += ".png";
    texture_dragon = texture_gpu(image_load_png(texture));
}

void joueur::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe, vec3 position, float angle, vcl::vec3 position_prev) {
    timer.update();
    const float t = timer.t;

    float rot_lateral = (position.x - position_prev.x);
    glPolygonOffset( 1.0, 1.0 );
    player.translation("corps") = position;
    player.rotation("corps") = rotation_from_axis_angle_mat3({1,0,0},-(angle+0.2f))*rotation_from_axis_angle_mat3({0,1,0},3.14f)*rotation_from_axis_angle_mat3({0,0,1}, 50*rot_lateral);
    
    const float aile = std::sin(2*3.14f*(t-0.4f));
    player.rotation("ag1") = rotation_from_axis_angle_mat3({0,0,1}, aile);
    player.rotation("ad1") = rotation_from_axis_angle_mat3({0,0,-1}, aile);
    player.rotation("tete") = rotation_from_axis_angle_mat3({1,0,0}, 0.25*aile);
    
    if(!wireframe) {
        glBindTexture(GL_TEXTURE_2D,texture_dragon);
        player.draw(shaders["mesh"], scene.camera);
    } else {
        glPolygonOffset( 1.0, 1.0 );
        player.draw(shaders["wireframe"], scene.camera);
    }
}