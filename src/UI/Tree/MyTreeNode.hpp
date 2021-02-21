#pragma once

#include <QObject>
#include <QQmlListProperty>
#include <QAbstractItemModel>

class MyTreeNode : public QObject
{
    Q_OBJECT
  public:
    Q_PROPERTY(QQmlListProperty<MyTreeNode> nodes READ nodes)
    Q_CLASSINFO("DefaultProperty", "nodes")
    MyTreeNode(QObject *parent = Q_NULLPTR);

    void setParentNode(MyTreeNode *parent);
    Q_INVOKABLE MyTreeNode *parentNode() const;
    bool insertNode(MyTreeNode *node, int pos = (-1));
    QQmlListProperty<MyTreeNode> nodes();

    MyTreeNode *childNode(int index) const;
    void clear();

    Q_INVOKABLE int pos() const;
    Q_INVOKABLE int count() const;

  private:
    QList<MyTreeNode *> m_nodes;
    MyTreeNode *m_parentNode;
};