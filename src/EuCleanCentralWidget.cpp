#include <fstream>
#include "EuCleanCentralWidget.h"

EUCleanCentralWidget::EUCleanCentralWidget(const string &config_loc_, QWidget *parent) : QWidget(parent),
                                                                                         config(config_loc_),
                                                                                         config_loc(config_loc_)
{
    cb_verbose = new QCheckBox("Verbose mode", this);
    cb_clean_invalid_tags = new QCheckBox("Invalid tags", this);
    cb_clean_wars = new QCheckBox("Wars", this);
    cb_clean_province_history = new QCheckBox("Province history", this);
    cb_clean_decisions = new QCheckBox("Decisions");
    cb_clean_advisors = new QCheckBox("Dead advisors", this);
    cb_clean_buildings = new QCheckBox("Buildings", this);
    cb_clean_occupations = new QCheckBox("Occupations", this);
    cb_clean_province_history2 = new QCheckBox("Additional province history", this);
    cb_clean_rulers = new QCheckBox("Dead rulers", this);
    cb_clean_heirs = new QCheckBox("Dead heirs", this);
    cb_clean_leaders = new QCheckBox("Dead leaders", this);

    set_checkbox_to_config();

    pb_clean = new QPushButton("Clean", this);
    connect(pb_clean, SIGNAL(clicked()), this, SLOT(clean_()));

    file_select = new QFileSelect("");
    connect(file_select, SIGNAL(file_changed()), this, SLOT(update_clean_button()));
    update_clean_button();

    layout = new QVBoxLayout(this);

    layout->addWidget(cb_verbose);
    layout->addWidget(cb_clean_invalid_tags);
    layout->addWidget(cb_clean_wars);
    layout->addWidget(cb_clean_province_history);
    layout->addWidget(cb_clean_decisions);
    layout->addWidget(cb_clean_advisors);
    layout->addWidget(cb_clean_buildings);
    layout->addWidget(cb_clean_occupations);
    layout->addWidget(cb_clean_province_history2);
    layout->addWidget(cb_clean_rulers);
    layout->addWidget(cb_clean_heirs);
    layout->addWidget(cb_clean_leaders);

    layout->addWidget(file_select);
    layout->addWidget(pb_clean);


}

void EUCleanCentralWidget::clean_()
{
    set_config_to_checkbox_state();
    config.save_config(config_loc);
    string loc = file_select->get_file();
    ifstream f(loc);
    if (f.good())
    {
        f.close();
        clean(loc, config);
    } else
    {
        f.close();
    }
}

void EUCleanCentralWidget::set_config_to_checkbox_state()
{
    config.verbose = cb_verbose->isChecked();
    config.clean_invalid_tags = cb_clean_invalid_tags->isChecked();
    config.clean_wars = cb_clean_wars->isChecked();
    config.clean_province_history = cb_clean_province_history->isChecked();
    config.clean_decisions = cb_clean_decisions->isChecked();
    config.clean_advisors = cb_clean_advisors->isChecked();
    config.clean_buildings = cb_clean_buildings->isChecked();
    config.clean_occupations = cb_clean_occupations->isChecked();
    config.clean_province_history2 = cb_clean_occupations->isChecked();
    config.clean_rulers = cb_clean_rulers->isChecked();
    config.clean_heirs = cb_clean_heirs->isChecked();
    config.clean_leaders = cb_clean_leaders->isChecked();
}

void EUCleanCentralWidget::set_checkbox_to_config()
{
    cb_verbose->setChecked(config.verbose);
    cb_clean_invalid_tags->setChecked(config.clean_invalid_tags);
    cb_clean_wars->setChecked(config.clean_wars);
    cb_clean_province_history->setChecked(config.clean_province_history);
    cb_clean_decisions->setChecked(config.clean_decisions);
    cb_clean_advisors->setChecked(config.clean_advisors);
    cb_clean_buildings->setChecked(config.clean_buildings);
    cb_clean_occupations->setChecked(config.clean_occupations);
    cb_clean_occupations->setChecked(config.clean_province_history2);
    cb_clean_rulers->setChecked(config.clean_rulers);
    cb_clean_heirs->setChecked(config.clean_heirs);
    cb_clean_leaders->setChecked(config.clean_leaders);
}

void EUCleanCentralWidget::update_clean_button()
{
    string loc = file_select->get_file();
    ifstream f(loc);
    if (f.good())
    {
        pb_clean->setEnabled(true);
    } else
    {
        pb_clean->setEnabled(false);
        pb_clean->setToolTip("You must select an existing file");
    }
    f.close();
}
