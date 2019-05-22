
#include <vector>
#include <queue>
#include <random>

#include "arbre.hpp"

using namespace vcl;

struct element {
    bool branche;
    float theta;
    float phi;

    std::vector<int> fils;
};

void generateTree(std::vector<element> &tree, int num_iter) {
    
    std::uniform_real_distribution<float> dist_lim_theta(3.14/4, 3.14/2);
    std::uniform_real_distribution<float> dist_theta(0, 3.14/2);
    std::uniform_real_distribution<float> dist_phi(0, 2*3.14);
    std::uniform_real_distribution<float> binary(0, 1);

    std::random_device rd;
    std::default_random_engine gen(rd());

    for(int k = 0; k < num_iter; k++) {
        int n = tree.size();

        // Choose index of element to subdivide
        for(int idx = 0; idx < n; idx++) {
            //std::uniform_int_distribution<int> d(0, n-1);
            //int idx = d(gen);

            element elem = tree[idx];

            // Rule no 1 : branch grows
            if(elem.branche) {
                if(binary(gen) < 0.3) {
                    element child_branch;
                    child_branch.branche = true;
                    child_branch.phi = 0;
                    child_branch.theta = 0;
                    child_branch.fils = elem.fils;

                    tree.push_back(child_branch);

                    elem.fils.clear();
                    elem.fils.push_back(tree.size() - 1);
                }
                else {
                    element child_branch;
                    child_branch.branche = false;
                    child_branch.phi = dist_phi(gen);
                    child_branch.theta = dist_lim_theta(gen);

                    tree.push_back(child_branch);
                    elem.fils.push_back(tree.size() - 1);

                    child_branch.phi += 3.14;

                    tree.push_back(child_branch);
                    elem.fils.push_back(tree.size() - 1);
                }
            }
            else { // Rule no 2 : leaf becomes branch
                elem.branche = true;

                // Create straigth leaf
                /*element straight;
                straight.branche = false;
                straight.phi = straight.theta = 0;
                tree.push_back(straight);
                elem.fils.push_back(tree.size() - 1);*/

                // Create side leaves
                for(int i = 0; i < 1; i++) {
                    element e;
                    e.branche = false;
                    e.phi = dist_phi(gen);
                    e.theta = dist_theta(gen);
                    tree.push_back(e);

                    elem.fils.push_back(tree.size() - 1);

                    //Symetric
                    e.phi += 3.14;
                    tree.push_back(e);

                    elem.fils.push_back(tree.size() - 1);
                }
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
    root.branche = false;
    root.theta = root.phi = 0;

    std::vector<element> tree;
    tree.push_back(root);

    generateTree(tree, 4);


    // Start rendering from root
    mesh cylinder = create_branch(cyl_radius, cyl_radius, cyl_length);
    hierarchy.add_element(cylinder, "0", "root");

    std::queue<std::pair<int, int> > to_treat;
    to_treat.push(std::pair<int, int>(0, 1));

    while(!to_treat.empty()) {
        std::pair<int, int> pair = to_treat.front();
        int idx = pair.first;
        int depth = pair.second;
        to_treat.pop();
        element elem = tree[idx];

        mesh cyl = create_branch(cyl_radius/depth, cyl_radius/(depth+1), cyl_length);
        //add children
        for(int c : elem.fils) {
            element child = tree[c];

            hierarchy.add_element(cyl, std::to_string(c), std::to_string(idx), {0, 0, cyl_length},
                rotation_between_vector_mat3({0, 0, 1}, {
                    std::sin(child.theta) * std::cos(child.phi),
                    std::sin(child.theta) * std::sin(child.phi),
                    std::cos(child.theta)
                })
            );

            to_treat.push(std::pair<int, int> (c, depth+1));
        }
    }


}


void arbre::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, vcl::vec3 position, vcl::mat3 rotation, vcl::vec3 scaling, bool wireframe) {
    //hierarchy.rotation("0") = rotation;
    //hierarchy.translation("0") = position;

    hierarchy.draw(shaders["mesh"], scene.camera);

    if(wireframe)
        hierarchy.draw(shaders["wireframe"], scene.camera);
}