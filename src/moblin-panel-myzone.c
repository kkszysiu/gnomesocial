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
#include <penge/penge-grid-view.h>


static GTimer *profile_timer = NULL;
static guint stage_paint_idle = 0;

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
  GtkWidget *window, *embed;
  ClutterActor *stage, *grid_view;
  GError *error = NULL;

  setlocale (LC_ALL, "");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);

  g_thread_init (NULL);
  profile_timer = g_timer_new ();

  clutter_init (&argc, &argv);
  gtk_init(&argc, &argv);
  
    mx_style_load_from_file (mx_style_get_default (),
                           THEMEDIR "/panel.css", NULL);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (window, "destroy",
                    G_CALLBACK (gtk_main_quit),
                    NULL);

    embed = gtk_clutter_embed_new ();
    gtk_container_add(GTK_CONTAINER(window), GTK_WIDGET(embed));
    gtk_widget_show (embed);

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
