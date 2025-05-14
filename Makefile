export STAGING_DIR

# Đường dẫn toolchain và thư viện
CC := ./toolchain/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/bin/arm-openwrt-linux-muslgnueabi-gcc

LIB_DIRS := \
  ./toolchain/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/lib \
  ./toolchain/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/usr/lib

# Tên project và đường dẫn output
NAME_PROJECT := agen_wan
PATH_OUTPUT := ./out

# Các thư mục chứa mã nguồn và header
INC_DIRS := ./inc ./toolchain/toolchain-arm_cortex-a7_gcc-5.2.0_musl-1.1.16_eabi/usr/include
SRC_DIRS := ./src

# Tìm tất cả file .c và .h
SRC_FILES := $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
INC_FILES := $(foreach dir, $(INC_DIRS), $(wildcard $(dir)/*.h))

# Biến danh sách object file tương ứng
OBJ_FILES := $(patsubst $(SRC_DIRS)/%.c,$(PATH_OUTPUT)/%.o,$(SRC_FILES))

# Compiler flags
CFLAGS := $(foreach dir, $(INC_DIRS), -I$(dir))

# Linker flags
LDFLAGS := -lm -luci -lubox \
  $(foreach dir, $(LIB_DIRS), -L$(dir)) \
  $(foreach dir, $(LIB_DIRS), -Wl,-rpath-link=$(dir))

# Mục mặc định
all: $(PATH_OUTPUT)/${NAME_PROJECT}
	@echo "[DONE]"
	@echo "Object files: $(OBJ_FILES)"

# Mục build file thực thi
$(PATH_OUTPUT)/${NAME_PROJECT}: $(OBJ_FILES)
	@echo "[LINK] $@"
	@mkdir -p $(PATH_OUTPUT)
	$(CC) $^ -o $@ $(LDFLAGS)

# Mục biên dịch từng file .c thành .o
$(PATH_OUTPUT)/%.o: $(SRC_DIRS)/%.c $(INC_FILES)
	@echo "[COMPILING] $< -> $@"
	@mkdir -p $(PATH_OUTPUT)
	$(CC) -c $< -o $@ $(CFLAGS)

# Mục chạy chương trình (nếu chạy thử trên máy build)
run: $(PATH_OUTPUT)/${NAME_PROJECT}
	@echo "[RUNNING] $<"
	@LD_LIBRARY_PATH=$(subst $(space),:,$(LIB_DIRS)) ./$<

# Mục dọn dẹp
clean:
	@echo "[CLEAN]"
	rm -rf $(PATH_OUTPUT)/*
