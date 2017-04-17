#include <pebble.h>
#include "wait_for_start.h"
#include "timerinterface.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_gothic_24_bold;
static TextLayer *s_textlayer_1;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_gothic_24_bold = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(18, 42, 109, 81));
  text_layer_set_background_color(s_textlayer_1, GColorClear);
  text_layer_set_text(s_textlayer_1, "Flick wrist or press a button to start");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_gothic_24_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void start_timer() {
  
  show_timerinterface();
  accel_tap_service_unsubscribe();
  hide_wait_for_start();

}

static void accel_tap_handler(AccelAxisType axis, int32_t direction) {
  // A tap event occured
  
  start_timer();
}


static void click_hander(ClickRecognizerRef recognizer, void *context) {
  start_timer();
}

static void click_config_provider(void *context) {
  
  window_single_click_subscribe(BUTTON_ID_SELECT, click_hander);
  window_single_click_subscribe(BUTTON_ID_UP, click_hander);
  window_single_click_subscribe(BUTTON_ID_DOWN, click_hander);
  
}

void show_wait_for_start(void) {
  
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  
  accel_tap_service_subscribe(accel_tap_handler);
  
  window_set_click_config_provider(s_window, click_config_provider);

}

void hide_wait_for_start(void) {
  accel_tap_service_unsubscribe();
  window_stack_remove(s_window, true);
}
