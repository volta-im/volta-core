// Copyright (c) 2009-2017 The Bitcoin Core developers
// Copyright (c) 2018-2018 The Volta Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VOLTA_QT_UTILITYDIALOG_H
#define VOLTA_QT_UTILITYDIALOG_H

#include <QDialog>
#include <QObject>

class VoltaGUI;

namespace interfaces {
    class Node;
}

namespace Ui {
    class HelpMessageDialog;
}

/** "Help message" dialog box */
class HelpMessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpMessageDialog(interfaces::Node& node, QWidget *parent, bool about);
    ~HelpMessageDialog();

    void printToConsole();
    void showOrPrint();

private:
    Ui::HelpMessageDialog *ui;
    QString text;

private Q_SLOTS:
    void on_okButton_accepted();
};


/** "Shutdown" window */
class ShutdownWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ShutdownWindow(QWidget *parent=0, Qt::WindowFlags f=0);
    static QWidget *showShutdownWindow(VoltaGUI *window);

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // VOLTA_QT_UTILITYDIALOG_H
