#pragma once

#include <map>

#include "../base_exercise/base_exercise.hpp"

class obstacle {
public: 
    obstacle();
    void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui, vcl::vec3 position, vcl::mat3 rotation);

private:
    vcl::mesh_drawable object;
};