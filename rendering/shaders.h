#define SH_IN_VPOS 0
#define SH_IN_VNORMAL 1
#define SH_UNIFORM_CAMERA 0
#define SH_UNIFORM_MVP 1
#define SH_UNIFORM_CAMERA_POS 2
#define SH_UNIFORM_TIME 3

namespace shaders
{
    static const char* vs_src = R"(
      #version 450

      layout(location = 0) in vec3 vPos;
      layout(location = 1) in vec3 vNormal;

      layout(location = 0) uniform mat4 uCamera;
      layout(location = 1) uniform mat4 uMvp;

      out vec3 fragPos;
      out vec3 fragNormal;

      void main()
      {
        vec4 worldPos = uMvp * vec4(vPos, 1.0);
        fragPos = worldPos.xyz;
        fragNormal = normalize((uMvp * vec4(vNormal,0)).xyz);
        gl_Position = uCamera * worldPos;
      }
    )";

    static const char* fs_src = R"(
      #version 450

      layout(location = 2) uniform vec3 uCamPos;
      layout(location = 3) uniform float uTime;

      in vec3 fragPos;
      in vec3 fragNormal;
      out vec3 color;

      void main()
      {
          vec3 materialColor = vec3(1);
          vec3 lightPos = vec3(10*sin(uTime), 10, 10*cos(uTime));
          vec3 lightCol = vec3(1, 0.8, 0.4) * 100;

          vec3 lightRay = lightPos - fragPos;
          float lightDis2 = dot(lightRay, lightRay);
          float falloff = 1.0f / lightDis2;
          vec3 lightNormal = lightRay / sqrt(lightDis2);
          float diffuse = max(dot(lightNormal, fragNormal), 0);

          vec3 eye = normalize(fragPos - uCamPos);
          vec3 refl = normalize(reflect(eye, fragNormal));
          float spec = pow(max(dot(refl, lightNormal), 0), 30) * 0.75;

          float ambient = 0.1f;
          color = ambient * materialColor + (diffuse + spec) * materialColor * lightCol * falloff;
      }
    )";
}
