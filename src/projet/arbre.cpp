
#include <vector>
#include <queue>
#include <random>

#include "arbre.hpp"

using namespace vcl;

#define WIDTH_DEC 0.707
#define BR_LEN_DEC 0.8
#define TR_LEN_DEC 0.9

enum branch_type {
    A,
    B, 
    C
};

struct element {
    branch_type type;
    mat3 orientation;

    float start_width;
    float end_width;
    float len;

    std::vector<int> fils;
};

void generateTree(std::vector<element> &tree, int num_iter) {
    
    std::uniform_real_distribution<float> dist_lim_theta(3.14/4, 3.14/2);
    std::uniform_real_distribution<float> dist_theta(0, 3.14/2);
    std::uniform_real_distribution<float> dist_phi(0, 2*3.14);
    std::uniform_real_distribution<float> binary(0, 1);
    std::uniform_real_distribution<float> roll(-3.14/3, 3.14/3);

    std::random_device rd;
    std::default_random_engine gen(rd());

    for(int k = 0; k < num_iter; k++) {
        int n = tree.size();

        // Choose index of element to subdivide
        for(int idx = 0; idx < n; idx++) {
            //std::uniform_int_distribution<int> d(0, n-1);
            //int idx = d(gen);

            element elem = tree[idx];

            float a0 = 3.14/4;
            float a2 = 3.14/4;
            float d = 137.5 * 3.14 / 180;
            float b = 0; //roll(gen);

            switch(elem.type) {
                case A: {
                    element child1;
                    child1.type = B;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * WIDTH_DEC;
                    child1.len = elem.len * BR_LEN_DEC;
                    child1.orientation = rotation_from_axis_angle_mat3({1, 0, 0}, a0);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = A;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * WIDTH_DEC;
                    child2.len = elem.len * TR_LEN_DEC;
                    child2.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, d);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child2);
                    
                    break;
                }
                case B: {
                    element child1;
                    child1.type = C;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * WIDTH_DEC;
                    child1.len = elem.len * BR_LEN_DEC;
                    child1.orientation = rotation_from_axis_angle_mat3({0, 1, 0}, -a2);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = C;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * WIDTH_DEC;
                    child2.len = elem.len * TR_LEN_DEC;
                    child2.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, b);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child2);

                    break;
                }
                case C: {
                    element child1;
                    child1.type = B;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * WIDTH_DEC;
                    child1.len = elem.len * BR_LEN_DEC;
                    child1.orientation = rotation_from_axis_angle_mat3({0, 1, 0}, a2);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = B;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * WIDTH_DEC;
                    child2.len = elem.len * TR_LEN_DEC;
                    child2.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, b);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child2);

                    break;
                }
                default:
                    break;
            }            

            tree[idx] = elem;

        }

    }

}

mesh create_branch(float rad_a, float rad_b, float length) {

    //Number of samples on the circle
    const size_t N = 20;
    const size_t size = 2*N;

    mesh cylinder;

    //Fill position
    for(size_t ku = 0; ku < N; ku++) {
        float u = 2 * 3.14159f * ku / N;
        float x1 = rad_a * std::cos(u);
        float y1 = rad_a * std::sin(u);
        float x2 = rad_b * std::cos(u);
        float y2 = rad_b * std::sin(u);
        vec3 n = normalize(vec3(x1, y1, 0));

        cylinder.position.push_back({x1, y1, 0});
        cylinder.normal.push_back(n);
        cylinder.position.push_back({x2, y2, length});

        cylinder.normal.push_back(n);
    }

    //Fill connectivity
    for(size_t i = 0; i < size; i++) {
        cylinder.connectivity.push_back({i%size, (i+1)%size, (i+2)%size});
    }

    return cylinder;
}

void arbre::setup() {
    // Set up constants
    const float cyl_length = 1;
    const float cyl_radius = 0.1;

    // Start generation with seed
    element root;
    root.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, 0);
    root.start_width = cyl_radius;
    root.end_width = cyl_radius;
    root.type = A;
    root.len = cyl_length;

    std::vector<element> tree;
    tree.push_back(root);

    generateTree(tree, 5);


    // Start rendering from root
    mesh cylinder = create_branch(root.start_width, root.end_width, root.len);
    hierarchy.add_element(cylinder, "0", "root");

    std::queue<std::pair<int, int> > to_treat;
    to_treat.push(std::pair<int, int>(0, 1));

    while(!to_treat.empty()) {
        std::pair<int, int> pair = to_treat.front();
        int idx = pair.first;
        int depth = pair.second;
        to_treat.pop();
        element elem = tree[idx];

        //mesh cyl = create_branch(cyl_radius/depth, cyl_radius/(depth+1), cyl_length);
        //add children
        for(int c : elem.fils) {
            element child = tree[c];

            hierarchy.add_element(create_branch(child.start_width, child.end_width, child.len),
                                    std::to_string(c),
                                    std::to_string(idx),
                                    {0, 0, elem.len},
                                    child.orientation
            );

            to_treat.push(std::pair<int, int> (c, depth+1));
        }
    }


}


void arbre::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe) {
    hierarchy.rotation("0") = rotation;
    hierarchy.translation("0") = position;

    hierarchy.draw(shaders["mesh"], scene.camera);

    if(wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);
}