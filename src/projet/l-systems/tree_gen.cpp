#include "tree_gen.hpp"

using namespace vcl;

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

mesh_drawable_hierarchy create_mesh(std::vector<element> &tree, bool only_branch = false) {
    
    mesh_drawable_hierarchy hierarchy;

    // Start rendering from root
    assert(tree.size() > 0);
    mesh cylinder = create_branch(tree[0].start_width, tree[0].end_width, tree[0].len);
    hierarchy.add_element(cylinder, "0", "root");

    std::queue<int> to_treat;
    to_treat.push(0);

    while(!to_treat.empty()) {
        int idx = to_treat.front();
        to_treat.pop();
        element elem = tree[idx];

        //add children
        for(int c : elem.fils) {
            element child = tree[c];

            if(only_branch && child.type != Branch)
                continue;

            hierarchy.add_element(create_branch(child.start_width, child.end_width, child.len),
                                    std::to_string(c),
                                    std::to_string(idx),
                                    {0, 0, elem.len},
                                    child.orientation
            );
            
            to_treat.push(c);
        }
        
    }

    return hierarchy;
}

mesh_drawable_hierarchy monopodial::generate(int num_iter) {
    // Set up seed
    const float cyl_length = 1;
    const float cyl_radius = 0.1;

    element root;
    root.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, 0);
    root.start_width = cyl_radius;
    root.end_width = cyl_radius;
    root.type = A;
    root.len = cyl_length;

    std::vector<element> tree;
    tree.push_back(root);

    monopodial::create_struct(tree, num_iter);

    return create_mesh(tree);
}

void monopodial::create_struct(std::vector<element> &tree, int num_iter) {
    //Set up constants
    const float width_dec = 0.707;
    const float br_len_dec = 0.8;
    const float tr_len_dec = 0.9;
    const float a0 = 3.14/4;
    const float a2 = 3.14/4;
    const float d = 137.5 * 3.14 / 180;


    std::uniform_real_distribution<float> dist_lim_theta(3.14/4, 3.14/2);
    std::uniform_real_distribution<float> dist_theta(0, 3.14/2);
    std::uniform_real_distribution<float> dist_phi(0, 2*3.14);
    std::uniform_real_distribution<float> binary(0, 1);
    std::uniform_real_distribution<float> noise(-3.14/6, 3.14/6);

    std::random_device rd;
    std::default_random_engine gen(rd());

    for(int k = 0; k < num_iter; k++) {
        int n = tree.size();

        // Choose index of element to subdivide
        for(int idx = 0; idx < n; idx++) {
            //std::uniform_int_distribution<int> d(0, n-1);
            //int idx = d(gen);

            element elem = tree[idx];

            float b = noise(gen);

            switch(elem.type) {
                case A: {
                    elem.type = Branch;

                    element child1;
                    child1.type = B;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * width_dec;
                    child1.len = elem.len * br_len_dec;
                    child1.orientation = rotation_from_axis_angle_mat3({1, 0, 0}, a0 + noise(gen));
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = A;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * width_dec;
                    child2.len = elem.len * tr_len_dec;
                    child2.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, d + noise(gen));
                    elem.fils.push_back(tree.size());
                    tree.push_back(child2);
                    
                    break;
                }
                case B: {
                    elem.type = Branch;

                    element child1;
                    child1.type = C;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * width_dec;
                    child1.len = elem.len * br_len_dec;
                    child1.orientation = rotation_from_axis_angle_mat3({0, 1, 0}, -a2 + noise(gen));
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = C;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * width_dec;
                    child2.len = elem.len * tr_len_dec;
                    child2.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, b);
                    elem.fils.push_back(tree.size());
                    tree.push_back(child2);

                    break;
                }
                case C: {
                    elem.type = Branch;

                    element child1;
                    child1.type = B;
                    child1.start_width = elem.end_width;
                    child1.end_width = child1.start_width * width_dec;
                    child1.len = elem.len * br_len_dec;
                    child1.orientation = rotation_from_axis_angle_mat3({0, 1, 0}, a2 + noise(gen));
                    elem.fils.push_back(tree.size());
                    tree.push_back(child1);

                    element child2;
                    child2.type = B;
                    child2.start_width = elem.end_width;
                    child2.end_width = child2.start_width * width_dec;
                    child2.len = elem.len * tr_len_dec;
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


mesh_drawable_hierarchy ternary::generate(int num_iter) {
    // Set up seed
    const float cyl_length = 1;
    const float cyl_radius = 0.1;

    element root;
    root.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, 0);
    root.start_width = cyl_radius;
    root.end_width = cyl_radius;
    root.type = Branch;
    root.len = cyl_length;
    root.vector = {0, 0, 1};
    root.fils.push_back(1);

    element apex;
    apex.type = A;
    apex.start_width = cyl_radius;
    apex.end_width = 0;
    apex.len = cyl_length;
    apex.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, 3.14/4);
    apex.vector = apex.orientation * root.vector;

    std::vector<element> tree;
    tree.push_back(root);
    tree.push_back(apex);

    ternary::create_struct(tree, num_iter);

    return create_mesh(tree, true);
}

void ternary::create_struct(std::vector<element> &tree, int num_iter) {
    //Set up constants
    const float width = 0.1;
    const float width_inc = 1.1;
    const float len = 0.5;
    const float len_inc = 1.1;
    const float a = 18.95 * 3.14/180;
    const float d1 = 94.74 * 3.14 / 180;
    const float d2 = 132.63 * 3.14 / 180;

    std::uniform_real_distribution<float> noise(-3.14/6, 3.14/6);

    std::random_device rd;
    std::default_random_engine gen(rd());

    for(int k = 0; k < num_iter; k++) {
        int n = tree.size();

        // Choose index of element to subdivide
        for(int idx = 0; idx < n; idx++) {
            //std::uniform_int_distribution<int> d(0, n-1);
            //int idx = d(gen);

            element elem = tree[idx];

            switch(elem.type) {
                case A: {
                    elem.type = Branch;
                    elem.start_width = width * width_inc;
                    elem.end_width = width;
                    elem.len = len;

                    element child;
                    child.type = A;
                    child.start_width = width;
                    child.end_width = 0;
                    child.len = len;
                    child.orientation = rotation_from_axis_angle_mat3({1, 0, 0}, a);
                    child.vector = child.orientation * elem.vector;
                    elem.fils.push_back(tree.size());
                    tree.push_back(child);

                    child.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, d1) *
                                        rotation_from_axis_angle_mat3({1, 0, 0}, a);
                    child.vector = child.orientation * elem.vector;
                    elem.fils.push_back(tree.size());
                    tree.push_back(child);

                    child.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, d1 + d2) *
                                        rotation_from_axis_angle_mat3({1, 0, 0}, a);
                    child.vector = child.orientation * elem.vector;
                    elem.fils.push_back(tree.size());
                    tree.push_back(child);
                    
                    break;
                }
                case Branch : {
                    elem.start_width *= width_inc;
                    elem.end_width *= width_inc;
                    elem.len *= len_inc;
                }
                default:
                    break;
            }            

            tree[idx] = elem;

        }

    }
}