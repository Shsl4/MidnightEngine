//
// Created by sl4sh on 26/04/22.
//

#include <Platform/Entry.h>

int main(int argc, const char** argv) {

#ifdef __APPLE__

    return macOS_main(argc, argv);

#else

    #if defined(_WIN64) && defined(DEBUG_LEAKS)

        // Enables memory leak check on Windows
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    #endif

    const auto entry = std::make_unique<Entry>();
    entry->entry(argc, argv);

#endif

    return 0;

}