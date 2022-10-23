#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QDir>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
extern "C"
{
    #ifdef __cplusplus
    #define __STDC_CONSTANT_MACROS
    #ifdef _STDINT_H
    #undef _STDINT_H
    #endif
    #include <stdint.h>
    #endif
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnPlay_clicked();

    void on_btnStop_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
