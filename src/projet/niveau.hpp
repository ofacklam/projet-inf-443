#pragma once

#include "obstacle.hpp"
#include "terrain.hpp"


class niveau {
    public:
        niveau() {}
        void setup(obstacle *o, terrain *t);
        void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe=false);
        bool collision(vcl::vec3 player_pos, float dist);

    private:
        obstacle *obs;
        terrain *ter;
        //skybox *sky;

        std::vector<vcl::vec3> obs_pos;
};

void generate_positions(uint N, std::vector<vcl::vec3> &arr, float min_dist, float z_off);