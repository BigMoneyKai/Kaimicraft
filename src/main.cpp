#include "app.h"

int main() {
    App app;

    Util::registerApp(&app);
    app.init();
    app.run();
    Util::quit(0);

    return 0;
}
