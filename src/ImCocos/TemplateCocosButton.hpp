#pragma once
#include "Hover.hpp"
#include "ImCocos.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;
class TemplateCocosButton : public HoverItemBase {
  protected:
	bool m_isHovered = false;
	bool m_IsClicked = false;

	~TemplateCocosButton() {
		Hover::get()->removeHoverableItem(this);
	};

  public:
  	ccColor4B m_normalColor;
	ccColor4B m_hoverColor;
	ccColor4B m_pressedColor;
	CCDrawNode *m_bg;
	CCLabelBMFont *m_label;
	float m_cornerRadius = 4.0f;
	
	
	bool init(const char *text, SEL_MenuHandler callback, cocos2d::CCObject *target) {
		if (!CCMenuItemSprite::initWithNormalSprite(nullptr, nullptr, nullptr, target, callback))
			return false;

		Hover::get()->addHoverableItem(this);
		
		m_normalColor = IM_Cocos::defaultTheme::Buttons::m_normalColor;
		m_hoverColor = IM_Cocos::defaultTheme::Buttons::m_hoverColor;
		m_pressedColor = IM_Cocos::defaultTheme::Buttons::m_pressedColor;

		m_bg = CCDrawNode::create();
		this->addChild(m_bg);

		m_label = CCLabelBMFont::create(text, "bigFont.fnt");
		m_label->setScale(0.5f);
		this->addChild(m_label);

		this->setContentSize(m_label->getScaledContentSize() + CCSize{20, 10});
		updateState(false);

		return true;
	}

	void updateState(bool pressed) {
		m_IsClicked = pressed;
		auto color = pressed ? m_pressedColor : (m_isHovered ? m_hoverColor : m_normalColor);

		m_bg->clear();

		ccColor4F bgColor = ccc4FFromccc4B(color);
		IM_Cocos::drawModule::drawRoundedRect(m_bg, CCRect{0, 0, m_obContentSize.width, m_obContentSize.height},
		                             m_cornerRadius, ccc4FFromccc4B(color));
		m_label->setPosition(m_obContentSize / 2);
	}
	void selected() {
		updateState(true);
		CCMenuItemSprite::selected();
	}
	void unselected() {
		updateState(false);
		CCMenuItemSprite::unselected();
	}
	void hover(cocos2d::CCObject *sender, CCPoint point, bool hovering) {
		if (m_isHovered != hovering) {
			m_isHovered = hovering;
			TemplateCocosButton::updateState(m_IsClicked);
		};
	};
};
