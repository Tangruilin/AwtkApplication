// #include "awtk.h"
// /*all the register func should be define at head*/

// // init the widget
// static ret_t init_widget(void *ctx, const void *iter)
// {
// 	widget_t *widget = WIDGET(iter);
// 	widget_t *win = widget_get_window(widget);
// 	(void)ctx;
// 	// registe the callbacks if the name is not nullptr
// 	if (widget->name != NULL)
// 	{
// 		const char *name = widget->name;
// 	}
// 	return RET_OK;
// }

// // foreach the widget
// static void init_children_widget(widget_t *widget, const char *name)
// {
// 	widget_foreach(widget, init_widget, widget);
// }

// // init the widget
// ret_t open_content_edit_window(void)
// {
// 	value_t v;
// 	widget *win = window_open("content/content_editing");
// 	if (win)
// 	{
// 		init_child_widget(win);
// 	}
// 	return RET_OK;
// }