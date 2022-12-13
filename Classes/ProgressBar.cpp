#include "ProgressBar.h"

USING_NS_CC;

ProgressBar::ProgressBar(float desiredWidth, unsigned int numParts) : m_desiredWidth(desiredWidth), m_numParts(numParts) {}

bool ProgressBar::init() {
	// The total length the hexagon parts will take up is w/2 * (numParts + 1) where w is the width to find
	// Rearranging gives us w = (2 * d) / (numParts + 1) where d is desiredWidth

	if (m_numParts == 0) return false;

	float hexWidth = (2 * m_desiredWidth) / (m_numParts + 1);

	PolygonInfo pinfo = AutoPolygon::generatePolygon("ComponentHexagon.png");

	// Create the first part outside of the for loop so that scale can be calculated for the RenderTexture
	Sprite* firstPart = Sprite::create();
	firstPart->initWithPolygon(pinfo);

	Size size = firstPart->getContentSize();
	float scale = hexWidth / size.width;

	for (unsigned int i = 0; i < m_numParts; i++) {
		Sprite* part = firstPart;
		if (i != 0) {
			part = Sprite::create();
			part->initWithPolygon(pinfo);
		}

		part->setScale(scale);
		part->setPosition(Vec2(i * (hexWidth / 2), 0));
		part->setAnchorPoint(Vec2(0.0, 0.0));

		m_parts.push_back(part);
		// Prevents premature autorelease
		this->addChild(part);
	}
	
	m_whole = RenderTexture::create(m_desiredWidth, scale * size.height);
	Texture2D* innerTexture = m_whole->getSprite()->getTexture();

	m_shader = SimpleShader::createWithFragmentShader("shaders/progressBar.frag");
	m_shader->setUniform<Vec2>("dims", innerTexture->getContentSize());

	m_wholeTex = Sprite::create();
	m_wholeTex->setTexture(innerTexture);
	m_wholeTex->setTextureRect(Rect(Vec2::ZERO, innerTexture->getContentSize()));
	m_wholeTex->setPosition(Vec2(0, 0));
	m_wholeTex->setAnchorPoint(Vec2::ZERO);
	m_wholeTex->setProgramState(m_shader->programState);

	this->addChild(m_whole);
	this->addChild(m_wholeTex);

	return true;
}

void ProgressBar::setProgress(float progress) {
	m_progress = progress;

	if (progress > 0.5f) {
		bool dbg = true;
	}

	m_shader->setUniform("progress", progress);
}

void ProgressBar::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) {
	m_whole->beginWithClear(0, 0, 0, 0);

	for (unsigned int i = 0; i < m_parts.size(); i++) {
		// Uses identity transform because this node's transform is given to `wholeTex`
		m_parts[i]->visit(renderer, Mat4::IDENTITY, parentFlags);
	}

	m_whole->end();

	m_wholeTex->visit(renderer, this->getNodeToParentTransform() * parentTransform, parentFlags);
}
