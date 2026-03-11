#pragma once
#include <QFileInfo>
#include <QVector>
#include <QFileIconProvider>
#include <QSortFilterProxyModel>

class TreeNode
{
public:
    explicit TreeNode(const QFileInfo& info, TreeNode* parent = nullptr)
        : m_info(info), m_parent(parent) {}

    ~TreeNode() {
        qDeleteAll(m_children);
    }

    void appendChild(TreeNode* child) {
        child->m_row = m_children.size();
        m_children.append(child);
        child->m_parent = this;
    }

    void removeChild(int row) {
        if (row < 0 || row >= m_children.size())
            return;

        delete m_children.takeAt(row);
    }

    TreeNode* child(int row) const {
        return m_children.value(row);
    }

    int childCount() const {
        return m_children.count();
    }
    
    int row() const {
        return m_row;
    }

    TreeNode* parent() const {
        return m_parent;
    }

    QString name() const {
        return m_info.fileName();
    }

    const QFileInfo& info() const {
        return m_info;
    }

    bool isDir() const {
        return m_info.isDir();
    }

    bool childrenLoaded() const {
    return m_childrenLoaded;
    }

    void setChildrenLoaded(bool loaded) {
        m_childrenLoaded = loaded;
    }

    QIcon icon() const {
        return m_iconProvider.icon(m_info);
    }

    qint64 size() const {
        return m_info.size();
    }

    time_t lastModified() const {
        return m_info.lastModified().toSecsSinceEpoch();
    }

private:
    QFileInfo m_info;
    TreeNode* m_parent;
    QVector<TreeNode*> m_children;
    int m_row = 0;
    bool m_childrenLoaded = false; // 标记子节点是否已加载
    QFileIconProvider m_iconProvider;
};

class FileSortProxy : public QSortFilterProxyModel
{
protected:
    //override this function to provide custom sorting logic based on file name, size, or last modified time.
    bool lessThan(const QModelIndex &left,
                  const QModelIndex &right) const override
    {
        // directory should always be sorted before files, we use userrole to store the QFileInfo of the item, so we can determine if it's a directory or file.
        QFileInfo leftInfo  = left.data(Qt::UserRole).value<QFileInfo>();
        QFileInfo rightInfo = right.data(Qt::UserRole).value<QFileInfo>();

        bool leftIsDir  = leftInfo.isDir();
        bool rightIsDir = rightInfo.isDir();

        if (leftIsDir != rightIsDir)
            return leftIsDir;

        QVariant l = left.data();
        QVariant r = right.data();
        switch (left.column())
        {
        case 0: // name
            return l.toString().toLower() < r.toString().toLower();

        case 1: // size
            return l.toLongLong() < r.toLongLong();

        case 2: // modified time
        {
            return l.toDateTime() < r.toDateTime();
        }

        default:
            return QSortFilterProxyModel::lessThan(left, right);
        }
    }
};