#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1
#include <Efl_Ui.h>
#include <Elementary.h>

// #define USE_EFL_UI_TEXTBOX_METHOD
#define SHOW_BUG

static void
_copy_click_callback(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *tb = (Eo*) data;
   efl_text_interactive_all_select(tb);
   efl_ui_textbox_selection_copy(tb);
}
static void
_selection_data_cb(void *data EINA_UNUSED, Eo *obj,
                   Efl_Ui_Selection_Data *sel_data)
{
   Efl_Text_Cursor *cur;
   char *buf = eina_slice_strdup(sel_data->content);

   cur = efl_text_interactive_main_cursor_get(obj);
   if (sel_data->format == EFL_UI_SELECTION_FORMAT_MARKUP)
     {
        efl_text_cursor_markup_insert(cur, buf);
     }
   else
     {
        efl_text_cursor_text_insert(cur, buf);
     }
   free(buf);
}

static void
_paste_click_callback(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *tb = (Eo*) data;
   efl_ui_selection_get(tb, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_TEXT | EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);
}

static void
_quit_cb(void *data EINA_UNUSED, const Efl_Event *event EINA_UNUSED)
{
   efl_exit(0);
}

EAPI_MAIN void
efl_main(void *data EINA_UNUSED, const Efl_Event *ev EINA_UNUSED)
{
   Eo *win, *box, *input;

   win = efl_add(EFL_UI_WIN_CLASS, efl_main_loop_get(),
                  efl_text_set(efl_added, "Hello world"),
                  efl_ui_win_autodel_set(efl_added, EINA_TRUE));
   efl_event_callback_add(win, EFL_UI_WIN_EVENT_DELETE_REQUEST, _quit_cb, NULL);
   efl_gfx_entity_size_set(win, EINA_SIZE2D(400, 240));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                 efl_content_set(win, efl_added),
                 efl_ui_layout_orientation_set(efl_added, EFL_UI_LAYOUT_ORIENTATION_VERTICAL));

   input = efl_add(EFL_UI_TEXTBOX_CLASS, box,
                   efl_gfx_entity_size_set(efl_added, EINA_SIZE2D(400, 50)),
                   efl_gfx_hint_weight_set(efl_added, EFL_GFX_HINT_EXPAND, 0.7),
                   efl_pack(box, efl_added));
   efl_canvas_textblock_style_apply(input, "font=DejaVuSans font_size=10 color=#fff");
   efl_text_markup_set(input, "<font=Serif>Hello World</><ps/>This is EFL<br/>Enlightenment");

   efl_add(EFL_UI_BUTTON_CLASS, box,
            efl_text_set(efl_added, "Copy All"),
            efl_gfx_hint_weight_set(efl_added, EFL_GFX_HINT_EXPAND, 0.1),
            efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED, _copy_click_callback, input),
            efl_pack(box, efl_added));
   efl_add(EFL_UI_BUTTON_CLASS, box,
            efl_text_set(efl_added, "Paste At End"),
            efl_gfx_hint_weight_set(efl_added, EFL_GFX_HINT_EXPAND, 0.1),
            efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED, _paste_click_callback, input),
            efl_pack(box, efl_added));

   // efl_text_interactive_all_select(input);

#ifdef USE_EFL_UI_TEXTBOX_METHOD
   efl_ui_textbox_selection_paste(input);
#ifdef SHOW_BUG
   efl_ui_textbox_selection_paste(input);
#endif
#else
   // or use this callback which is called in efl_ui_textbox_selection_paste method!
   efl_ui_selection_get(input, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);

#ifdef SHOW_BUG
   efl_ui_selection_get(input, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);
#endif
#endif
}
EFL_MAIN()