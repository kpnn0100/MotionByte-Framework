#include "../pertyG-1.0/Base/Window.h"
using namespace pertyG;
int main() {
    Window window;
    window.show();
    window.setSize(200,300);
    window.waitToClose();
    return 0;
}
