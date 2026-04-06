#pragma once
#include <QFileInfo>
#include <QDir>
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

    void setName(const QString &name) {
        QString path;
        if (!m_info.absolutePath().isEmpty())
            path = m_info.absolutePath() + "/" + name;
        else
            path = name;
        m_info = QFileInfo(path);
    }

    void updateChildInfos(const QString &oldParentPath, const QString &newParentPath) {
        for (TreeNode* child : m_children) {
            QString childPath = child->m_info.absoluteFilePath();
            if (childPath.startsWith(oldParentPath)) {
                QString relativePath = childPath.mid(oldParentPath.length());
                if (relativePath.startsWith('/') || relativePath.startsWith('\\'))
                    relativePath = relativePath.mid(1);
                child->m_info = QFileInfo(QDir(newParentPath).filePath(relativePath));
            }
            child->updateChildInfos(oldParentPath, newParentPath);
        }
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


class FileFilterProxy : public QSortFilterProxyModel
{
public:
    void setShowDirsOnly(bool enable)
    {
        m_showDirsOnly = enable;
        invalidateFilter();
    }

    void setSuffixFilter(const QString &suffix)
    {
        m_suffix = suffix;
        invalidateFilter();
    }

    void setSearchText(const QString &text)
    {
        m_searchText = text;
        invalidateFilter();
    }
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override
    {
        if (!m_showDirsOnly && m_suffix.isEmpty() && m_searchText.isEmpty())
            return true;

        QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

        if (matches(index))
            return true;

        if (hasAcceptedParent(sourceParent))
            return true;

        if (hasAcceptedChildren(index))
            return true;

        return false;
    }

private:

    bool matches(const QModelIndex &index) const
    {
        if (!m_showDirsOnly && m_suffix.isEmpty() && m_searchText.isEmpty())
            return true;

        QFileInfo info = index.data(Qt::UserRole).value<QFileInfo>();

        if (m_showDirsOnly && !info.isDir())
            return false;

        if (!m_suffix.isEmpty() && !info.fileName().endsWith(m_suffix, Qt::CaseInsensitive) && !info.isDir())
            return false;

        if (!m_searchText.isEmpty() && !info.fileName().contains(m_searchText, Qt::CaseInsensitive))
            return false;

        return true;
    }

    bool hasAcceptedParent(QModelIndex parent) const
    {
        while (parent.isValid())
        {
            if (matches(parent))
                return true;

            parent = parent.parent();
        }

        return false;
    }

    bool hasAcceptedChildren(const QModelIndex &parent) const
    {
        int rows = sourceModel()->rowCount(parent);

        for (int i = 0; i < rows; ++i)
        {
            QModelIndex child = sourceModel()->index(i, 0, parent);

            if (matches(child))
                return true;

            if (hasAcceptedChildren(child))
                return true;
        }

        return false;
    }

private:
    bool m_showDirsOnly = false;
    QString m_suffix;
    QString m_searchText;
};