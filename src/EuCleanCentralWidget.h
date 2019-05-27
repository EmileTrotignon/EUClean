#ifndef EUCLEAN_EUCLEANCENTRALWIDGET_H
#define EUCLEAN_EUCLEANCENTRALWIDGET_H

#include <QApplication>
#include <QWidget>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QtWidgets>

#include "eu_clean_logic.h"
#include "CleanConfig.h"
#include "QFileSelect.h"

class EUCleanCentralWidget : public QWidget
{
Q_OBJECT

private:

    CleanConfig config;

    QCheckBox *cb_verbose;
    QCheckBox *cb_clean_invalid_tags;
    QCheckBox *cb_clean_wars;
    QCheckBox *cb_clean_province_history;
    QCheckBox *cb_clean_decisions;
    QCheckBox *cb_clean_advisors;
    QCheckBox *cb_clean_buildings;
    QCheckBox *cb_clean_occupations;
    QCheckBox *cb_clean_province_history2;
    QCheckBox *cb_clean_rulers;
    QCheckBox *cb_clean_heirs;
    QCheckBox *cb_clean_leaders;

    QFileSelect *file_select;

    QPushButton *pb_clean;

    QVBoxLayout *layout;

    void set_config_to_checkbox_state();

    void set_checkbox_to_config();


public:

    explicit EUCleanCentralWidget(CleanConfig config_, QWidget *parent = nullptr);

private slots:

    void clean_();
};


#endif //EUCLEAN_EUCLEANCENTRALWIDGET_H
