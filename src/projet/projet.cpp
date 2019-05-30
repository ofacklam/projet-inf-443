
#include "projet.hpp"


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;



/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{
    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 1.0f;
    scene.camera.translation = {0.0f, 0.0f, 0.0f};


    theta = 0;
    move = 0;
    pos_joueur = {move, r, 0};
    keyframe_time = {0,1};

    timer.t_min = keyframe_time[0];
    timer.t_max = keyframe_time[1];
    timer.t = timer.t_min;
    timer.scale = 0.2f;
    rayon = 51.0f;

    //Set up level
    level.setup(new arbre(), new terrain());
    player.setup();
    sky.setup(  "data/ely_hills/hills_ft.png",
                "data/ely_hills/hills_bk.png",
                "data/ely_hills/hills_lf.png",
                "data/ely_hills/hills_rt.png",
                "data/ely_hills/hills_up.png",
                "data/ely_hills/hills_dn.png");
}


/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui)
{
    set_gui();
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe

    if(!end_game) {
        move_camera(scene);
        theta = theta + d_theta;

        sky.draw(shaders, scene);
        timer.update();
        const float t = timer.t;
        const float r = rayon + 0.1f*std::sin(2*3.14*t);
        keyframe_position = {pos_joueur,{move, r*std::cos(theta + 3.14f*0.01),-r*std::sin(theta + 3.14f*0.01)}};
        level.draw(shaders, scene, pos_joueur, gui_scene.wireframe);
        pos_joueur = linear_interpolation(t,keyframe_time[0],keyframe_time[1],keyframe_position[0],keyframe_position[1]);
        player.draw(shaders, scene, gui_scene.wireframe, pos_joueur, theta); 
        
        /*mesh_drawable test = mesh_primitive_parallelepiped();
        test.uniform_parameter.translation = gaussienne_canyon(-theta, 0.15);
        test.uniform_parameter.scaling = 2;
        test.draw(shaders["mesh"], scene.camera);*/

        //end_game = level.collision(pos_joueur, 1);
    }
    else {
        //glfwSetWindowShouldClose(gui.window, 1);
    }
}

void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
}

void scene_exercise::move_camera(scene_structure& scene) {
    const float r = 51.0f;
    const float z0 = r*std::sin(theta);
    const float y0 = -r*std::cos(theta);
    const float x0 = scene.camera.translation[0];

    scene.camera.orientation = rotation_from_axis_angle_mat3({-1, 0, 0}, theta);
    scene.camera.translation = vec3(x0, y0, z0);
    
}

vec3 scene_exercise::linear_interpolation(float t, float t1, float t2, const vec3& p1, const vec3& p2) {
    const float alpha = (t-t1)/(t2-t1);
    const vec3 p = (1-alpha)*p1 + alpha*p2;

    return p;
}