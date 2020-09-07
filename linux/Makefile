TARGET		:=	lib-wooting-analog

HID_DIR		:=	../hidapi

SRC_DIR		:=	../src
INC_DIR		:=	../src
BLD_DIR		:=	build

HID_INC		:=	$(HID_DIR)/hidapi

C_FILES		:=	$(wildcard $(SRC_DIR)/*.c)
O_FILES		:=	$(patsubst $(SRC_DIR)/%.c,$(BLD_DIR)/%.o,$(C_FILES))

CC			:=	clang
LD			:=	clang

C_FLAGS		:=	-c -fPIC $(addprefix -I,$(INC_DIR) $(HID_INC))
LD_FLAGS	:=	-shared

all: buildDir library

clean:
	rm -vf $(O_FILES)

buildDir:
	mkdir -p $(BLD_DIR)

library: $(O_FILES)
	$(LD) $(LD_FLAGS) -o $(TARGET).so $?

$(BLD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(C_FLAGS) -o $@ $<

