TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        analyse.c \
        cgen.c \
        globals.c \
        lex.yy.c \
        main.c \
        parse.c \
        symbol.c

HEADERS += \
    analyse.h \
    cgen.h \
    globals.h \
    hashtable.h \
    parse.h \
    symbol.h

DISTFILES += \
    Makefile \
    scanner.l \
    test1.spp \
    test2.spp \
    test3.spp
