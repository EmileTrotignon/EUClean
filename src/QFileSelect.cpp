#include "QFileSelect.h"


QFileSelect::QFileSelect(QWidget *parent) : QWidget(parent)
{
    line_edit = new QLineEdit(this);
    connect(line_edit, SIGNAL(textChanged(
                                      const QString &)), this, SLOT(emit_file_changed()));
    button = new QPushButton("Browse");
    connect(button, SIGNAL(clicked()), this, SLOT(browse()));
    layout = new QHBoxLayout();
    layout->addWidget(line_edit);
    layout->addWidget(button);
    setLayout(layout);
}

QFileSelect::QFileSelect(const string &default_loc, QWidget *parent) : QFileSelect(parent)
{
    line_edit->setText(QString::fromStdString(default_loc));
}

void QFileSelect::browse()
{
    QString old_loc = line_edit->text();
    QString file_loc = QFileDialog::getOpenFileName(this, tr("Pick save"), line_edit->text(), "EU4 saves (*.eu4)");
    line_edit->setText(file_loc);
}

string QFileSelect::get_file()
{
    return line_edit->text().toStdString();
}

void QFileSelect::emit_file_changed()
{
    emit file_changed();
}