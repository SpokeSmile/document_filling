#ifndef STUDENTITEMWIDGET_H
#define STUDENTITEMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QCheckBox>
#include <QHBoxLayout>

class StudentItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StudentItemWidget(QString studentName, QWidget *parent = nullptr);

signals:
    void stateChanged(bool isSelected);

private:
    QLabel *m_label;
    QCheckBox *m_checkbox;
};

#endif // STUDENTITEMWIDGET_H
