#include "terrain.hpp"

using namespace vcl;

//Takes x e [0,1] and returns u e [-10,10]
float normalize(float x) {
    return 20*(x-0.5f);
}

// Evaluate height of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_z(float u, float v)
{
    float z = 0;
    
    return z;
}

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
vec3 evaluate_terrain(float u, float v, float &c)
{
    const float x = normalize(u);
    const float y = normalize(v);
    float z = evaluate_terrain_z(u,v);
    // Evaluate Perlin noise
    const float scaling = 4.0f;
    const int octave = 4;
    const float persistency = 0.5f;
    const float noise = perlin(scaling*u, scaling*v, octave, persistency);
    z += noise;
    c = 0.5f+0.5f*noise;

    return {x,y,z};
}

// Generate terrain mesh
mesh create_terrain()
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // Compute coordinates
            float c;
            terrain.position[kv+N*ku] = evaluate_terrain(u,v,c);
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for(unsigned int ku=0; ku<Ns-1; ++ku)
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

vec3 gaussienne_canyon(float u, float v) {
    const float x = normalize(u);
    const float y = normalize(v);
    const float z = std::exp(x-5) + std::exp(5-x) -0.2f;
    return {x,y,z};
}

mesh create_canyon() {
    const size_t N = 100;
    //Cote gauche

    //Cote droit
}
