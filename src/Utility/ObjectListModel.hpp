#pragma once

#include <QAbstractListModel>

/// \brief Static ObjectListModel used for QML Views.
/// \todo Make it dynamic.
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
    QHash<int, QByteArray> roleNames() const;

    // ==============================
    // Internal methods used to work directly with the data.
    // WARNING: be careful to insert/removeRows()
    // in case of dynamic object lists.
    // ==============================

    /// \brief Clear and populate the data with a list of QObjects.
    /// \param objects List of QObjects to store as data.
    /// \todo Check if handling parent ownership is needed or not.
    void populate(const QObjectList &objects);

    /// \brief Returns the Object at the corresponding index.
    /// \param index Index in the list.
    inline const QObject *
    at(const size_t index) const
    {
        return _data.at(index);
    }

  private:
    /// Internal list of QObjects.
    QObjectList _data;
};