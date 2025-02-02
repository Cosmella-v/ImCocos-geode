// from https://github.com/Alphalaneous/Creative-Mode/blob/main/src/hooks/CCEGLView.hpp
#include "Hover.hpp"
#include <Geode/Geode.hpp>
using namespace geode::prelude;

void Hover::addHoverableItem(HoverItemBase *item) {
	m_hoverableItems.push_back(item);
}

bool Hover::isNodeVisible(CCNode *node) {
	if (!node) {
		return false;
	}
	CCNode* currentNode = node;

	while (currentNode) {
		if (!currentNode->isVisible()) {
			return false;
		}
		if (!currentNode->getParent()) {
			CCNode* b;
			if (CCNode* N = typeinfo_cast<CCNode*>(node)) {
				b = N;
			} else {
				b = N;
			};
			return currentNode != b;
		}
		currentNode = currentNode->getParent();
	}

	return false;
}

void Hover::removeHoverableItem(HoverItemBase *item) {
	auto it = std::find(m_hoverableItems.begin(), m_hoverableItems.end(), item);
	if (it != m_hoverableItems.end())
		m_hoverableItems.erase(it);
}

void Hover::updateHover(CCPoint point) {
	CCPoint local;
	bool currentParentVisible = false;
	CCMenu *currentParent = nullptr;
	for (HoverItemBase* item : m_hoverableItems) {
		CCMenu *parent = static_cast<CCMenu *>(item->getParent());

		currentParentVisible = parent && parent->isVisible();
		if (currentParentVisible) {
			local = parent->convertToNodeSpace(point);
		}

		if (currentParentVisible && isNodeVisible(item)) {
			item->hover(item, point, item->boundingBox().containsPoint(local));
		}
	}
}
Hover *Hover::get() {
	static Hover *instance = nullptr;
	if (!instance) {
		instance = new Hover();
	}
	return instance;
}

void HoverItemBase::hover(CCObject *sender, CCPoint point, bool hovering) {

};

#ifdef GEODE_IS_WINDOWS
// windows only
#include <Geode/modify/CCEGLView.hpp>
class $modify(CCEGLView) {
	void onGLFWMouseMoveCallBack(GLFWwindow *w, double x, double y) {
		CCEGLView::onGLFWMouseMoveCallBack(w, x, y);
		Hover::get()->updateHover(getMousePos());
	}
};
#endif

#ifdef __APPLE__
// gaygpt said this would work, i don't belive it will but i have to see lol
#import <Cocoa/Cocoa.h>
class $modify(CCEGLView) {
public:
    - (void)viewDidMoveToWindow {
        [super viewDidMoveToWindow];
        [[self window] setAcceptsMouseMovedEvents:YES];
    }
    - (void)mouseMoved:(NSEvent *)event {
        [super mouseMoved:event];
        CGPoint pos = [event locationInWindow];
        Hover::get()->updateHover(pos.x, pos.y);
    }
    - (void)mouseDragged:(NSEvent *)event {
        [super mouseDragged:event];
        CGPoint pos = [event locationInWindow];
        Hover::get()->updateHover(pos.x, pos.y);
    }
};
#endif

#ifdef GEODE_IS_ANDROID
auto listener = cocos2d::EventListenerTouchOneByOne::create();
listener->onTouchBegan = [](cocos2d::Touch* touch, cocos2d::Event* event) -> bool {
    auto location = touch->getLocation();
    Hover::get()->updateHover(location.x, location.y);
    return true;
};
listener->onTouchMoved = [](cocos2d::Touch* touch, cocos2d::Event* event) {
    auto location = touch->getLocation();
    Hover::get()->updateHover(location.x, location.y);
};
_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, yourTargetNode);
#endif


