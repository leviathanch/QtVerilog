#-------------------------------------------------
#
# Project created by QtCreator 2017-08-12T20:44:07
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtVerilog
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	verilogschematics.cpp \
  	asttree.cpp

HEADERS += \
	mainwindow.h \
	verilogschematics.h \
  	asttree.h \
	verilog_ast_common.h \
	verilog_ast.h \
	verilog_ast_mem.h

FORMS += \
	mainwindow.ui

FLEXSOURCES += verilog.l
BISONSOURCES += verilog.y

flex.commands = flex++ --c++ -o verilog.yy.cc ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = verilog.yy.cc
flex.variable_out = SOURCES
flex.depends = verilog.tab.h
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex
 
bison.commands = bison -Lc++ -d -t ${QMAKE_FILE_IN}
bison.input = BISONSOURCES
bison.output = verilog.tab.cc
bison.variable_out = SOURCES
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison
 
bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = verilog.tab.h
bisonheader.variable_out = HEADERS
bisonheader.name = bison header
bisonheader.depends = verilog.tab.cc
QMAKE_EXTRA_COMPILERS += bisonheader

