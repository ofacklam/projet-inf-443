
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
    v_theta  = 0.5f;
    move = 0;
    pos_joueur = {move, r, 0};

    timer.scale = 0.2f;
    rayon = 51.0f;

    //Set up level
    nb_obstacles = 50;

    level.setup<arbre>(new terrain(), nb_obstacles);
    player.setup();
    sky.setup(  "data/ely_hills/hills_ft.png",
                "data/ely_hills/hills_bk.png",
                "data/ely_hills/hills_lf.png",
                "data/ely_hills/hills_rt.png",
                "data/ely_hills/hills_up.png",
                "data/ely_hills/hills_dn.png");
    sky_wireframe.setup(  "data/ame_nebula/purplenebula_ft.png",
                "data/ame_nebula/purplenebula_bk.png",
                "data/ame_nebula/purplenebula_lf.png",
                "data/ame_nebula/purplenebula_rt.png",
                "data/ame_nebula/purplenebula_up.png",
                "data/ame_nebula/purplenebula_dn.png");
    texture_lost = texture_gpu(image_load_png("data/lost.png"));
    lost = mesh_primitive_quad({-1,1,0},{1,1,0},{1,-1,0},{-1,-1,0});
    lvlup = mesh_primitive_quad({-8,rayon+7,0},{8,rayon+7,0},{8,rayon-2,0},{-8,rayon-2,0});
    lvlup.uniform_parameter.shading.specular = 0.0f;
    lvlup.uniform_parameter.shading.ambiant = 1.0f;
    texture_lvlup = texture_gpu(image_load_png("data/lvlup.png"));
}


/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */
void scene_exercise::frame_calc(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui)
{
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe

    float dt = timer.update();
    float d_theta = v_theta * dt;
    if(theta > 2*3.14) {
        gui_scene.wireframe = !gui_scene.wireframe;
        v_theta += 0.05f;
        theta -= 2*3.14;
        nb_obstacles += 5;
        level.generate_positions(nb_obstacles);
    }

    if(!end_game) {
        

        move_camera(scene);
        
        theta += d_theta;

        if(left) move -= vitesse * dt;
        if(right) move += vitesse * dt;

        level.draw(shaders, scene, pos_joueur, gui_scene.wireframe);

        float x = move;
        float y =  rayon*std::cos(theta -d_theta+ 3.14f*0.01);
        float z = -rayon*std::sin(theta -d_theta+ 3.14f*0.01);

        old_pos = pos_joueur;

        const float alpha = 0.05;
        pos_joueur = {alpha * x + (1-alpha) * pos_joueur.x, y, z};
        player.draw(shaders, scene, gui_scene.wireframe, pos_joueur, theta, 30 * (pos_joueur.x - old_pos.x));

        //end_game = level.collision(pos_joueur, 1);
    }
    else {
        timer.t = 0;
        while(timer.t < 0.8) timer.update();
        glfwSetWindowShouldClose(gui.window, 1);
    }
}

void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui)
{
    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe

    glBindTexture(GL_TEXTURE_2D, texture_lvlup);
    lvlup.draw(shaders["mesh"], scene.camera);

    if(!end_game) {
        if(!gui_scene.wireframe) sky.draw(shaders, scene);
        if(gui_scene.wireframe)  sky_wireframe.draw(shaders,scene);

        level.draw(shaders, scene, pos_joueur, gui_scene.wireframe);

        player.draw(shaders, scene, gui_scene.wireframe, pos_joueur, theta, 30 * (pos_joueur.x - old_pos.x));

        //end_game = level.collision(pos_joueur, 1);
    }
    else {
        lost.uniform_parameter.rotation = rotation_from_axis_angle_mat3({-1,0,0},theta);
        lost.uniform_parameter.translation = pos_joueur - vec3(pos_joueur.x,0,0);
        glBindTexture(GL_TEXTURE_2D, texture_lost);
        lost.draw(shaders["mesh"], scene.camera);
    }
}


void scene_exercise::move_camera(scene_structure& scene) {
    const float retard = 0.25f;
    const float r = 51.0f;

    const float z0 = r*std::sin(theta);
    const float zl = 1.5*r*std::sin(theta-retard);

    const float y0 = -r*std::cos(theta);
    const float yl = -1.5*r*std::cos(theta-retard);

    const float x0 = scene.camera.translation[0];

    scene.camera.orientation = rotation_from_axis_angle_mat3({-1, 0, 0}, theta);
    scene.camera.light_orientation = rotation_from_axis_angle_mat3({-1, 0, 0}, theta + 3.14/2 - 3.14/3);// * rotation_from_axis_angle_mat3({0, 1, 0}, -3.14/6);
    scene.camera.translation = vec3(x0, y0, z0);
    scene.camera.light_translation = vec3(x0, yl, zl); //+ vec3(30, 0, 0);
    
}

vec3 scene_exercise::linear_interpolation(float t, float t1, float t2, const vec3& p1, const vec3& p2) {
    const float alpha = (t-t1)/(t2-t1);
    const vec3 p = (1-alpha)*p1 + alpha*p2;

    return p;
}