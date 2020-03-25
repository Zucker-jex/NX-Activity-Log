#ifndef SCREEN_SETTINGS_HPP
#define SCREEN_SETTINGS_HPP

#include "Application.hpp"

// Forward declaration due to circular dependency
namespace Main {
    class Application;
};

namespace Screen {
    // Application Settings screen
    class Settings : public Aether::Screen {
        private:
            // Pointer to main app object for config
            Main::Application * app;

            // Pointers to elements
            Aether::Text * heading;
            Aether::Image * image;
            Aether::List * list;
            Aether::Menu * menu;

            // Pointers to options so the value can be updated
            Aether::ListOption * optionScreen;
            Aether::ListOption * optionSort;
            Aether::ListOption * optionTheme;
            Aether::ListOption * optionView;

            Aether::ListOption * optionGraph;
            Aether::ListOption * optionDeleted;
            Aether::ListOption * optionPage;

            // MessageBox for forwarder/setting info
            Aether::MessageBox * msgbox;
            // PopupList for list-related options
            Aether::PopupList * popuplist;

            void installForwarder();

            // Set elements and highlight one in overlay
            void preparePopupList(std::string);
            void setupScreenOverlay();
            void setupSortOverlay();
            void setupThemeOverlay();
            void setupViewOverlay();

        public:
            // Passed main application object
            Settings(Main::Application *);

            // Prepare user-specific elements
            void onLoad();
            // Delete elements created in onLoad()
            void onUnload();
    };
};

#endif