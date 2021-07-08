TARGET		:=	wooting-analog-sdk

FILE_SRC	:=	../src
FILE_INC	:=	../hidapi/hidapi
FILE_BLD	:=	build

FILES_C		:=	$(wildcard $(FILE_SRC)/*.c)
FILES_O		:=	$(patsubst $(FILE_SRC)/%.c,$(FILE_BLD)/%.o,$(FILES_C))

CC			:=	clang
LD			:=	clang

FLAGS_C		:=	-Wall -Werror -c -fPIC -O2 -I$(FILE_INC)
FLAGS_LD	:=	-Wall -Werror -shared

all: build lib$(TARGET).so

build:
	mkdir -p $(FILE_BLD)

lib$(TARGET).so: $(FILES_O)
	$(LD) $(FLAGS_LD) -o $@ $^

$(FILE_BLD)/%.o: $(FILE_SRC)/%.c
	$(CC) $(FLAGS_C) -o $@ $^

clean:
	rm -vf $(FILES_O) lib$(TARGET).so
