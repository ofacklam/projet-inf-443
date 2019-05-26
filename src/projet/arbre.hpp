#pragma once

#include <map>

#include "obstacle.hpp"
#include "l-systems/tree_gen.hpp"

class arbre : public obstacle {
public: 
    arbre() {};
    virtual void setup();
    virtual void draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe=false);

private:
    vcl::mesh_drawable_hierarchy hierarchy;
};