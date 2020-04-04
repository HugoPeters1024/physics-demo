#ifndef SHADERS_H
#define SHADERS_H
#include <array>
#include <string>
#include <sstream>
#include <fstream>

inline static GLuint CompileShader(GLint type, const char* source)
{
  // Preprocess macro's
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check the compilation of the shader 
  GLint success = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (success == GL_FALSE)
  {
    GLint maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    GLchar* errorLog = (GLchar*)malloc(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);

    printf("Error in shader: %s", errorLog);

    free(errorLog);
    return -1;
  }

  return shader;
};

inline static GLuint GenerateProgram(GLuint vs, GLuint fs)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (!isLinked)
  {
    GLint maxLength = 0;  
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar* errorLog = (GLchar*)malloc(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);

    printf("Shader linker error: %s", errorLog);

    glDeleteProgram(program);
    exit(5);
  }
  return program;
}

inline static GLuint GenerateProgram(GLuint vs, GLuint gs, GLuint fs)
{
  GLuint program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, gs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  GLint isLinked = 0;
  glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
  if (!isLinked)
  {
    GLint maxLength = 0;  
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
    GLchar* errorLog = (GLchar*)malloc(maxLength);
    glGetProgramInfoLog(program, maxLength, &maxLength, errorLog);

    printf("Shader linker error: %s", errorLog);

    glDeleteProgram(program);
    exit(5);
  }
  return program;
}

#endif
