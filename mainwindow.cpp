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
    //editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString file_name = path;
    int index = 0;

    if (file_name.isNull())
        file_name = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Scripts (*.s *.inc)");

    if (!file_name.isEmpty()) {
        QFile file(file_name);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            index = editor->addTab(new Document(project, GlobalThemes["monokai"]), "file 1");
            editor->setCurrentIndex(index);
            static_cast<Document*>(editor->currentWidget())->setPlainText(file.readAll());
            //editor->setPlainText(file.readAll());
        }
        QFont font;
        //font.setFamily("Courier");
        font.setFamily("Source Code Pro");
        font.setFixedPitch(true);
        font.setPointSize(12);
        static_cast<Document*>(editor->currentWidget())->setFont(font);
        highlighter = new Highlighter(static_cast<Document*>(editor->currentWidget()));
        QStringList dir = file_name.split(QRegularExpression("/"));
        QString short_name;
        if (dir.size() > 1) {
            short_name = dir.at(dir.size() - 2) + dir.at(dir.size() - 1);
        } else {
            short_name = dir.last();
        }
        editor->setTabText(index, short_name);
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

void MainWindow::setupEditor() {
    //editor = new Document(project, GlobalThemes["monokai"]);
    //editor->setFont(font);
    //highlighter = new Highlighter(editor);

    editor = new QTabWidget();
    QPixmap pixmap(10,10);
    pixmap.fill(QColor("red"));
    QIcon close_icon(pixmap);
    editor->setStyleSheet("QTabWidget::tab-bar { left: 0; }");
    // TODO: add icon for QTabBar::close-button {  }");
    editor->setTabsClosable(true);
    // TODO: make own slot for this so can handle the things i need to handle (like what)
    connect(editor, &QTabWidget::tabCloseRequested, editor, &QTabWidget::removeTab);

    openFile("example_scripts.inc");
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

void MainWindow::on_actionSave_triggered() {
    // save current tab's 
    //project->saveTextFile();
}

void MainWindow::on_actionSave_All_triggered() {
    //
}

