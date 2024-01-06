include config.mk

SHELL := /bin/bash

COMPILE_ARGS = \
	$(DEFINES) \
	$(CFLAGS) \

OUTPUT_ARGS = \
	-o $(OUT)\
	$(LIBS)

all: options $(OUT)

options:
	@echo $(OUT) build options:
	@echo ======================
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "CC       = $(CC)"

build: ./src/N2DE/*.h ./src/nyquist2DEngine.c
	$(CC) $^\
		$(COMPILE_ARGS)\
		$(OUTPUT_ARGS)

build-steamdeck: ./src/N2DE/*.h ./src/nyquist2DEngine.c
	$(CC) $^\
		$(COMPILE_ARGS)\
        -D STEAMDECK \
		$(OUTPUT_ARGS)

build_debug: ./src/N2DE/*.h ./src/nyquist2DEngine.c
	$(CC) $^ \
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

test: ./src/N2DE/*.h
	$(CC) $^ \
		./src/$(TEST).c \
		$(COMPILE_ARGS) \
		-g \
		-o $(TEST)\
		$(LIBS)

libdothings: libdothings.c
	$(CC) $^ -shared $(DEFINES) $(CFLAGS) -o ./extensions/$@.dll $(LIBS)


libaudio: libaudio.c
	$(CC) $^ -shared $(DEFINES) $(CFLAGS) -o ./extensions/$@.dll $(LIBS)
	@eval "./scripts/setup_extension $@"


$(OUT): build
steamdeck: build-steamdeck
debug: build_debug
mapeditor: build_mapeditor
mapeditor_debug: build_mapeditor_debug

install: all

clean:
	rm -rf $(OUT) *.o

uinstall: 
	# remove all program files

.PHONY: clean all install $(OUT) debug mapeditor
