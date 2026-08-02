#include "window.hpp"

#include <pfx/engine.hpp>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <cstring>
#include <string>
#ifdef _WIN32
#include <windows.h>
#include "../../resource.h"
#include <FL/x.H>
#endif

namespace
{
    constexpr int WindowWidth = 720;
    constexpr int WindowHeight = 560;

    constexpr int Margin = 20;

    constexpr int MenuHeight = 28;

    constexpr int InputY = 125;

    constexpr int ButtonY = 180;

    constexpr int CompatibilityY = 280;
    constexpr int StandardY = 360;
    constexpr int MaximumY = 440;

    constexpr int InputWidth = 680;
    constexpr int InputHeight = 34;

    constexpr int ButtonWidth = 140;
    constexpr int ButtonHeight = 36;

    constexpr int OutputWidth = 540;
    constexpr int OutputHeight = 30;

    constexpr int CopyWidth = 110;

    constexpr int StatusHeight = 24;
}

namespace pfx::gui
{

    MainWindow::MainWindow()

        : window_(
              WindowWidth,
              WindowHeight,
              "PFX"),

          menu_(
              0,
              0,
              WindowWidth,
              MenuHeight),

          status_(
              8,
              WindowHeight - StatusHeight,
              WindowWidth - 16,
              StatusHeight,
              "Ready"),

          input_(
              Margin,
              InputY,
              InputWidth,
              InputHeight),

          transform_button_(
              (WindowWidth - ButtonWidth) / 2,
              ButtonY,
              ButtonWidth,
              ButtonHeight,
              "Transform"),

          compatibility_output_(
              Margin,
              CompatibilityY,
              OutputWidth,
              OutputHeight),

          standard_output_(
              Margin,
              StandardY,
              OutputWidth,
              OutputHeight),

          maximum_output_(
              Margin,
              MaximumY,
              OutputWidth,
              OutputHeight),

          compatibility_copy_(
              WindowWidth - Margin - CopyWidth,
              CompatibilityY,
              CopyWidth,
              OutputHeight,
              "Copy"),

          standard_copy_(
              WindowWidth - Margin - CopyWidth,
              StandardY,
              CopyWidth,
              OutputHeight,
              "Copy"),

          maximum_copy_(
              WindowWidth - Margin - CopyWidth,
              MaximumY,
              CopyWidth,
              OutputHeight,
              "Copy")
    {
        Fl::background(37, 37, 38);
        Fl::background2(31, 31, 31);
        Fl::foreground(240, 240, 240);

        window_.color(fl_rgb_color(37, 37, 38));

        window_.size_range(
            WindowWidth,
            WindowHeight,
            WindowWidth,
            WindowHeight);

        window_.begin();

        menu_.add(
            "&File/&Exit",
            FL_CTRL + 'Q',
            on_menu,
            this);

        menu_.add(
            "&Help/&Help",
            FL_F + 1,
            on_menu,
            this);

        menu_.add(
            "&Help/&About",
            0,
            on_menu,
            this);

        auto *title = new Fl_Box(
            Margin,
            42,
            WindowWidth - Margin * 2,
            30,
            "PFX");

        title->box(FL_NO_BOX);
        title->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        title->labelfont(FL_HELVETICA_BOLD);
        title->labelsize(24);
        title->labelcolor(fl_rgb_color(245, 245, 245));

        auto *subtitle = new Fl_Box(
            Margin,
            72,
            WindowWidth - Margin * 2,
            18,
            "Transform memorable text into secure passwords");

        subtitle->box(FL_NO_BOX);
        subtitle->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        subtitle->labelsize(12);
        subtitle->labelcolor(fl_rgb_color(170, 170, 170));

        auto *input_label = new Fl_Box(
            Margin,
            102,
            WindowWidth - Margin * 2,
            20,
            "Memorable Plaintext");

        input_label->box(FL_NO_BOX);
        input_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        input_label->labelsize(13);
        input_label->labelcolor(fl_rgb_color(210, 210, 210));

        auto *compatibility_label = new Fl_Box(
            Margin,
            CompatibilityY - 24,
            WindowWidth - Margin * 2,
            20,
            "Compatibility");

        compatibility_label->box(FL_NO_BOX);
        compatibility_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        compatibility_label->labelsize(13);
        compatibility_label->labelcolor(fl_rgb_color(210, 210, 210));

        auto *standard_label = new Fl_Box(
            Margin,
            StandardY - 24,
            WindowWidth - Margin * 2,
            20,
            "Standard");

        standard_label->box(FL_NO_BOX);
        standard_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        standard_label->labelsize(13);
        standard_label->labelcolor(fl_rgb_color(210, 210, 210));

        auto *maximum_label = new Fl_Box(
            Margin,
            MaximumY - 24,
            WindowWidth - Margin * 2,
            20,
            "Maximum");

        maximum_label->box(FL_NO_BOX);
        maximum_label->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
        maximum_label->labelsize(13);
        maximum_label->labelcolor(fl_rgb_color(210, 210, 210));

        input_.color(fl_rgb_color(31, 31, 31));
        input_.textcolor(FL_WHITE);

        compatibility_output_.color(fl_rgb_color(31, 31, 31));
        standard_output_.color(fl_rgb_color(31, 31, 31));
        maximum_output_.color(fl_rgb_color(31, 31, 31));

        compatibility_output_.textcolor(FL_WHITE);
        standard_output_.textcolor(FL_WHITE);
        maximum_output_.textcolor(FL_WHITE);

        compatibility_output_.type(FL_NORMAL_OUTPUT);
        standard_output_.type(FL_NORMAL_OUTPUT);
        maximum_output_.type(FL_NORMAL_OUTPUT);

        transform_button_.color(fl_rgb_color(79, 142, 247));
        transform_button_.labelcolor(FL_WHITE);

        compatibility_copy_.color(fl_rgb_color(60, 60, 60));
        standard_copy_.color(fl_rgb_color(60, 60, 60));
        maximum_copy_.color(fl_rgb_color(60, 60, 60));

        compatibility_copy_.labelcolor(FL_WHITE);
        standard_copy_.labelcolor(FL_WHITE);
        maximum_copy_.labelcolor(FL_WHITE);

        status_.box(FL_FLAT_BOX);
        status_.color(fl_rgb_color(45, 45, 45));
        status_.labelcolor(fl_rgb_color(180, 180, 180));
        status_.align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);

