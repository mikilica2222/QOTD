#include <pebble.h>

static Window *window;
static TextLayer *text_layer;

static BitmapLayer *image_layer;
static GBitmap *quote_bitmap;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void add_quote_header_layer() {
  quote_bitmap = gbitmap_create_with_resource(RESOURCE_ID_QUOTE);
  image_layer = bitmap_layer_create(GRect(0, 0, 144, 40));
  bitmap_layer_set_bitmap(image_layer, quote_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(image_layer));
}

static void add_text_layer(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_load(Window *window) {
  add_quote_header_layer( );
  add_text_layer(window);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);

  gbitmap_destroy(quote_bitmap);
  bitmap_layer_destroy(image_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
