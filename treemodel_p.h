#pragma once
#include <QFileInfo>
#include <QVector>

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

private:
    QFileInfo m_info;
    TreeNode* m_parent;
    QVector<TreeNode*> m_children;
    int m_row = 0;
};