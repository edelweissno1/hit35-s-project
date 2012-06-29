#include <gtk/gtk.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include "string.h"

using namespace std;

struct infomation {
    string commands;
    string devices;
    int PID;
} process[300];

vector <infomation> ufproc;

int ReadData(char*rbuf) {
    char ch;
    string command;
    string dev;
    unsigned int rchar, get, data;
    int i = 0;
    FILE *fp;

    fp = fopen("testfile", "r+");

    if (fp == NULL) {
        return -1;
    }
    rchar = 0;

    while ((get = fgetc(fp)) != EOF) {
        if ((char) get == '\n') {

            rchar = ftell(fp);
            rbuf = (char*) malloc(rchar * sizeof (char));
            //fseek(fp, -rchar, SEEK_CUR);
            if (fgets(rbuf, rchar, fp) != NULL) {

                fseek(fp, -1, SEEK_CUR);
                //	printf("%s", rbuf);
                int j = 0;
                int k = 64;
                while (j < 14) {

                    ch = rbuf[j];
                    if (ch != ' ' && (ch < 48 || ch > 57)) {
                        //strcat(command,&ch);
                        command.append(1, ch);
                        j++;
                        ch = rbuf[j];
                        while (ch != ' ' && (ch < 48 || ch > 57)) {
                            //strcat(command,&ch);
                            command.append(1, ch);
                            j++;
                            ch = rbuf[j];
                        }

                    }
                    if (ch >= 48 && ch <= 57) {
                        data = ch - '0';
                        j++;
                        ch = rbuf[j];

                        while (ch >= 48 && ch <= 57) {
                            data = data * 10 + (ch - '0');

                            j++;
                            ch = rbuf[j];

                        }

                    } else {

                        j++;
                    }

                }

                while (k < rchar) {

                    ch = rbuf[k];
                    if (ch != '\n')
                        dev.append(1, ch);
                    k++;



                }

            }
            //    printf("\n");
            process[i].PID = data;
            process[i].commands = command;
            process[i].devices = dev;
            command.clear();
            dev.clear();
            process[0].devices = "null";

            i++;

        }

    }
    return i;
}
FILE *stream;
FILE *wstream;
FILE *fp;
char buf[20000];
char readbuf[81];
char ch;

int PIDdata[1024];

void Mydata() {
    int i = 0;
    memset(buf, '0', sizeof (buf));
    //system("");
    stream = popen("sudo lsof /dev", "r");
    wstream = fopen("testfile", "w+");

    fread(buf, sizeof (char), sizeof (buf), stream);
    fwrite(buf, 1, sizeof (buf), wstream);
    char *c;

    pclose(stream);
    fclose(wstream);
    string str1;
    string str3;
    string str2 = "null";
    i = ReadData(readbuf);
    int m = 1;
    while (m < i) {
        str1 = process[m].devices;
        str3 = process[m - 1].devices;
        //  if (str1 == "")
        // cout<<"find"<<endl;
        //  process[m].devices = "unname device";
        if (str1.compare(0, 4, str2, 0, 4) == 0) // )
        {

            m++;
        } else {
            if ((str1.compare(0, 4, str3, 0, 4) == 0) && (process[m].PID == process[m - 1].PID)) {
                m++;

            } else {
                ufproc.push_back(process[m]);
                m++;
            }
        }
    }

    //    for (int n = 0; n < ufproc.size(); n++)
    //
    //        if (ufproc[n].PID != 0) {
    //
    //           cout << ufproc[n].commands << "  " << ufproc[n].PID << "  " << ufproc[n].devices << endl;
    //            //  cout << sizeof(ufproc[n].devices)<<"    "<<;
    //        }

    // return 0;
}
GtkWidget *gtklist;
GtkWidget *button;
GtkWidget *list_item;
GList *dlist;
guint i;
//guint x = 1;
gchar buffer[64];

