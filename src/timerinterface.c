#include <pebble.h>
#include "timerinterface.h"

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_bitham_42_bold;
static GFont s_res_gothic_18_bold;
static TextLayer *s_textlayer_1;
static TextLayer *s_clock_layer;

static void initialise_ui(void) {
  s_window = window_create();
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_bold = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  s_res_gothic_18_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  // s_textlayer_1
  s_textlayer_1 = text_layer_create(GRect(8, 57, 130, 50));
  text_layer_set_text(s_textlayer_1, "00:00");
  text_layer_set_text_alignment(s_textlayer_1, GTextAlignmentCenter);
  text_layer_set_font(s_textlayer_1, s_res_bitham_42_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_textlayer_1);
  
  // s_clock_layer
  s_clock_layer = text_layer_create(GRect(0, 0, 144, 27));
  text_layer_set_background_color(s_clock_layer, GColorBlack);
  text_layer_set_text_color(s_clock_layer, GColorWhite);
  text_layer_set_text(s_clock_layer, "1:00 PM");
  text_layer_set_text_alignment(s_clock_layer, GTextAlignmentCenter);
  text_layer_set_font(s_clock_layer, s_res_gothic_18_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_clock_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(s_textlayer_1);
  text_layer_destroy(s_clock_layer);
}
// END AUTO-GENERATED UI CODE

static AppTimer *s_timer;

int total_seconds = 0;

int timer_mode = 3;

char labelText[20];

static int presetTimes[] = {5*60, 4*60, 60};
//static int threeMin[] = {3*60, 2*60, 60 + 30, 60, 30, 20, 10};

static int singleAlertTimes[] = {4*60,3*60,2*60,60+30,60,50,40,30,20,10,9,8,7,6};
static int doubleAlertTimes[] = {5,4,3,2,1}; 

//int *presetTimes;

void handleShortVibe() { 
  for (int i = 0; i < (int)(sizeof(singleAlertTimes) / sizeof(singleAlertTimes[0])); i++) {
    if (total_seconds == singleAlertTimes[i]) {
      vibes_cancel();
      vibes_short_pulse();
      return;
    }
  }
}

void handleDoubleVibe() { 
  for (int i = 0; i < (int)(sizeof(doubleAlertTimes) / sizeof(doubleAlertTimes[0])); i++) {
    if (total_seconds == doubleAlertTimes[i]) {
      vibes_cancel();
      vibes_double_pulse();
      return;
    }
  }
}


void syncedNotification() {
  static const uint32_t segments[] = { 50, 50, 50, 50, 50 };
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);
}

void upDownNotification() {
  static const uint32_t segments[] = { 10 };
  VibePattern pat = {
    .durations = segments,
    .num_segments = ARRAY_LENGTH(segments),
  };
  vibes_enqueue_custom_pattern(pat);
}

void updateUI() {
  snprintf(labelText, 10,"%d : %02d", total_seconds / 60, total_seconds % 60);
  text_layer_set_text(s_textlayer_1, labelText);
}

static void timer_callback(void *data) {
  s_timer = app_timer_register(1000, timer_callback, NULL);
  total_seconds--;
  
  int seconds = abs(total_seconds) % 60;
  int minutes = abs(total_seconds) / 60;
  
  snprintf(labelText, 10,"%d : %02d",minutes,seconds);
  
  if (total_seconds < 0)
    snprintf(labelText, 10,"+%d:%02d",abs(total_seconds)/(60*60),abs(total_seconds)/60%60);
  
  text_layer_set_text(s_textlayer_1, labelText);
  
  handleShortVibe();
  handleDoubleVibe();
  if (total_seconds == 0) {
    vibes_cancel();
    vibes_long_pulse();
  }
  
}

static char clock_buffer[] = "00:00";

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    strftime(clock_buffer, sizeof("00:00"), "%H:%M", tick_time);
    text_layer_set_text(s_clock_layer, clock_buffer);
}

void setMinutes(int min) {
   //if (min == 5) presetTimes = fiveMin; 
   //if (min == 3) presetTimes = threeMin;
   total_seconds = min * 60; 
  
  timer_mode = min;
}

int findClosest() {
  
  int closest = 0; 
  int closeness = presetTimes[0];
  for (int j = 0; j < (int)sizeof(presetTimes) / (int)sizeof(presetTimes[0]); j++) {
    int diff = abs(total_seconds - presetTimes[j]);
    if (diff < closeness) {
      closeness = diff; 
      closest = j;
    }
  }
   return closest;  
}


static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  //text_layer_set_text(text_layer, "Select");
  
   

  total_seconds = presetTimes[findClosest()];
  updateUI();
  syncedNotification();
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  total_seconds += 10; 
  updateUI();
  upDownNotification();
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  total_seconds -= 10;
  updateUI();
  upDownNotification();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}



static void handle_window_unload(Window* window) {
  app_timer_cancel(s_timer);
  destroy_ui();
}

void show_timerinterface(void) {
  initialise_ui();
  
  tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);
  
    window_set_click_config_provider(s_window, click_config_provider);

  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
  updateUI();
  
  s_timer = app_timer_register(1000, timer_callback, NULL);
}

void hide_timerinterface(void) {
  
  window_stack_remove(s_window, true);
}
