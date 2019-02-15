#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mainwindow.h"

#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    project = new Project();

    setupEditor();

    setCentralWidget(editor);
    setWindowTitle(tr("script editor demo"));
}

MainWindow::~MainWindow()
{
    delete project;
    delete ui;
}


void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString file_name = path;

    if (file_name.isNull())
        file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Scripts (*.s *.inc)");

    if (!file_name.isEmpty()) {
        QFile file(file_name);
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setPlainText(file.readAll());
    }
}

void MainWindow::openProject(const QString &root)
{
    QString project_dir = root;

    if (project_dir.isNull())
        project_dir = QFileDialog::getExistingDirectory(
            this, "Open Directory", project_dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if (!project_dir.isEmpty()) {
        project->loadProject(project_dir);
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    //font.setFamily("Courier");
    font.setFamily("Source Code Pro");
    font.setFixedPitch(true);
    font.setPointSize(12);

    //editor = new QTextEdit;
    editor = new Document(project, GlobalThemes["monokai"]);
    editor->setFont(font);

    highlighter = new Highlighter(editor);

    QFile file("example_scripts.inc");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}

void MainWindow::on_actionNew_triggered() {
    newFile();
}

void MainWindow::on_actionOpen_triggered() {
    //
    openFile();
}

void MainWindow::on_actionOpen_Project_triggered() {
    //
    openProject();
}


