#include <stdio.h>
#include <err.h>
#include <errno.h>
#include <window.h>

struct wl_compositor *compositor;
struct wl_display *display;
//struct wl_pointer *pointer;
struct wl_seat *seat;
struct wl_shell *shell;
struct wl_shm *shm;

static const struct wl_registry_listener registry_listener;
//static const struct wl_pointer_listener pointer_listener;

#define min(a, b)               (a <= b ? a : b)

static void
registry_global(
        void *data, struct wl_registry *registry, uint32_t name,
        const char *interface, uint32_t version)
{
    if (strcmp(interface, wl_compositor_interface.name) == 0)
        compositor = wl_registry_bind(registry, name,
            &wl_compositor_interface, min(version, 4));
    else if (strcmp(interface, wl_shm_interface.name) == 0)
        shm = wl_registry_bind(registry, name,
            &wl_shm_interface, min(version, 1));
    else if (strcmp(interface, wl_shell_interface.name) == 0)
        shell = wl_registry_bind(registry, name,
            &wl_shell_interface, min(version, 1));
#if 0
    else if (strcmp(interface, wl_seat_interface.name) == 0) {
        seat = wl_registry_bind(registry, name,
            &wl_seat_interface, min(version, 2));
        pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(pointer, &pointer_listener,
            NULL);
    }
#endif
}

static void 
registry_global_remove(void *a, struct wl_registry *b, uint32_t c)
{ }

static const struct wl_registry_listener registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove
};

static void shell_surface_ping(void *data,
    struct wl_shell_surface *shell_surface, uint32_t serial)
{
    wl_shell_surface_pong(shell_surface, serial);
}

static void shell_surface_configure(void *data,
    struct wl_shell_surface *shell_surface,
    uint32_t edges, int32_t width, int32_t height) { }

static const struct wl_shell_surface_listener
    shell_surface_listener = {
    .ping = shell_surface_ping,
    .configure = shell_surface_configure,
};


int
main(int argc, char **argv)
{
    struct wl_display *dpy = wl_display_connect(NULL);
    if (!dpy)
        errx(1, "Error connecting to wayland server: wl_display_connect\n");

    struct wl_registry *registry = wl_display_get_registry(dpy);
    wl_registry_add_listener(registry, &registry_listener, NULL);
    wl_display_roundtrip(dpy);
    wl_registry_destroy(registry);

    struct wl_surface *surface;
    struct wl_shell_surface *shell_surface;

    surface = wl_compositor_create_surface(compositor);

    if (surface == NULL)
        errx(1, "Error creating surface: wl_compositor_create_surface\n");

    shell_surface = wl_shell_get_shell_surface(shell, surface);

    if (shell_surface == NULL) {
        wl_surface_destroy(surface);
        errx(1, "Error getting shell surface: wl_shell_get_shell_surface\n");
    }

    wl_shell_surface_add_listener(shell_surface,
        &shell_surface_listener, 0);
    wl_shell_surface_set_toplevel(shell_surface);
    wl_shell_surface_set_user_data(shell_surface, surface);
    wl_surface_set_user_data(surface, NULL);


    while (1)
        wl_display_dispatch(dpy);

    surface = wl_shell_surface_get_user_data(shell_surface);
    wl_shell_surface_destroy(shell_surface);
    wl_surface_destroy(surface);

    //wl_pointer_destroy(pointer);
    //wl_seat_destroy(seat);
    wl_shell_destroy(shell);
    wl_shm_destroy(shm);
    wl_compositor_destroy(compositor);
    wl_display_disconnect(dpy);

    return 0;
}
