#pragma once

#include "../base_exercise/base_exercise.hpp"
#include "obstacle.hpp"
#include "arbre.hpp"
#include "terrain.hpp"
#include "niveau.hpp"
#include "joueur.hpp"
#include "skybox.hpp"


// Stores some parameters that can be set from the GUI
struct gui_scene_structure
{
    bool wireframe;
};

struct scene_exercise : base_scene_exercise
{

    /** A part must define two functions that are called from the main function:
     * setup_data: called once to setup data before starting the animation loop
     * frame_draw: called at every displayed frame within the animation loop
     *
     * These two functions receive the following parameters
     * - shaders: A set of shaders.
     * - scene: Contains general common object to define the 3D scene. Contains in particular the camera.
     * - data: The part-specific data structure defined previously
     * - gui: The GUI structure allowing to create/display buttons to interact with the scene.
    */

    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void set_gui();
    void move_camera(scene_structure& scene);

    // visual representation of a level
    niveau level;
    joueur player;
    Skybox sky;

    float theta; //Angle pour affichage de la camera et progression dans le niveau
    const float d_theta = 0.005f;
    float move; //Contient la translation avec les fleches

    gui_scene_structure gui_scene;
};