void Reprintdata() {
    GtkWidget *label;
    gchar *string;
    ufproc.clear();
    Mydata();
  //  sprintf(buffer, "\n\n----------------Refresh %d--------------------------\n", x);
   // x++;

    label = gtk_label_new(buffer);
    gtk_label_set_markup(GTK_LABEL(label),
   "<span foreground='red'font_desc='18'>\n\n----------------Refresh--------------------------\n</span>");
    list_item = gtk_list_item_new();
    gtk_container_add(GTK_CONTAINER(list_item), label);
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(gtklist), list_item);
    gtk_widget_show(list_item);
    gtk_label_get(GTK_LABEL(label), &string);
  //   sprintf(buffer, " COMMAND           PID            DEVICE\n", x);
    
    label = gtk_label_new(buffer);
    list_item = gtk_list_item_new();
    gtk_label_set_markup(GTK_LABEL(label),
   "<span foreground='purple' underline='double' underline_color='blue' font_desc='12'>COMMAND      PID        DEVICE</span>");
    gtk_container_add(GTK_CONTAINER(list_item), label);
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(gtklist), list_item);
    gtk_widget_show(list_item);
    gtk_label_get(GTK_LABEL(label), &string);
    for (i = 0; i < ufproc.size() - 1; i++) {
        GtkWidget *label;
        gchar *string;
        const char *cmds = ufproc[i].commands.c_str();
        const char *devs = ufproc[i].devices.c_str();
        //sprintf(buffer, "ListItemContainer with Label #%d", i);

        //  cout << ufproc[i].commands << "  " << ufproc[i].PID << "  " << ufproc[i].devices << endl;
        sprintf(buffer, "  %-20s  %10d  %20s", cmds, ufproc[i].PID, devs);

        label = gtk_label_new(buffer);
        list_item = gtk_list_item_new();
        gtk_container_add(GTK_CONTAINER(list_item), label);
        gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
        gtk_widget_show(label);
        gtk_container_add(GTK_CONTAINER(gtklist), list_item);
        gtk_widget_show(list_item);
        gtk_label_get(GTK_LABEL(label), &string);
        //g_object_set_data (G_OBJECT (list_item), "ufproc[1].commands", string);
    }


}

//static void  sigh_print_selection( GtkWidget *gtklist,
//                                   gpointer   func_data);

static void sigh_button_event(GtkWidget *gtklist,
        GdkEventButton *event,
        GtkWidget *frame);

/* Main function to set up the user interface */
//    void gtk_reload()
//{
//    Mydata();
//
//}

gint main(int argc, gchar *argv[]) {
    GdkColor color;
    GtkWidget *separator;
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *scrolled_window;
    GtkWidget *frame;
    //    GtkWidget *gtklist;
    //    GtkWidget *button;
    //    GtkWidget *list_item;
    //    GList *dlist;
    //    guint i;
    //    gchar buffer[64];
    Mydata();
    //  buffer[1] = ch;
    /* Initialize GTK (and subsequently GDK) */

    gtk_init(&argc, &argv);



    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "DevicesMonitor by HIT CS-35");
    g_signal_connect(G_OBJECT(window), "destroy",
            G_CALLBACK(gtk_main_quit),
            NULL);



    vbox = gtk_vbox_new(FALSE, 5);
    gtk_container_set_border_width(GTK_CONTAINER(vbox), 5);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show(vbox);

    /* This is the scrolled window to put the List widget inside */
    scrolled_window = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_set_size_request(scrolled_window, 350, 350);
    gtk_container_add(GTK_CONTAINER(vbox), scrolled_window);
    gtk_widget_show(scrolled_window);



    gtklist = gtk_list_new();
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_window),
            gtklist);
    gtk_widget_show(gtklist);
    //    g_signal_connect (G_OBJECT (gtklist), "selection_changed",
    //                      G_CALLBACK (sigh_print_selection),
    //                      NULL);


    frame = gtk_frame_new("Translation");
    gtk_widget_set_size_request(frame, 200, 50);
    gtk_container_set_border_width(GTK_CONTAINER(frame), 5);
    gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_OUT);

    gtk_container_add(GTK_CONTAINER(vbox), frame);
    gtk_widget_show(frame);


  //  g_signal_connect(G_OBJECT(gtklist), "button_release_event",
           // G_CALLBACK(sigh_button_event),
          //  frame);

    /* Create a separator */
    separator = gtk_hseparator_new();
    gtk_container_add(GTK_CONTAINER(vbox), separator);
    gtk_widget_show(separator);
     GtkWidget *label;
     gchar *string;
    // sprintf(buffer, " COMMAND           PID            DEVICE\n", x);
