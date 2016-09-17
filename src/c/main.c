#include "pebble.h"
#include "timerinterface.h"

#define NUM_MENU_SECTIONS 1
#define NUM_MENU_ICONS 0
#define NUM_FIRST_MENU_ITEMS 2
#define NUM_SECOND_MENU_ITEMS 1

static Window *s_main_window;
static MenuLayer *s_menu_layer;

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data) {
  return NUM_MENU_SECTIONS;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
      return 2;
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data) {
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
  menu_cell_basic_header_draw(ctx, cell_layer, "Race Timer");
}

static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  // Determine which section we're going to draw in

      // Use the row to specify which item we'll draw
  switch (cell_index->row) {
    case 0:
      // This is a basic menu item with a title and subtitle
      menu_cell_basic_draw(ctx, cell_layer, "3 minute", "race timer", NULL);
      break;
    case 1:
      // This is a basic menu icon with a cycling icon
      menu_cell_basic_draw(ctx, cell_layer, "5 minute", "race timer", NULL);
      //menu_cell_basic_draw(ctx, cell_layer, "", "", NULL);
      break;
  }

}

static void menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data) {
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Selected Row %d", cell_index->row);
  

  // Use the row to specify which item will receive the select action
  switch (cell_index->row) {
    // This is the menu item with the cycling icon
    case 1:
      setMinutes(5);
      break;
    case 0:
      setMinutes(3);
      break;
    }

    show_timerinterface();
}

static void main_window_load(Window *window) {
  // Here we load the bitmap assets
  // And also load the background

  // Now we prepare to initialize the menu layer
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  // Create the menu layer
  s_menu_layer = menu_layer_create(bounds);
  menu_layer_set_callbacks(s_menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = menu_get_header_height_callback,
    .draw_header = menu_draw_header_callback,
    .draw_row = menu_draw_row_callback,
    .select_click = menu_select_callback,
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(s_menu_layer, window);
  
 // window_set_fullscreen(window, false);

  layer_add_child(window_layer, menu_layer_get_layer(s_menu_layer));
}

static void main_window_unload(Window *window) {
  // Destroy the menu layer
  menu_layer_destroy(s_menu_layer);
}

static void init() {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}