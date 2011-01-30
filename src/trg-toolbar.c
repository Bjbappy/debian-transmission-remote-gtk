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

#include <gtk/gtk.h>
#include "trg-toolbar.h"

enum {
    PROP_0,
    PROP_CONNECT_BUTTON,
    PROP_DISCONNECT_BUTTON,
    PROP_ADD_BUTTON,
    PROP_ADD_URL_BUTTON,
    PROP_REMOVE_BUTTON,
    PROP_DELETE_BUTTON,
    PROP_RESUME_BUTTON,
    PROP_PAUSE_BUTTON,
    /*PROP_VERIFY_BUTTON, */
    PROP_PROPS_BUTTON,
    PROP_REMOTE_PREFS_BUTTON,
    PROP_LOCAL_PREFS_BUTTON
};

G_DEFINE_TYPE(TrgToolbar, trg_toolbar, GTK_TYPE_TOOLBAR)
#define TRG_TOOLBAR_GET_PRIVATE(o) \
  (G_TYPE_INSTANCE_GET_PRIVATE ((o), TRG_TYPE_TOOLBAR, TrgToolbarPrivate))
typedef struct _TrgToolbarPrivate TrgToolbarPrivate;

struct _TrgToolbarPrivate {
    GtkWidget *tb_connect;
    GtkWidget *tb_disconnect;
    GtkWidget *tb_add;
    /*GtkWidget *tb_add_url; */
    GtkWidget *tb_remove;
    GtkWidget *tb_delete;
    GtkWidget *tb_resume;
    GtkWidget *tb_pause;
    /*GtkWidget *tb_verify; */
    GtkWidget *tb_props;
    GtkWidget *tb_remote_prefs;
    GtkWidget *tb_local_prefs;
};

static void
trg_toolbar_get_property(GObject * object, guint property_id,
			 GValue * value, GParamSpec * pspec)
{
    TrgToolbarPrivate *priv = TRG_TOOLBAR_GET_PRIVATE(object);

    switch (property_id) {
    case PROP_CONNECT_BUTTON:
	g_value_set_object(value, priv->tb_connect);
	break;
    case PROP_DISCONNECT_BUTTON:
	g_value_set_object(value, priv->tb_disconnect);
	break;
    case PROP_ADD_BUTTON:
	g_value_set_object(value, priv->tb_add);
	break;
/*	case PROP_ADD_URL_BUTTON:
	        g_value_set_object(value, priv->tb_add_url);
	        break;*/
    case PROP_REMOVE_BUTTON:
	g_value_set_object(value, priv->tb_remove);
	break;
    case PROP_DELETE_BUTTON:
	g_value_set_object(value, priv->tb_delete);
	break;
    case PROP_RESUME_BUTTON:
	g_value_set_object(value, priv->tb_resume);
	break;
    case PROP_PAUSE_BUTTON:
	g_value_set_object(value, priv->tb_pause);
	break;
	/*case PROP_VERIFY_BUTTON:
	   g_value_set_object(value, priv->tb_verify);
	   break; */
    case PROP_PROPS_BUTTON:
	g_value_set_object(value, priv->tb_props);
	break;
    case PROP_REMOTE_PREFS_BUTTON:
	g_value_set_object(value, priv->tb_remote_prefs);
	break;
    case PROP_LOCAL_PREFS_BUTTON:
	g_value_set_object(value, priv->tb_local_prefs);
	break;
    default:
	G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
trg_toolbar_install_widget_prop(GObjectClass * class, guint propId,
				const gchar * name, const gchar * nick)
{
    g_object_class_install_property(class,
				    propId,
				    g_param_spec_object(name,
							nick,
							nick,
							GTK_TYPE_WIDGET,
							G_PARAM_READABLE
							|
							G_PARAM_STATIC_NAME
							|
							G_PARAM_STATIC_NICK
							|
							G_PARAM_STATIC_BLURB));
}

static void trg_toolbar_class_init(TrgToolbarClass * klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->get_property = trg_toolbar_get_property;

    trg_toolbar_install_widget_prop(object_class, PROP_CONNECT_BUTTON,
				    "connect-button", "Connect Button");
    trg_toolbar_install_widget_prop(object_class,
				    PROP_DISCONNECT_BUTTON,
				    "disconnect-button",
				    "Disconnect Button");
    trg_toolbar_install_widget_prop(object_class, PROP_ADD_BUTTON,
				    "add-button", "Add Button");
    trg_toolbar_install_widget_prop(object_class, PROP_ADD_URL_BUTTON,
				    "add-url-button", "Add URL Button");
    trg_toolbar_install_widget_prop(object_class, PROP_REMOVE_BUTTON,
				    "remove-button", "Remove Button");
    trg_toolbar_install_widget_prop(object_class, PROP_DELETE_BUTTON,
				    "delete-button", "Delete Button");
    trg_toolbar_install_widget_prop(object_class, PROP_RESUME_BUTTON,
				    "resume-button", "Resume Button");
    trg_toolbar_install_widget_prop(object_class, PROP_PAUSE_BUTTON,
				    "pause-button", "Pause Button");
    /*trg_toolbar_install_widget_prop(object_class, PROP_VERIFY_BUTTON,
       "verify-button", "Verify Button"); */
    trg_toolbar_install_widget_prop(object_class, PROP_PROPS_BUTTON,
				    "props-button", "Props Button");
    trg_toolbar_install_widget_prop(object_class,
				    PROP_REMOTE_PREFS_BUTTON,
				    "remote-prefs-button",
				    "Remote Prefs Button");
    trg_toolbar_install_widget_prop(object_class,
				    PROP_LOCAL_PREFS_BUTTON,
				    "local-prefs-button",
				    "Local Prefs Button");

    g_type_class_add_private(klass, sizeof(TrgToolbarPrivate));
}

static
GtkWidget *trg_toolbar_item_new(TrgToolbar * toolbar, gchar * text,
				int *index, gchar * icon,
				gboolean sensitive)
{
    GtkToolItem *w = gtk_tool_button_new_from_stock(icon);
    gtk_widget_set_sensitive(GTK_WIDGET(w), sensitive);
    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), w, (*index)++);
    return GTK_WIDGET(w);
}

