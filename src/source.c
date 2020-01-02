#define EFL_EO_API_SUPPORT 1
#define EFL_BETA_API_SUPPORT 1
#include <Efl_Ui.h>
#include <Elementary.h>

static void
_selection_data_cb(void *data EINA_UNUSED, Eo *obj EINA_UNUSED,
                   Efl_Ui_Selection_Data *sel_data EINA_UNUSED)
{
   printf("_selection_data_cb called\n");
}

static void
_paste_one_click_callback(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *tb = (Eo*) data;
   efl_ui_selection_get(tb, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_TEXT | EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);
}

static void
_paste_click_callback(void *data, const Efl_Event *event EINA_UNUSED)
{
   Eo *tb = (Eo*) data;
   efl_ui_selection_get(tb, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_TEXT | EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);
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
   Eo *win, *box;

   win = efl_add(EFL_UI_WIN_CLASS, efl_main_loop_get(),
                  efl_text_set(efl_added, "Hello world"),
                  efl_ui_win_autodel_set(efl_added, EINA_TRUE));
   efl_event_callback_add(win, EFL_UI_WIN_EVENT_DELETE_REQUEST, _quit_cb, NULL);
   efl_gfx_entity_size_set(win, EINA_SIZE2D(400, 240));

   box = efl_add(EFL_UI_BOX_CLASS, win,
                 efl_content_set(win, efl_added),
                 efl_ui_layout_orientation_set(efl_added, EFL_UI_LAYOUT_ORIENTATION_VERTICAL));

   Eo *btn = efl_add(EFL_UI_BUTTON_CLASS, box,
            efl_text_set(efl_added, "Paste one time"),
            efl_gfx_hint_weight_set(efl_added, EFL_GFX_HINT_EXPAND, 0.1),
            efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED, _paste_one_click_callback, efl_added),
            efl_pack(box, efl_added));
   efl_add(EFL_UI_BUTTON_CLASS, box,
            efl_text_set(efl_added, "Paste two times (This is bug)"),
            efl_gfx_hint_weight_set(efl_added, EFL_GFX_HINT_EXPAND, 0.1),
            efl_event_callback_add(efl_added, EFL_INPUT_EVENT_CLICKED, _paste_click_callback, btn),
            efl_pack(box, efl_added));

   efl_ui_selection_get(btn, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);

   efl_ui_selection_get(btn, EFL_UI_SELECTION_TYPE_CLIPBOARD, EFL_UI_SELECTION_FORMAT_MARKUP,
         NULL, _selection_data_cb, NULL, 1);
}
EFL_MAIN()