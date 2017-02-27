#include <forms.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "libforms.h"
#include "gui.h"
#include "list/list.h"
#include "medium/medium.h"

// @global
FD_libformsGUI* gui;
tList* targetList;

// @private
// hide the current gui and trigger it to make the detection of the visibility change possible
void hideGUIHandler(int signal) {
    if (gui != NULL) {
        fl_hide_form(gui->libformsGUI);
        // trigger a object on the form, so the fl_do_forms function exits
        fl_trigger_object(gui->trigger_object);
    }
}

// insert a new record to the current datastore using the input fieds of the current gui
void insertRecord() {
    if (gui == NULL || targetList == NULL) return;

    const char* title = fl_get_input(gui->insert_input_title);
    const char* artist = fl_get_input(gui->insert_input_artist);
    const char* borrower = fl_get_input(gui->insert_input_borrower);

    if (!insert(targetList, createMedium(title, artist, borrower))) fputs("Error while inserting into the datastore", stderr);
}

// search records in the current datastore based on the current GUIs search fields and update the table view
void searchRecords() {
    if (gui == NULL || targetList == NULL) return;

    tList* bufList = NULL; // buffer reference for freeing result lists
    tList* resultList = NULL;

    const char* title = fl_get_input(gui->search_input_title);
    const char* artist = fl_get_input(gui->search_input_artist);
    const char* borrower = fl_get_input(gui->search_input_borrower);

    if (strlen(title) == 0 && strlen(artist) == 0 && strlen(borrower) == 0) {
        setTableView(targetList, gui);
        return;
    }

    // set result if searched by title
    if (strlen(title) > 0) {
        resultList = getWhere(targetList, &hasTitlePredicate, 1, title);
    }

    // set or update result if searched by artist
    if (strlen(artist) > 0) {
        if (resultList != NULL) {
            bufList = resultList;
            resultList = getWhere(resultList, &hasArtistPredicate, 1, artist);
            deleteList(bufList, NULL);
        } else {
            resultList = getWhere(targetList, &hasArtistPredicate, 1, artist);
        }

    }

    // set or update result if searched by borrower
    if (strlen(borrower) > 0) {
        if (resultList != NULL) {
            bufList = resultList;
            resultList = getWhere(resultList, &hasBorrowerPredicate, 1, borrower);
            deleteList(bufList, NULL);
        } else {
            resultList = getWhere(targetList, &hasBorrowerPredicate, 1, borrower);
        }

    }

    setTableView(resultList, gui);

    deleteList(resultList, NULL);
}

// delete the record with the specified ID from the datastrore
void deleteRecord(unsigned long id) {
   if (targetList == NULL) return;

    deleteWhere(targetList, &deleteMediumDeleter, &hasIdPredicate, 1, id);
}

// run a simple graphical user interface based on libforms
void runLibformsInterface(int argc, char* argv[], tList* data) {
    if (data == NULL) return;

    targetList = data;

    // initialize libforms
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
