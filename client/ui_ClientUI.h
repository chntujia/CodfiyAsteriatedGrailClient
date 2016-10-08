/********************************************************************************
** Form generated from reading UI file 'ClientUI.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTUI_H
#define UI_CLIENTUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientUI
{
public:
    QFrame *frameLogin;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *editLoginName;
    QLabel *LabError;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *editPassword;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnLogin;
    QPushButton *btnGuest;
    QPushButton *btnRegist;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QCheckBox *chkSaveUser;
    QCheckBox *chkSavePwd;
    QFrame *frameRegist;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_15;
    QLabel *label_14;
    QLineEdit *regLoginName;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_5;
    QLineEdit *regNickname;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_10;
    QLineEdit *regPassword;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_11;
    QLineEdit *regPasswordAgain;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_13;
    QLineEdit *regMobile;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_12;
    QLineEdit *regEmail;
    QLabel *LabErrorReg;
    QLabel *label_6;
    QLabel *label_8;
    QLabel *label_7;
    QHBoxLayout *horizontalLayout_11;
    QPushButton *btnRegistCommit;
    QPushButton *btnBackLogin;

    void setupUi(QDialog *ClientUI)
    {
        if (ClientUI->objectName().isEmpty())
            ClientUI->setObjectName(QStringLiteral("ClientUI"));
        ClientUI->resize(259, 317);
        frameLogin = new QFrame(ClientUI);
        frameLogin->setObjectName(QStringLiteral("frameLogin"));
        frameLogin->setEnabled(true);
        frameLogin->setGeometry(QRect(2, 3, 251, 301));
        frameLogin->setFrameShape(QFrame::StyledPanel);
        frameLogin->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(frameLogin);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 60, 189, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout->addWidget(label_3);

        editLoginName = new QLineEdit(layoutWidget);
        editLoginName->setObjectName(QStringLiteral("editLoginName"));
        editLoginName->setMaxLength(12);

        horizontalLayout->addWidget(editLoginName);

        LabError = new QLabel(frameLogin);
        LabError->setObjectName(QStringLiteral("LabError"));
        LabError->setGeometry(QRect(30, 130, 191, 16));
        LabError->setAlignment(Qt::AlignCenter);
        layoutWidget_2 = new QWidget(frameLogin);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(30, 100, 189, 22));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(layoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        editPassword = new QLineEdit(layoutWidget_2);
        editPassword->setObjectName(QStringLiteral("editPassword"));
        editPassword->setMaxLength(20);

        horizontalLayout_2->addWidget(editPassword);

        layoutWidget1 = new QWidget(frameLogin);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 180, 239, 25));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        btnLogin = new QPushButton(layoutWidget1);
        btnLogin->setObjectName(QStringLiteral("btnLogin"));

        horizontalLayout_3->addWidget(btnLogin);

        btnGuest = new QPushButton(layoutWidget1);
        btnGuest->setObjectName(QStringLiteral("btnGuest"));
        btnGuest->setEnabled(false);

        horizontalLayout_3->addWidget(btnGuest);

        btnRegist = new QPushButton(layoutWidget1);
        btnRegist->setObjectName(QStringLiteral("btnRegist"));
        btnRegist->setEnabled(true);

        horizontalLayout_3->addWidget(btnRegist);

        horizontalLayoutWidget = new QWidget(frameLogin);
        horizontalLayoutWidget->setObjectName(QStringLiteral("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(40, 150, 168, 31));
        horizontalLayout_5 = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        chkSaveUser = new QCheckBox(horizontalLayoutWidget);
        chkSaveUser->setObjectName(QStringLiteral("chkSaveUser"));

        horizontalLayout_5->addWidget(chkSaveUser);

        chkSavePwd = new QCheckBox(horizontalLayoutWidget);
        chkSavePwd->setObjectName(QStringLiteral("chkSavePwd"));

        horizontalLayout_5->addWidget(chkSavePwd);

        frameRegist = new QFrame(ClientUI);
        frameRegist->setObjectName(QStringLiteral("frameRegist"));
        frameRegist->setGeometry(QRect(9, 9, 241, 301));
        frameRegist->setFrameShape(QFrame::StyledPanel);
        frameRegist->setFrameShadow(QFrame::Raised);
        layoutWidget2 = new QWidget(frameRegist);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(0, 0, 242, 301));
        verticalLayout = new QVBoxLayout(layoutWidget2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setSpacing(6);
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        label_14 = new QLabel(layoutWidget2);
        label_14->setObjectName(QStringLiteral("label_14"));

        horizontalLayout_15->addWidget(label_14);

        regLoginName = new QLineEdit(layoutWidget2);
        regLoginName->setObjectName(QStringLiteral("regLoginName"));
        regLoginName->setMaxLength(12);

        horizontalLayout_15->addWidget(regLoginName);


        verticalLayout->addLayout(horizontalLayout_15);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_5 = new QLabel(layoutWidget2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_4->addWidget(label_5);

        regNickname = new QLineEdit(layoutWidget2);
        regNickname->setObjectName(QStringLiteral("regNickname"));

        horizontalLayout_4->addWidget(regNickname);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_10 = new QLabel(layoutWidget2);
        label_10->setObjectName(QStringLiteral("label_10"));

        horizontalLayout_10->addWidget(label_10);

        regPassword = new QLineEdit(layoutWidget2);
        regPassword->setObjectName(QStringLiteral("regPassword"));
        regPassword->setMaxLength(20);
        regPassword->setEchoMode(QLineEdit::Password);

        horizontalLayout_10->addWidget(regPassword);


        verticalLayout->addLayout(horizontalLayout_10);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_11 = new QLabel(layoutWidget2);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_12->addWidget(label_11);

        regPasswordAgain = new QLineEdit(layoutWidget2);
        regPasswordAgain->setObjectName(QStringLiteral("regPasswordAgain"));
        regPasswordAgain->setMaxLength(20);
        regPasswordAgain->setEchoMode(QLineEdit::Password);

        horizontalLayout_12->addWidget(regPasswordAgain);


        verticalLayout->addLayout(horizontalLayout_12);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_13 = new QLabel(layoutWidget2);
        label_13->setObjectName(QStringLiteral("label_13"));

        horizontalLayout_14->addWidget(label_13);

        regMobile = new QLineEdit(layoutWidget2);
        regMobile->setObjectName(QStringLiteral("regMobile"));
        regMobile->setInputMethodHints(Qt::ImhDigitsOnly);
        regMobile->setMaxLength(12);

        horizontalLayout_14->addWidget(regMobile);


        verticalLayout->addLayout(horizontalLayout_14);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_12 = new QLabel(layoutWidget2);
        label_12->setObjectName(QStringLiteral("label_12"));

        horizontalLayout_13->addWidget(label_12);

        regEmail = new QLineEdit(layoutWidget2);
        regEmail->setObjectName(QStringLiteral("regEmail"));
        regEmail->setMaxLength(50);

        horizontalLayout_13->addWidget(regEmail);


        verticalLayout->addLayout(horizontalLayout_13);

        LabErrorReg = new QLabel(layoutWidget2);
        LabErrorReg->setObjectName(QStringLiteral("LabErrorReg"));

        verticalLayout->addWidget(LabErrorReg);

        label_6 = new QLabel(layoutWidget2);
        label_6->setObjectName(QStringLiteral("label_6"));
        QFont font;
        font.setFamily(QStringLiteral("Aharoni"));
        font.setPointSize(7);
        font.setBold(true);
        font.setUnderline(true);
        font.setWeight(75);
        label_6->setFont(font);

        verticalLayout->addWidget(label_6);

        label_8 = new QLabel(layoutWidget2);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setFont(font);

        verticalLayout->addWidget(label_8);

        label_7 = new QLabel(layoutWidget2);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setFont(font);

        verticalLayout->addWidget(label_7);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        btnRegistCommit = new QPushButton(layoutWidget2);
        btnRegistCommit->setObjectName(QStringLiteral("btnRegistCommit"));

        horizontalLayout_11->addWidget(btnRegistCommit);

        btnBackLogin = new QPushButton(layoutWidget2);
        btnBackLogin->setObjectName(QStringLiteral("btnBackLogin"));

        horizontalLayout_11->addWidget(btnBackLogin);


        verticalLayout->addLayout(horizontalLayout_11);

        frameRegist->raise();
        frameLogin->raise();
#ifndef QT_NO_SHORTCUT
        label_14->setBuddy(regLoginName);
        label_5->setBuddy(regNickname);
        label_10->setBuddy(regPassword);
        label_11->setBuddy(regPasswordAgain);
        label_13->setBuddy(regMobile);
        label_12->setBuddy(regEmail);
#endif // QT_NO_SHORTCUT

        retranslateUi(ClientUI);

        QMetaObject::connectSlotsByName(ClientUI);
    } // setupUi

    void retranslateUi(QDialog *ClientUI)
    {
        ClientUI->setWindowTitle(QApplication::translate("ClientUI", "ClientUI", 0));
        label_3->setText(QApplication::translate("ClientUI", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        LabError->setText(QString());
        label_4->setText(QApplication::translate("ClientUI", "\345\257\206  \347\240\201\357\274\232", 0));
        btnLogin->setText(QApplication::translate("ClientUI", "\347\231\273\345\205\245", 0));
        btnGuest->setText(QApplication::translate("ClientUI", "\346\270\270\345\256\242", 0));
        btnRegist->setText(QApplication::translate("ClientUI", "\346\263\250\345\206\214", 0));
        chkSaveUser->setText(QApplication::translate("ClientUI", "\344\277\235\345\255\230\347\224\250\346\210\267\345\220\215", 0));
        chkSavePwd->setText(QApplication::translate("ClientUI", "\344\277\235\345\255\230\345\257\206\347\240\201", 0));
        label_14->setText(QApplication::translate("ClientUI", "\347\224\250\346\210\267\345\220\215\357\274\232", 0));
        label_5->setText(QApplication::translate("ClientUI", "\346\230\265  \347\247\260\357\274\232", 0));
        label_10->setText(QApplication::translate("ClientUI", "\345\257\206  \347\240\201\357\274\232", 0));
        label_11->setText(QApplication::translate("ClientUI", "\345\257\206  \347\240\201\357\274\232", 0));
        label_13->setText(QApplication::translate("ClientUI", "\346\211\213\346\234\272\345\217\267\357\274\232", 0));
        label_12->setText(QApplication::translate("ClientUI", "\351\202\256  \347\256\261\357\274\232", 0));
        LabErrorReg->setText(QString());
        label_6->setText(QApplication::translate("ClientUI", "\347\224\250\346\210\267\345\220\215\345\217\252\350\203\275\344\273\245\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\343\200\201\344\270\213\345\210\222\347\272\277\347\273\204\346\210\220\357\274\214\345\271\266\344\273\245\345\255\227\346\257\215\345\274\200\345\244\264", 0));
        label_8->setText(QApplication::translate("ClientUI", "\346\230\265\347\247\260\346\234\200\345\244\23210\344\270\252\346\261\211\345\255\227\346\210\226\345\255\227\347\254\246", 0));
        label_7->setText(QApplication::translate("ClientUI", "\345\257\206\347\240\201\345\217\252\350\203\275\344\273\245\345\255\227\346\257\215\343\200\201\346\225\260\345\255\227\343\200\201\344\270\213\345\210\222\347\272\277\347\273\204\346\210\220", 0));
        btnRegistCommit->setText(QApplication::translate("ClientUI", "\346\263\250\345\206\214", 0));
        btnBackLogin->setText(QApplication::translate("ClientUI", "\350\277\224\345\233\236", 0));
    } // retranslateUi

};

namespace Ui {
    class ClientUI: public Ui_ClientUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTUI_H
