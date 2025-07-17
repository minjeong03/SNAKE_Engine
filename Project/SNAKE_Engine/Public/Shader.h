#pragma once

#include <string>
#include <vector>
#include "../ThirdParty/glm/glm.hpp"

/**
 * @brief Stages of a shader program in the OpenGL pipeline.
 *
 * @details
 * Used to specify which part of the GPU pipeline a shader source corresponds to.
 */
enum class ShaderStage
{
    Vertex,
    Fragment,
    Geometry,
    TessControl,
    TessEval,
    Compute
};

class RenderManager;
class Material;

using GLuint = unsigned int;
using GLenum = unsigned int;
using FilePath = std::string;

/**
 * @brief Represents an OpenGL shader program composed of multiple shader stages.
 *
 * @details
 * The Shader class handles loading, compiling, linking, and using GPU shader programs.
 * It supports uniform uploading and optional instancing checks. Shaders can be built
 * from external source files, one per stage.
 *
 * Typical usage includes loading a shader, linking, and then binding it before rendering.
 */
class Shader {
    friend Material;
    friend RenderManager;

public:
    /**
     * @brief Constructs an empty Shader object.
     */
    Shader();

    /**
     * @brief Deletes the OpenGL shader program and attached stages.
     */
    ~Shader();

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (int)
     */
    void SendUniform(const std::string& name, int value) const;

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (float)
     */
    void SendUniform(const std::string& name, float value) const;

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (vec2)
     */
    void SendUniform(const std::string& name, const glm::vec2& value) const;

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (vec3)
     */
    void SendUniform(const std::string& name, const glm::vec3& value) const;

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (vec4)
     */
    void SendUniform(const std::string& name, const glm::vec4& value) const;

    /**
     * @brief Uploads a uniform variable to the active shader.
     *
     * @details
     * If the uniform name does not exist in the current program, a warning is logged.
     * Values are passed using OpenGL's glUniform* APIs.
     *
     * @param name Name of the GLSL uniform variable.
     * @param value Value to be uploaded (mat4)
     */
    void SendUniform(const std::string& name, const glm::mat4& value) const;

    /**
     * @brief Returns the OpenGL program ID used by this shader.
     *
     * @return OpenGL GLuint program identifier.
     */
    [[nodiscard]] GLuint GetProgramID() const { return programID; }

private:
    /**
     * @brief Activates this shader for subsequent rendering operations.
     *
     * Internally calls glUseProgram().
     */
    void Use() const;

    /**
     * @brief Deactivates any shader program currently in use.
     *
     * Internally calls glUseProgram(0).
     */
    void Unuse() const;

    /**
     * @brief Returns whether this shader is compatible with GPU instancing.
     *
     * @return True if "i_Model" attribute was detected after linking.
     */
    bool SupportsInstancing() const;

    /**
     * @brief Links all attached shaders into a single GPU program.
     *
     * @details
     * Validates presence of paired tessellation shaders (TCS + TES) if either is used.
     * Reports and logs OpenGL linking errors.
     * Also checks whether the shader supports instancing via a specific attribute ("i_Model").
     *
     * After linking, all shaders are detached but not deleted.
     */
    void Link();

    /**
     * @brief Loads, compiles, and attaches a shader from a GLSL source file.
     *
     * @details
     * Reads the source code from the provided file path, compiles it for the given shader stage,
     * and attaches the resulting shader object to this program.
     *
     * This does not link the shader. Link() must be called afterward.
     *
     * @param stage Shader stage (e.g., Vertex, Fragment)
     * @param filepath Path to the GLSL shader file.
     */
    void AttachFromFile(ShaderStage stage, const FilePath& filepath);

    /**
     * @brief Loads shader source code from a text file.
     *
     * @details
     * Opens and reads the full contents of the given file path into memory.
     * If the file cannot be opened, logs an error and returns an empty string.
     *
     * @param filepath Absolute or relative path to the GLSL file.
     * @return Entire shader source code as a single string.
     */
    [[nodiscard]] std::string LoadShaderSource(const FilePath& filepath);


    /**
     * @brief Compiles raw GLSL source code into a shader object.
     *
     * @details
     * Converts the ShaderStage to OpenGL type, compiles the code, and logs
     * any compilation errors with stage-specific context.
     *
     * @param stage The shader stage (e.g., Vertex, Fragment)
     * @param source GLSL source code as string.
     * @return Compiled OpenGL shader ID (GLuint)
     */
    [[nodiscard]] GLuint CompileShader(ShaderStage stage, const std::string& source);

    /**
     * @brief Detects if the shader supports GPU instancing.
     *
     * @details
     * Checks for the presence of a vertex attribute named "i_Model".
     * If present, the shader is flagged as instancing-compatible.
     */
    void CheckSupportsInstancing();

    GLuint programID;
    std::vector<GLuint> attachedShaders;
    std::vector<ShaderStage> attachedStages;

    bool isSupportInstancing;
};
