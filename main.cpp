#include <QApplication>
#include <QTreeView>
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
    QPushButton* button = new QPushButton("Add Child");

    TreeModel* model = new TreeModel;
    treeView->setModel(model);

    layout->addWidget(treeView);
    layout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked,
                     [&]() {
        QModelIndex current = treeView->currentIndex();
        model->addChild(current, "New Node");
    });


    QPushButton* deleteBtn = new QPushButton("Delete Node");
    layout->addWidget(deleteBtn);

    QObject::connect(deleteBtn, &QPushButton::clicked,
                    [&]() {
        QModelIndex current = treeView->currentIndex();
        model->removeNode(current);
    });

    window.show();
    return app.exec();
}