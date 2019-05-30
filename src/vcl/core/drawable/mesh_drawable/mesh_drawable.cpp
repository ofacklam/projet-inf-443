#include "mesh_drawable.hpp"

#include "vcl/core/opengl/opengl.hpp"

namespace vcl
{

mesh_drawable::mesh_drawable()
    :uniform_parameter(),data_gpu()
{}

mesh_drawable::mesh_drawable(const mesh_gpu& data)
    :uniform_parameter(),data_gpu(data)
{}
mesh_drawable::mesh_drawable(const mesh& data)
    :uniform_parameter(),data_gpu(mesh_gpu(data))
{}

void mesh_drawable::draw(GLuint shader, const camera_scene& camera)
{
    opengl_debug();
    GLint current_shader = 0; opengl_debug();
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_shader); opengl_debug();

    if(shader!=GLuint(current_shader))
        glUseProgram(shader); opengl_debug();



    uniform(shader, "rotation", uniform_parameter.rotation); opengl_debug();
    uniform(shader, "translation", uniform_parameter.translation); opengl_debug();
    uniform(shader, "color", uniform_parameter.color); opengl_debug();
    uniform(shader, "scaling", uniform_parameter.scaling); opengl_debug();
    uniform(shader, "scaling_axis", uniform_parameter.scaling_axis); opengl_debug();

    uniform(shader,"perspective",camera.perspective.matrix()); opengl_debug();
    uniform(shader,"view",camera.view_matrix()); opengl_debug();
    uniform(shader,"camera_position",camera.camera_position()); opengl_debug();

    camera_scene light_cam(camera);
    light_cam.translation = 2 * camera.translation + vec3(30, 0, 0);
    light_cam.orientation = rotation_from_axis_angle_mat3({0, 0, 1}, 3.14/6) * rotation_from_axis_angle_mat3({1, 0, 0}, -3.14/2) * camera.orientation;
    light_cam.perspective = perspective_structure(70 * 3.14 / 180, 1, 0.1, 100);
    uniform(shader,"light_perspective",light_cam.perspective.matrix()); opengl_debug();
    uniform(shader,"light_view",light_cam.view_matrix()); opengl_debug();
    uniform(shader,"light",light_cam.camera_position()); opengl_debug();

    //Set up textures
    uniform(shader, "texture_sampler", 0);
    uniform(shader, "shadow_sampler", 1);



    uniform(shader, "ambiant", uniform_parameter.shading.ambiant); opengl_debug();
    uniform(shader, "diffuse", uniform_parameter.shading.diffuse); opengl_debug();
    uniform(shader, "specular", uniform_parameter.shading.specular); opengl_debug();

    vcl::draw(data_gpu);opengl_debug();
}

}
