#include <QApplication>
#include <QTreeView>
#include <QListView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include "treemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout* layout = new QVBoxLayout(&window);

    QTreeView* treeView = new QTreeView;
    // QListView *listView = new QListView;
    // QListView *iconView = new QListView;

    // listView->setViewMode(QListView::ListMode);
    // iconView->setViewMode(QListView::IconMode);
    // QPushButton* button = new QPushButton("Add Child");

    TreeModel* sourceModel = new TreeModel("F:/practice");
    // treeView->setModel(model);

    // add sortModel
    FileSortProxy *sortProxy = new FileSortProxy;
    sortProxy->setSourceModel(sourceModel);

    treeView->setModel(sortProxy);
    treeView->setSortingEnabled(true); // enable sorting, view will call proxy's lessThan() to compare items, and proxy will call sourceModel's data() to get the data for comparison. So we need to reimplement TreeModel::data() to return the appropriate data for sorting when role is Qt::SortRole.

    // listView->setModel(model);
    // iconView->setModel(model);

    layout->addWidget(treeView);
    // layout->addWidget(listView);
    // layout->addWidget(iconView);
    // layout->addWidget(button);

    // QObject::connect(button, &QPushButton::clicked,
    //                  [&]() {
    //     QModelIndex current = treeView->currentIndex();
    //     model->addChild(current, "New Node");
    // });


    // QPushButton* deleteBtn = new QPushButton("Delete Node");
    // layout->addWidget(deleteBtn);

    // QObject::connect(deleteBtn, &QPushButton::clicked,
    //                 [&]() {
    //     QModelIndex current = treeView->currentIndex();
    //     model->removeNode(current);
    // });

    window.show();
    return app.exec();
}