#pragma once

#include <cocos2d.h>

#include "SimpleShader.h"
#include "Maths.h"

// All the children of a ShadedElement will be shaded by the given shader and outputted to a RenderTexture
class ShadedElement : public cocos2d::Node {
private:
	bool init(int width, int height);

	ShadedElement(SimpleShaderPtr shader); 

	// applyChildren is ignored for all but the RenderTexture
	void setCameraMask(unsigned short mask, bool applyChildren = true) override;

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

	inline cocos2d::Texture2D* getShadedRenderTexture() const { return m_shaded->getSprite()->getTexture(); }
	void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

	void addShadedChild(cocos2d::Node* child, int localZOrder = 0);

	inline void setContentSize(const cocos2d::Size& contentSize) override { m_shaded->setContentSize(contentSize); cocos2d::Node::setContentSize(contentSize); }

private:

	// Result after the shader has applied
	cocos2d::RenderTexture* m_shaded = nullptr;

protected:

	SimpleShaderPtr m_shader = nullptr;
};