/*
 * Copyright (C) 2009 Intel Corporation.
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

#include <mojito-client/mojito-client.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib/gi18n.h>
#include <moblin-panel/mpl-utils.h>

#include "penge-everything-pane.h"
#include "penge-recent-file-tile.h"
#include "penge-people-tile.h"
#include "penge-block-layout.h"

G_DEFINE_TYPE (PengeEverythingPane, penge_everything_pane, CLUTTER_TYPE_BOX)

#define GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), PENGE_TYPE_EVERYTHING_PANE, PengeEverythingPanePrivate))

typedef struct _PengeEverythingPanePrivate PengeEverythingPanePrivate;

struct _PengeEverythingPanePrivate {
  MojitoClient *client;
  MojitoClientView *view;
  GtkRecentManager *recent_manager;
  ClutterLayoutManager *layout;
  GHashTable *pointer_to_actor;

  gint block_count;

  guint update_timeout_id;
};

static void
penge_everything_pane_get_property (GObject *object, guint property_id,
                              GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
penge_everything_pane_set_property (GObject *object, guint property_id,
                              const GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
  }
}

static void
penge_everything_pane_dispose (GObject *object)
{
  G_OBJECT_CLASS (penge_everything_pane_parent_class)->dispose (object);
}

static void
penge_everything_pane_finalize (GObject *object)
{
  G_OBJECT_CLASS (penge_everything_pane_parent_class)->finalize (object);
}

static void
penge_everything_pane_constructed (GObject *object)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (object);
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);

  clutter_box_set_layout_manager (CLUTTER_BOX (pane), priv->layout);

  if (G_OBJECT_CLASS (penge_everything_pane_parent_class))
    G_OBJECT_CLASS (penge_everything_pane_parent_class)->finalize (object);
}

/* These are needed since the layout manager allocates {0, 0, 0, 0} for
 * children it can't lay out.
 */
static void
_paint_foreach_cb (ClutterActor *actor,
                   gpointer      data)
{
  ClutterActorBox actor_box;
  gfloat w, h;

  clutter_actor_get_allocation_box (actor, &actor_box);

  clutter_actor_box_get_size (&actor_box, &w, &h);

  if (w > 0 && h > 0)
  {
    clutter_actor_paint (actor);
  }
}

static void
penge_everything_pane_pick (ClutterActor       *actor,
                            const ClutterColor *color)
{
  clutter_container_foreach (CLUTTER_CONTAINER (actor),
                             (ClutterCallback)_paint_foreach_cb,
                             actor);
}

static void
penge_everything_pane_paint (ClutterActor *actor)
{
  clutter_container_foreach (CLUTTER_CONTAINER (actor),
                             (ClutterCallback)_paint_foreach_cb,
                             actor);
}

static void
penge_everything_pane_class_init (PengeEverythingPaneClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  ClutterActorClass *actor_class = CLUTTER_ACTOR_CLASS (klass);

  g_type_class_add_private (klass, sizeof (PengeEverythingPanePrivate));

  object_class->get_property = penge_everything_pane_get_property;
  object_class->set_property = penge_everything_pane_set_property;
  object_class->dispose = penge_everything_pane_dispose;
  object_class->finalize = penge_everything_pane_finalize;
  object_class->constructed = penge_everything_pane_constructed;

  actor_class->paint = penge_everything_pane_paint;
  actor_class->pick = penge_everything_pane_pick;
}

/* Sort funct for sorting recent files */
static gint
_recent_files_sort_func (GtkRecentInfo *a,
                         GtkRecentInfo *b)
{
  time_t time_a;
  time_t time_b;

  if (gtk_recent_info_get_modified (a) > gtk_recent_info_get_visited (a))
    time_a = gtk_recent_info_get_modified (a);
  else
    time_a = gtk_recent_info_get_visited (a);

  if (gtk_recent_info_get_modified (b) > gtk_recent_info_get_visited (b))
    time_b = gtk_recent_info_get_modified (b);
  else
    time_b = gtk_recent_info_get_visited (b);

  if (time_a > time_b)
  {
    return -1;
  } else if (time_a < time_b) {
    return 1;
  } else {
    return 0;
  }
}

/* Compare a MojitoItem with a GtkRecentInfo */
static gint
_compare_item_and_info (MojitoItem    *item,
                        GtkRecentInfo *info)
{
  time_t time_a;
  time_t time_b;

  /* Prefer info */
  if (item == NULL)
    return 1;

  /* Prefer item */
  if (info == NULL)
    return -1;

  time_a = item->date.tv_sec;

  if (gtk_recent_info_get_modified (info) > gtk_recent_info_get_visited (info))
    time_b = gtk_recent_info_get_modified (info);
  else
    time_b = gtk_recent_info_get_visited (info);

  if (time_a > time_b)
  {
    return -1;
  } else if (time_a < time_b) {
    return 1;
  } else {
    return 0;
  }
}

