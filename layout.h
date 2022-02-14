void layout_stack(Manager_session *manager_session) {
  int main_x = 0;
  int main_y = 0;
  int main_width = manager_session->screen->width_in_pixels;
  int main_height = manager_session->screen->height_in_pixels;

  if (manager_session->window_count > 1) {
    main_x = (int)(manager_session->setting->stack_width_percent * 0.01f *
                   manager_session->screen->width_in_pixels);

    main_width =
        manager_session->screen->width_in_pixels - main_x - manager_session->setting->gap_width;
  }

  int stack_window_index = 0;
  int i;

  for (i = 0; i < manager_session->window_count; i++) {
    Manager_window *window = manager_session->window[i];
    if (window->id == manager_session->active_window) {
      window->x = main_x;
      window->y = main_y;
      window->width = main_width;
      window->height = main_height;
    } else {
      window->x = 0;
      window->y =
          (manager_session->screen->height_in_pixels / (manager_session->window_count - 1) +
           0.5f) *
          stack_window_index;
      window->width = manager_session->screen->width_in_pixels - main_width;
      window->height =
          manager_session->screen->height_in_pixels / (manager_session->window_count - 1) +
          manager_session->setting->gap_width;

      if (stack_window_index == manager_session->window_count - 2) {
        window->height = window->height - manager_session->setting->gap_width;
      }

      stack_window_index++;
    }

    manager_window_update(manager_session, window);
  }
}
