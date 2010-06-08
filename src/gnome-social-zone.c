/*
 * Copyright (C) 2008 - 2009 Intel Corporation.
 *
 * Author: Rob Bradford <rob@linux.intel.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU Lesser General Public License,
 * version 2.1, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <locale.h>
#include <config.h>
#include <glib/gi18n-lib.h>
#include <clutter/clutter.h>
#include <clutter-gtk/clutter-gtk.h>
#include <gtk/gtk.h>
#include <mx/mx.h>
#include <socialzone/penge-grid-view.h>


static GTimer *profile_timer = NULL;
static guint stage_paint_idle = 0;

/* Our menu, an array of GtkItemFactoryEntry structures that defines each menu item */
static GtkItemFactoryEntry menu_items[] = {
    { "/_Zone",             NULL,           NULL,               0, "<Branch>" },
    { "/Zone/_New",         "<control>N",   NULL,               0, "<StockItem>", GTK_STOCK_NEW },
    { "/Zone/_Open",        "<control>O",   NULL,               0, "<StockItem>", GTK_STOCK_OPEN },
    { "/Zone/_Save",        "<control>S",   NULL,               0, "<StockItem>", GTK_STOCK_SAVE },
    { "/Zone/Save _As",     NULL,           NULL,               0, "<Item>" },
    { "/Zone/sep1",         NULL,           NULL,               0, "<Separator>" },
    { "/Zone/_Quit",        "<CTRL>Q",      gtk_main_quit,      0, "<StockItem>", GTK_STOCK_QUIT },
    { "/_Options",          NULL,           NULL,               0, "<Branch>" },
    { "/Options/tear",      NULL,           NULL,               0, "<Tearoff>" },
    { "/Options/Check",     NULL,           NULL,               1, "<CheckItem>" },
    { "/Options/sep",       NULL,           NULL,               0, "<Separator>" },
    { "/Options/Rad1",      NULL,           NULL,               1, "<RadioItem>" },
    { "/Options/Rad2",      NULL,           NULL,               2, "/Options/Rad1" },
    { "/Options/Rad3",      NULL,           NULL,               3, "/Options/Rad1" },
    { "/_Help",             NULL,           NULL,               0, "<LastBranch>" },
    { "/_Help/About",       NULL,           NULL,               0, "<Item>" },
};

static gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

/* Returns a menubar widget made from the above menu */
static GtkWidget *get_menubar_menu(GtkWidget *window)
{
    GtkItemFactory *item_factory;
    GtkAccelGroup *accel_group;

    accel_group = gtk_accel_group_new ();

    item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",
                                       accel_group);

    gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);

    gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

    return gtk_item_factory_get_widget (item_factory, "<main>");
}

static GtkWidget *get_text_panel(void)
{
    GtkWidget *textview;

    textview = gtk_text_view_new ();
    gtk_text_view_set_editable (GTK_TEXT_VIEW (textview), TRUE);
    gtk_text_view_set_accepts_tab (GTK_TEXT_VIEW (textview), TRUE);
    gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview), TRUE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD_CHAR);
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW (textview), 2);
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW (textview), 2);
    gtk_text_view_set_pixels_above_lines (GTK_TEXT_VIEW (textview), 2);
    gtk_text_view_set_pixels_below_lines (GTK_TEXT_VIEW (textview), 2);
    
    gtk_container_set_border_width(GTK_CONTAINER(textview), 5);

    return textview;
}

static void _stage_paint_cb (ClutterActor *actor,
                             gpointer      userdata);

static gboolean
_stage_paint_idle_cb (gpointer userdata)
{
  g_message (G_STRLOC ": PROFILE: Idle stage painted: %f",
             g_timer_elapsed (profile_timer, NULL));

  g_signal_handlers_disconnect_by_func (userdata,
                                        _stage_paint_cb,
                                        NULL);
  return FALSE;
}

static void
_stage_paint_cb (ClutterActor *actor,
                 gpointer      userdata)
{
  if (stage_paint_idle == 0)
  {
    stage_paint_idle = g_idle_add_full (G_PRIORITY_LOW,
                                        _stage_paint_idle_cb,
                                        actor,
                                        NULL);
  }
}

int
main (int    argc,
      char **argv)
{
  GtkWidget *window, *embed, *vbox, *menubar, *vpaned, *textpanel;
  ClutterActor *stage, *grid_view;

  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  g_thread_init (NULL);
  profile_timer = g_timer_new ();

  clutter_init (&argc, &argv);
  gtk_init(&argc, &argv);
  g_set_application_name("GNOME Social Zone");
  
    mx_style_load_from_file (mx_style_get_default (),
                           THEMEDIR "/panel.css", NULL);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), _("GNOME Social Zone"));
    gtk_window_set_icon_name (GTK_WINDOW (window), "gnome-social-zone");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);
                    
    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    
    menubar = get_menubar_menu (GTK_WIDGET(window));
    vpaned = gtk_vpaned_new ();
    
    gtk_box_pack_start(GTK_BOX (vbox), menubar, FALSE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX (vbox), vpaned, TRUE, TRUE, 0);

    embed = gtk_clutter_embed_new ();
    
    gtk_paned_pack1 (GTK_PANED (vpaned), GTK_WIDGET(embed), FALSE, FALSE);
    
    textpanel = get_text_panel();

    gtk_paned_pack2 (GTK_PANED (vpaned), textpanel, TRUE, TRUE);

    stage = gtk_clutter_embed_get_stage (GTK_CLUTTER_EMBED (embed));
    clutter_actor_realize (stage);
    
    grid_view = g_object_new (PENGE_TYPE_GRID_VIEW,
                              NULL);
    
    clutter_container_add_actor (CLUTTER_CONTAINER (stage),
                                 (ClutterActor *)grid_view);
    clutter_actor_set_size ((ClutterActor *)grid_view, 1016, 536);
    gtk_widget_set_size_request (embed, 1016, 536);
    
    clutter_actor_show_all (stage);
    
    gtk_widget_show_all (GTK_WIDGET(window));
    
  g_signal_connect_after (stage,
                          "paint",
                          (GCallback)_stage_paint_cb,
                          NULL);

  g_message (G_STRLOC ": PROFILE: Main loop started: %f",
             g_timer_elapsed (profile_timer, NULL));
  gtk_main();

  return 0;
}
