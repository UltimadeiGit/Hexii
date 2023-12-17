#pragma once

#include <cocos2d.h>

// Node that combines three parts together: a constant label, a variable label and an icon
class CompoundLabel :
	public cocos2d::Node {
public:
	enum class VariablePartFloat {
		// The variable part is aligned with the const part
		LEFT, 
		// The variable part has equal spacing on both sides
		CENTER,
		// The variable part is aligned with the icon
		RIGHT
	};

	enum class ConstraintMode {
		SPACING,
		WIDTH
	};
private:
	CompoundLabel(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);

public:
	bool initWithFonts(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);

	// Set style attributes of the const part and/or the variable part at once
	// In all cases, give a value of 0 to leave the current value unchanged
	void setStyle(bool applyConstPart, bool applyVariablePart, unsigned int fontSize, const cocos2d::Color4B& textColor = cocos2d::Color4B(0, 0, 0, 0), const cocos2d::Color4B& outlineColor = cocos2d::Color4B::BLACK, int outlineSize = 0,
		const cocos2d::Color4B& shadowColor = cocos2d::Color4B::BLACK, const cocos2d::Size& shadowOffset = cocos2d::Size::ZERO, int shadowBlurRadius = 0
	);

	// Sets the horizontal distance between the const part and the variable part, and between the variable part and the icon
	void setSpacingConstraint(float constVariableSpacing, float variableIconSpacing);
	// Sets the horizontal distance between each part
	inline void setSpacingConstraint(float spaceBetweenParts) 
		{ setSpacingConstraint(spaceBetweenParts, spaceBetweenParts); }
	// Automatically spaces the parts so that the total width is `width` (if possible), 
	// with a minimum spacing of `minSpacing` between each part
	// `variablePartFloat` describes how the variable part should be aligned
	// `setAutoSpacing` determines whether or not the parts should continue to be automatically respaced when the text changes
	void setWidthConstrant(float width, float minSpacing, VariablePartFloat variablePartFloat);
	// Set whether or not the parts should be automatically respaced when the text changes
	void setConstraintMode(ConstraintMode mode) { m_constraintMode = mode; }

	void setVariablePartString(const std::string& text);
	void setIconTexture(const std::string& texturePath);

	inline void setConstPartVisible(bool visible) { if (m_constPart) m_constPart->setVisible(visible); }
	inline void setVariablePartVisible(bool visible) { if (m_variablePart) m_variablePart->setVisible(visible); }
	inline void setIconVisible(bool visible) { if (m_iconPart) m_iconPart->setVisible(visible); }

	// Sets style attributes for a Label. Helpful utility function
	static void setSingleStyle(cocos2d::Label* part, unsigned int fontSize, const cocos2d::Color4B& textColor = cocos2d::Color4B(0, 0, 0, 0), const cocos2d::Color4B& outlineColor = cocos2d::Color4B::BLACK, int outlineSize = 0,
		const cocos2d::Color4B& shadowColor = cocos2d::Color4B::BLACK, const cocos2d::Size& shadowOffset = cocos2d::Size::ZERO, int shadowBlurRadius = 0);

	// Only to be used for styles that aren't set by setStyle
	cocos2d::Label* getConstPart() { return m_constPart; }
	// Only to be used for styles that aren't set by setStyle
	cocos2d::Label* getVariablePart() { return m_variablePart; }
public: 
	static CompoundLabel* create(const std::string& constPartString, const std::string& constPartFontFilepath, const std::string& variablePartFontFilepath);
private:
	// Updates the content size of this node, to reflect changes to the parts' sizes
	void updateSize();
	// Updates the positions of the parts, to reflect changes to the parts' sizes
	void updatePositions();

	ConstraintMode m_constraintMode = ConstraintMode::SPACING;
	float m_widthConstraint = 0.0f;
	float m_widthConstraintMinSpacing = 0.0f;
	VariablePartFloat m_widthConstraintFloatType = VariablePartFloat::LEFT;
	
	// Space between the const part and the variable part
	float m_spacingConstraintFirst = 0.0f;
	// Space between the variable part and the icon
	float m_spacingConstraintSecond = 0.0f;

	cocos2d::Label* m_constPart;
	cocos2d::Label* m_variablePart;
	cocos2d::Sprite* m_iconPart;
};