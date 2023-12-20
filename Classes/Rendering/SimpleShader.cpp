#include "SimpleShader.h"
#include "Console.h"

#include <ccRandom.h>

std::string SimpleShader::s_defaultVert = R"(
    attribute vec4 a_position;
    attribute vec2 a_texCoord;
    attribute vec4 a_color;

    uniform mat4 u_MVPMatrix;

    #ifdef GL_ES
    varying lowp vec4 cc_FragColor;
    varying mediump vec2 cc_FragTexCoord1;
    #else
    varying vec4 cc_FragColor;
    varying vec2 cc_FragTexCoord1;
    #endif

    void main()
    {
        gl_Position = u_MVPMatrix * a_position;
        cc_FragColor = a_color;
        cc_FragTexCoord1 = a_texCoord;
    }
    )";

std::string SimpleShader::s_fragHead = R"(
    #ifdef GL_ES
    precision lowp float;
    #endif

    varying vec4 cc_FragColor;
    varying vec2 cc_FragTexCoord1;

    uniform sampler2D u_texture;
    uniform float cc_Time;
    )";

SimpleShader::SimpleShader(ShaderProgramPtr program) {
    m_program = program;
    m_programState = new cocos2d::backend::ProgramState(program.get());

    m_currentTextureSlot = 1;

    // Add instance to shader manager so that common uniforms such as time can be updated
    SimpleShaderManager::getInstance()->shaders.push_back(this);
}

SimpleShader::SimpleShader(const std::string& vertSource, const std::string& fragSource)
{
    warn("Shader being constructed the old fashioned way!");
    SimpleShader(ShaderProgramPtr(cocos2d::backend::Device::getInstance()->newProgram(vertSource, fragSource)));
}

SimpleShader::~SimpleShader()
{
    // Cleanup
    delete m_programState;

    auto& vec = SimpleShaderManager::getInstance()->shaders;
    vec.erase(std::remove(vec.begin(), vec.end(), this), vec.end());
}

ShaderProgramPtr SimpleShader::createShaderProgramWithFragmentShader(const std::string& fragShaderPath) {
    // Custom fragment shader 
    auto fragSourceRaw = cocos2d::FileUtils::getInstance()->getStringFromFile(fragShaderPath);

    // Build full fragment shader
    auto fragSource = s_fragHead + fragSourceRaw;

    return ShaderProgramPtr(cocos2d::backend::Device::getInstance()->newProgram(s_defaultVert, fragSource));
}

ShaderProgramPtr SimpleShader::createShaderProgramWithVertexAndFragmentShader(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
    // Custom vertex shader 
    auto vertSource = cocos2d::FileUtils::getInstance()->getStringFromFile(vertShaderPath);

    // Custom fragment shader 
    auto fragSourceRaw = cocos2d::FileUtils::getInstance()->getStringFromFile(fragShaderPath);

    // Build full fragment shader
    auto fragSource = s_fragHead + fragSourceRaw;

    return ShaderProgramPtr(cocos2d::backend::Device::getInstance()->newProgram(vertSource, fragSource));
}

void SimpleShader::setUniform(std::string uniform, cocos2d::Texture2D* value)
{
    // Determine texture slot
    int slot = m_currentTextureSlot;
    if (m_textureToSlot.count(uniform) > 0)
    {
        // Texture already has a slot
        slot = m_textureToSlot[uniform];
    }
    else
    {
        // New texture
        m_textureToSlot[uniform] = slot;
        m_currentTextureSlot++;
    }

    auto uniformLocation = m_programState->getUniformLocation(uniform);
    m_programState->setTexture(uniformLocation, slot, value->getBackendTexture());
}


///////////////// Simple Shader Manager /////////////////

SimpleShaderManager::SimpleShaderManager() {
    baseTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

SimpleShaderManager* SimpleShaderManager::getInstance() {
    static auto instance = new SimpleShaderManager();

    return instance;
}

ShaderProgramPtr SimpleShaderManager::getProgressShaderProgram() {
    static ShaderProgramPtr program = SimpleShader::createShaderProgramWithFragmentShader("shaders/hexProgress.frag");

    return program;
}

ShaderProgramPtr SimpleShaderManager::getEncouragementGlowProgram() {
    static ShaderProgramPtr program = SimpleShader::createShaderProgramWithFragmentShader("shaders/EncouragementGlow.frag");

    return program;
}

ShaderProgramPtr SimpleShaderManager::getDentsProgram() {
    static ShaderProgramPtr program = SimpleShader::createShaderProgramWithFragmentShader("shaders/dents.frag");

    return program;
}

void SimpleShaderManager::updateShaderTime() {
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    float seconds = (milliseconds - baseTime) / 1000.0f;

    for (auto shader : shaders)
    {
        shader->setUniform<float>("cc_Random01", cocos2d::RandomHelper::random_real<float>(0.0f, 1.0f));
        shader->setUniform("cc_Time", seconds);
    }
}