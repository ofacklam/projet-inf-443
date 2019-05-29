#pragma once

#include <map>

#include "../base_exercise/base_exercise.hpp"

class joueur {
public: 
    virtual void setup();
    virtual void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe=false, vcl::vec3 position = {0,0,0}, float angle = 0);

private:
    vcl::mesh_drawable player;
};