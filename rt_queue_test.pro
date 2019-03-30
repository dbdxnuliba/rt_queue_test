TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    consumer_task.cpp \
    msgb_producer_task.cpp \
    msga_producer_task.cpp \
    msg_queue_manager.cpp \
    msgc_producer_task.cpp

HEADERS += \
    consumer_task.h \
    msgb_producer_task.h \
    msga_producer_task.h \
    msg_queue_manager.h \
    msgc_producer_task.h

LIBS += -lpthread

LIBS += -L$$(SDK_PATH_TARGET)/lib/ -lethercat_rtdm

LIBS += -L$$(SDK_PATH_TARGET)/usr/xenomai/lib/ -ltrank
LIBS += -L$$(SDK_PATH_TARGET)/usr/xenomai/lib/ -lalchemy
LIBS += -L$$(SDK_PATH_TARGET)/usr/xenomai/lib/ -lcopperplate
LIBS += -L$$(SDK_PATH_TARGET)/usr/xenomai/lib/ -lcobalt
LIBS += -L$$(SDK_PATH_TARGET)/usr/xenomai/lib/ -lmodechk

LIBS += -L$$(SDK_PATH_TARGET)/usr/local/lib -lluajit-5.1
LIBS += -L$$(SDK_PATH_TARGET)/usr/local/lib -llua
LIBS += -ldl
LIBS += -lm

INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/include
INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/xenomai/include/
INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/xenomai/include/cobalt/
INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/xenomai/include/alchemy/
INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/xenomai/include/trank/

INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/include/boost
INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/include/eigen3/Eigen

INCLUDEPATH += $$(SDK_PATH_TARGET)/usr/local/include/

LIBS += -L$$(SDK_PATH_TARGET)/usr/lib -lboost_thread
LIBS +=  -L$$(SDK_PATH_TARGET)/usr/lib -lboost_system

#INCLUDEPATH += /home/ygx/project/PPSDK/opt/sysroot-arm/usr/include/xenomai
#INCLUDEPATH += /home/ygx/project/PPSDK/opt/sysroot-arm/usr/include

#INCLUDEPATH += /usr/include/luajit-2.0

TARGET = que1
target.path = /home/debian
INSTALLS += target

DEFINES +=BOOST_USE_LIB __COBALT__  __COBALT_WRAP__ __XENO_COMPAT__ _GNU_SOURCE _REENTRANT

OTHER_FILES +=
