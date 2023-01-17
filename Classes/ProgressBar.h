#pragma once

#include <vector>
#include <cocos/cocos2d.h>

#include "SimpleShader.h"

class ProgressBar : public cocos2d::Node {
public:
	virtual bool init() override;

	// desiredWidth, numParts
	CREATE_FUNC_WITH_CTOR_2(ProgressBar, float, unsigned int);

	void setProgress(float progress);
	virtual void visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;

private:
	const float m_desiredWidth;
	const unsigned int m_numParts;

	cocos2d::RenderTexture* m_whole = nullptr;
	cocos2d::Sprite* m_wholeTex = nullptr;

	SimpleShader* m_shader = nullptr;

	float m_progress = 0.0f;

	std::vector<cocos2d::Sprite*> m_parts;
};