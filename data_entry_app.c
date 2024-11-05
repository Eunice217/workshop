#include <gtk/gtk.h>
#include <string.h>

typedef struct {
    GtkWidget *entry;
    GtkWidget *text_view;
    GString *data;  // Holds the saved data
} AppWidgets;

static void on_save_button_clicked(GtkButton *button, AppWidgets *app) {
    const gchar *input_text = gtk_editable_get_text(GTK_EDITABLE(app->entry));  // Use gtk_editable_get_text instead

    // Append the input text to the data buffer
    g_string_append(app->data, input_text);
    g_string_append(app->data, "\n");

    // Clear the entry after saving
    gtk_editable_set_text(GTK_EDITABLE(app->entry), "");  // Use gtk_editable_set_text instead
}

static void on_display_button_clicked(GtkButton *button, AppWidgets *app) {
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(app->text_view));
    gtk_text_buffer_set_text(buffer, app->data->str, -1);
}

static void activate(GtkApplication *app, gpointer user_data) {
    AppWidgets *widgets = g_malloc(sizeof(AppWidgets));
    widgets->data = g_string_new("");  // Initialize empty data buffer

    // Create a new window
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Data Entry App");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    // Create a grid to hold widgets
    GtkWidget *grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(window), grid);

    // Create entry widget for data input
    widgets->entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), widgets->entry, 0, 0, 2, 1);

    // Create save button
    GtkWidget *save_button = gtk_button_new_with_label("Save Data");
    gtk_grid_attach(GTK_GRID(grid), save_button, 0, 1, 1, 1);
    g_signal_connect(save_button, "clicked", G_CALLBACK(on_save_button_clicked), widgets);

    // Create display button
    GtkWidget *display_button = gtk_button_new_with_label("Display Data");
    gtk_grid_attach(GTK_GRID(grid), display_button, 1, 1, 1, 1);
    g_signal_connect(display_button, "clicked", G_CALLBACK(on_display_button_clicked), widgets);

    // Create text view widget for displaying saved data
    widgets->text_view = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(widgets->text_view), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(widgets->text_view), FALSE);
    GtkWidget *scrolled_window = gtk_scrolled_window_new();
    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), widgets->text_view);
    gtk_grid_attach(GTK_GRID(grid), scrolled_window, 0, 2, 2, 1);

    // Make the window visible
    gtk_widget_set_visible(window, TRUE);
}

int main(int argc, char **argv) {
    GtkApplication *app = gtk_application_new("com.example.DataEntryApp", G_APPLICATION_DEFAULT_FLAGS);  // Use G_APPLICATION_DEFAULT_FLAGS
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}

