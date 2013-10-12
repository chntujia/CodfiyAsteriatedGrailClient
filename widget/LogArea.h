#ifndef LOGAREA_H
#include "Logic.h"
#define LOGAREA_H

#include <QTextEdit>
#include <QLineEdit>
class LogArea : public QTextEdit
{
public:
    explicit LogArea(QWidget *parent = 0);
    void append(QString text);
    
};
class ChatArea : public QTextEdit
{
public:
    ChatArea(QWidget *parent=0);
    void append(int id, QString text);
};
class ChatLine : public QLineEdit
{
    Q_OBJECT
public:
    ChatLine(QWidget*parent=0);
public slots:
    void onReturnPressed();
    void onTextChanged(QString);
signals:
    void toChat(quint16 proto_type, google::protobuf::Message*);
};

#endif // LOGAREA_H
