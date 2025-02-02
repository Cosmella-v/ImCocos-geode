#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
class $modify(CCEGLView) {
public:
    // When the view is attached to a window, enable mouse-moved events:
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