//    color.red = 0xffff;
//    color.green = 0xffff;
//    color.blue = 0xffff;
  // gtk_label_set_markup(GTK_LABEL(label),
 //  "<span foreground='red' underline='double' underline_color='blue' font_desc='32'>label for test!</span>");
    //gtk_widget_modify_bg(list_item,GTK_STATE_NORMAL, &color);
    label = gtk_label_new(buffer);
    gtk_label_set_markup(GTK_LABEL(label),
   "<span foreground='red' underline='double' underline_color='blue' font_desc='12'>COMMAND      PID       DEVICE</span>");
   // gtk_label_set_markup(GTK_LABEL(label));
    list_item = gtk_list_item_new();
    gtk_container_add(GTK_CONTAINER(list_item), label);
    gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
    gtk_widget_show(label);
    gtk_container_add(GTK_CONTAINER(gtklist), list_item);
    gtk_widget_show(list_item);
    gtk_label_get(GTK_LABEL(label), &string);

    for (i = 0; i < ufproc.size() - 1; i++) {
       
       
        const char *cmds = ufproc[i].commands.c_str();
        const char *devs = ufproc[i].devices.c_str();
        //sprintf(buffer, "ListItemContainer with Label #%d", i);

        //  cout << ufproc[i].commands << "  " << ufproc[i].PID << "  " << ufproc[i].devices << endl;
        sprintf(buffer, "  %-20s  %10d  %20s", cmds, ufproc[i].PID, devs);

        label = gtk_label_new(buffer);
        list_item = gtk_list_item_new();
        gtk_container_add(GTK_CONTAINER(list_item), label);
        gtk_misc_set_alignment(GTK_MISC(label), 0, 0);
        gtk_widget_show(label);
        gtk_container_add(GTK_CONTAINER(gtklist), list_item);
        gtk_widget_show(list_item);
        gtk_label_get(GTK_LABEL(label), &string);
        //g_object_set_data (G_OBJECT (list_item), "ufproc[1].commands", string);
    }

    /* Finally create a button and connect its "clicked" signal
     * to the destruction of the window */
    button = gtk_button_new_with_label("Refresh");
    gtk_container_add(GTK_CONTAINER(vbox), button);
    gtk_widget_show(button);
    g_signal_connect_swapped(G_OBJECT(button), "clicked",
            G_CALLBACK(Reprintdata),
            window);


   //cout << ufproc[n].commands << "  " << ufproc[n].PID << "  " << ufproc[n].devices << endl;
    // char *cmds[50];
    //char *devs[50];

    //strncpy(cmds, cmds.c_str(), sizeof(ufproc[n].commands));
    //   for (int n = 0;n<ufproc.size();n++){
    //  *cmds[n] = ufproc[n].commands.c_str();
    // cmds[n] =  (char) ufproc[n].commands;
    // devs[n] = (char) ufproc[n].devices;

    //  }


    dlist = NULL;
    //    for (; i < 10; i++) {
    //	sprintf(buffer, "List Item with Label %d", i);
    //	list_item = gtk_list_item_new_with_label (buffer);
    //	dlist = g_list_prepend (dlist, list_item);
    //	gtk_widget_show (list_item);
    //	g_object_set_data (G_OBJECT (list_item),
    //                           list_item_data_key,
    //                           "ListItem with integrated Label");
    //    }
    gtk_list_append_items(GTK_LIST(gtklist), dlist);


    gtk_widget_show(window);



    gtk_main();




    return 0;
}

void sigh_button_event(GtkWidget *gtklist,
        GdkEventButton *event,
        GtkWidget *frame) {



    if (event->type == GDK_BUTTON_RELEASE &&
            event->button == 3) {
        GList *dlist, *free_list;
        GtkWidget *new_prisoner;




        dlist = GTK_LIST(gtklist)->selection;
        if (dlist)
            new_prisoner = GTK_WIDGET(dlist->data);
        else
            new_prisoner = NULL;


        dlist = gtk_container_children(GTK_CONTAINER(frame));
        free_list = dlist;
        while (dlist) {
            GtkWidget *list_item;

            list_item = (GtkWidget*) dlist->data;

            gtk_widget_reparent(list_item, gtklist);

            dlist = dlist->next;
        }
        g_list_free(free_list);


        if (new_prisoner) {
            GList static_dlist;

            static_dlist.data = new_prisoner;
            static_dlist.next = NULL;
            static_dlist.prev = NULL;

            gtk_list_unselect_child(GTK_LIST(gtklist),
                    new_prisoner);
            gtk_widget_reparent(new_prisoner, frame);
        }
    }
}


//void sigh_print_selection( GtkWidget *gtklist,
//                           gpointer   func_data )
//{
//    GList   *dlist;
//
//    /* Fetch the doubly linked list of selected items
//     * of the List, remember to treat this as read-only!
//     */
//    dlist = GTK_LIST (gtklist)->selection;
//
//    /* If there are no selected items there is nothing more
//     * to do than just telling the user so
//     */
//    if (!dlist) {
//	g_print ("Selection cleared\n");
//	return;
//    }
//    /* Ok, we got a selection and so we print it
//     */
//    g_print ("The selection is a ");
//
//    /* Get the list item from the doubly linked list
//     * and then query the data associated with list_item_data_key.
//     * We then just print it */
//    while (dlist) {
//	const gchar *item_data_string;
//
//	item_data_string = g_object_get_data (G_OBJECT (dlist->data),
//	 				      list_item_data_key);
//	g_print("%s ", item_data_string);
//
//	dlist = dlist->next;
//    }
//    g_print ("\n");
//}