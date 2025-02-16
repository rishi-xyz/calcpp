#include <gtkmm.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "exprtk.hpp" 

class Calculator : public Gtk::Window {
public:
    Calculator();
    virtual ~Calculator() {}

private:
    void on_button_clicked(const Glib::ustring& value);
    void on_equals_clicked();
    void on_clear_clicked();

    Gtk::Box m_vbox;
    Gtk::Entry m_entry;
    Gtk::Grid m_grid;
    std::string expression;
};

Calculator::Calculator()
    : m_vbox(Gtk::Orientation::VERTICAL), m_entry(), m_grid() {
    set_title("GTKmm Calculator");
    set_default_size(350, 500);
    set_margin(10);

    // Entry field styling
    m_entry.set_editable(false);
    m_entry.set_hexpand(true);
    m_entry.set_margin(10);
    m_entry.set_alignment(Gtk::Align::CENTER);
    m_entry.set_width_chars(15);
    m_entry.set_max_length(50);

    // Add entry field and grid to vbox
    m_vbox.append(m_entry);
    m_vbox.append(m_grid);
    set_child(m_vbox);

    std::vector<std::vector<Glib::ustring>> buttons = {
        {"C", "(", ")", "/"},
        {"7", "8", "9", "*"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"0", ".", "\u232b", "="}
    };

    for (size_t i = 0; i < buttons.size(); ++i) {
        for (size_t j = 0; j < buttons[i].size(); ++j) {
            auto button = Gtk::make_managed<Gtk::Button>(buttons[i][j]);

            // Styling buttons
            button->set_margin(5);
            button->set_hexpand(true);
            button->set_vexpand(true);

            // Click event handling
            if (buttons[i][j] == "=") {
                button->signal_clicked().connect(sigc::mem_fun(*this, &Calculator::on_equals_clicked));
                button->set_css_classes({"suggested-action"});  // Make it stand out
            } else if (buttons[i][j] == "C") {
                button->signal_clicked().connect(sigc::mem_fun(*this, &Calculator::on_clear_clicked));
                button->set_css_classes({"destructive-action"});  // Highlight as clear
            } else if (buttons[i][j] == "\u232b") {
                button->signal_clicked().connect([this]() {
                    if (!expression.empty()) {
                        expression.pop_back();
                        m_entry.set_text(expression);
                    }
                });
            } else {
                button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &Calculator::on_button_clicked), buttons[i][j]));
            }

            m_grid.attach(*button, j, i);
        }
    }
}

// Append clicked value to expression
void Calculator::on_button_clicked(const Glib::ustring& value) {
    expression += value;
    m_entry.set_text(expression);
}

// Evaluate expression using exprtk
void Calculator::on_equals_clicked() {
    exprtk::expression<double> expr;
    exprtk::parser<double> parser;
    exprtk::symbol_table<double> symbol_table;

    expr.register_symbol_table(symbol_table);

    if (parser.compile(expression, expr)) {
        double result = expr.value();
        std::ostringstream out;
        out << std::fixed << std::setprecision(6) << result;
        m_entry.set_text(out.str());
        expression = out.str();
    } else {
        m_entry.set_text("Error");
        expression.clear();
    }
}

// Clear expression and reset entry
void Calculator::on_clear_clicked() {
    expression.clear();
    m_entry.set_text("");
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create("org.gtkmm.calculator");
    return app->make_window_and_run<Calculator>(argc, argv);
}
