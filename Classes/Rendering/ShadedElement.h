#pragma once

#include <cocos2d.h>

#include "SimpleShader.h"
#include "Maths.h"

// All the children of a ShadedElement will be shaded by the given shader and outputted to a RenderTexture
class ShadedElement : public cocos2d::Node {
private:
	bool init(int width, int height);

	ShadedElement(SimpleShaderPtr shader); 

public: 
	// width and height are the dimensions of the RenderTexture, so the maximum size of the shaded element
	inline static ShadedElement* create(SimpleShaderPtr shader, int width, int height) {
		ShadedElement* pRet = new(std::nothrow) ShadedElement(shader); if (pRet && pRet->init(width, height)) {
			pRet->autorelease(); 
			return pRet;
		}
		else {
			delete pRet; 
			pRet = nullptr;
			return nullptr;
		}
	}

	// applyChildren is ignored for all but the RenderTexture
	void setCameraMask(unsigned short mask, bool applyChildren = true) override;

	// Sets the shader on or off.
	// Uses a uniform u_enabled, which must be implemented by the shader
	void toggleShader(bool enabled);

	void setProgramState(cocos2d::backend::ProgramState* programState) override;

	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_output->getSprite()->getTexture(); }
	void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	void addShadedChild(cocos2d::Node* child, int localZOrder = 0);

	inline void setContentSize(const cocos2d::Size& contentSize) override { m_output->setContentSize(contentSize); cocos2d::Node::setContentSize(contentSize); }

private:

	// Result after the shader has applied
	cocos2d::RenderTexture* m_output = nullptr;

	bool m_shaderEnabled = true;

protected:

	SimpleShaderPtr m_shader = nullptr;
};