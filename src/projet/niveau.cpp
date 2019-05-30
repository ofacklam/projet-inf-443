#include "niveau.hpp"

using namespace vcl;



void niveau::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vec3 pos_joueur, bool wireframe) {
    ter->draw(shaders, scene, wireframe);


    for(vec3 pos : obs_pos) {
        if(norm(pos-pos_joueur) < 20)
            obs->draw(shaders, scene, pos, rotation_between_vector_mat3({0, 0, 1}, {0, pos.y, pos.z}), vec3(1, 1, 1), wireframe);
    }
}

bool niveau::collision(vec3 player_pos, float dist) {
    //Collision with terrain
    float terrain_left = evaluate_terrain(0, 0.3).x;
    float terrain_right = evaluate_terrain(0, 0.7).x;

    if(player_pos.x < terrain_left || player_pos.x > terrain_right)
        return true;


    //Collision with obstacles 
    for(vec3 pos : obs_pos) {
        if(norm(player_pos - pos) < dist)
            return true;
    }
    return false;
}


void niveau::generate_positions(uint N, float min_dist, float z_off)
{

    std::uniform_real_distribution<float> dist_1(0.35, 0.65);
    std::uniform_real_distribution<float> dist_2pi(0.5, 2*3.14-0.5);
    std::random_device rd;
    std::default_random_engine gen(rd());
    obs_pos.clear();

    for(uint k = 0; k < N; k++) {
        float theta = dist_2pi(gen);
        float v = dist_1(gen);

        vec3 pos = evaluate_terrain(theta, v);

        bool too_close = false;
        for(uint i = 0; i < k; i++) {
            if(norm(pos - obs_pos[i]) <= min_dist) {
                too_close = true;
                break;
            }
        }

        if(too_close)
            k--;
        else
            obs_pos.push_back(pos + vec3(0, z_off*std::cos(theta), z_off*std::sin(theta)));
    }
}