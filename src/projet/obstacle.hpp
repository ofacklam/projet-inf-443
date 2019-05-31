#pragma once

#include <map>

#include "../base_exercise/base_exercise.hpp"

class obstacle {
public: 
    obstacle();
    virtual void setup();
    virtual void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, bool wireframe=false);

private:
    vcl::mesh_drawable object;
};