#ifndef QNRBFFORMAT_MAINWINDOW_H
#define QNRBFFORMAT_MAINWINDOW_H

#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "Utils/NrbfRegistry.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QWidget *mainWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QLineEdit *lineEdit;

    QPushButton *browseButton;
    QPushButton *loadButton;

    QTreeWidget *treeWidget;

    QNrbf::NrbfRegistry registry;

    void preExpandItem(QTreeWidgetItem *item);

private:
    void _q_browseButtonClicked();
    void _q_loadButtonClicked();
    void _q_treeItemExpanded(QTreeWidgetItem *item);
};


#endif // QNRBFFORMAT_MAINWINDOW_H
