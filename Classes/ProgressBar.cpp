#include "ProgressBar.h"

USING_NS_CC;

ProgressBar::ProgressBar(float desiredWidth, unsigned int numParts) : m_desiredWidth(desiredWidth), m_numParts(numParts) {}

bool ProgressBar::init() {
	// The total length the hexagon parts will take up is w/2 * (numParts + 1) where w is the width to find
	// Rearranging gives us w = (2 * d) / (numParts + 1) where d is desiredWidth

	if (m_numParts == 0) return false;

	float hexWidth = (2 * m_desiredWidth) / (m_numParts + 1);

	PolygonInfo pinfo = AutoPolygon::generatePolygon("widgets/ProgressBarHex.png");

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
	m_whole->getSprite()->setAnchorPoint(Vec2(0.0, 0.0));

	setContentSize(m_whole->getSprite()->getContentSize());

	m_shader = SimpleShader::createWithFragmentShader("shaders/progressBar.frag");
	m_shader->setUniform<Vec2>("dims", getContentSize());

	m_whole->getSprite()->setProgramState(m_shader->programState);

	this->addChild(m_whole);

	return true;
}

void ProgressBar::setProgress(float progress) {
	if (progress == m_progress) return;

	printf("Progress: %.2f -> %.2f\n", m_progress, progress);
	m_progress = progress;

	m_shader->setUniform("progress", progress);
}

void ProgressBar::visit(Renderer* renderer, const Mat4& parentTransform, uint32_t parentFlags) {
	m_whole->beginWithClear(0, 0, 0, 0);

	for (unsigned int i = 0; i < m_parts.size(); i++) {
		m_parts[i]->setVisible(true);
		// Uses identity transform because this node's transform is given to `wholeTex`
		m_parts[i]->visit(renderer, Mat4::IDENTITY, 0);
		// Ensure the part is invisble so that it isn't drawn again when `this` is visited in a few lines
		m_parts[i]->setVisible(false);
	}

	m_whole->end();

	Node::visit(renderer, parentTransform, parentFlags);
}
