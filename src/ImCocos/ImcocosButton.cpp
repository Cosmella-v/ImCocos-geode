#include "ImcocosButton.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
using namespace geode::prelude;

bool ImGuiButton::init(const char *text, SEL_MenuHandler callback, cocos2d::CCObject* target) {
	if (!TemplateCocosButton::init(text,callback,target))
		return false;

	return true;
}

ImGuiButton *ImGuiButton::create(const char *text, SEL_MenuHandler callback, cocos2d::CCObject* target) {
	auto ret = new ImGuiButton();
	if (ret && ret->init(text, callback, target)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

ImGuiButton *ImGuiButton::createAndParent(const char *text, SEL_MenuHandler callback,  cocos2d::CCNode* Parent, cocos2d::CCObject* target) {
	ImGuiButton *p = ImGuiButton::create(text, callback,target);
	if (p) {
		Parent->addChild(p);
		return p;
	}
	return nullptr;
};

