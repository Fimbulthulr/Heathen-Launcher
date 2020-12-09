CFLAGS = -O3 -Wall -g
COMPILER := clang 
LINK := -g -lncurses -lcjson
INCLUDE_OPTIONS := -I./inc
TARGET := heathen

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc





INCLUDES := $(shell find $(INC_DIR) -type d | sed s/^/-I/) $(INCLUDE_OPTIONS) 
SRC := $(shell find $(SRC_DIR) -name '*.c')
INC := $(shell find $(INC_DIR) -name '*.h')
.PHONY: all database directories clean depend
all: depend directories $(TARGET)
.NOTPARALLEL: depend
OBJ := $(subst $(SRC_DIR),$(OBJ_DIR),$(patsubst %.c,%.o,$(SRC)))
OBJDIRS := $(sort $(dir $(OBJ)))
SRCDIRS := $(sort $(dir $(SRC)))
$(TARGET):	$(OBJ) 
	$(COMPILER) -o $@ $(OBJ) $(LINK)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | depend; $(COMPILER) $(CFLAGS) $(INCLUDES) -c $< -o $@ 
database: ;make clean; bear make
directories:; mkdir -p $(OBJDIRS)
clean: ; rm -f $(TARGET) $(OBJ) 
depend: ; gcc-makedepend $(addprefix -p ,$(OBJDIRS)) $(CFLAGS) $(INCLUDES) $(SRC)
format:; uncrustify --no-backup -c /home/freyja/.defaults/uncrustify.cfg $(SRC) $(INC)
# DO NOT DELETE
obj/launch_game.o: src/launch_game.c inc/launch_game.h
obj/state_switcher.o: src/state_switcher.c inc/state_switcher.h inc/defines.h \
 inc/main_menu.h inc/instance_menu.h inc/instance_check.h
obj/instance_menu.o: src/instance_menu.c inc/instance_menu.h inc/defines.h \
 inc/launch_game.h
obj/instance_check.o: src/instance_check.c inc/instance_check.h inc/defines.h
obj/main_menu.o: src/main_menu.c inc/main_menu.h inc/defines.h
obj/main.o: src/main.c inc/defines.h inc/launch_game.h inc/state_switcher.h \
 inc/defines.h
obj/read_mods.o: src/read_mods.c inc/read_mods.h inc/mod_defines.h
obj/playset_management.o: src/playset_management.c inc/playset_management.h \
 inc/mod_defines.h
