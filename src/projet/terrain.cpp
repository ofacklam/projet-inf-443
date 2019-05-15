#include "terrain.hpp"

using namespace vcl;

//Takes x e [0,1] and returns u e [-10,10]
float normalize(float x) {
    return 20*(x-0.5f);
}

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
vec3 evaluate_terrain(float theta, float v)
{
    // Evaluate Perlin noise
    const float scaling = 2.0f;
    const int octave = 4;
    const float persistency = 0.5f;
    const float noise = perlin(scaling*std::cos(theta), scaling*v, scaling*std::sin(theta), octave, persistency);

    const float x = normalize(v);
    const float y = (r+0.2*noise)*std::cos(theta);
    float z = (r+0.2*noise)*std::sin(theta);

    return {x,y,z};
}

// Generate terrain mesh
mesh create_terrain()
{
    // Number of samples of the terrain is N x N
    const size_t N = 500;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*(N+1));

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float theta = 2*3.14*ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            terrain.position[kv+N*ku] = evaluate_terrain(theta,v);
        }
    }
    for(size_t kv=0; kv<N; ++kv)
        terrain.position[kv + N*N] = terrain.position[kv];


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for(unsigned int ku=0; ku<Ns; ++ku)
    {
        for(unsigned int kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + N*ku; // current vertex offset

            const index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}

vec3 gaussienne_canyon(float theta, float v) {
    const float radius = 4.0f + std::exp(5*(v-0.5f)) + std::exp(5*(0.5f-v));

    const float scaling = 2.0f;
    const int octave = 4;
    const float persistency = 0.5f;
    float noise = perlin(scaling*std::cos(theta), scaling*v, scaling*std::sin(theta), octave, persistency);
    //noise = (u>0.5f) ? -noise : noise;
    
    const float x = normalize(v);
    const float y = (radius+noise)*std::cos(theta);
    float z = (radius+noise)*std::sin(theta);

    return {x,y,z};
}

mesh create_canyon() {
    const size_t N = 500;
    mesh canyon;
    canyon.position.resize(N*(N+1));

    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float theta = 2*3.14*ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            canyon.position[kv+N*ku] = gaussienne_canyon(theta,v);
        }
    }
    for(size_t kv=0; kv<N; ++kv)
        canyon.position[kv + N*N] = canyon.position[kv];


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for(unsigned int ku=0; ku<Ns; ++ku)
    {
        for(unsigned int kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + N*ku; // current vertex offset

            const index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            canyon.connectivity.push_back(triangle_1);
            canyon.connectivity.push_back(triangle_2);
        }
    }
    /*
    std::uniform_real_distribution<float> distrib(0.5,1.0);
    std::default_random_engine generator;

    for(int i = 0; i<10; i++) {
        mesh_drawable patate = mesh_primitive_sphere(distrib(generator));
        patate.uniform_parameter.translation = {i,2.0f,0.0f};
        canyon.push_back(patate);
    }
*/
    return canyon;
}
