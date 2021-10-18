#include "awtk.h"

static ret_t on_close(void *ctx, event_t *e)
{
	widget_t *win = WIDGET(ctx);
	value_t v;
	(void)e;

	return window_close(win);
}

static ret_t init_widget(void *ctx, const void *iter)
{
	widget_t *widget = WIDGET(iter);
	widget_t *win = widget_get_window(widget);
	(void)ctx;
	if (widget->name != NULL)
	{
		const char *name = widget->name;
		if (tk_str_eq(name, "close") || tk_str_eq(name, "ok") || tk_str_eq(name, "cancel"))
		{
			widget_on(widget, EVT_CLICK, on_close, win);
		}
	}
	return RET_OK;
}

static void init_child_widget(widget_t *widget)
{
	widget_foreach(widget, init_widget, widget);
}

ret_t open_info_window()
{
	value_t v;
	widget_t *win = window_open("app_info");
	if (win)
	{
		init_child_widget(win);
	}
	return RET_OK;
}