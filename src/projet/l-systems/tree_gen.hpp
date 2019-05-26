#pragma once

#include <vector>
#include <queue>
#include <random>
#include "vcl/vcl.hpp"

enum branch_type {
    A,
    B, 
    C,
    Branch
};

struct element {
    branch_type type;
    vcl::mat3 orientation;
    vcl::vec3 vector;

    float start_width;
    float end_width;
    float len;

    std::vector<int> fils;
};


class monopodial {
    public:
        static void create_struct(std::vector<element> &tree, int num_iter);
        static vcl::mesh_drawable_hierarchy generate(int num_iter);
        
};

class ternary {
    public:
        static void create_struct(std::vector<element> &tree, int num_iter);
        static vcl::mesh_drawable_hierarchy generate(int num_iter);

};