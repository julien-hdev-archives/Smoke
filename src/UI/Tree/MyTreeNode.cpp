#include "MyTreeNode.hpp"

void
append_element(QQmlListProperty<MyTreeNode> *property, MyTreeNode *value)
{
    MyTreeNode *parent = (qobject_cast<MyTreeNode *>(property->object));
    value->setParentNode(parent);
    parent->insertNode(value, -1);
}

int
count_element(QQmlListProperty<MyTreeNode> *property)
{
    MyTreeNode *parent = (qobject_cast<MyTreeNode *>(property->object));
    return parent->count();
}

MyTreeNode *
at_element(QQmlListProperty<MyTreeNode> *property, int index)
{
    MyTreeNode *parent = (qobject_cast<MyTreeNode *>(property->object));
    if (index < 0 || index >= parent->count()) return nullptr;
    return parent->childNode(index);
}

void
clear_element(QQmlListProperty<MyTreeNode> *property)
{
    MyTreeNode *parent = (qobject_cast<MyTreeNode *>(property->object));
    parent->clear();
}


MyTreeNode::MyTreeNode(QObject *parent)
    : QObject(parent), m_parentNode(nullptr)
{}


void
MyTreeNode::setParentNode(MyTreeNode *parent)
{
    m_parentNode = parent;
}

MyTreeNode *
MyTreeNode::parentNode() const
{
    return m_parentNode;
}

QQmlListProperty<MyTreeNode>
MyTreeNode::nodes()
{
    QQmlListProperty<MyTreeNode> list(this, 0, &append_element, &count_element,
                                      &at_element, &clear_element);
    return list;
}

MyTreeNode *
MyTreeNode::childNode(int index) const
{
    if (index < 0 || index >= m_nodes.length()) return nullptr;
    return m_nodes.at(index);
}

void
MyTreeNode::clear()
{
    qDeleteAll(m_nodes);
    m_nodes.clear();
}

bool
MyTreeNode::insertNode(MyTreeNode *node, int pos)
{
    if (pos > m_nodes.count()) return false;
    if (pos < 0) pos = m_nodes.count();
    m_nodes.insert(pos, node);
    return true;
}

int
MyTreeNode::pos() const
{
    MyTreeNode *parent = parentNode();
    if (parent) return parent->m_nodes.indexOf(const_cast<MyTreeNode *>(this));
    return 0;
}

int
MyTreeNode::count() const
{
    return m_nodes.size();
}


