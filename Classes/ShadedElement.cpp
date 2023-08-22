#include "ShadedElement.h"

USING_NS_CC;

ShadedElement::ShadedElement(SimpleShaderPtr shader) : m_shader(shader) {}

void ShadedElement::setCameraMask(unsigned short mask, bool applyChildren) {
	Node::setCameraMask(mask, false);
	if(applyChildren) m_shaded->setCameraMask(mask, applyChildren);
}

bool ShadedElement::init(int width, int height)
{
	m_shaded = RenderTexture::create(width, height);
	m_shaded->getSprite()->setAnchorPoint(Vec2(0, 0));

	setContentSize(Size(width, height));

	addChild(m_shaded);

	return true;
}

void ShadedElement::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) {
	// Only the default camera should be used to render to the render texture.

	if ((uint)Camera::getVisitingCamera()->getCameraFlag() & (uint)CameraFlag::DEFAULT) { // !isVisitableByVisitingCamera()) {
		m_shaded->beginWithClear(0, 0, 0, 0);

		for (Node* child : getChildren()) {
			if (child == m_shaded) continue;

			child->setVisible(true);
			child->visit(renderer, Mat4::IDENTITY, parentFlags);
			// Prevents the sprite from being drawn in addition to the shaded version later on
			child->setVisible(false);
		}

		m_shaded->end();
	}
	
	// Only the visiting camera should render the shaded render texture
	if(isVisitableByVisitingCamera()) Node::visit(renderer, parentTransform, parentFlags);
}

void ShadedElement::addShadedChild(cocos2d::Node* child, int localZOrder) {
	addChild(child, localZOrder);
	if(m_shader) child->setProgramState(m_shader->programState);
}