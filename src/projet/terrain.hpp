#pragma once

#include "../base_exercise/base_exercise.hpp"
#include <cmath>
#include <random>

float normalize(float u);
float evaluate_terrain_z(float u, float v);
vcl::vec3 evaluate_terrain(float u, float v);
vcl::mesh create_terrain();
vcl::vec3 gaussienne_canyon(float u, float v);
vcl::mesh create_canyon();

const float r = 9.0f;