#include <iostream>
#include <cmath>

#include "monch/rendering/vertex_buffer/VertexBuffer.h"
#include "monch/rendering/gl.h"
#include "monch/rendering/renderer/Renderer.h"
#include <monch/editor/app/EditorApp.h>
#include <monch/rendering/font/manager/FontManager.h>
#include <monch/rendering/font/font/Font.h>
#include <monch/editor/text_area/TextArea.h>

int main()
{
    EditorApp &win = EditorApp::ref();
    win.run();
}
