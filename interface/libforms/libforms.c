#include <forms.h>
#include <signal.h>

#include "libforms.h"
#include "gui.h"
#include "list/list.h"
#include "medium/medium.h"

// @global
FD_libformsGUI* gui;
tList* targetList;

// @private
void hideGUIHandler(int signal) {
    if (gui != NULL) {
        fl_hide_form(gui->libformsGUI);
        fl_trigger_object(gui->trigger_object);
    }
}

void insertRecord() {
    if (gui == NULL || targetList == NULL) return;

    const char* title = fl_get_input(gui->insert_input_title);
    const char* artist = fl_get_input(gui->insert_input_artist);
    const char* borrower = fl_get_input(gui->insert_input_borrower);

    insert(targetList, createMedium(title, artist, borrower));
}

void runLibformsInterface(int argc, char* argv[], tList* data) {
    targetList = data;

    fl_initialize(&argc, argv, "FormDemo", 0, 0);

    gui = create_form_libformsGUI();

    // establish a handler for hiding the gui, so the interface function is left 
    signal(SIGINT, &hideGUIHandler);
    signal(SIGTERM, &hideGUIHandler);

    fl_show_form(gui->libformsGUI, FL_PLACE_FREE, FL_FULLBORDER, "Media Manager");

    // set up initial table view
    setTableView(data, gui);

    // keep GUI open while it is visible
    do {
        fl_do_forms();
    } while (fl_form_is_visible(gui->libformsGUI));

    // tear down gui
    fl_free_form(gui->libformsGUI);
    if (gui->table_view) {fl_free_form(gui->table_view);}
    fl_free(gui);
    fl_finish();
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
}
