
// dear imgui: Renderer for modern OpenGL with shaders / programmatic pipeline
// - Desktop GL: 2.x 3.x 4.x
// - Embedded GL: ES 2.0 (WebGL 1.0), ES 3.0 (WebGL 2.0)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID!
//  [x] Renderer: Desktop GL only: Support for large meshes (64k+ vertices) with 16-bit indices.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2020-05-08: OpenGL: Made default GLSL version 150 (instead of 130) on OSX.
//  2020-04-21: OpenGL: Fixed handling of glClipControl(GL_UPPER_LEFT) by inverting projection matrix.
//  2020-04-12: OpenGL: Fixed context version check mistakenly testing for 4.0+ instead of 3.2+ to enable ImGuiBackendFlags_RendererHasVtxOffset.
//  2020-03-24: OpenGL: Added support for glbinding 2.x OpenGL loader.
//  2020-01-07: OpenGL: Added support for glbinding 3.x OpenGL loader.
//  2019-10-25: OpenGL: Using a combination of GL define and runtime GL version to decide whether to use glDrawElementsBaseVertex(). Fix building with pre-3.2 GL loaders.
//  2019-09-22: OpenGL: Detect default GL loader using __has_include compiler facility.
//  2019-09-16: OpenGL: Tweak initialization code to allow application calling ImGui_ImplOpenGL3_CreateFontsTexture() before the first NewFrame() call.
//  2019-05-29: OpenGL: Desktop GL only: Added support for large mesh (64K+ vertices), enable ImGuiBackendFlags_RendererHasVtxOffset flag.
//  2019-04-30: OpenGL: Added support for special ImDrawCallback_ResetRenderState callback to reset render state.
//  2019-03-29: OpenGL: Not calling glBindBuffer more than necessary in the render loop.
//  2019-03-15: OpenGL: Added a dummy GL call + comments in ImGui_ImplOpenGL3_Init() to detect uninitialized GL function loaders early.
//  2019-03-03: OpenGL: Fix support for ES 2.0 (WebGL 1.0).
//  2019-02-20: OpenGL: Fix for OSX not supporting OpenGL 4.5, we don't try to read GL_CLIP_ORIGIN even if defined by the headers/loader.
//  2019-02-11: OpenGL: Projecting clipping rectangles correctly using draw_data->FramebufferScale to allow multi-viewports for retina display.
//  2019-02-01: OpenGL: Using GLSL 410 shaders for any version over 410 (e.g. 430, 450).
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-11-13: OpenGL: Support for GL 4.5's glClipControl(GL_UPPER_LEFT) / GL_CLIP_ORIGIN.
//  2018-08-29: OpenGL: Added support for more OpenGL loaders: glew and glad, with comments indicative that any loader can be used.
//  2018-08-09: OpenGL: Default to OpenGL ES 3 on iOS and Android. GLSL version default to "#version 300 ES".
//  2018-07-30: OpenGL: Support for GLSL 300 ES and 410 core. Fixes for Emscripten compilation.
//  2018-07-10: OpenGL: Support for more GLSL versions (based on the GLSL version string). Added error output when shaders fail to compile/link.
//  2018-06-08: Misc: Extracted imgui_impl_opengl3.cpp/.h away from the old combined GLFW/SDL+OpenGL3 examples.
//  2018-06-08: OpenGL: Use draw_data->DisplayPos and draw_data->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-05-25: OpenGL: Removed unnecessary backup/restore of GL_ELEMENT_ARRAY_BUFFER_BINDING since this is part of the VAO state.
//  2018-05-14: OpenGL: Making the call to glBindSampler() optional so 3.2 context won't fail if the function is a NULL pointer.
//  2018-03-06: OpenGL: Added const char* glsl_version parameter to ImGui_ImplOpenGL3_Init() so user can override the GLSL version e.g. "#version 150".
//  2018-02-23: OpenGL: Create the VAO in the render function so the setup can more easily be used with multiple shared GL context.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplSdlGL3_RenderDrawData() in the .h file so you can call it yourself.
//  2018-01-07: OpenGL: Changed GLSL shader version from 330 to 150.
//  2017-09-01: OpenGL: Save and restore current bound sampler. Save and restore current polygon mode.
//  2017-05-01: OpenGL: Fixed save and restore of current blend func state.
//  2017-05-01: OpenGL: Fixed save and restore of current GL_ACTIVE_TEXTURE.
//  2016-09-05: OpenGL: Fixed save and restore of current scissor rectangle.
//  2016-07-29: OpenGL: Explicitly setting GL_UNPACK_ROW_LENGTH to reduce issues because SDL changes it. (#752)

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif


