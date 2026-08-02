#pragma once

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

namespace pfx::gui
{
    class MainWindow
    {
    public:
        MainWindow();

        void show();

    private:
        static void on_transform(
            Fl_Widget *,
            void *);

        static void on_copy(
            Fl_Widget *,
            void *);

        static void on_menu(
            Fl_Widget *,
            void *);

        void transform();

        void copy_output(
            Fl_Output &output);

        void show_help();

        void show_about();

        Fl_Window window_;

        Fl_Menu_Bar menu_;

        Fl_Box status_;

        Fl_Input input_;

        Fl_Button transform_button_;

        Fl_Output compatibility_output_;
        Fl_Output standard_output_;
        Fl_Output maximum_output_;

        Fl_Button compatibility_copy_;
        Fl_Button standard_copy_;
        Fl_Button maximum_copy_;
    };

    void run();
}