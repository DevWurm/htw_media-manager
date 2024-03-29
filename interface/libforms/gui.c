/*
    MIT License

    Copyright (c) 2017 Leo Lindhorst

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

/* Form definition file generated by fdesign on Sat Feb 25 18:19:48 2017 */

#include <stdlib.h>
#include "gui.h"
#include "libforms.h"
#include "medium/medium.h"
#include "list/list.h"

// @private
// unmount and delete a possibly existing table view from the specified gui
void clearTableView(FD_libformsGUI* gui) {
    if (gui != NULL && gui->table_view != NULL) {
        fl_delete_formbrowser(gui->table_view_container, gui->table_view);
        fl_free_form(gui->table_view);
        gui->table_view = NULL;
    }
}

// update the GUIs table view via the its FormBrowser container object
void setTableView(tList* source, FD_libformsGUI* gui) {
    // remove a previously existing table view
    if (gui != NULL) {
        clearTableView(gui);
    }

    if (source == NULL || gui == NULL) {
        return;
    }

    FL_OBJECT *obj;
    int yCord = 30;
    tIterator* it = toIterator(source);
    tMedium* currMedium;

    // creating a form which simulates a table view
    gui->table_view = fl_bgn_form(FL_NO_BOX, 700, 700);

    // create table head
    fl_add_text( FL_NORMAL_TEXT, 50, 10, 50, 20, "Title");
    obj = fl_add_button( FL_NORMAL_BUTTON, 100, 10, 20, 20, "A");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_TITLE_ASC);
    obj = fl_add_button( FL_NORMAL_BUTTON, 125, 10, 25, 20, "D");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_TITLE_DESC);

    fl_add_text( FL_NORMAL_TEXT, 200, 10, 50, 20, "Artist");
    obj = fl_add_button( FL_NORMAL_BUTTON, 250, 10, 20, 20, "A");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_ARTIST_ASC);
    obj = fl_add_button( FL_NORMAL_BUTTON, 275, 10, 25, 20, "D");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_ARTIST_DESC);

    fl_add_text( FL_NORMAL_TEXT, 350, 10, 100, 20, "Borrower");
    obj = fl_add_button( FL_NORMAL_BUTTON, 425, 10, 20, 20, "A");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_BORROWER_ASC);
    obj = fl_add_button( FL_NORMAL_BUTTON, 450, 10, 25, 20, "D");
    fl_set_object_callback( obj, sortRecordsCallback, SORT_BORROWER_DESC);

    // insert the elements to the table view
    for (currMedium = getNext(it); currMedium; currMedium = getNext(it)) {
        fl_add_text( FL_NORMAL_TEXT, 50, yCord, 150, 20, currMedium->title);
        fl_add_text( FL_NORMAL_TEXT, 200, yCord, 150, 20, currMedium->artist);
        fl_add_text( FL_NORMAL_TEXT, 350, yCord, 150, 20, currMedium->borrower);
        obj = fl_add_button( FL_NORMAL_BUTTON, 500, yCord, 50, 20, "X" );
        fl_set_object_callback( obj, deleteRecordCallback, (long) currMedium->id);

        yCord += 30;
    }

    fl_end_form();

    // mounting the created table view form in the FormBrowser
    fl_addto_formbrowser(gui->table_view_container, gui->table_view);

    deleteIterator(it);
}

// create a new application GUI
FD_libformsGUI * create_form_libformsGUI( void ) {
    FL_OBJECT *obj;
    FD_libformsGUI *fdui = ( FD_libformsGUI * ) fl_malloc( sizeof *fdui );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->libformsGUI = fl_bgn_form( FL_NO_BOX, 1276, 1386 );

    // create an empty trigger object to emit general application events
    fdui->trigger_object = fl_bgn_group();
    fl_end_group( );

    obj = fl_add_box( FL_FLAT_BOX, 0, 0, 1276, 1386, "" );

    obj = fl_add_text( FL_NORMAL_TEXT, 70, 40, 470, 60, "Media Manager" );
    fl_set_object_lsize( obj, FL_HUGE_SIZE );

    // Insert view
    fdui->insert_view = fl_bgn_group( );

    obj = fl_add_text( FL_NORMAL_TEXT, 70, 140, 230, 30, "Insert" );
    fl_set_object_lsize( obj, FL_LARGE_SIZE );

    fdui->insert_input_title = obj = fl_add_input( FL_NORMAL_INPUT, 120, 200, 220, 30, "Title" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->insert_input_artist = obj = fl_add_input( FL_NORMAL_INPUT, 120, 250, 220, 30, "Artist" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->insert_input_borrower = obj = fl_add_input( FL_NORMAL_INPUT, 120, 300, 220, 30, "Borrower" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->insert_button = obj = fl_add_button( FL_NORMAL_BUTTON, 120, 350, 130, 30, "Insert" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_callback( obj, insertRecordCallback, 0 );

    fl_end_group( );

    // Show view
    fdui->show_view = fl_bgn_group( );

    obj = fl_add_text( FL_NORMAL_TEXT, 70, 400, 230, 30, "Show" );
    fl_set_object_lsize( obj, FL_LARGE_SIZE );

    obj = fl_add_text( FL_NORMAL_TEXT, 90, 440, 130, 30, "Search" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );

    fdui->search_input_title = obj = fl_add_input( FL_NORMAL_INPUT, 140, 480, 220, 30, "Titel" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->search_input_artist = obj = fl_add_input( FL_NORMAL_INPUT, 420, 480, 220, 30, "Artist" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->search_input_borrower = fl_add_input( FL_NORMAL_INPUT, 740, 480, 220, 30, "Borrower" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->search_button = obj = fl_add_button( FL_NORMAL_BUTTON, 1010, 480, 130, 30, "Search" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_callback( obj, searchRecordsCallback, 0 );

    // Table view
    fdui->table_view_container = obj = fl_add_formbrowser(FL_NORMAL_FORMBROWSER, 100, 550, 860, 560, "");

    fl_end_group( );

    obj = fl_add_button( FL_NORMAL_BUTTON, 100, 1200, 130, 30, "Close" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_callback( obj, closeCallback, 0 );

    fl_end_form( );

    fdui->table_view = NULL;

    fdui->libformsGUI->fdui = fdui;

    return fdui;
}

// Callbacks
void insertRecordCallback( FL_OBJECT* source, long arg) {
    insertRecord();
    searchRecords(); // update table view
}

void searchRecordsCallback( FL_OBJECT* source, long arg) {
    searchRecords(); 
}

void deleteRecordCallback( FL_OBJECT* source, long arg) {
    deleteRecord((unsigned long) arg);
    searchRecords(); // update table view
}

void sortRecordsCallback( FL_OBJECT* source, long arg ) {
    sortRecords(arg);
    searchRecords(); // update table view
}

void closeCallback( FL_OBJECT* source, long arg ) {
    hideGUIHandler(-1);
}
