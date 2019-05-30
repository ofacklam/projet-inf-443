#pragma once

#include "obstacle.hpp"
#include "terrain.hpp"


class niveau {
    public:
        niveau() {}
        template<class T>
        void setup(terrain *t, int difficulte) {
            for(int i = 0; i<nb_obstacle_differents; i++) {
                obs.push_back(new T());
                obs[i]->setup();
            }
            ter = t;
            ter->setup();

            generate_positions(difficulte, 3, 0);
        }

        void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 pos_joueur, bool wireframe=false);
        bool collision(vcl::vec3 player_pos, float dist);
        void generate_positions(uint N, float min_dist = 3, float z_off = 0);

    private:
        std::vector<obstacle*> obs;
        terrain *ter;
        const int nb_obstacle_differents = 10;

        std::vector<vcl::vec3> obs_pos;
        std::vector<int> obs_type;
};
