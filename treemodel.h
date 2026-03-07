#pragma once
#include <QAbstractItemModel>
#include "treemodel_p.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(const QString& rootPath, QObject* parent = nullptr);
    ~TreeModel();

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    bool canFetchMore(const QModelIndex& index) const override;
    void fetchMore(const QModelIndex& index) override;
    bool hasChildren(const QModelIndex& index = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addChild(const QModelIndex& parentIndex, const QString& path);
    void removeNode(const QModelIndex& index);
    void buildTree(TreeNode* node);

private:
    TreeNode* m_rootNode;
};