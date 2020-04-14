#ifndef H_DEPS
#define H_DEPS

#include <cstdio>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>

#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES      1

#include <reactphysics3d.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "gl_utils/gl_debug.h"
#include "gl_utils/shader_utils.h"
#include "gl_utils/vec.h"
#include "gl_utils/obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "gl_utils/stb_image.h"
#include "keyboard.h"
#include "camera.h"
#include "CameraRayCaster.h"
#include "Rendering/Textures/_deps.h"
#include "Rendering/Framebuffers/_deps.h"
#include "Rendering/Shaders/_deps.h"
#include "Rendering/Meshes/_deps.h"
#include "Rendering/ResourceRepo.h"
#include "Rendering/Objects/_deps.h"
#include "Rendering/Renderer/_deps.h"
#include "generate/perlin.cpp"
#include "generate/heightmap.h"

#include <vector>

#endif