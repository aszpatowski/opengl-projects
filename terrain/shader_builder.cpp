//
// Created by aleksander on 01.02.2022.
//

#include <fstream>
#include <iostream>
#include <vector>

using std::string;

std::vector<string> ReadShader(string shadername) {
    std::vector<string> shader;
    std::fstream shaderfile;
    shaderfile.open(shadername, std::ios::in);
    if (!shaderfile) {
        std::cout << "couldn't read " << shadername << std::endl;
        std::cout << "terminating";
        exit(1);
    } else {
        string line;
        while(std::getline(shaderfile, line)) {
            line += '\n';
            shader.push_back(line);
        }
    }

    shaderfile.close();
    return shader;
}

void CompileShader(GLuint shader_handle, std::vector<string> shader_source, string shadername) {
    std::cout << "-----------------------------" << std::endl;
    char compiler_errors[1024];
    int coompilation_status;

    int *sourcelens = (int *)malloc(sizeof(int) * shader_source.size());
    const char** sources = (const char **)malloc(sizeof(char *) * shader_source.size());

    for(int i = 0; i < shader_source.size(); i++) {
        sourcelens[i] = shader_source[i].length();
        sources[i] = shader_source[i].c_str();
    }


    glShaderSource(shader_handle, shader_source.size(), sources, sourcelens);
    glCompileShader(shader_handle);
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &coompilation_status);
    glGetShaderInfoLog(shader_handle, sizeof(compiler_errors), 0, compiler_errors);
    AGLErrors("failed when compiling");
    
    for(int i = 0; i < shader_source.size(); i++)
        std::cout << shader_source[i];
    std::cout << compiler_errors << std::endl;

    if (coompilation_status != 1) {
        std::cout << "compilation of shader " << shadername << " failed" << std::endl;
        std::cout << compiler_errors << std::endl;
    }
}

std::pair<GLuint, GLuint> BuildPipeline() {

    std::vector<string> VertexShaderSource = ReadShader("shaders/VertexShader.glsl");
    GLuint VertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);
    CompileShader(VertexShaderHandle, VertexShaderSource, "shaders/VertexShader.glsl");

    std::vector<string> TesselationControlShaderSource = ReadShader("shaders/TesselationControlShader.glsl");
    GLuint TesselationControlShaderHandle = glCreateShader(GL_TESS_CONTROL_SHADER);
    CompileShader(TesselationControlShaderHandle, TesselationControlShaderSource, "shaders/TesselationControlShader.glsl");

    std::vector<string> TesselationEvaluationShaderSource = ReadShader("shaders/TesselationEvaluationShader.glsl");
    GLuint TesselationEvaluationShaderHandle = glCreateShader(GL_TESS_EVALUATION_SHADER);
    CompileShader(TesselationEvaluationShaderHandle, TesselationEvaluationShaderSource, "shaders/TesselationEvaluationShader.glsl");

    std::vector<string> FragmentShaderSource = ReadShader("shaders/FragmentShader.glsl");
    GLuint FragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
    CompileShader(FragmentShaderHandle, FragmentShaderSource, "shaders/FragmentShader.glsl");
    std::cout << "finished compiling fragment shader" << std::endl;

    GLuint ShaderHandle = glCreateProgram();
    glAttachShader(ShaderHandle, VertexShaderHandle);
    glAttachShader(ShaderHandle, TesselationControlShaderHandle);
    glAttachShader(ShaderHandle, TesselationEvaluationShaderHandle);
    glAttachShader(ShaderHandle, FragmentShaderHandle);

    GLuint PositionSlot = 0;
    glBindAttribLocation(ShaderHandle, PositionSlot, "Position");

    std::cout << "linking shader" << std::endl;
    glLinkProgram(ShaderHandle);

    char linker_errors[256];
    int linking_status;
    int t;
    glGetProgramiv(ShaderHandle, GL_LINK_STATUS, &linking_status);
    glGetProgramInfoLog(ShaderHandle, sizeof(linker_errors), &t, linker_errors);

    std::cout << linking_status << std::endl;
    std::cout << linker_errors << std::endl;


    if(linking_status != 1) {
        std::cout << "Linker failed" << std::endl;
        std::cout << linker_errors << std::endl;
        std::cout << "terminating";
        exit(1);
    }

    glUseProgram(ShaderHandle);
    AGLErrors("failed during loading program");

    std::cout << "compilation finished" << std::endl;

    return std::make_pair<int, int>(ShaderHandle, PositionSlot);
}