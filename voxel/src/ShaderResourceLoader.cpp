#include "ShaderResourceLoader.h"

#include "Logger.h"

std::pair<std::string, std::unique_ptr<ls::gl::ShaderProgram>> ResourceLoader<ls::gl::ShaderProgram>::load(const std::string& vertexPath, const std::string& fragmentPath)
{
    auto onFailVertex = [&vertexPath](const ls::gl::ShaderLoadResult& log)
    {
        Logger::instance().log(Logger::Priority::Error, "Failed to compile shader: " + vertexPath);
        Logger::instance().log(Logger::Priority::Error, log.compilationLog);
    };
    auto onFailFragment = [&fragmentPath](const ls::gl::ShaderLoadResult& log)
    {
        Logger::instance().log(Logger::Priority::Error, "Failed to compile shader: " + fragmentPath);
        Logger::instance().log(Logger::Priority::Error, log.compilationLog);
    };
    auto onFailLink = [](const ls::gl::ShaderLinkResult& log)
    {
        Logger::instance().log(Logger::Priority::Error, "Failed to link shader");
        Logger::instance().log(Logger::Priority::Error, log.linkLog);
    };

    ls::gl::ShaderProgramBuilder shaderBuilder;
    {
        ls::gl::ShaderLoadResult compLog;
        shaderBuilder.addShaderFromFile<GL_VERTEX_SHADER>(vertexPath.c_str(), &compLog);
        if (!compLog.success)
        {
            onFailVertex(compLog);
            return std::make_pair(vertexPath, nullptr);
        }
        shaderBuilder.addShaderFromFile<GL_FRAGMENT_SHADER>(fragmentPath.c_str(), &compLog);
        if (!compLog.success)
        {
            onFailFragment(compLog);
            return std::make_pair(vertexPath, nullptr);
        }
    }

    std::unique_ptr<ls::gl::ShaderProgram> program;
    {
        ls::gl::ShaderLinkResult linkLog;
        program = std::make_unique<ls::gl::ShaderProgram>(shaderBuilder.linkAndClear(&linkLog));
        if (!linkLog.success)
        {
            onFailLink(linkLog);
            return std::make_pair(vertexPath, nullptr);
        }
    }

    Logger::instance().logLazy(Logger::Priority::Info, [&]()->std::string {return
        std::string("Loaded shader: ") +
        ";vertex: " + vertexPath +
        ";fragment: " + fragmentPath;
    });

    return std::make_pair(vertexPath, std::move(program));
}