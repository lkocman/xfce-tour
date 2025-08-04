#include <gtk/gtk.h>
#include <libxfce4ui/libxfce4ui.h>
#include <librsvg/rsvg.h>

static GtkStack *stack;
static GtkWidget *prev_btn;
static GtkWidget *next_btn;
static GtkWidget *close_btn;
static gint num_pages = 3;

static void switch_page(gint direction) {
    const gchar *current = gtk_stack_get_visible_child_name(stack);
    gint index = atoi(current);
    index += direction;

    if (index < 0) index = 0;
    if (index >= num_pages) index = num_pages - 1;

    gchar next_name[8];
    snprintf(next_name, sizeof(next_name), "%d", index);
    gtk_stack_set_visible_child_name(stack, next_name);

    gtk_widget_set_sensitive(prev_btn, index > 0);
    gtk_widget_set_visible(next_btn, index < num_pages - 1);
    gtk_widget_set_visible(close_btn, index == num_pages - 1);
}

static void on_prev_clicked(GtkButton *btn, gpointer user_data) {
    switch_page(-1);
}

static void on_next_clicked(GtkButton *btn, gpointer user_data) {
    switch_page(1);
}

// Load an SVG image from file path into a GtkImage
static GtkWidget *load_svg_image(const gchar *filepath, gint width, gint height) {
    GdkPixbuf *pixbuf = rsvg_pixbuf_from_file_at_size(filepath, width, height, NULL);
    if (!pixbuf) return gtk_label_new("Image load failed");

    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuf);
    g_object_unref(pixbuf);
    return image;
}

GtkWidget* make_page(const gchar *svg_path, const gchar *text) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);

    GtkWidget *image = load_svg_image(svg_path, 300, 180);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);

    GtkWidget *label = gtk_label_new(text);
    gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
    gtk_label_set_justify(GTK_LABEL(label), GTK_JUSTIFY_CENTER);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    return box;
}

int main(int argc, char **argv) {
    gtk_init(&argc, &argv);

    GtkWidget *dialog = xfce_titled_dialog_new();
    gtk_window_set_title(GTK_WINDOW(dialog), "Welcome to XFCE");
    gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 360);
    gtk_dialog_set_default_response(GTK_DIALOG(dialog), GTK_RESPONSE_NONE);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))), main_vbox);

    // Stack for carousel pages
    stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_type(stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    gtk_stack_set_transition_duration(stack, 300);

    gtk_box_pack_start(GTK_BOX(main_vbox), GTK_WIDGET(stack), TRUE, TRUE, 0);

    // Pages
    gtk_stack_add_titled(stack, make_page("page1.svg", "Welcome to your XFCE desktop!"), "0", "Welcome");
    gtk_stack_add_titled(stack, make_page("page2.svg", "Customize your system in Settings > Appearance."), "1", "Customize");
    gtk_stack_add_titled(stack, make_page("page3.svg", "Need help? Visit docs.xfce.org or forums.xfce.org."), "2", "Support");

    // Navigation
    GtkWidget *nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_set_homogeneous(GTK_BOX(nav_box), TRUE);
    gtk_box_pack_end(GTK_BOX(main_vbox), nav_box, FALSE, FALSE, 8);

    prev_btn = gtk_button_new_with_label("Previous");
    g_signal_connect(prev_btn, "clicked", G_CALLBACK(on_prev_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(nav_box), prev_btn, TRUE, TRUE, 0);

    next_btn = gtk_button_new_with_label("Next");
    g_signal_connect(next_btn, "clicked", G_CALLBACK(on_next_clicked), NULL);
    gtk_box_pack_end(GTK_BOX(nav_box), next_btn, TRUE, TRUE, 0);

    close_btn = gtk_button_new_with_label("Close");
    g_signal_connect_swapped(close_btn, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    gtk_box_pack_end(GTK_BOX(nav_box), close_btn, TRUE, TRUE, 0);

    gtk_widget_set_visible(close_btn, FALSE);
    gtk_widget_set_sensitive(prev_btn, FALSE);
    gtk_stack_set_visible_child_name(stack, "0");

    gtk_widget_show_all(dialog);
    gtk_main();

    gtk_widget_destroy(dialog);
    return 0;
}
