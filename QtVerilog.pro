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

#QMAKE_CXXFLAGS += -Werror=missing-prototypes
#QMAKE_CXXFLAGS += -Werror=missing-declarations
QMAKE_CXXFLAGS += -g
#QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++0x

SOURCES += \
	main.cpp \
	mainwindow.cpp \
	verilogschematics.cpp \
	verilogcode.cpp \
	verilog_ast_common.cc \
	verilog_ast_mem.cc \
	verilog_ast_util.cc \
	verilog_preprocessor.cc \
	verilogscanner.cpp \
        verilog_ast.cc \
	verilog_parser_wrapper.cc

HEADERS += \
	mainwindow.h \
	verilogschematics.h \
	verilog_ast_common.hh \
       	verilog_ast.hh \
       	verilog_ast_mem.hh \
	verilog_preprocessor.hh \
	verilogcode.h \
	verilogscanner.hh

FORMS += \
	mainwindow.ui

FLEXSOURCES += verilog_scanner.ll
BISONSOURCES += verilog_parser.yy

flex.commands = flex++ --c++ --header-file=verilog.ll.hh -o verilog.ll.cc ${QMAKE_FILE_IN}
flex.input = FLEXSOURCES
flex.output = verilog.ll.cc
flex.variable_out = SOURCES
flex.depends = verilog.yy.hh
flex.name = flex
QMAKE_EXTRA_COMPILERS += flex

flexheader.commands = @true
flexheader.input = FLEXSOURCES
flexheader.output = verilog.ll.hh
flexheader.variable_out = HEADERS
flexheader.name = flex header
flexheader.depends = verilog.ll.cc
QMAKE_EXTRA_COMPILERS += flexheader
 
bison.commands = bison -Lc++ --defines=verilog.yy.hh --output=verilog.yy.cc -t ${QMAKE_FILE_IN}
bison.input = BISONSOURCES
bison.output = verilog.yy.cc
bison.variable_out = SOURCES
bison.name = bison
QMAKE_EXTRA_COMPILERS += bison
 
bisonheader.commands = @true
bisonheader.input = BISONSOURCES
bisonheader.output = verilog.yy.hh
bisonheader.variable_out = HEADERS
bisonheader.name = bison header
bisonheader.depends = verilog.yy.cc
QMAKE_EXTRA_COMPILERS += bisonheader

