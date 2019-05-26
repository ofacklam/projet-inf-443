#include "skybox.hpp"

using namespace vcl;

void Skybox::setup(string f, string b, string l, string r, string t, string bot) {
    front = texture_gpu(image_load_png(f));
    back = texture_gpu(image_load_png(b));
    left = texture_gpu(image_load_png(l));
    right = texture_gpu(image_load_png(r));
    top = texture_gpu(image_load_png(t));
    bottom = texture_gpu(image_load_png(bot));


    f_mesh = mesh_primitive_quad({-a, -a, a}, {-a, a, a}, {-a, a, -a}, {-a, -a, -a});
    f_mesh.uniform_parameter.shading = {1, 0, 0};

    b_mesh = mesh_primitive_quad({a, a, a}, {a, -a, a}, {a, -a, -a}, {a, a, -a});
    b_mesh.uniform_parameter.shading = {1, 0, 0};

    l_mesh = mesh_primitive_quad({-a, a, a}, {a, a, a}, {a, a, -a}, {-a, a, -a});
    l_mesh.uniform_parameter.shading = {1, 0, 0};

    r_mesh = mesh_primitive_quad({a, -a, a}, {-a, -a, a}, {-a, -a, -a}, {a, -a, -a});
    r_mesh.uniform_parameter.shading = {1, 0, 0};

    t_mesh = mesh_primitive_quad({a, a, a}, {-a, a, a}, {-a, -a, a}, {a, -a, a});
    t_mesh.uniform_parameter.shading = {1, 0, 0};

    bot_mesh = mesh_primitive_quad({-a, -a, -a}, {a, -a, -a}, {a, a, -a}, {-a, a, -a});
    bot_mesh.uniform_parameter.shading = {1, 0, 0};
}

void Skybox::bind(GLuint texture) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
}

void Skybox::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene) {
    mat3 R = rotation_from_axis_angle_mat3({-1, 0, 0}, 5*3.14/12) * scene.camera.orientation;

    //Front 
    bind(front);
    f_mesh.uniform_parameter.rotation = R;
    f_mesh.draw(shaders["mesh"], scene.camera);

    //Back
    bind(back);
    b_mesh.uniform_parameter.rotation = R;
    b_mesh.draw(shaders["mesh"], scene.camera);

    //Left
    bind(left);
    l_mesh.uniform_parameter.rotation = R;
    l_mesh.draw(shaders["mesh"], scene.camera);

    //Right
    bind(right);
    r_mesh.uniform_parameter.rotation = R;
    r_mesh.draw(shaders["mesh"], scene.camera);

    //Top
    bind(top);
    t_mesh.uniform_parameter.rotation = R;
    t_mesh.draw(shaders["mesh"], scene.camera);

    //Bottom
    bind(bottom);
    bot_mesh.uniform_parameter.rotation = R;
    bot_mesh.draw(shaders["mesh"], scene.camera);


    //Bind back to white texture
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
}