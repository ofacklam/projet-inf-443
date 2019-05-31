#include "terrain.hpp"

using namespace vcl;

void terrain::setup() {
    // Create visual terrain surface
    terrain_ = create_terrain();
    canyon_ = create_canyon();
    terrain_.uniform_parameter.shading.specular = 0.0f; // non-specular terrain material
    canyon_.uniform_parameter.shading.specular = 0.0f;
    texture_canyon = texture_gpu(image_load_png("data/maxresdefault.png"));
    texture_sol = texture_gpu(image_load_png("data/grass.png"));
    texture_lune = texture_gpu(image_load_png("data/moon.png"));
}

void terrain::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe) {
    // Display terrain
    glPolygonOffset( 1.0, 1.0 );
    if(!wireframe){
        terrain_.uniform_parameter.color = {0.6f,0.85f,0.5f};
        terrain_.uniform_parameter.shading.ambiant = 0.3f;
        glBindTexture(GL_TEXTURE_2D, texture_sol);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        terrain_.draw(shaders["mesh"], scene.camera);
        glBindTexture(GL_TEXTURE_2D, texture_canyon);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        canyon_.draw(shaders["mesh"], scene.camera);
        glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    } else { // wireframe if asked from the GUI
        glPolygonOffset( 1.0, 1.0 );
        terrain_.uniform_parameter.color = {1.0f,1.0f,1.0};
        terrain_.uniform_parameter.shading.ambiant = 0.0f;
        glBindTexture(GL_TEXTURE_2D, texture_lune);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        terrain_.draw(shaders["mesh"], scene.camera);
        canyon_.draw(shaders["wireframe"], scene.camera);
    }
}


//Takes x e [0,1] and returns u e [-10,10]
float normalize(float x) {
    return 20*(x-0.5f);
}

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
vec3 evaluate_terrain(float theta, float v)
{
    // Evaluate Perlin noise
    const float scaling = 0.2f;
    const int octave = 4;
    const float persistency = 0.5f;
    const float noise = perlin(scaling*r*std::cos(theta), scaling*r*v, scaling*r*std::sin(theta), octave, persistency);

    const float x = normalize(v)/2;
    const float y = (r+0.2*noise)*std::cos(theta);
    float z = (r+0.2*noise)*std::sin(theta);

    return {x,y,z};
}

// Generate terrain mesh
mesh create_terrain()
{
    // Number of samples of the terrain is N x (N+1)
    const size_t N = 1000;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*(N+1));
    terrain.texture_uv.resize(N*(N+1));
    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float theta = 2*3.14*ku/(N-1.0f);
            const float u = ku/(N-1.0f); 
            const float v = kv/(N-1.0f);

            // Compute coordinates
            terrain.position[kv+N*ku] = evaluate_terrain(theta,v);
            terrain.texture_uv[kv+N*ku] = vec2(50*u,10*v);
        }
    }
    for(size_t kv=0; kv<N; ++kv) {
        terrain.position[kv + N*N] = terrain.position[kv];
        terrain.texture_uv[kv + N*N] = terrain.texture_uv[kv];
    }


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
    const float radius = 44.0f + std::exp(5*(v-0.4f)) + std::exp(5*(0.6f-v));

    const float scaling = 2.0f;
    const int octave = 4;
    const float persistency = 0.5f;
    float noise = perlin(scaling*std::cos(theta), scaling*v, scaling*std::sin(theta), octave, persistency);
    
    const float x = normalize(v);
    const float y = (radius+noise)*std::cos(theta);
    float z = (radius+noise)*std::sin(theta);

    return {x,y,z};
}

mesh create_canyon() {
    const size_t N = 500;
    mesh canyon;
    canyon.position.resize(N*(N+1));
    canyon.texture_uv.resize(N*(N+1));

    for(size_t ku=0; ku<(N+1); ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float theta = 2*3.14*u;
            const float v = kv/(N-1.0f);

            // Compute coordinates
            canyon.position[kv+N*ku] = gaussienne_canyon(theta,v);
            canyon.texture_uv[kv+N*ku] = vec2(50*u, 50*v);
        }
    }


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
    return canyon;
}
