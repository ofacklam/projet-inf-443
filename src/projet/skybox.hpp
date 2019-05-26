#pragma once

#include <string>
#include <map>
#include "../base_exercise/base_exercise.hpp"

using std::string;

class Skybox {
    const float a = 20;

    GLuint front, back, left, right, top, bottom;
    vcl::mesh_drawable f_mesh;
    vcl::mesh_drawable b_mesh;
    vcl::mesh_drawable l_mesh;
    vcl::mesh_drawable r_mesh;
    vcl::mesh_drawable t_mesh;
    vcl::mesh_drawable bot_mesh;

    void bind(GLuint texture);

  public:
    Skybox() {};
    void setup(string front, string back, string left, string right, string top, string bottom);
    void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene);

};