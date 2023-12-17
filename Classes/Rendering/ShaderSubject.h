#pragma once

#include "SimpleShader.h"

// A shader subject is something that can switch between shader effects.
class ShaderSubject 
{
public:
	ShaderSubject(const ShaderSubject&) = delete;
	ShaderSubject(ShaderSubject&&) = delete;

	bool hasShaderEffect() const { return m_effect != nullptr; }
	SimpleShaderPtr getShaderEffect() const { return m_effect; }

	// Sets the current shader effect
	void setShaderEffect(SimpleShaderPtr shader) { onShaderEffectChanged(shader); m_effect = shader; }
	void clearShaderEffect() { setShaderEffect(nullptr); }

protected:
	// Should handle the actual switching of shader effects, including to nullptr
	virtual void onShaderEffectChanged(SimpleShaderPtr shader) = 0;

	inline ShaderSubject() {}

private:
	SimpleShaderPtr m_effect;
};