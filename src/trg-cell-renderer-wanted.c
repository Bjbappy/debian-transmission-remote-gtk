/*
 * transmission-remote-gtk - Transmission RPC client for GTK
 * Copyright (C) 2011  Alan Fitton

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdint.h>
#include <gtk/gtk.h>

#include "trg-cell-renderer-wanted.h"
#include "util.h"

enum {
    PROP_0,
    PROP_WANTED_VALUE
};

G_DEFINE_TYPE(TrgCellRendererWanted, trg_cell_renderer_wanted,
	      GTK_TYPE_CELL_RENDERER_TEXT)
#define TRG_CELL_RENDERER_WANTED_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), TRG_TYPE_CELL_RENDERER_WANTED, TrgCellRendererWantedPrivate))
typedef struct _TrgCellRendererWantedPrivate TrgCellRendererWantedPrivate;

struct _TrgCellRendererWantedPrivate {
    gboolean wanted_value;
};

static void
trg_cell_renderer_wanted_get_property(GObject * object, guint property_id,
				      GValue * value, GParamSpec * pspec)
{
    TrgCellRendererWantedPrivate *priv =
	TRG_CELL_RENDERER_WANTED_GET_PRIVATE(object);
    switch (property_id) {
    case PROP_WANTED_VALUE:
	g_value_set_boolean(value, priv->wanted_value);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
trg_cell_renderer_wanted_set_property(GObject * object, guint property_id,
				      const GValue * value,
				      GParamSpec * pspec)
{
    TrgCellRendererWantedPrivate *priv =
	TRG_CELL_RENDERER_WANTED_GET_PRIVATE(object);
    if (property_id == PROP_WANTED_VALUE) {
	priv->wanted_value = g_value_get_boolean(value);
	if (priv->wanted_value) {
	    g_object_set(object, "text", "Yes", NULL);
	} else {
	    g_object_set(object, "text", "No", NULL);
	}
    } else {
	G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
trg_cell_renderer_wanted_class_init(TrgCellRendererWantedClass * klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->get_property = trg_cell_renderer_wanted_get_property;
    object_class->set_property = trg_cell_renderer_wanted_set_property;

    g_object_class_install_property(object_class,
				    PROP_WANTED_VALUE,
				    g_param_spec_boolean
				    ("wanted-value", "Wanted Value",
				     "Wanted Value", TRUE,
				     G_PARAM_READWRITE |
				     G_PARAM_STATIC_NAME |
				     G_PARAM_STATIC_NICK |
				     G_PARAM_STATIC_BLURB));

    g_type_class_add_private(klass, sizeof(TrgCellRendererWantedPrivate));
}

static void trg_cell_renderer_wanted_init(TrgCellRendererWanted * self)
{
}

GtkCellRenderer *trg_cell_renderer_wanted_new(void)
{
    return
	GTK_CELL_RENDERER(g_object_new
			  (TRG_TYPE_CELL_RENDERER_WANTED, NULL));
}
