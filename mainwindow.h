#include <QMainWindow>
#include <QModelIndex>

class QTreeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    void initUI();
    void onRename();
    void onDelete();
    void onNewFolder();
    void onRefresh();

private slots:
    void onContextMenu(const QPoint &pos);

private:
    QTreeView* m_treeView;
    QModelIndex m_contextIndex;
};