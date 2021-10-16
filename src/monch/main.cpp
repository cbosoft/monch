#include <monch/editor/app/EditorApp.h>
#include <monch/rendering/renderer/Renderer.h>

int main()
{
    Renderer::ref();
    EditorApp &win = EditorApp::ref();
    win.run();
}
