#pragma once
#include <Geode/Geode.hpp>
using namespace geode::prelude;
class HoverItemBase : public CCMenuItemSprite {
	public:
		virtual void hover(CCObject *sender, cocos2d::CCPoint point, bool hovering);
};




// from https://github.com/Alphalaneous/Creative-Mode/blob/main/src/hooks/CCEGLView.hpp
class Hover {
  public:
	std::vector<HoverItemBase*> m_hoverableItems;

	void addHoverableItem(HoverItemBase *item);
	bool isNodeVisible(CCNode* node);

	void removeHoverableItem(HoverItemBase *item);

	void updateHover(cocos2d::CCPoint point);
	static Hover *get();
};
