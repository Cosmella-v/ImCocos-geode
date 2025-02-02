// most is from https://github.com/geode-sdk/MouseAPI/blob/479366791ad4e5bc3f9d13fa91e75381d4b6f43d/src/MacOS.mm
#ifdef GEODE_IS_MACOS
#include "Hover.hpp"
#include <Geode/cocos/platform/mac/CCEventDispatcher.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <Geode/cocos/platform/mac/EAGLView.h>
#import <objc/runtime.h>
using namespace geode::prelude;

using EventType = void(*)(id, SEL, NSEvent*);

@interface MacMouseEvent : NSObject {
    float m_xPosition;
    float m_yPosition;
}   

    +(MacMouseEvent*) sharedEvent;

    -(void) moved:(NSEvent*)event;

    -(cocos2d::CCPoint) getMousePosition;

@end

static MacMouseEvent* s_sharedEvent = nil;



@implementation MacMouseEvent

// i just want to get this working
#define GEODE_API_OBJC_SWIZZLE_METHODS(mouseType, prefix, originalPrefix)               \
static EventType originalPrefix##Dragged;                                               \
- (void)prefix##DraggedHook:(NSEvent*)event {                                           \
/*	originalPrefix##Dragged(self, @selector(prefix##Dragged:), event);                */\
	[[MacMouseEvent sharedEvent] moved:event];                                          \
}                                                                                       \

static EventType s_originalMouseMoved;
- (void)mouseMovedHook:(NSEvent*)event {
	// s_originalMouseMoved(self, @selector(mouseMoved:), event);
	[[MacMouseEvent sharedEvent] moved:event];
}

#define GEODE_API_OBJC_SWIZZLE(method, swizzle, original)                               \
Method method##Method = class_getInstanceMethod(class_, @selector(method:));            \
Method swizzle##Method = class_getInstanceMethod([self class], @selector(swizzle:));    \
original = (decltype(original))method_getImplementation(method##Method);                \
method_exchangeImplementations(method##Method, swizzle##Method);


+ (void)load {
	static dispatch_once_t onceToken;
	dispatch_once(&onceToken, ^{
		Class class_ = NSClassFromString(@"EAGLView");

		GEODE_API_OBJC_SWIZZLE(mouseMoved, mouseMovedHook, s_originalMouseMoved)

		GEODE_API_OBJC_SWIZZLE(mouseDragged, mouseDraggedHook, s_originalMouseDragged)

		GEODE_API_OBJC_SWIZZLE(rightMouseDragged, rightMouseDraggedHook, s_originalRightMouseDragged)

		GEODE_API_OBJC_SWIZZLE(otherMouseDragged, otherMouseDraggedHook, s_originalOtherMouseDragged)
	});
}

+(MacMouseEvent*) sharedEvent {
	@synchronized(self) {
		if (s_sharedEvent == nil) {
			s_sharedEvent = [[self alloc] init]; // assignment not done here
		}
	}
	
	return s_sharedEvent;
}

-(void) moved:(NSEvent*)event {
	NSPoint event_location = [event locationInWindow];
	NSPoint local_point = [[NSClassFromString(@"EAGLView") sharedEGLView] convertPoint:event_location fromView:nil];
	
	float x = local_point.x;
	float y = [[NSClassFromString(@"EAGLView") sharedEGLView] getHeight] - local_point.y;

	m_xPosition = x / [[NSClassFromString(@"EAGLView") sharedEGLView] frameZoomFactor];
	m_yPosition = y / [[NSClassFromString(@"EAGLView") sharedEGLView] frameZoomFactor];

	Loader::get()->queueInGDThread([=]() {
		Hover::get()->updateHover(cocos2d::CCPoint(m_xPosition, m_yPosition));
	});
}

-(cocos2d::CCPoint) getMousePosition {
	static auto cachedMousePos = CCPointZero;
	auto mpos = cocos2d::CCPoint(m_xPosition, m_yPosition);
	if (mpos == cachedMousePos) return cachedMousePos;
	
	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto winSizePx = CCDirector::sharedDirector()->getOpenGLView()->getViewPortRect();
	auto ratio_w = winSize.width / winSizePx.size.width;
	auto ratio_h = winSize.height / winSizePx.size.height;
	mpos.y = winSizePx.size.height - mpos.y;
	mpos.x *= ratio_w;
	mpos.y *= ratio_h;
	return mpos;
}

@end

#endif