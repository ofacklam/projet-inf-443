
// Include VCL library
#include "vcl/vcl.hpp"

// Include common part for exercises
#include "helper_scene.hpp"

// Include exercises
#include "projet/projet.hpp"




// ************************************** //
// Global data declaration
// ************************************** //

// Storage for shaders indexed by their names
std::map<std::string,GLuint> shaders;

// General shared elements of the scene such as camera and its controler, visual elements, etc
scene_structure scene;

// The graphical interface. Contains Window object and GUI related variables
gui_structure gui;

// Part specific data - you will specify this object in the corresponding exercise part
scene_exercise exercise;

// Frame buffer and depth map
unsigned int depthMapFBO;
uint depthMap;
const unsigned int SHADOW_WIDTH = 10000, SHADOW_HEIGHT = 10000;


// ************************************** //
// GLFW event listeners
// ************************************** //

void window_size_callback(GLFWwindow* /*window*/, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


// ************************************** //
// Start program
// ************************************** //

void framebuffer_setup();
void calculate_shadows();
void render_scene();

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

int main()
{

    // ************************************** //
    // Initialization and data setup
    // ************************************** //

    // Initialize external libraries and window
    initialize_interface(gui);

    // Set GLFW events listener
    glfwSetCursorPosCallback(gui.window, cursor_position_callback );
    glfwSetMouseButtonCallback(gui.window, mouse_click_callback);
    glfwSetWindowSizeCallback(gui.window, window_size_callback);
    glfwSetKeyCallback(gui.window, key_callback);

    load_shaders(shaders);
    setup_scene(scene, gui);

    framebuffer_setup();

    opengl_debug();
    std::cout<<"*** Setup Data ***"<<std::endl;
    exercise.setup_data(shaders, scene, gui);
    std::cout<<"\t [OK] Data setup"<<std::endl;
    opengl_debug();


    // ************************************** //
    // Animation loop
    // ************************************** //



    std::cout<<"*** Start GLFW loop ***"<<std::endl;
    vcl::glfw_fps_counter fps_counter;
    while( !glfwWindowShouldClose(gui.window) )
    {
        opengl_debug();

        // Create the basic gui structure with ImGui
        gui_start_basic_structure(gui,scene, shaders);

        // Perform computation and draw calls for each iteration loop
        calculate_shadows();
        render_scene();

        /*// render Depth map to quad for visual debugging
        // ---------------------------------------------
        clear_screen();opengl_debug();
        glUseProgram(shaders["debug"]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUniform1i(glGetUniformLocation(shaders["debug"], "depthMap"), 0);
        renderQuad();*/

        // Render GUI and update window
        ImGui::End();
        scene.camera_control.update = !(ImGui::IsAnyWindowFocused());
        vcl::imgui_render_frame(gui.window);

        update_fps_title(gui.window, gui.window_title, fps_counter);

        glfwSwapBuffers(gui.window);
        glfwPollEvents();
        opengl_debug();

    }
    std::cout<<"*** Stop GLFW loop ***"<<std::endl;

    // Cleanup ImGui and GLFW
    vcl::imgui_cleanup();

    glfwDestroyWindow(gui.window);
    glfwTerminate();

    return 0;
}

void framebuffer_setup() {
    //Create framebuffer to generate frame to
    glGenFramebuffers(1, &depthMapFBO);

    //Create depth map
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
                SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    //Attach to frame_buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
}

void calculate_shadows() {

    // Render to depth map
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    // Clear all color and zbuffer information before drawing on the screen
    clear_screen();opengl_debug();
    glCullFace(GL_FRONT);

    // Change shaders
    GLuint mesh_shader = shaders["mesh"];
    shaders["mesh"] = shaders["shadows"];
    exercise.frame_draw(shaders, scene, gui); opengl_debug();
    

    //Reset everything
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Set a white image texture by default
    glBindTexture(GL_TEXTURE_2D,scene.texture_white);
    // Set viewport to window size
    int width, height;
    glfwGetWindowSize(gui.window, &width, &height);
    glViewport(0, 0, width, height);
    glCullFace(GL_BACK);
    // Shaders
    shaders["mesh"] = mesh_shader;
}

void render_scene() {
    // Clear all color and zbuffer information before drawing on the screen
    clear_screen();opengl_debug();

    //Bind texture
    glActiveTexture(GL_TEXTURE1); //activate depth texture
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glActiveTexture(GL_TEXTURE0); //reactivate normal texture

    //Render scene
    exercise.frame_draw(shaders, scene, gui); opengl_debug();
}

void window_size_callback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
    scene.camera.perspective.image_aspect = width / static_cast<float>(height);;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    scene.camera_control.update_mouse_move(scene.camera, window, float(xpos), float(ypos));
    exercise.mouse_move(scene, window);
}
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGui::SetWindowFocus(nullptr);
    scene.camera_control.update_mouse_click(scene.camera, window, button, action, mods);
    exercise.mouse_click(scene, window,button,action,mods);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float increment = 0.05f;
    if (key == GLFW_KEY_LEFT)
        exercise.move -= increment;
    if (key == GLFW_KEY_RIGHT)
        exercise.move += increment;
}
