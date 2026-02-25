#include "treemodel.h"

TreeModel::TreeModel(QObject* parent)
    : QAbstractItemModel(parent), m_rootNode(new TreeNode("Root"))
{
    // 构建树结构
    TreeNode* child1 = new TreeNode("Child 1", m_rootNode);
    TreeNode* child2 = new TreeNode("Child 2", m_rootNode);
    m_rootNode->appendChild(child1);
    m_rootNode->appendChild(child2);

    child1->appendChild(new TreeNode("Grandchild 1.1", child1));
    child1->appendChild(new TreeNode("Grandchild 1.2", child1));
    child2->appendChild(new TreeNode("Grandchild 2.1", child2));
}

TreeModel::~TreeModel()
{
    delete m_rootNode;
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode* parentNode = !parent.isValid() ? m_rootNode : static_cast<TreeNode*>(parent.internalPointer());
    TreeNode* childNode = parentNode->child(row);
    if (childNode)
        return createIndex(row, column, childNode);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode* childNode = static_cast<TreeNode*>(index.internalPointer());
    TreeNode* parentNode = childNode->parent();

    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeNode* parentNode = !parent.isValid() ? m_rootNode : static_cast<TreeNode*>(parent.internalPointer());
    return parentNode->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    return 1; // 只有一列显示节点名称
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    return node->name();
}