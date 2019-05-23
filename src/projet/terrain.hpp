#pragma once

#include "../base_exercise/base_exercise.hpp"
#include <cmath>
#include <random>

class terrain {
    public:
        terrain() {}
        void setup();
        void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe=false);

    private:
        vcl::mesh_drawable terrain_;
        vcl::mesh_drawable canyon_;  
        int texture_canyon;
};

float normalize(float u);
float evaluate_terrain_z(float u, float v);
vcl::vec3 evaluate_terrain(float u, float v);
vcl::mesh create_terrain();
vcl::vec3 gaussienne_canyon(float u, float v);
vcl::mesh create_canyon();

const float r = 9.0f;