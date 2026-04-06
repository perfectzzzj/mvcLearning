#include "mainwindow.h"
#include "treemodel.h"

#include <QTreeView>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QMenu>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
}


void MainWindow::initUI()
{
    resize(800, 600);
    setContextMenuPolicy(Qt::CustomContextMenu);

    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout* layout = new QVBoxLayout(central);

    QPushButton* button = new QPushButton("Only Directory", central);
    button->setCheckable(true);
    QLineEdit* suffixEdit = new QLineEdit(central);
    suffixEdit->setPlaceholderText("Filter by suffix (e.g. .txt)");
    QLineEdit* searchEdit = new QLineEdit(central);
    searchEdit->setPlaceholderText("Search by name");

    TreeModel* sourceModel = new TreeModel("F:/practice", this);
    // treeView->setModel(model);

    // add sortModel
    FileSortProxy *sortProxy = new FileSortProxy;
    sortProxy->setSourceModel(sourceModel);

    FileFilterProxy *filterProxy = new FileFilterProxy;
    filterProxy->setSourceModel(sortProxy);

    m_treeView = new QTreeView(this);
    m_treeView->setModel(filterProxy);
    m_treeView->setSortingEnabled(true); // enable sorting, view will call proxy's lessThan() to compare items, and proxy will call sourceModel's data() to get the data for comparison. So we need to reimplement TreeModel::data() to return the appropriate data for sorting when role is Qt::SortRole.
    m_treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(m_treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::onContextMenu);

    layout->addWidget(m_treeView);
    layout->addWidget(button);
    layout->addWidget(suffixEdit);
    layout->addWidget(searchEdit);

    QObject::connect(button, &QPushButton::clicked,
                     [filterProxy, button]() {
        bool showDirsOnly = !button->isChecked();
        filterProxy->setShowDirsOnly(showDirsOnly);
    });


    QObject::connect(suffixEdit, &QLineEdit::textChanged,
                     [filterProxy](const QString &text) {
        filterProxy->setSuffixFilter(text);
    });

    QObject::connect(searchEdit, &QLineEdit::textChanged,
                     [filterProxy](const QString &text) {
        filterProxy->setSearchText(text);
    });
}

void MainWindow::onContextMenu(const QPoint &pos)
{
    m_contextIndex = m_treeView->indexAt(pos);
    // mouse position is invalid when you move to menu action
    // so we need to store the index at the time of right-clicking, and use it when handling menu actions

    if (!m_contextIndex.isValid())
        return;

    QMenu menu(this);

    QAction *renameAction = menu.addAction("Rename");
    QAction *deleteAction = menu.addAction("Delete");
    QAction *newFolderAction = menu.addAction("New Folder");

    menu.addSeparator();

    QAction *refreshAction = menu.addAction("Refresh");

    QAction *selected =
        menu.exec(m_treeView->viewport()->mapToGlobal(pos));

    if (selected == renameAction)
        onRename();
    else if (selected == deleteAction)
        onDelete();
    else if (selected == newFolderAction)
        onNewFolder();
    else if (selected == refreshAction)
        onRefresh();
}

void MainWindow::onRename()
{
    if (!m_contextIndex.isValid())
        return;

    m_treeView->edit(m_contextIndex);
}

void MainWindow::onDelete()
{
    if (!m_contextIndex.isValid())
        return;

    m_treeView->edit(m_contextIndex);
}

void MainWindow::onNewFolder()
{
}

void MainWindow::onRefresh()
{
}