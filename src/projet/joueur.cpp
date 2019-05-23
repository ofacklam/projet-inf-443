#include "joueur.hpp"
#include "OBJ_Loader.h"

using namespace vcl;


void joueur::setup() {
    objl::Loader loader;
    loader.LoadFile("data/Dragon/Dragon/Dragon.obj");
    mesh player_;
    player_.position = loader.LoadedMeshes[0].Vertices;
    player_.connectivity = loader.LoadedMeshes[0].Indices;
}

void joueur::draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, bool wireframe) {

    glPolygonOffset( 1.0, 1.0 );
    player.draw(shaders["mesh"], scene.camera);

    if(wireframe) {
        glPolygonOffset( 1.0, 1.0 );
        player.draw(shaders["wireframe"], scene.camera);
    }
}