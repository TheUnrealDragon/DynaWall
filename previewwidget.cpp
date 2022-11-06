#include "previewwidget.h"

PreviewWidget::PreviewWidget(QWidget *parent)
    : QWidget{parent}
{
    layout = new QVBoxLayout();

}

PreviewWidget::PreviewWidget(QString picpath, QString label, QWidget *parent) :
    QWidget{parent}
{
    layout = new QVBoxLayout(this);
    check = new QCheckBox(this);
    pixmap = new QLabel(this);
    this->check->setText(label);
    QPixmap map(picpath);
    map = map.scaled(300,300,Qt::KeepAspectRatio);
    this->pixmap->setPixmap(map);
    this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    this->pixmap->resize(300,300);
    layout->addWidget(this->pixmap);
    layout->addWidget(this->check);
    connect(check,&QCheckBox::toggled,this,[this](bool ch){
        emit CheckBoxChecked(ch);
    });
}
