######################################################################
# Automatically generated by qmake (3.0) ?? ?? 12 22:51:01 2013
######################################################################

TEMPLATE = app
TARGET = gui
INCLUDEPATH += . widget data client logic
QT += network widgets multimedia
INCLUDEPATH += include
LIBS += lib/libprotobuf.lib

# Input
HEADERS += client/Client.h \
           client/ClientUI.h \
           client/codec.h \
           client/Lobby.h \
           client/ui_ClientUI.h \
           data/Card.h \
           data/DataInterface.h \
           data/Player.h \
           data/Team.h \
           data/Common.h \
           logic/AnSha.h \
           logic/DieWu.h \
           logic/FengYin.h \
           logic/GeDouJia.h \
           logic/GongNv.h \
           logic/HongLian.h \
           logic/JianDi.h \
           logic/JianSheng.h \
           logic/KuangZhan.h \
           logic/Lingfu.h \
           logic/LingHun.h \
           logic/Logic.h \
           logic/MaoXian.h \
           logic/MoDao.h \
           logic/MoGong.h \
           logic/MoJian.h \
           logic/MoQiang.h \
           logic/QiDao.h \
           logic/Role.h \
           logic/ShengNv.h \
           logic/ShengQiang.h \
           logic/ShenGuan.h \
           logic/SiLing.h \
           logic/TianShi.h \
           logic/WuNv.h \
           logic/XianZhe.h \
           logic/YongZhe.h \
           logic/YuanSu.h \
           logic/ZhongCai.h \
           protocol/action_respond.pb.h \
           protocol/base.pb.h \
           protocol/GameMessage.pb.h \
           widget/Animation.h \
           widget/BPArea.h \
           widget/Button.h \
           widget/ButtonArea.h \
           widget/CardItem.h \
           widget/GUI.h \
           widget/HandArea.h \
           widget/LogArea.h \
           widget/PlayerArea.h \
           widget/PlayerItem.h \
           widget/RoleItem.h \
           widget/RoomScene.h \
           widget/RoomView.h \
           widget/ShowArea.h \
           widget/TeamArea.h \
           widget/TipArea.h \
    data/Common.h
FORMS += client/ClientUI.ui client/Lobby.ui
SOURCES += main.cpp \
           client/Client.cpp \
           client/ClientUI.cpp \
           client/codec.cpp \
           client/Lobby.cpp \
           data/Card.cpp \
           data/DataInterface.cpp \
           data/Player.cpp \
           data/Team.cpp \
           logic/AnSha.cpp \
           logic/DieWu.cpp \
           logic/FengYin.cpp \
           logic/GeDouJia.cpp \
           logic/GongNv.cpp \
           logic/HongLian.cpp \
           logic/JianDi.cpp \
           logic/JianSheng.cpp \
           logic/KuangZhan.cpp \
           logic/Lingfu.cpp \
           logic/LingHun.cpp \
           logic/Logic.cpp \
           logic/MaoXian.cpp \
           logic/MoDao.cpp \
           logic/MoGong.cpp \
           logic/MoJian.cpp \
           logic/MoQiang.cpp \
           logic/QiDao.cpp \
           logic/Role.cpp \
           logic/ShengNv.cpp \
           logic/ShengQiang.cpp \
           logic/ShenGuan.cpp \
           logic/SiLing.cpp \
           logic/TianShi.cpp \
           logic/WuNv.cpp \
           logic/XianZhe.cpp \
           logic/YongZhe.cpp \
           logic/YuanSu.cpp \
           logic/ZhongCai.cpp \
           protocol/action_respond.pb.cc \
           protocol/base.pb.cc \
           protocol/GameMessage.pb.cc \
           widget/Animation.cpp \
           widget/BPArea.cpp \
           widget/Button.cpp \
           widget/ButtonArea.cpp \
           widget/CardItem.cpp \
           widget/GUI.cpp \
           widget/HandArea.cpp \
           widget/LogArea.cpp \
           widget/PlayerArea.cpp \
           widget/PlayerItem.cpp \
           widget/RoleItem.cpp \
           widget/RoomScene.cpp \
           widget/RoomView.cpp \
           widget/ShowArea.cpp \
           widget/TeamArea.cpp \
           widget/TipArea.cpp
