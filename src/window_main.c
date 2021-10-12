#include <stdio.h>
#include "awtk.h"

// define the canva in the window

struct FONT_BUF
{
  int buf_size;
  unsigned char *buffer;
};

struct FONT_BUF font_buf[256];
// get the content of .fnt file
void get_dxt_font_data(char *filename)
{
  FILE *fp;
  int i;
  WORD data;
  if ((fp = fopen(filename, "rb")) == NULL)
  {
    for (i = 0; i < 256; i++)
      font_buf[i].buf_size = 0;
  }
  else
  {
    for (i = 0; i < 256; i++) //生成索引区 1024=4*256 字节
      fread(&font_buf[i].buf_size, sizeof(int), 1, fp);
    for (i = 0; i < 255; i++)
      font_buf[i].buf_size = font_buf[i + 1].buf_size - font_buf[i].buf_size - 2;
    font_buf[255].buf_size = 0;
    for (i = 0; i < 256; i++) //生成数据区
    {
      if (font_buf[i].buf_size == 0)
        fread(&data, sizeof(WORD), 1, fp);
      else
      {
        font_buf[i].buffer = (unsigned char *)malloc(font_buf[i].buf_size + 2);
        fread(font_buf[i].buffer, sizeof(unsigned char), font_buf[i].buf_size + 2, fp);
      }
    }
    fclose(fp);
  }
}

/*all the registered func*/
// the open of show_info window
static ret_t
open_info_window_click(void *ctx, event_t *e)
{
  (void)ctx;
  (void)e;
  open_info_window();
  return RET_OK;
}

//在这里使用ctx传递需要打印的文本内容
static ret_t
show_font(void *ctx, event_t *e)
{
  if (ctx == NULL)
    return RET_OK;
  wchar_t *str = (wchar_t *)ctx;
  size_t len = wcslen(str);
  printf("%ls\n", str);
  paint_event_t *evt = (paint_event_t *)e;
  canvas_t *c = evt->c;
  vgcanvas_t *vg = canvas_get_vgcanvas(c);
  // for (int i = 0; i < len; i++)
  // {
  unsigned char *point = font_buf[100].buffer;
  int num = *point + *(point + 1) * 256;
  while (num != 0)
  {
    for (int j = 0; j < num; j++)
    {
      if (j == 0)
        vgcanvas_move_to(vg, *point + 100, 320 - *(point + 1));
      else
        vgcanvas_line_to(vg, *point + 100, 320 - *(point + 1));
      point += 2;
    }
    num = *point + *(point + 1) * 256;
    point += 2;
  }
  // }
  vgcanvas_stroke(vg);
  return RET_OK;
}

static ret_t
edit_info(void *ctx, event_t *e)
{
  static uint32_t widget_ret = 0;
  widget_t *edit = WIDGET(e->target);
  wchar_t *str = widget_get_text(edit);
  widget_t *win = widget_get_window(edit);
  widget_t *canvas;
  if (win)
    canvas = widget_lookup(win, "canvas", TRUE);
  if (widget_ret != 0)
    widget_off(canvas, widget_ret);
  widget_ret = widget_on(canvas, EVT_PAINT, show_font, (void *)str);
  // printf("%ls\n", str);
  return RET_OK;
}
// open_content_edit_window_click is to open the window content/content_editing
static ret_t
open_content_edit_window_click(void *ctx, event_t *e);

// init the widget
static ret_t init_widget(void *ctx, const void *iter)
{
  widget_t *widget = WIDGET(iter);
  widget_t *win = widget_get_window(widget);
  (void)ctx;
  // 对info按钮注册一个信息回调函数
  if (widget->name != NULL)
  {
    printf("the widget name is %s\n", widget->name);
    const char *name = widget->name;
    if (tk_str_eq(name, "Info"))
      widget_on(widget, EVT_CLICK, open_info_window_click, NULL);
    else if (tk_str_eq(name, "canvas"))
    {
      printf("enter the canvas\n");
      // widget_on(widget, EVT_PAINT, show_font, NULL); //画布的绘制初始化
    }
    else if (tk_str_eq(name, "MyEdit"))
    {
      printf("The Edit\n");
      widget_on(widget, EVT_IM_ACTION, edit_info, NULL);
    }
  }
  return RET_OK;
}

/*遍历窗口控件，对所有控件进行注册*/
static void init_children_widget(widget_t *widget)
{
  widget_foreach(widget, init_widget, widget);
}
/**
 * 初始化
 */
ret_t application_init(void)
{

  widget_t *system_bar = NULL;
  widget_t *system_bar_b = NULL;
  widget_t *win = window_open("home_page");
  FILE *fp = fopen("gothice.fnt", "r");
  get_dxt_font_data("gothice.fnt");
  if (win)
  {
    init_children_widget(win);
  }
  return RET_OK;
}

/**
 * 退出
 */
ret_t application_exit(void)
{
  log_debug("application_exit\n");
  return RET_OK;
}
