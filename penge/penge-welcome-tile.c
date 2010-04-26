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

#include <config.h>
#include <glib/gi18n-lib.h>

#include "penge-welcome-tile.h"

G_DEFINE_TYPE (PengeWelcomeTile, penge_welcome_tile, MX_TYPE_TABLE)

#define NEW_PLACEHOLDER_TEXT _("When you’ve set up your web " \
"accounts and used some files they " \
"will appear here on the Myzone " \
"for easy access whenever you " \
"want. "

#define TMP_PLACEHOLDER _("Play intro video")

static void
penge_welcome_tile_dispose (GObject *object)
{
  G_OBJECT_CLASS (penge_welcome_tile_parent_class)->dispose (object);
}

static void
penge_welcome_tile_finalize (GObject *object)
{
  G_OBJECT_CLASS (penge_welcome_tile_parent_class)->finalize (object);
}

static void
penge_welcome_tile_class_init (PengeWelcomeTileClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->dispose = penge_welcome_tile_dispose;
  object_class->finalize = penge_welcome_tile_finalize;
}

static void
penge_welcome_tile_init (PengeWelcomeTile *tile)
{
  ClutterActor *label;
  ClutterActor *tmp_text;

  mx_stylable_set_style_class (MX_STYLABLE (tile),
                               "PengeWelcomeTile");

  label = mx_label_new_with_text (_("Welcome to MeeGo"));
  clutter_actor_set_name ((ClutterActor *)label,
                          "penge-welcome-primary-text");
  tmp_text = mx_label_get_clutter_text (MX_LABEL (label));
  clutter_text_set_line_wrap (CLUTTER_TEXT (tmp_text), TRUE);
  clutter_text_set_line_wrap_mode (CLUTTER_TEXT (tmp_text),
                                   PANGO_WRAP_WORD_CHAR);
  clutter_text_set_ellipsize (CLUTTER_TEXT (tmp_text),
                              PANGO_ELLIPSIZE_NONE);
  mx_table_add_actor_with_properties (MX_TABLE (tile),
                                      (ClutterActor *)label,
                                      0, 0,
                                      "x-expand", TRUE,
                                      "x-fill", TRUE,
                                      "y-expand", FALSE,
                                      "y-fill", TRUE,
                                      NULL);
  mx_table_set_row_spacing (MX_TABLE (tile), 6);

  label = mx_label_new_with_text (_("This is the Myzone, where your recently "
                                    "used files and content from your web feeds will "
                                    "appear."));


  clutter_actor_set_name ((ClutterActor *)label,
                          "penge-welcome-secondary-text");
  tmp_text = mx_label_get_clutter_text (MX_LABEL (label));
  clutter_text_set_line_wrap (CLUTTER_TEXT (tmp_text), TRUE);
  clutter_text_set_line_wrap_mode (CLUTTER_TEXT (tmp_text),
                                   PANGO_WRAP_WORD_CHAR);
  clutter_text_set_ellipsize (CLUTTER_TEXT (tmp_text),
                              PANGO_ELLIPSIZE_NONE);
  mx_table_add_actor_with_properties (MX_TABLE (tile),
                                      (ClutterActor *)label,
                                      1, 0,
                                      "x-expand", TRUE,
                                      "x-fill", TRUE,
                                      "y-expand", TRUE,
                                      "y-fill", TRUE,
                                      NULL);
}

ClutterActor *
penge_welcome_tile_new (void)
{
  return g_object_new (PENGE_TYPE_WELCOME_TILE, NULL);
}


