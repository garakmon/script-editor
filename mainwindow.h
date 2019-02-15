#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "project.h"
#include "document.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //QString root;

    void newFile();
    void openFile(const QString &path = QString());
    void openProject(const QString &root = QString());

private:
    Ui::MainWindow *ui;

    Project *project;
    Document *editor;
    Highlighter *highlighter;

    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

private slots:
    void on_actionNew_triggered();// new file
    void on_actionOpen_triggered();
    void on_actionOpen_Project_triggered();
    // new script action
    
};

#endif // MAINWINDOW_H
