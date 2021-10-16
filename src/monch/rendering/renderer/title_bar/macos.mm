
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3native.h>

#import <objc/runtime.h>

// Dummy class for supplying a fake mouseDownCanMoveWindow selector.
@interface FakeView : NSView
@end
@implementation FakeView
- (BOOL)fakeMouseDownCanMoveWindow { return YES; }
@end

void set_title_bar_colour(GLFWwindow* window, float r, float g, float b)
{
    // For each new window we open, the following code needs to be run.
    NSWindow* wnd = glfwGetCocoaWindow(window);
    wnd.titlebarAppearsTransparent = YES;
    wnd.titleVisibility = NSWindowTitleHidden;
    wnd.backgroundColor = [NSColor colorWithDeviceRed:r green:g blue:b alpha:1.f];

    // We only need override/swizzle the selector once.
//    static bool run_once = true;
//    if (run_once)
//    {
//        run_once = false;
//        NSView* glView = [wnd contentView];
//        Method originalMethod = class_getInstanceMethod([glView class], @selector(mouseDownCanMoveWindow));
//        Method categoryMethod = class_getInstanceMethod(FakeView.class, @selector(fakeMouseDownCanMoveWindow));
//        method_exchangeImplementations(originalMethod, categoryMethod);
//    }
}