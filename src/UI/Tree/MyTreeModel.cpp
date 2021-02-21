#include "MyTreeModel.hpp"

MyTreeNode *
MyTreeModel::getNodeByIndex(const QModelIndex &index)
{
    if (!index.isValid()) return nullptr;
    return static_cast<MyTreeNode *>(index.internalPointer());
}

QModelIndex
MyTreeModel::getIndexByNode(MyTreeNode *node)
{
    QVector<int> positions;
    QModelIndex result;
    if (node)
    {
        do
        {
            int pos = node->pos();
            positions.append(pos);
            node = node->parentNode();
        } while (node != nullptr);

        for (int i = positions.size() - 2; i >= 0; i--)
        {
            result = index(positions[i], 0, result);
        }
    }
    return result;
}

bool
MyTreeModel::insertNode(MyTreeNode *childNode, const QModelIndex &parent,
                        int pos)
{
    MyTreeNode *parentElement = getNode(parent);
    if (pos >= parentElement->count()) return false;
    if (pos < 0) pos = parentElement->count();

    childNode->setParentNode(parentElement);
    beginInsertRows(parent, pos, pos);
    bool retValue = parentElement->insertNode(childNode, pos);
    endInsertRows();
    return retValue;
}

MyTreeNode *
MyTreeModel::getNode(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<MyTreeNode *>(index.internalPointer());
    return m_rootNode;
}

MyTreeModel::MyTreeModel(QObject *parent) : QAbstractItemModel(parent)
{
    m_rootNode = new MyTreeNode(nullptr);
}
MyTreeModel::~MyTreeModel() { delete m_rootNode; }

QHash<int, QByteArray>
MyTreeModel::roleNames() const
{
    return m_roles;
}

QVariant
MyTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    MyTreeNode *item = static_cast<MyTreeNode *>(index.internalPointer());
    QByteArray roleName = m_roles[role];
    QVariant name = item->property(roleName.data());
    return name;
}

Qt::ItemFlags
MyTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return 0;

    return QAbstractItemModel::flags(index);
}

QModelIndex
MyTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) return QModelIndex();

    MyTreeNode *parentItem = getNode(parent);
    MyTreeNode *childItem = parentItem->childNode(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex
MyTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) return QModelIndex();

    MyTreeNode *childItem = static_cast<MyTreeNode *>(index.internalPointer());
    MyTreeNode *parentItem
        = static_cast<MyTreeNode *>(childItem->parentNode());

    if (parentItem == m_rootNode) return QModelIndex();

    return createIndex(parentItem->pos(), 0, parentItem);
}

int
MyTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) return 0;
    MyTreeNode *parentItem = getNode(parent);
    return parentItem->count();
}

int
MyTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QQmlListProperty<MyTreeNode>
MyTreeModel::nodes()
{
    return m_rootNode->nodes();
}

QVariantList
MyTreeModel::roles() const
{
    QVariantList list;
    QHashIterator<int, QByteArray> i(m_roles);
    while (i.hasNext())
    {
        i.next();
        list.append(i.value());
    }

    return list;
}

void
MyTreeModel::setRoles(const QVariantList &roles)
{
    static int nextRole = Qt::UserRole + 1;
    foreach (auto role, roles)
    {
        m_roles.insert(nextRole, role.toByteArray());
        nextRole++;
    }
}