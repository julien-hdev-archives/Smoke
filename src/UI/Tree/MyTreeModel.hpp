#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <QAbstractItemModel>
#include "MyTreeNode.hpp"

class MyTreeModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    Q_PROPERTY(QQmlListProperty<MyTreeNode> nodes READ nodes)
    Q_PROPERTY(
        QVariantList roles READ roles WRITE setRoles NOTIFY rolesChanged)
    Q_CLASSINFO("DefaultProperty", "nodes")

    MyTreeModel(QObject *parent = Q_NULLPTR);
    ~MyTreeModel();

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent
                      = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent
                 = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent
                    = QModelIndex()) const Q_DECL_OVERRIDE;
    QQmlListProperty<MyTreeNode> nodes();

    QVariantList roles() const;
    void setRoles(const QVariantList &roles);

    Q_INVOKABLE MyTreeNode *getNodeByIndex(const QModelIndex &index);
    Q_INVOKABLE QModelIndex getIndexByNode(MyTreeNode *node);
    Q_INVOKABLE bool insertNode(MyTreeNode *childNode,
                                const QModelIndex &parent = QModelIndex(),
                                int pos = (-1));

  protected:
    MyTreeNode *getNode(const QModelIndex &index) const;

  private:
    MyTreeNode *m_rootNode;
    QHash<int, QByteArray> m_roles;

  signals:
    void rolesChanged();
};