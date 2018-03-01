
CC = gcc
CFLAGS = -std=c99 -Iinclude -g `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv` -lwiringPi
TARGETNAME = gobang
OBJS = gobang.o main.o imageprocess.o hardware.o
BUILD_DIR = build
OUTPUT_DIR = bin
vpath %.c src
vpath %.h include 
vpath %.o build

all:$(TARGETNAME) $(OBJS)

$(TARGETNAME):$(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(addprefix $(BUILD_DIR)/,$(OBJS)) -o $(OUTPUT_DIR)/$@
%.o:%.c
	$(CC) $(CFLAGS)  -c $< -o $(BUILD_DIR)/$@
clean:
	-rm -rf $(addprefix $(BUILD_DIR)/,$(OBJS)) 
veryclean:clean
	-rm -rf $(OUTPUT_DIR)/$(TARGETNAME)
rebuild:veryclean all

.PHONY : all clean veryclean rebuild
