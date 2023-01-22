#pragma once

#include <cocos2d.h>

// Node that combines three parts together: a constant label, a variable label and an icon
class CompoundLabel :
	public cocos2d::Node {
private:
	CompoundLabel(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);

public:
	bool initWithFonts(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);

	// Set style attributes of the const part and/or the variable part at once
	// In all cases, give a value of 0 to leave the current value unchanged
	void setStyle(bool applyConstPart, bool applyVariablePart, unsigned int fontSize, const cocos2d::Color4B& textColor = cocos2d::Color4B(0, 0, 0, 0), const cocos2d::Color4B& outlineColor = cocos2d::Color4B::BLACK, int outlineSize = 0,
		const cocos2d::Color4B& shadowColor = cocos2d::Color4B::BLACK, const cocos2d::Size& shadowOffset = cocos2d::Size::ZERO, int shadowBlurRadius = 0
	);

	// Sets the horizontal distance between each part
	void setSpacing(float spaceBetweenParts);

	void setVariablePartString(const std::string& text);
	void setIconTexture(const std::string& texturePath);

	inline void setConstPartVisible(bool visible) { if (m_constPart) m_constPart->setVisible(visible); }
	inline void setVariablePartVisible(bool visible) { if (m_variablePart) m_variablePart->setVisible(visible); }
	inline void setIconVisible(bool visible) { if (m_iconPart) m_iconPart->setVisible(visible); }

	// Sets style attributes for a Label. Helpful utility function
	static void setSingleStyle(cocos2d::Label* part, unsigned int fontSize, const cocos2d::Color4B& textColor = cocos2d::Color4B(0, 0, 0, 0), const cocos2d::Color4B& outlineColor = cocos2d::Color4B::BLACK, int outlineSize = 0,
		const cocos2d::Color4B& shadowColor = cocos2d::Color4B::BLACK, const cocos2d::Size& shadowOffset = cocos2d::Size::ZERO, int shadowBlurRadius = 0);
public: 
	static CompoundLabel* create(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);
private:
	// Updates the content size of this node, to reflect changes to the parts' sizes
	void updateSize();

	

	float m_spaceBetweenParts;

	cocos2d::Label* m_constPart;
	cocos2d::Label* m_variablePart;
	cocos2d::Sprite* m_iconPart;
};