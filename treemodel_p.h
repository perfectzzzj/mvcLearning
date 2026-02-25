#pragma once
#include <QString>
#include <QVector>

class TreeNode
{
public:
    explicit TreeNode(const QString& name, TreeNode* parent = nullptr)
        : m_name(name), m_parent(parent) {}

    ~TreeNode() {
        qDeleteAll(m_children);
    }

    void appendChild(TreeNode* child) {
        m_children.append(child);
    }

    TreeNode* child(int row) const {
        return m_children.value(row);
    }

    int childCount() const {
        return m_children.count();
    }
    
    int row() const {
        if (m_parent) {
            return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
        }
        return 0;
    }
    
    QString name() const {
        return m_name;
    }

    TreeNode* parent() const {
        return m_parent;
    }

private:
    QString m_name;
    TreeNode* m_parent;
    QVector<TreeNode*> m_children;
};