// GL includes
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#elif defined(IMGUI_IMPL_OPENGL_ES3)
#if (defined(__APPLE__) && (TARGET_OS_IOS || TARGET_OS_TV))
#include <OpenGLES/ES3/gl.h>    // Use GL ES 3
#else
#include <GLES3/gl3.h>          // Use GL ES 3
#endif
#else
// About Desktop OpenGL function loaders:
//  Modern desktop OpenGL doesn't have a standard portable header file to load OpenGL function pointers.
//  Helper libraries are often used for this purpose! Here we are supporting a few common ones (gl3w, glew, glad).
//  You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>            // Needs to be initialized with gl3wInit() in user's code
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>            // Needs to be initialized with glewInit() in user's code.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>          // Needs to be initialized with gladLoadGL() in user's code.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#endif
#include <glbinding/Binding.h>  // Needs to be initialized with glbinding::Binding::initialize() in user's code.
#include <glbinding/gl/gl.h>
using namespace gl;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE       // GLFW including OpenGL headers causes ambiguity or multiple definition errors.
#endif
#include <glbinding/glbinding.h>// Needs to be initialized with glbinding::initialize() in user's code.
#include <glbinding/gl/gl.h>
using namespace gl;
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif
#endif

// Desktop GL 3.2+ has glDrawElementsBaseVertex() which GL ES and WebGL don't have.
#if defined(IMGUI_IMPL_OPENGL_ES2) || defined(IMGUI_IMPL_OPENGL_ES3) || !defined(GL_VERSION_3_2)
#define IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET   0
#else
#define IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET   1
#endif

// OpenGL Data
static GLuint       g_GlVersion = 0;                // Extracted at runtime using GL_MAJOR_VERSION, GL_MINOR_VERSION queries (e.g. 320 for GL 3.2)
static char         g_GlslVersionString[32] = "";   // Specified by user or detected based on compile time GL settings.
static GLuint       g_FontTexture = 0;
static GLuint       g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;                                // Uniforms location
static int          g_AttribLocationVtxPos = 0, g_AttribLocationVtxUV = 0, g_AttribLocationVtxColor = 0; // Vertex attributes location
static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

// Functions
bool    ImGui_ImplOpenGL3_Init(const char* glsl_version)
{
    // Query for GL version (e.g. 320 for GL 3.2)
#if !defined(IMGUI_IMPL_OPENGL_ES2)
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    g_GlVersion = major * 100 + minor * 10;
#else
    g_GlVersion = 200; // GLES 2
#endif

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_opengl3";
#if IMGUI_IMPL_OPENGL_MAY_HAVE_VTX_OFFSET
    if (g_GlVersion >= 320)
        io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;  // We can honor the ImDrawCmd::VtxOffset field, allowing for large meshes.
#endif

    // Store GLSL version string so we can refer to it later in case we recreate shaders.
    // Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
#if defined(IMGUI_IMPL_OPENGL_ES2)
    if (glsl_version == NULL)
        glsl_version = "#version 100";
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    if (glsl_version == NULL)
        glsl_version = "#version 300 es";
#elif defined(__APPLE__)
    if (glsl_version == NULL)
        glsl_version = "#version 150";
#else
    if (glsl_version == NULL)
        glsl_version = "#version 130";
#endif
    IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
    strcpy(g_GlslVersionString, glsl_version);
    strcat(g_GlslVersionString, "\n");

    // Dummy construct to make it easily visible in the IDE and debugger which GL loader has been selected.
    // The code actually never uses the 'gl_loader' variable! It is only here so you can read it!
    // If auto-detection fails or doesn't select the same GL loader file as used by your application,
    // you are likely to get a crash below.
    // You can explicitly select a loader by using '#define IMGUI_IMPL_OPENGL_LOADER_XXX' in imconfig.h or compiler command-line.
    const char* gl_loader = "Unknown";
    IM_UNUSED(gl_loader);
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    gl_loader = "GL3W";
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    gl_loader = "GLEW";