CFLAGS = -O3 -Wall -g
COMPILER := clang 
LINK := -g
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
obj/main.o: src/main.c inc/launch_game.h
obj/launch_game.o: src/launch_game.c inc/launch_game.h
