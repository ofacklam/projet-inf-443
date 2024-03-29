#include "helper_scene.hpp"
#include <GLFW/glfw3.h>

using namespace vcl;




GLFWwindow* create_window(const std::string& window_title)
{
    const int opengl_version_major = 3;
    const int opengl_version_minor = 3;

    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    GLFWwindow* window = vcl::glfw_create_window(mode->width, mode->height, window_title, opengl_version_major, opengl_version_minor,glfwGetPrimaryMonitor());
    return window;
}

void initialize_interface(gui_structure& gui)
{
    std::cout<<"*** Init GLFW ***"<<std::endl;
    vcl::glfw_init();
    std::cout<<"\t [OK] GLFW Initialized"<<std::endl;


    std::cout<<"*** Create window ***"<<std::endl;
    gui.window_title = "OpenGL Window";
    gui.window = create_window(gui.window_title);
    std::cout<<"\t [OK] Window Created"<<std::endl;

    std::cout<<"*** Init GLAD ***"<<std::endl;
    vcl::glad_init();
    std::cout<<"\t [OK] GLAD Initialized"<<std::endl;

    std::cout<<"*** OPENGL Information ***"<<std::endl;
    std::cout<<"======================================================="<<std::endl;
    vcl::opengl_debug_print_version();
    std::cout<<"======================================================="<<std::endl;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    std::cout<<"*** Init imgui ***"<<std::endl;
    imgui_init(gui.window);
    std::cout<<"\t [OK] imgui Initialized"<<std::endl;
}

void load_shaders(std::map<std::string,GLuint>& shaders)
{
    std::cout<<"*** Setup Shader ***"<<std::endl;

    shaders["mesh"] = create_shader_program("shaders/mesh/shader.vert.glsl","shaders/mesh/shader.frag.glsl");
    shaders["wireframe"] = create_shader_program("shaders/wireframe/shader.vert.glsl","shaders/wireframe/shader.geom.glsl","shaders/wireframe/shader.frag.glsl");
    shaders["curve"] = create_shader_program("shaders/curve/shader.vert.glsl","shaders/curve/shader.frag.glsl");
    shaders["segment_im"] = create_shader_program("shaders/segment_immediate_mode/shader.vert.glsl","shaders/segment_immediate_mode/shader.frag.glsl");
    shaders["shadows"] = create_shader_program("shaders/shadow/shader.vert.glsl", "shaders/shadow/shader.frag.glsl");
    shaders["debug"] = create_shader_program("shaders/debug/shader.vert.glsl", "shaders/debug/shader.frag.glsl");

    std::cout<<"\t [OK] Shader loaded"<<std::endl;
}

void setup_scene(scene_structure &scene, gui_structure& gui)
{
    scene.frame_camera = mesh_drawable(mesh_primitive_frame(0.15f, 0.05f, 0.15f, 0.3f));
    scene.frame_camera.uniform_parameter.scaling = 0.2f;

    scene.frame_worldspace = mesh_drawable(mesh_primitive_frame(0.05f, 0.015f, 0.05f, 0.1f));

    int width=0, height=0;
    glfwGetWindowSize(gui.window, &width, &height);
    const float aspect_ratio = width/static_cast<float>(height);

    scene.camera.perspective = perspective_structure( 70*3.14f/180, aspect_ratio, 0.01f, 500.0f);
    scene.camera.light_perspective = perspective_structure(50 * 3.14 / 180, 1, 0.05, 60);

    const image white{1,1,{255,255,255,255}};
    scene.texture_white = texture_gpu(white);
}

void clear_screen()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
}

void update_fps_title(GLFWwindow* window, const std::string& title, glfw_fps_counter& fps_counter)
{
    if ( fps_counter.update() )
    {
        const std::string new_window_title = title+" ("+std::to_string(fps_counter.fps())+" fps)";
        glfwSetWindowTitle(window, new_window_title.c_str());
        fps_counter.reset();
    }
}

void gui_start_basic_structure(gui_structure& gui)
{
    imgui_create_frame();

    ImGui::Begin("Si vous etes nuls cliquez ici",NULL,ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("GOD MODE                    ", &gui.godmode);
}