void trg_toolbar_connected_change(TrgToolbar * tb, gboolean connected)
{
    TrgToolbarPrivate *priv = TRG_TOOLBAR_GET_PRIVATE(tb);

    gtk_widget_set_sensitive(priv->tb_add, connected);
    /*gtk_widget_set_sensitive(priv->tb_add_url, connected); */
    gtk_widget_set_sensitive(priv->tb_connect, !connected);
    gtk_widget_set_sensitive(priv->tb_disconnect, connected);
    gtk_widget_set_sensitive(priv->tb_remote_prefs, connected);
}

void trg_toolbar_torrent_actions_sensitive(TrgToolbar * tb,
					   gboolean sensitive)
{
    TrgToolbarPrivate *priv = TRG_TOOLBAR_GET_PRIVATE(tb);

    gtk_widget_set_sensitive(priv->tb_props, sensitive);
    gtk_widget_set_sensitive(priv->tb_remove, sensitive);
    gtk_widget_set_sensitive(priv->tb_delete, sensitive);
    gtk_widget_set_sensitive(priv->tb_resume, sensitive);
    gtk_widget_set_sensitive(priv->tb_pause, sensitive);
    /*gtk_widget_set_sensitive(priv->tb_verify, sensitive); */
}

static void trg_toolbar_init(TrgToolbar * self)
{
    GtkToolItem *separator;
    TrgToolbarPrivate *priv;
    int position = 0;

    priv = TRG_TOOLBAR_GET_PRIVATE(self);

    gtk_orientable_set_orientation(GTK_ORIENTABLE(self),
				   GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_icon_size(GTK_TOOLBAR(self),
			      GTK_ICON_SIZE_LARGE_TOOLBAR);
    gtk_toolbar_set_style(GTK_TOOLBAR(self), GTK_TOOLBAR_ICONS);

    priv->tb_connect =
	trg_toolbar_item_new(self, "Connect", &position,
			     GTK_STOCK_CONNECT, TRUE);
    priv->tb_disconnect =
	trg_toolbar_item_new(self, "Disconnect", &position,
			     GTK_STOCK_DISCONNECT, FALSE);
    priv->tb_add =
	trg_toolbar_item_new(self, "Add", &position, GTK_STOCK_ADD, FALSE);

    /*priv->tb_add_url =
       trg_toolbar_item_new(self, "Add URL", 3, GTK_STOCK_ADD, FALSE); */

    separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(self), separator, position++);

    priv->tb_resume =
	trg_toolbar_item_new(self, "Resume", &position,
			     GTK_STOCK_MEDIA_PLAY, FALSE);
    priv->tb_pause =
	trg_toolbar_item_new(self, "Pause", &position,
			     GTK_STOCK_MEDIA_PAUSE, FALSE);

    priv->tb_props =
	trg_toolbar_item_new(self, "Properties", &position,
			     GTK_STOCK_PROPERTIES, FALSE);

    priv->tb_remove =
	trg_toolbar_item_new(self, "Remove", &position,
			     GTK_STOCK_REMOVE, FALSE);

    priv->tb_delete =
	trg_toolbar_item_new(self, "Remove with data",
			     &position, GTK_STOCK_DELETE, FALSE);

    /*priv->tb_verify =
       trg_toolbar_item_new(self, "Verify", 11,
       GTK_STOCK_REFRESH, FALSE); */

    separator = gtk_separator_tool_item_new();
    gtk_toolbar_insert(GTK_TOOLBAR(self), separator, position++);

    priv->tb_local_prefs =
	trg_toolbar_item_new(self, "Local Preferences", &position,
			     GTK_STOCK_PREFERENCES, TRUE);

    priv->tb_remote_prefs =
	trg_toolbar_item_new(self, "Remote Preferences",
			     &position, GTK_STOCK_NETWORK, FALSE);
}

TrgToolbar *trg_toolbar_new(void)
{
    return g_object_new(TRG_TYPE_TOOLBAR, NULL);
}