static ClutterActor *
_add_from_mojito_item (PengeEverythingPane *pane,
                       MojitoItem          *item)
{
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);
  ClutterActor *actor;

  actor = g_object_new (PENGE_TYPE_PEOPLE_TILE,
                        "item", item,
                        NULL);

  clutter_container_add_actor (CLUTTER_CONTAINER (pane), actor);

  if (penge_people_tile_is_double_size (PENGE_PEOPLE_TILE (actor)))
  {
    clutter_layout_manager_child_set (priv->layout,
                                      CLUTTER_CONTAINER (pane),
                                      actor,
                                      "col-span", 2,
                                      NULL);
  }

  return actor;
}

static ClutterActor *
_add_from_recent_file_info (PengeEverythingPane *pane,
                            GtkRecentInfo       *info,
                            const gchar         *thumbnail_path)
{
  ClutterActor *actor;

  actor = g_object_new (PENGE_TYPE_RECENT_FILE_TILE,
                        "info", info,
                        "thumbnail-path", thumbnail_path,
                        NULL);

  clutter_container_add_actor (CLUTTER_CONTAINER (pane), actor);

  return actor;
}

static void
penge_everything_pane_update (PengeEverythingPane *pane)
{
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);
  GList *mojito_items, *recent_file_items, *l;
  GList *old_actors = NULL;
  ClutterActor *actor;

  /* Get recent files and sort */
  recent_file_items = gtk_recent_manager_get_items (priv->recent_manager);
  recent_file_items = g_list_sort (recent_file_items,
                                   (GCompareFunc)_recent_files_sort_func);

  /* Get Mojito items */
  mojito_items = mojito_client_view_get_sorted_items (priv->view);

  old_actors = g_hash_table_get_values (priv->pointer_to_actor);

  while (mojito_items || recent_file_items)
  {
    MojitoItem *mojito_item = NULL;
    GtkRecentInfo *recent_file_info = NULL;

    /* If no mojito items -> force compare to favour recent file */
    if (mojito_items)
      mojito_item = (MojitoItem *)mojito_items->data;
    else
      mojito_item = NULL;

    /* If no recent files -> force compare to favour mojito stuff */
    if (recent_file_items)
      recent_file_info = (GtkRecentInfo *)recent_file_items->data;
    else
      recent_file_info = NULL;

    if (_compare_item_and_info (mojito_item, recent_file_info) < 1)
    {
      /* Mojito item is newer */

      actor = g_hash_table_lookup (priv->pointer_to_actor,
                                   mojito_item);
      if (!actor)
      {
        actor = _add_from_mojito_item (pane, mojito_item);
        g_hash_table_insert (priv->pointer_to_actor,
                             mojito_item,
                             actor);

        /* Needed to remove from hash when we kill the actor */
        g_object_set_data (G_OBJECT (actor), "data-pointer", mojito_item);
      }

      mojito_items = g_list_remove (mojito_items, mojito_item);
    } else {
      /* Recent file item is newer */

      actor = g_hash_table_lookup (priv->pointer_to_actor,
                                   recent_file_info);

      if (!actor)
      {
        const gchar *uri = NULL;
        gchar *thumbnail_path = NULL;

        /* Skip *local* non-existing files */
        if (gtk_recent_info_is_local (recent_file_info) &&
          !gtk_recent_info_exists (recent_file_info))
        {
          gtk_recent_info_unref (recent_file_info);
          recent_file_items = g_list_remove (recent_file_items,
                                             recent_file_info);

          continue;
        }

        uri = gtk_recent_info_get_uri (recent_file_info);
        thumbnail_path = mpl_utils_get_thumbnail_path (uri);

        /* Skip those without thumbnail */
        if (!g_file_test (thumbnail_path, G_FILE_TEST_EXISTS))
        {
          gtk_recent_info_unref (recent_file_info);
          recent_file_items = g_list_remove (recent_file_items,
                                             recent_file_info);
          g_free (thumbnail_path);
          continue;
        }

        actor = _add_from_recent_file_info (pane,
                                            recent_file_info,
                                            thumbnail_path);

        g_free (thumbnail_path);
        g_hash_table_insert (priv->pointer_to_actor,
                             recent_file_info,
                             actor);

        /* Needed to remove from hash when we kill the actor */
        g_object_set_data (G_OBJECT (actor), "data-pointer", recent_file_info);
      }

      gtk_recent_info_unref (recent_file_info);
      recent_file_items = g_list_remove (recent_file_items,
                                         recent_file_info);
    }

    clutter_container_lower_child (CLUTTER_CONTAINER (pane),
                                   actor,
                                   NULL);

    old_actors = g_list_remove (old_actors, actor);
  }

  for (l = old_actors; l; l = l->next)
  {
    gpointer p;
    p = g_object_get_data (G_OBJECT (l->data), "data-pointer");
    clutter_container_remove_actor (CLUTTER_CONTAINER (pane),
                                    CLUTTER_ACTOR (l->data));
    g_hash_table_remove (priv->pointer_to_actor, p);
  }

  g_list_free (old_actors);

  g_list_free (mojito_items);
  g_list_free (recent_file_items);
}

