#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
//gcc shotlinux.c -o shot `pkg-config --cflags --libs gtk+-3.0`


#define GRID_SIZE 16
#define CELL_SIZE 40
#define NUM_CIRCLES 16

typedef struct {
    int x;
    int y;
} Circle;

Circle circles[NUM_CIRCLES];
GtkWidget *drawing_area;

void draw_grid(cairo_t *cr);
void draw_circles(cairo_t *cr);
void initialize_circles();
void add_new_circle();
int check_circle_hit(int x, int y);
void remove_circle(int index);

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    draw_grid(cr);
    draw_circles(cr);
    return FALSE;
}

gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    int x = event->x / CELL_SIZE;
    int y = event->y / CELL_SIZE;
    int hit_index = check_circle_hit(x, y);
    if (hit_index != -1) {
        remove_circle(hit_index);
        add_new_circle();
        gtk_widget_queue_draw(widget);
    }
    return TRUE;
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    srand((unsigned int)time(NULL));

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Circle Shooting Game");
    gtk_window_set_default_size(GTK_WINDOW(window), GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_WINDOW(window),GTK_CONTAINER(drawing_area));
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_button_press_event), NULL);
    gtk_widget_set_events(drawing_area, gtk_widget_get_events(drawing_area) | GDK_BUTTON_PRESS_MASK);

    initialize_circles();
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}

void draw_grid(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0, 0, 0);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        cairo_move_to(cr, i * CELL_SIZE, 0);
        cairo_line_to(cr, i * CELL_SIZE, GRID_SIZE * CELL_SIZE);
        cairo_move_to(cr, 0, i * CELL_SIZE);
        cairo_line_to(cr, GRID_SIZE * CELL_SIZE, i * CELL_SIZE);
    }
    cairo_stroke(cr);
}

void draw_circles(cairo_t *cr) {
    cairo_set_source_rgb(cr, 0, 0, 1);
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        int x1 = circles[i].x * CELL_SIZE + 5;
        int y1 = circles[i].y * CELL_SIZE + 5;
        int x2 = (circles[i].x + 1) * CELL_SIZE - 5;
        int y2 = (circles[i].y + 1) * CELL_SIZE - 5;
        cairo_arc(cr, (x1 + x2) / 2, (y1 + y2) / 2, (x2 - x1) / 2, 0, 2 * G_PI);
        cairo_fill(cr);
    }
}

void initialize_circles() {
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        add_new_circle();
    }
}

void add_new_circle() {
    int x, y;
    int unique;
    do {
        unique = 1;
        x = rand() % GRID_SIZE;
        y = rand() % GRID_SIZE;
        for (int i = 0; i < NUM_CIRCLES; ++i) {
            if (circles[i].x == x && circles[i].y == y) {
                unique = 0;
                break;
            }
        }
    } while (!unique);
    circles[NUM_CIRCLES - 1].x = x;
    circles[NUM_CIRCLES - 1].y = y;
}

int check_circle_hit(int x, int y) {
    for (int i = 0; i < NUM_CIRCLES; ++i) {
        if (circles[i].x == x && circles[i].y == y) {
            return i;
        }
    }
    return -1;
}

void remove_circle(int index) {
    for (int i = index; i < NUM_CIRCLES - 1; ++i) {
        circles[i] = circles[i + 1];
    }
    circles[NUM_CIRCLES - 1].x = -1;
    circles[NUM_CIRCLES - 1].y = -1;
}
