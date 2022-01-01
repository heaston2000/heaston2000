#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageview.h"

class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT

    ImageView *view;
    int maxWidth, maxHeight;

public:
    QString fName;
    QPushButton *saveBtn;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loadBtnClk();
    void clearBtnClk();
    void saveBtnClk();
    void cartoonBtnClk();

};
#endif // MAINWINDOW_H
