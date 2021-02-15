#pragma once

#include <QAbstractListModel>

/// \brief Dynamic ObjectListModel used for QML Views.
/// \todo Add remove().
class ObjectListModel : public QAbstractListModel
{
    Q_OBJECT

  public:
    explicit ObjectListModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {}
    ~ObjectListModel() = default;

    // ==============================
    // Virtual methods needed for the
    // ListModel to work properly.
    // ==============================

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const override;

    // ==============================
    // Internal methods used to work directly with the data.
    // WARNING: be careful to insert/removeRows()
    // in case of dynamic object lists.
    // ==============================

    /// \brief Add the object inside the list, if not already in it.
    /// \remarks If the object is added to the list, the
    /// ListModel class takes its ownership.
    /// \param obj Pointer to the object.
    void append(QObject *obj);

    /// \brief Returns the Object at the corresponding index.
    /// \param index Index in the list.
    inline const QObject *
    at(const size_t index) const
    {
        return _data.at(index);
    }

    /// \brief Check if the object is inside the list.
    /// \param obj Object to check.
    /// \returns True if inside, False otherwise.
    inline bool
    contains(QObject *obj) const
    {
        return _data.contains(obj);
    }

  private:
    /// Internal list of QObjects.
    QObjectList _data;
};