#include "app.h"
#include "backdrop.h"
#include "instance.h"
#include "log.h"

static void app_close_instance(app_t* app, unsigned i) {
    if (i >= app->num_instances) {
        log_error_and_abort("Invalid index (%u > %u)",
                            i, app->num_instances);
    }
    instance_delete(app->instances[i]);
    app->instances[i] = NULL;
}

instance_t* app_open(app_t* app, const char* filename) {
    instance_t* instance = instance_new(app, filename);

    /*  If loading failed, then do a special one-time drawing of
     *  the backdrop, show an error dialog, and mark the window
     *  as closing in the next event loop */
    if (instance->error) {
        backdrop_draw(instance->backdrop);
        glfwSwapBuffers(instance->window);
        platform_warning("Loading the file failed", instance->error);
        glfwSetWindowShouldClose(instance->window, 1);
    }

    /*  Search for an unused instance slot, which is marked
     *  by a NULL pointer in the instances array */
    unsigned i;
    for (i=0; i < app->num_instances; ++i) {
        if (app->instances[i] == NULL) {
            break;
        }
    }
    /*  Otherwise, extend the array by one */
    if (i == app->num_instances) {
        app->instances = (instance_t**)realloc(
                app->instances, sizeof(instance_t*) * (i + 1));
        app->num_instances++;
    }
    app->instances[i] = instance;
    return instance;
}

bool app_run(app_t* app) {
    bool any_active = false;
    for (unsigned i=0; i < app->num_instances; ++i) {
        if (app->instances[i] != NULL) {
            instance_run(app->instances[i]);
            if (glfwWindowShouldClose(app->instances[i]->window)) {
                app_close_instance(app, i);
            } else {
                any_active = true;
            }
        }
    }
    return any_active;
}
