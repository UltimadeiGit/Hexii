#pragma once
#include "cocos2d.h"
#include "renderer/backend/Device.h"

#include <vector>
#include <map>
#include <string>
#include <chrono>

class SimpleShader;
typedef std::shared_ptr<cocos2d::backend::Program> ShaderProgramPtr;
typedef std::shared_ptr<SimpleShader> SimpleShaderPtr;

class SimpleShader
{
public:
    SimpleShader(ShaderProgramPtr program);
    SimpleShader(const std::string& vertSource, const std::string& fragSource);
    ~SimpleShader();

    ShaderProgramPtr getProgram() const { return m_program; }
    cocos2d::backend::ProgramState* getProgramState() const { return m_programState; }

    static SimpleShaderPtr create(ShaderProgramPtr program) { return std::make_shared<SimpleShader>(program); }

    static ShaderProgramPtr createShaderProgramWithFragmentShader(const std::string& fragShaderPath);
    static ShaderProgramPtr createShaderProgramWithVertexAndFragmentShader(const std::string& vertShaderPath, const std::string& fragShaderPath);

    //static SimpleShader* createWithFragmentShader(const std::string& fragShaderPath);
    //static SimpleShader* createWithVertexAndFragmentShader(const std::string& vertShaderPath, const std::string& fragShaderPath);

    template <typename T>
    void setUniform(std::string uniform, T value);
    void setUniform(std::string uniform, cocos2d::Texture2D* value);

private:
    static std::string s_defaultVert;
    static std::string s_fragHead;

    int m_currentTextureSlot;
    std::map<std::string, int> m_textureToSlot;

    ShaderProgramPtr m_program;
    cocos2d::backend::ProgramState* m_programState;
};

template<typename T>
void SimpleShader::setUniform(std::string uniform, T value)
{
    auto uniformLocation = m_programState->getUniformLocation(uniform);
    m_programState->setUniform(uniformLocation, &value, sizeof(value));
}


class SimpleShaderManager
{
private:
    unsigned long long baseTime;
    std::vector<SimpleShader*> shaders;

    SimpleShaderManager();

public:
    static SimpleShaderManager* getInstance();

    static ShaderProgramPtr getProgressShaderProgram();
    static ShaderProgramPtr getEncouragementGlowProgram();

    void updateShaderTime();

    friend SimpleShader;
};