        transform_button_.callback(
            MainWindow::on_transform,
            this);

        compatibility_copy_.callback(
            MainWindow::on_copy,
            this);

        standard_copy_.callback(
            MainWindow::on_copy,
            this);

        maximum_copy_.callback(
            MainWindow::on_copy,
            this);

        input_.when(
            FL_WHEN_ENTER_KEY_ALWAYS);

        input_.callback(
            MainWindow::on_transform,
            this);

        compatibility_output_.value("");
        standard_output_.value("");
        maximum_output_.value("");

        input_.take_focus();

        window_.end();
    }

    void MainWindow::show()
    {
        window_.show();

#ifdef _WIN32

        HWND hwnd = fl_xid(&window_);

        HICON icon = static_cast<HICON>(
            LoadImage(
                GetModuleHandle(nullptr),
                MAKEINTRESOURCE(IDI_APP_ICON),
                IMAGE_ICON,
                0,
                0,
                LR_DEFAULTSIZE));

        if (icon)
        {
            SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)icon);
            SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
        }

#endif
    }

    void MainWindow::transform()
    {
        const char *plaintext = input_.value();

        if (plaintext == nullptr || *plaintext == '\0')
        {
            compatibility_output_.value("");
            standard_output_.value("");
            maximum_output_.value("");

            status_.label("Ready");

            return;
        }

        auto result =
            pfx::transform(plaintext);

        compatibility_output_.value(
            result.compatibility.c_str());

        standard_output_.value(
            result.standard.c_str());

        maximum_output_.value(
            result.maximum.c_str());

        compatibility_output_.redraw();
        standard_output_.redraw();
        maximum_output_.redraw();

        status_.label("Password transformed.");

        window_.redraw();
    }

    void MainWindow::copy_output(
        Fl_Output &output)
    {
        const char *text = output.value();

        if (text == nullptr || *text == '\0')
        {
            return;
        }

        Fl::copy(
            text,
            static_cast<int>(strlen(text)),
            1);

        status_.label("Copied to clipboard.");

        window_.redraw();
    }

    void MainWindow::show_help()
    {
        fl_message(
            "PFX\n\n"
            "1. Enter memorable plaintext.\n"
            "2. Press Transform.\n"
            "3. Choose one of the generated passwords.\n"
            "4. Click Copy.");
    }

    void MainWindow::show_about()
    {
        fl_message(
            "PFX\n\n"
            "Password Transformation Utility\n\n"
            "Version 0.1.0\n"
            "Built with C++20 + FLTK");
    }

    void MainWindow::on_transform(
        Fl_Widget *,
        void *data)
    {
        auto *self =
            static_cast<MainWindow *>(data);

        self->transform();
    }

    void MainWindow::on_copy(
        Fl_Widget *widget,
        void *data)
    {
        auto *self =
            static_cast<MainWindow *>(data);

        if (widget == &self->compatibility_copy_)
        {
            self->copy_output(
                self->compatibility_output_);
        }
        else if (widget == &self->standard_copy_)
        {
            self->copy_output(
                self->standard_output_);
        }
        else if (widget == &self->maximum_copy_)
        {
            self->copy_output(
                self->maximum_output_);
        }
    }

    void MainWindow::on_menu(
        Fl_Widget *,
        void *data)
    {
        auto *self =
            static_cast<MainWindow *>(data);

        const Fl_Menu_Item *item =
            self->menu_.mvalue();

        if (item == nullptr)
        {
            return;
        }

        std::string label =
            item->label();

        if (label == "Exit")
        {
            self->window_.hide();
        }
        else if (label == "Help")
        {
            self->show_help();
        }
        else if (label == "About")
        {
            self->show_about();
        }
    }

    void run()
    {
        MainWindow window;

        window.show();

        Fl::run();
    }

} // namespace pfx::gui