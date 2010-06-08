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


#ifndef _PENGE_GRID_VIEW
#define _PENGE_GRID_VIEW

#include <glib-object.h>
#include <mx/mx.h>

G_BEGIN_DECLS

#define PENGE_TYPE_GRID_VIEW penge_grid_view_get_type()

#define PENGE_GRID_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), PENGE_TYPE_GRID_VIEW, PengeGridView))

#define PENGE_GRID_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), PENGE_TYPE_GRID_VIEW, PengeGridViewClass))

#define PENGE_IS_GRID_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PENGE_TYPE_GRID_VIEW))

#define PENGE_IS_GRID_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), PENGE_TYPE_GRID_VIEW))

#define PENGE_GRID_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), PENGE_TYPE_GRID_VIEW, PengeGridViewClass))

typedef struct _PengeGridViewPrivate PengeGridViewPrivate;

typedef struct {
  MxTable parent;
  PengeGridViewPrivate *priv;
} PengeGridView;

typedef struct {
  MxTableClass parent_class;
} PengeGridViewClass;

GType penge_grid_view_get_type (void);

G_END_DECLS

#endif /* _PENGE_GRID_VIEW */

