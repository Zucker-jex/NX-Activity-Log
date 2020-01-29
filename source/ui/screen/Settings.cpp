#include "Settings.hpp"

namespace Screen {
    Settings::Settings(Main::Application * a) {
        this->app = a;

        // Create "static" elements
        Aether::Rectangle * r = new Aether::Rectangle(400, 88, 850, 559);
        r->setColour(Aether::Theme::Dark.altBG);
        this->addElement(r);
        r = new Aether::Rectangle(30, 87, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        r = new Aether::Rectangle(30, 647, 1220, 1);
        r->setColour(Aether::Theme::Dark.fg);
        this->addElement(r);
        Aether::Controls * c = new Aether::Controls();
        c->addItem(new Aether::ControlItem(Aether::Button::A, "OK"));
        c->addItem(new Aether::ControlItem(Aether::Button::B, "Back"));
        c->setColour(Aether::Theme::Dark.text);
        this->addElement(c);

        // Create side menu
        Aether::Menu * menu = new Aether::Menu(30, 88, 388, 559);
        menu->addElement(new Aether::MenuOption("Recent Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::RecentActivity);
        }));
        menu->addElement(new Aether::MenuOption("All Activity", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, [this](){
            this->app->setScreen(Main::ScreenID::AllActivity);
        }));
        menu->addElement(new Aether::MenuSeparator(Aether::Theme::Dark.mutedLine));
        Aether::MenuOption * opt = new Aether::MenuOption("Settings", Aether::Theme::Dark.accent, Aether::Theme::Dark.text, nullptr);
        opt->setActive(true);
        menu->addElement(opt);
        this->addElement(menu);

        // Create list
        this->list = new Aether::List(420, 88, 810, 559);
        this->list->setCatchup(11);
        this->list->setScrollBarColour(Aether::Theme::Dark.mutedLine);

        // DEFAULT SORT METHOD
        std::string str = "";
        switch (this->app->config()->gSort()) {
            case SortType::AlphaAsc:
                str = "By Name";
                break;

            case SortType::HoursAsc:
                str = "Longest Playtime";
                break;

            case SortType::HoursDec:
                str = "Shortest Playtime";
                break;

            case SortType::LaunchAsc:
                str = "Most Launched";
                break;

            case SortType::LaunchDec:
                str = "Least Launched";
                break;

            case SortType::FirstPlayed:
                str = "First Playtime";
                break;

            case SortType::LastPlayed:
                str = "Most Recently Played";
                break;
        }
        this->optionSort = new Aether::ListOption("Default Sort Method", str, [this](){
            this->setupSortOverlay();
        });
        this->optionSort->setHintColour(Aether::Theme::Dark.text);
        this->optionSort->setValueColour(Aether::Theme::Dark.accent);
        this->optionSort->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(this->optionSort);
        Aether::ListComment * lc = new Aether::ListComment("Sets the sort method used upon application launch.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        // THEME
        switch (this->app->config()->gTheme()) {
            case T_Auto:
                str = "Auto";
                break;

            case T_Custom:
                str = "Custom";
                break;

            case T_Dark:
                str = "Basic Black";
                break;

            case T_Light:
                str = "Basic White";
                break;
        }
        this->optionTheme = new Aether::ListOption("Theme", str, [this](){
            this->setupThemeOverlay();
        });
        this->optionTheme->setHintColour(Aether::Theme::Dark.text);
        this->optionTheme->setValueColour(Aether::Theme::Dark.accent);
        this->optionTheme->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(this->optionTheme);
        lc = new Aether::ListComment("Sets the theme for the application. Auto chooses black/white based on your switch settings. Note: This currently requires the app to be restarted to take effect.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // HIDE DELETED
        str = (this->app->config()->hDeleted() ? "Yes" : "No");
        this->optionDeleted = new Aether::ListOption("Hide Deleted Games", str, [this](){
            this->app->config()->setHDeleted(!this->app->config()->hDeleted());
            this->optionDeleted->setValue((this->app->config()->hDeleted() ? "Yes" : "No"));
        });
        this->optionDeleted->setHintColour(Aether::Theme::Dark.text);
        this->optionDeleted->setValueColour(Aether::Theme::Dark.accent);
        this->optionDeleted->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(this->optionDeleted);
        lc = new Aether::ListComment("Excludes deleted games from 'All Activity'.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // REPLACE USER PAGE
        this->optionPage = new Aether::ListOption("Replace User Page", "function()", [](){
            // call function
            // update value
        });
        this->optionPage->setHintColour(Aether::Theme::Dark.text);
        this->optionPage->setValueColour(Aether::Theme::Dark.accent);
        this->optionPage->setLineColour(Aether::Theme::Dark.mutedLine);
        this->list->addElement(this->optionPage);

        lc = new Aether::ListComment("Uses LayeredFS to replace the User Page with this app. Atmosphere 0.10.0+, ReiNX and SXOS supported.");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);
        this->list->addElement(new Aether::ListSeparator());

        // INFORMATION
        lc = new Aether::ListComment("NX Activity Log v" + std::string(VER_STRING) + "\nThanks for using my app! I hope it's been useful! :)\n\nYou can support me on Ko-fi:\nhttps://ko-fi.com/tallbl0nde");
        lc->setTextColour(Aether::Theme::Dark.mutedText);
        this->list->addElement(lc);

        this->addElement(this->list);

        // Create popuplist overlays
        this->sortOverlay = new Aether::PopupList("Default Sort Method");
        this->sortOverlay->setBackgroundColour(Aether::Theme::Dark.altBG);
        this->sortOverlay->setHighlightColour(Aether::Theme::Dark.accent);
        this->sortOverlay->setLineColour(Aether::Theme::Dark.fg);
        this->sortOverlay->setListLineColour(Aether::Theme::Dark.mutedLine);
        this->sortOverlay->setTextColour(Aether::Theme::Dark.text);
        this->themeOverlay = new Aether::PopupList("Application Theme");
        this->themeOverlay->setBackgroundColour(Aether::Theme::Dark.altBG);
        this->themeOverlay->setHighlightColour(Aether::Theme::Dark.accent);
        this->themeOverlay->setLineColour(Aether::Theme::Dark.fg);
        this->themeOverlay->setListLineColour(Aether::Theme::Dark.mutedLine);
        this->themeOverlay->setTextColour(Aether::Theme::Dark.text);

        // Add button callbacks
        this->onButtonPress(Aether::Button::B, [this](){
            this->app->setScreen(Main::ScreenID::UserSelect);
        });
    }

    void Settings::setupSortOverlay() {
        // Remove previous entries
        this->sortOverlay->close(false);
        this->sortOverlay->removeEntries();

        // Add an entry for each sort method
        SortType t = this->app->config()->gSort();
        this->sortOverlay->addEntry("Name", [this](){
            this->app->config()->setGSort(SortType::AlphaAsc);
            this->optionSort->setValue("By Name");
        }, t == SortType::AlphaAsc);
        this->sortOverlay->addEntry("First Playtime", [this](){
            this->app->config()->setGSort(SortType::FirstPlayed);
            this->optionSort->setValue("First Playtime");
        }, t == SortType::FirstPlayed);
        this->sortOverlay->addEntry("Most Recently Played", [this](){
            this->app->config()->setGSort(SortType::LastPlayed);
            this->optionSort->setValue("Most Recently Played");
        }, t == SortType::LastPlayed);
        this->sortOverlay->addEntry("Longest Playtime", [this](){
            this->app->config()->setGSort(SortType::HoursAsc);
            this->optionSort->setValue("Longest Playtime");
        }, t == SortType::HoursAsc);
        this->sortOverlay->addEntry("Shortest Playtime", [this](){
            this->app->config()->setGSort(SortType::HoursDec);
            this->optionSort->setValue("Shortest Playtime");
        }, t == SortType::HoursDec);
        this->sortOverlay->addEntry("Most Launched", [this](){
            this->app->config()->setGSort(SortType::LaunchAsc);
            this->optionSort->setValue("Most Launched");
        }, t == SortType::LaunchAsc);
        this->sortOverlay->addEntry("Least Launched", [this](){
            this->app->config()->setGSort(SortType::LaunchDec);
            this->optionSort->setValue("Least Launched");
        }, t == SortType::LaunchDec);

        this->app->addOverlay(this->sortOverlay);
    }

    void Settings::setupThemeOverlay() {
        // Remove previous entries
        this->themeOverlay->close(false);
        this->themeOverlay->removeEntries();

        // Add an entry for each sort method
        ThemeType t = this->app->config()->gTheme();
        this->themeOverlay->addEntry("Auto", [this](){
            this->app->config()->setGTheme(ThemeType::T_Auto);
            this->optionTheme->setValue("Auto");
        }, t == ThemeType::T_Auto);
        this->themeOverlay->addEntry("Basic Black", [this](){
            this->app->config()->setGTheme(ThemeType::T_Dark);
            this->optionTheme->setValue("Basic Black");
        }, t == ThemeType::T_Dark);
        this->themeOverlay->addEntry("Basic White", [this](){
            this->app->config()->setGTheme(ThemeType::T_Light);
            this->optionTheme->setValue("Basic White");
        }, t == ThemeType::T_Light);
        // this->themeOverlay->addEntry("Custom", [this](){
            // this->app->config()->setGTheme(ThemeType::T_Custom);
            // this->optionTheme->setValue("Custom");
        // }, t == ThemeType::T_Custom);

        this->app->addOverlay(this->themeOverlay);
    }

    void Settings::onLoad() {
        // Create heading using user's name
        this->heading = new Aether::Text(150, 45, this->app->activeUser()->username() + "'s Activity", 28);
        this->heading->setY(this->heading->y() - this->heading->h()/2);
        this->heading->setColour(Aether::Theme::Dark.text);
        this->addElement(this->heading);

        // Render user's image
        this->image = new Aether::Image(65, 14, this->app->activeUser()->imgPtr(), this->app->activeUser()->imgSize());
        this->image->setWH(60, 60);
        this->addElement(this->image);
    }

    void Settings::onUnload() {
        this->removeElement(this->heading);
        this->removeElement(this->image);
    }

    Settings::~Settings() {
        // Delete overlays
        delete this->sortOverlay;
        delete this->themeOverlay;
    }
};
