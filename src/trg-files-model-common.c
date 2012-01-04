/*
 * transmission-remote-gtk - A GTK RPC client to Transmission
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

#include <stdlib.h>

#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "protocol-constants.h"
#include "trg-files-model-common.h"

struct SubtreeForeachData {
    gint column;
    gint new_value;
    GtkTreePath *path;
};

static void set_wanted_foreachfunc(GtkTreeModel * model,
        GtkTreePath * path G_GNUC_UNUSED, GtkTreeIter * iter, gpointer data) {
    struct SubtreeForeachData* args = (struct SubtreeForeachData*) data;

    gtk_tree_store_set(GTK_TREE_STORE(model), iter, args->column,
            args->new_value, -1);

    trg_files_tree_model_setSubtree(model, path, iter, args->column,
            args->new_value);
}

static void set_priority_foreachfunc(GtkTreeModel * model, GtkTreePath * path,
        GtkTreeIter * iter, gpointer data) {
    struct SubtreeForeachData* args = (struct SubtreeForeachData*) data;
    GValue value = { 0 };

    g_value_init(&value, G_TYPE_INT);
    g_value_set_int(&value, args->new_value);

    gtk_tree_store_set_value(GTK_TREE_STORE(model), iter, args->column, &value);

    trg_files_tree_model_setSubtree(model, path, iter, args->column,
            args->new_value);
}

void trg_files_model_set_wanted(GtkTreeView *tv, gint column,
        gint new_value) {
    struct SubtreeForeachData args;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(tv);

    args.column = column;
    args.new_value = new_value;

    gtk_tree_selection_selected_foreach(selection, set_wanted_foreachfunc,
            &args);
}

void trg_files_tree_model_set_priority(GtkTreeView *tv, gint column,
        gint new_value) {
    struct SubtreeForeachData args;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(tv);

    args.column = column;
    args.new_value = new_value;

    gtk_tree_selection_selected_foreach(selection, set_priority_foreachfunc,
            &args);

}

static gboolean setSubtreeForeach(GtkTreeModel * model, GtkTreePath * path,
        GtkTreeIter * iter, gpointer gdata) {
    struct SubtreeForeachData *data = gdata;

    if (!gtk_tree_path_compare(path, data->path)
            || gtk_tree_path_is_descendant(path, data->path)) {
        GValue value = { 0 };

        g_value_init(&value, G_TYPE_INT);
        g_value_set_int(&value, data->new_value);

        gtk_tree_store_set_value(GTK_TREE_STORE(model), iter, data->column,
                &value);
    }

    return FALSE; /* keep walking */
}

void trg_files_tree_model_propogateChangeUp(GtkTreeModel * model,
        GtkTreeIter * iter, gint column, gint new_value) {
    GtkTreeIter back_iter = *iter;
    gint result;

    while (1) {
        GtkTreeIter tmp_iter;
        gint n_children, i;

        if (!gtk_tree_model_iter_parent(model, &tmp_iter, &back_iter))
            break;

        n_children = gtk_tree_model_iter_n_children(model, &tmp_iter);

        for (i = 0; i < n_children; i++) {
            GtkTreeIter child;
            gint current_value;

            if (!gtk_tree_model_iter_nth_child(model, &child, &tmp_iter, i))
                continue;

            gtk_tree_model_get(model, &child, column, &current_value, -1);
            if (current_value != new_value) {
                result = TR_PRI_MIXED;
                break;
            }
        }

        gtk_tree_store_set(GTK_TREE_STORE(model), &tmp_iter, column, result,
                -1);

        back_iter = tmp_iter;
    }
}

void trg_files_tree_model_setSubtree(GtkTreeModel * model, GtkTreePath * path,
        GtkTreeIter * iter, gint column, gint new_value) {
    GtkTreeIter back_iter = *iter;

    if (gtk_tree_model_iter_has_child(model, iter)) {
        struct SubtreeForeachData tmp;
        GtkTreeIter parent;

        tmp.column = column;
        tmp.new_value = new_value;
        tmp.path = path;

        gtk_tree_model_foreach(model, setSubtreeForeach, &tmp);
        gtk_tree_model_iter_parent(model, &parent, iter);
    } else {
        gtk_tree_store_set(GTK_TREE_STORE(model), &back_iter, column, new_value,
                -1);
    }

    trg_files_tree_model_propogateChangeUp(model, iter, column, new_value);
}

struct UpdateParentsSizeForeachData {
    gint size_column;
    GtkTreeIter *descendent_iter;
};

static gboolean trg_files_model_update_parents_size_foreach(GtkTreeModel *model,
        GtkTreePath *path, GtkTreeIter *iter, gpointer data) {
    struct UpdateParentsSizeForeachData *args = (struct UpdateParentsSizeForeachData*)data;

    GtkTreePath *descendentPath = gtk_tree_model_get_path(model,
            args->descendent_iter);

    if (gtk_tree_path_is_ancestor(path, descendentPath)
            && gtk_tree_model_get_iter(model, args->descendent_iter, descendentPath)) {
        gint64 size, oldSize;
        gtk_tree_model_get(model, args->descendent_iter, args->size_column, &size, -1);
        gtk_tree_model_get(model, iter, args->size_column, &oldSize, -1);
        gtk_tree_store_set(GTK_TREE_STORE(model), iter, args->size_column,
                size + oldSize, -1);
    }

    gtk_tree_path_free(descendentPath);

    return FALSE;
}

void trg_files_model_update_parents(GtkTreeModel *model, GtkTreeIter *iter, gint size_column) {
    struct UpdateParentsSizeForeachData args;
    args.descendent_iter = iter;
    args.size_column = size_column;
    gtk_tree_model_foreach(model,
            trg_files_model_update_parents_size_foreach, &args);
}