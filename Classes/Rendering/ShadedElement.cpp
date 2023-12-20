#include "ShadedElement.h"

USING_NS_CC;

ShadedElement::ShadedElement(SimpleShaderPtr shader) : m_shader(shader) {}

void ShadedElement::setCameraMask(unsigned short mask, bool applyChildren) {
	Node::setCameraMask(mask, false);
	if(applyChildren) m_output->setCameraMask(mask, applyChildren);
}

void ShadedElement::toggleShader(bool enabled) {
	if (m_shaderEnabled == enabled) return;

	m_shader->setUniform("u_enabled", enabled);

	m_shaderEnabled = enabled;
}

bool ShadedElement::init(int width, int height)
{
	m_output = RenderTexture::create(width, height);
	m_output->getSprite()->setAnchorPoint(Vec2(0, 0));

	setContentSize(Size(width, height));

	addChild(m_output);

	return true;
}

void ShadedElement::setProgramState(cocos2d::backend::ProgramState* programState) {
	Node::setProgramState(programState);
	if (programState == nullptr) m_output->getSprite()->setProgramState(
		new backend::ProgramState(backend::Program::getBuiltinProgram(backend::ProgramType::POSITION_TEXTURE_COLOR))
	);
	else m_output->getSprite()->setProgramState(programState);
}

void ShadedElement::visit(cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) {
	// Only the default camera should be used to render to the render texture.

	if ((uint)Camera::getVisitingCamera()->getCameraFlag() & (uint)CameraFlag::DEFAULT) { // !isVisitableByVisitingCamera()) {
		m_output->beginWithClear(0, 0, 0, 0);

		for (Node* child : getChildren()) {
			if (child == m_output) continue;

			child->setVisible(true);
			child->visit(renderer, Mat4::IDENTITY, parentFlags);
			// Prevents the sprite from being drawn in addition to the shaded version later on
			child->setVisible(false);
		}

		m_output->end();
	}
	
	// Only the visiting camera should render the shaded render texture
	if(isVisitableByVisitingCamera()) Node::visit(renderer, parentTransform, parentFlags);
}

void ShadedElement::addShadedChild(cocos2d::Node* child, int localZOrder) {
	addChild(child, localZOrder);
	if(m_shader) child->setProgramState(m_shader->getProgramState());
}