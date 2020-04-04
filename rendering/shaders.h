#define SH_IN_VPOS 0
#define SH_UNIFORM_CAMERA 0
#define SH_UNIFORM_MVP 1

namespace shaders
{
    static const char* vs_src = R"(
      #version 450

      layout(location = 0) in vec3 vPos;

      layout(location = 0) uniform mat4 uCamera;
      layout(location = 1) uniform mat4 uMvp;

      out vec3 color;

      void main()
      {
        vec4 worldPos = uMvp * vec4(vPos, 1.0);
        gl_Position = uCamera * worldPos;
        color = abs(worldPos.xyz)*2;
      }
    )";

    static const char* fs_src = R"(
      #version 450

      in vec3 color;
      out vec3 o_color;

      void main()
      {
          o_color = color;
      }
    )";
}
