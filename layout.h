/*void layout_stack(Manager_session *manager_session) {
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
		Manager_window* window = manager_session->window[i];

		printf("%d", window->id);

		if (i == manager_session->active_window) {

			manager_window_update(manager_session, window);
		}
	}
}*/
