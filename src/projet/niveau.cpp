#include "niveau.hpp"

using namespace vcl;

void niveau::setup(obstacle* o, terrain *t) {
    obs = o;
    ter = t;
    ter->setup();
    obs->setup();

    generate_positions(10, obs_pos, 3, 0);
}

void niveau::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe) {
    //ter->draw(shaders, scene, wireframe);

    for(vec3 pos : obs_pos) {
        obs->draw(shaders, scene, pos, rotation_between_vector_mat3({0, 0, 1}, {0, pos.y, pos.z}), vec3(1, 1, 1));
    }
}


void generate_positions(uint N, std::vector<vcl::vec3> &arr, float min_dist, float z_off)
{

    std::uniform_real_distribution<float> dist_1(0.35, 0.65);
    std::uniform_real_distribution<float> dist_2pi(0.05, 2*3.14-0.05);
    std::random_device rd;
    std::default_random_engine gen(rd());

    for(uint k = 0; k < N; k++) {
        float theta = dist_2pi(gen);
        float v = dist_1(gen);

        vec3 pos = evaluate_terrain(theta, v);

        bool too_close = false;
        for(uint i = 0; i < k; i++) {
            if(norm(pos - arr[i]) <= min_dist) {
                too_close = true;
                break;
            }
        }

        if(too_close)
            k--;
        else
            arr.push_back(pos + vec3(0, z_off*std::cos(theta), z_off*std::sin(theta)));
    }
}