static gboolean
_update_timeout_cb (gpointer userdata)
{
  PengeEverythingPane *pane = (PengeEverythingPane *)userdata;
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);

  penge_everything_pane_update (pane);

  if (priv->block_count > 0)
  {
    /* We do this hack since we need to have something in the container to get
     * going.
     */
    clutter_actor_set_opacity (CLUTTER_ACTOR (pane), 0xff);
  }

  priv->update_timeout_id = 0;

  return FALSE;
}

static void
penge_everything_pane_queue_update (PengeEverythingPane *pane)
{
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);

  if (priv->update_timeout_id)
  {
    /* No need to update, we already have an update queued */
  } else {
    priv->update_timeout_id = g_timeout_add_seconds (1,
                                                     _update_timeout_cb,
                                                     pane);
  }
}

static void
_view_item_added_cb (MojitoClientView *view,
                     MojitoItem       *item,
                     gpointer          userdata)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (userdata);

  penge_everything_pane_queue_update (pane);
}

static void
_view_item_removed_cb (MojitoClientView *view,
                       MojitoItem       *item,
                       gpointer          userdata)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (userdata);

  penge_everything_pane_queue_update (pane);
}

static void
_client_open_view_cb (MojitoClient     *client,
                      MojitoClientView *view,
                      gpointer          userdata)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (userdata);
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);

  priv->view = view;

  g_signal_connect (view,
                    "item-added",
                    (GCallback)_view_item_added_cb,
                    userdata);
  g_signal_connect (view,
                    "item-removed",
                    (GCallback)_view_item_removed_cb,
                    userdata);
  mojito_client_view_start (view);
}

static void
_client_get_services_cb (MojitoClient *client,
                         const GList  *services,
                         gpointer      userdata)
{
  mojito_client_open_view (client,
                           (GList *)services,
                           20,
                           _client_open_view_cb,
                           userdata);
}

static void
_recent_manager_changed_cb (GtkRecentManager *manager,
                            gpointer          userdata)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (userdata);

  penge_everything_pane_queue_update (pane);
}

static void
_layout_count_changed_cb (PengeBlockLayout *layout,
                          gint              count,
                          gpointer          userdata)
{
  PengeEverythingPane *pane = PENGE_EVERYTHING_PANE (userdata);
  PengeEverythingPanePrivate *priv = GET_PRIVATE (pane);

  if (priv->block_count != count)
  {
    priv->block_count = count;
    penge_everything_pane_queue_update (pane);
  }
}

static void
penge_everything_pane_init (PengeEverythingPane *self)
{
  PengeEverythingPanePrivate *priv = GET_PRIVATE (self);

  /* pointer to pointer */
  priv->pointer_to_actor = g_hash_table_new (NULL, NULL);

  priv->client = mojito_client_new ();
  mojito_client_get_services (priv->client, _client_get_services_cb, self);

  priv->recent_manager = gtk_recent_manager_new ();

  g_signal_connect (priv->recent_manager,
                    "changed",
                    (GCallback)_recent_manager_changed_cb,
                    self);

  priv->layout = penge_block_layout_new ();
  penge_block_layout_set_spacing (PENGE_BLOCK_LAYOUT (priv->layout), 10);
  penge_block_layout_set_min_tile_size (PENGE_BLOCK_LAYOUT (priv->layout),
                                        140,
                                        92);

  g_signal_connect (priv->layout,
                    "count-changed",
                    (GCallback)_layout_count_changed_cb,
                    self);

  clutter_actor_set_opacity (CLUTTER_ACTOR (self), 0x0);
}
