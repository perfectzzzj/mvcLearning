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

//权限表，做很多操作前，主动查询
Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsSelectable
         | Qt::ItemIsEnabled
         | Qt::ItemIsEditable;   // 节点可编辑
}

bool TreeModel::setData(const QModelIndex& index,
                        const QVariant& value,
                        int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());

    node->setName(value.toString());

    // 通知 View 数据改变(结构已有的数据)
    emit dataChanged(index, index);

    return true;
}

/*----------------------------------------------*/

void TreeModel::addChild(const QModelIndex& parentIndex, const QString& name)
{
    TreeNode* parentNode = !parentIndex.isValid() ? m_rootNode : static_cast<TreeNode*>(parentIndex.internalPointer());

    int row = parentNode->childCount();

    beginInsertRows(parentIndex, row, row);
    TreeNode* newNode = new TreeNode(name, parentNode);
    parentNode->appendChild(newNode);
    endInsertRows();


    /*TreeNode* newNode = new TreeNode(name, parentNode);
    parentNode->appendChild(newNode);

    // 通知视图数据已更改，但结构未改变。下一次刷新时，添加的节点才显示。
    // 因为dataChanged只能通知“已有结构”的内容改变，而不能通知“结构本身”改变，所以需要调用beginInsertRows和endInsertRows来通知视图结构发生了变化。
    int row = parentNode->childCount() - 1;
    QModelIndex newIndex = createIndex(row, 0, newNode);
    emit dataChanged(newIndex, newIndex);*/
}

void TreeModel::removeNode(const QModelIndex& index)
{
    if (!index.isValid())
        return;

    TreeNode* node = static_cast<TreeNode*>(index.internalPointer());
    TreeNode* parentNode = node->parent();
    if (parentNode->childCount() == 0)
        return;

    int row = node->row();
    beginRemoveRows(index.parent(), row, row);
    parentNode->removeChild(row);
    endRemoveRows();
}