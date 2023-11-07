CC = gcc
CFLAGS = -lpthread

# 使用通配符获取所有的.c文件
SOURCES = $(wildcard *.c)

# 根据源文件生成目标文件列表
OBJECTS = $(SOURCES:.c=.o)

# 目标文件依赖于对应的源文件
all: bw_iotest

bw_iotest: $(OBJECTS)
	$(CC) -o bw_iotest $(OBJECTS) $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f iotest $(OBJECTS)
