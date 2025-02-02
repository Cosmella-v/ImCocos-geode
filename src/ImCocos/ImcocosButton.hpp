#pragma once
#include <Geode/Geode.hpp>
#include "TemplateCocosButton.hpp"
using namespace geode::prelude;

class ImGuiButton : public TemplateCocosButton {
	public:
		bool init(const char *text, SEL_MenuHandler callback, cocos2d::CCObject* target);
		static ImGuiButton *create(const char *text, SEL_MenuHandler callback,cocos2d::CCObject* target);
		static ImGuiButton *createAndParent(const char *text, SEL_MenuHandler callback, cocos2d::CCNode* Parent, cocos2d::CCObject* target);
};
