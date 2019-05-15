#pragma once

#include "projet.hpp"


class niveau {
    public:
        niveau(obstacle *o) : obs(o) {}
        void setup();
        void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui, bool wireframe=false);

    private:
        obstacle *obs;
        //terrain *ter;
        //skybox *sky;
};