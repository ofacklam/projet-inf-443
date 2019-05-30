#pragma once

#include <map>
#include <cstring>
#include <random>

#include "../base_exercise/base_exercise.hpp"

class joueur {
public: 
    virtual void setup();
    virtual void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe=false, vcl::vec3 position = {0,0,0}, float angle = 0, float rot_lateral = 0, int text = 0);

private:
    vcl::mesh_drawable_hierarchy player;
    vcl::mesh_drawable player2;
    vcl::timer_interval timer;
    int texture_dragon[3];
    int texture_player2;
};