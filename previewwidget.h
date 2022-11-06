#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPixmap>
#include <QCheckBox>
#include <QVBoxLayout>

class PreviewWidget : public QWidget
{
    Q_OBJECT
    QLabel *pixmap;
    QCheckBox *check;
    QVBoxLayout *layout;
public:
    explicit PreviewWidget(QWidget *parent = nullptr);
    explicit PreviewWidget(QString picpath, QString label, QWidget *parent = nullptr);

signals:
    void CheckBoxChecked(bool check);
};

#endif // PREVIEWWIDGET_H
