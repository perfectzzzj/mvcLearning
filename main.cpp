#include <QApplication>
#include <QTreeView>
#include "treemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTreeView view;
    TreeModel model;

    view.setModel(&model);
    view.setHeaderHidden(true);
    view.show();

    return app.exec();
}