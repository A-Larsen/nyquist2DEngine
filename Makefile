
SHELL := /bin/bash

COMPILE_ARGS = \
	$(DEFINES) \
	$(CFLAGS) \

OUTPUT_ARGS = \
	-o $(ENGINE)\
	$(LIBS)

ifdef LINUX


include linux.mk
LIBEXTENSION = so

else

include windows.mk
LIBEXTENSION = dll

endif

ifdef PRODUCTION

DEFINES := $(DEFINES) -D PRODUCTION
CFLAGS := $(CFLAGS) -O2

else

ENGINE := $(ENGINE)_dev
CFLAGS := $(CFLAGS) -Wextra -Wall
DEBUGFLAGS = -g

endif

ifdef EXTENSION
start: options $(EXTENSION)
endif

all: options $(ENGINE)

options:
	@echo $(ENGINE) build options:
	@echo ======================
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "CC       = $(CC)"

build: ./src/N2DE/*.h ./src/nyquist2DEngine.c
	$(CC) $^\
		$(COMPILE_ARGS)\
		$(DEBUGFLAGS) \
		$(OUTPUT_ARGS)

build_mapeditor: ./src/N2DE/*h ./src/mapeditor.c
	$(CC) $^ \
		$(COMPILE_ARGS) \
		-g \
		-o mapeditor\
		$(LIBS)

build_mapeditor_debug: ./src/N2DE/*h ./src/mapeditor.c
	$(CC) $^ \
		$(COMPILE_ARGS) \
		$(DEBUGFLAGS) \
		-g \
		-o mapeditor\
		$(LIBS)

$(EXTENSION): $(EXTENSION).c
	@eval "./scripts/setup_extension $@"
	$(CC) $^ -shared $(DEFINES) $(CFLAGS) -o ./extensions/$@.$(LIBEXTENSION) $(LIBS)

all_extensions: lib*.c
	@eval "./scripts/buildAllExtensions"

$(ENGINE): options build
mapeditor: build_mapeditor
mapeditor_debug: build_mapeditor_debug

install: all

clean:
	rm -rf $(ENGINE) *.o *.exe extensions

uinstall: 
	# remove all program files

.PHONY: clean all install $(ENGINE) debug mapeditor thing
