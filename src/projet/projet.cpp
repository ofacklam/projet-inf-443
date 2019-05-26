
#include "projet.hpp"


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;



/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{
    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    //scene.camera.apply_rotation(0,0,0,1.8f);
    scene.camera.translation = {0.0f, 0.0f, 0.0f};
    theta = 0;
    move = 0;

    //Set up level
    level.setup(new arbre(), new terrain());
    player.setup();
<<<<<<< HEAD
    sky.setup("data/ely_hills/hills_ft.png","data/ely_hills/hills_bk.png","data/ely_hills/hills_lf.png","data/ely_hills/hills_rt.png","data/ely_hills/hills_up.png","data/ely_hills/hills_dn.png");

=======
    sky.setup(  "data/ely_hills/hills_ft.png",
                "data/ely_hills/hills_bk.png",
                "data/ely_hills/hills_lf.png",
                "data/ely_hills/hills_rt.png",
                "data/ely_hills/hills_up.png",
                "data/ely_hills/hills_dn.png");
>>>>>>> 0d94b7850edb26abd927c65a1fb2f84bcc318b94
}


/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui)
{
    set_gui();
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe
    move_camera(scene);
    theta = theta + d_theta;

    sky.draw(shaders, scene);

<<<<<<< HEAD
    const float z = 10*std::cos(theta + 3.14f*0.4);
    const float y = 10*std::sin(theta + 3.14f*0.4);
    const float x = move;
    level.draw(shaders, scene, gui_scene.wireframe);
    vec3 pos_joueur = vec3(x,y,z);
    player.draw(shaders, scene, gui_scene.wireframe, pos_joueur, theta); 
=======
    level.draw(shaders, scene, gui_scene.wireframe);
    //player.draw(shaders, scene, gui_scene.wireframe); 
>>>>>>> 0d94b7850edb26abd927c65a1fb2f84bcc318b94
    
    /*t.draw(shaders, scene, gui_scene.wireframe);

    o.draw(shaders, scene, vec3(), mat3(), vec3(1, 1, 1), gui_scene.wireframe);*/
}

void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
}

void scene_exercise::move_camera(scene_structure& scene) {
    const float z0 = 11.0f*std::sin(theta);
    const float y0 = -11.0f*std::cos(theta);
    const float x0 = scene.camera.translation[0];

    scene.camera.orientation = rotation_from_axis_angle_mat3({-1, 0, 0}, theta);
    scene.camera.translation = vec3(x0, y0, z0);
    
}

