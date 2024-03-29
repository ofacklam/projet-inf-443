#pragma once

#include "vcl/vcl.hpp"
#include <map>

struct scene_structure
{
    vcl::camera_scene camera;
    vcl::camera_control_glfw camera_control;
    vcl::mesh_drawable frame_camera;
    vcl::mesh_drawable frame_worldspace;
    GLuint texture_white;
};


struct gui_structure
{
    GLFWwindow* window;
    std::string window_title;

    bool godmode     = false;
};


GLFWwindow* create_window(const std::string& window_title);
void initialize_interface(gui_structure& gui);
void load_shaders(std::map<std::string,GLuint>& shaders);
void setup_scene(scene_structure &scene, gui_structure& gui);
void clear_screen();
void update_fps_title(GLFWwindow* window, const std::string& title, vcl::glfw_fps_counter& fps_counter);
void gui_start_basic_structure(gui_structure& gui);
 