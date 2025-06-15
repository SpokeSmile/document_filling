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
    void selectionChanged(bool selected);

private:
    QLabel *m_label;
    QCheckBox *m_checkbox;
private slots:
    void onCheckboxStateChanged(int state);

};

#endif // STUDENTITEMWIDGET_H
