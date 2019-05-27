//
// Created by emile on 24/05/19.
//

#ifndef EUCLEAN_QFILESELECT_H
#define EUCLEAN_QFILESELECT_H


#include <QtWidgets/QWidget>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/QtWidgets>

using namespace std;

class QFileSelect : public QWidget
{
Q_OBJECT
private:
    QLineEdit *line_edit;
    QPushButton *button;
    QHBoxLayout *layout;


public :

    explicit QFileSelect(QWidget *parent = nullptr);

    explicit QFileSelect(const string &default_loc = nullptr, QWidget *parent = nullptr);

    string get_file();

private slots:

    void browse();
};


#endif //EUCLEAN_QFILESELECT_H
