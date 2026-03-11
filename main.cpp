#include <QApplication>
#include <QTreeView>
#include <QListView>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include "treemodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout* layout = new QVBoxLayout(&window);

    QTreeView* treeView = new QTreeView;

    QPushButton* button = new QPushButton("Only Directory");
    QLineEdit* suffixEdit = new QLineEdit;
    suffixEdit->setPlaceholderText("Filter by suffix (e.g. .txt)");
    QLineEdit* searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Search by name");

    TreeModel* sourceModel = new TreeModel("F:/practice");
    // treeView->setModel(model);

    // add sortModel
    FileSortProxy *sortProxy = new FileSortProxy;
    sortProxy->setSourceModel(sourceModel);

    FileFilterProxy *filterProxy = new FileFilterProxy;
    filterProxy->setSourceModel(sortProxy);

    treeView->setModel(filterProxy);
    treeView->setSortingEnabled(true); // enable sorting, view will call proxy's lessThan() to compare items, and proxy will call sourceModel's data() to get the data for comparison. So we need to reimplement TreeModel::data() to return the appropriate data for sorting when role is Qt::SortRole.

    layout->addWidget(treeView);
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

    window.show();
    return app.exec();
}