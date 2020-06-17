CFLAGS   := -Wall -Wextra -pedantic -fdiagnostics-color
INCLUDES := -I ./src/inc
LIBS     := -lm
OBJDIR   := build

# keep track of these files
KEEP_TRACK := Makefile

SERVER_FILES := server.c server_lib.c ack_manager.c device.c err_exit.c defines.c shared_memory.c semaphore.c fifo.c position.c files.c
SERVER_SRCS := $(addprefix src/, $(SERVER_FILES))
SERVER_OBJS := $(addprefix $(OBJDIR)/, $(SERVER_FILES:.c=.o))

CLIENT_FILES := client.c defines.c client_lib.c position.c fifo.c err_exit.c
CLIENT_SRCS := $(addprefix src/, $(CLIENT_FILES))
CLIENT_OBJS := $(addprefix $(OBJDIR)/, $(CLIENT_FILES:.c=.o))

# must be first rule
default: cmake_release


$(OBJDIR):
	@mkdir -p $(OBJDIR)

%.h:
	@echo "(missing header $@)"

$(OBJDIR)/%.o: src/%.c src/inc/%.h $(KEEP_TRACK)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $< $(LIBS)

server: $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o build/$@ $^ $(LIBS)
	@echo "Server compiled."
	@echo

client: $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o build/$@ $^ $(LIBS)
	@echo "Client compiled."
	@echo


run: server
	@./server 100 input/file_posizioni.txt

clean:
	@rm -vf $(SERVER_OBJS) $(CLIENT_OBJS)
	@rm -vf server
	@rm -vf client
	@rm -vf /tmp/dev_fifo.*
	@ipcrm -a
	@echo "Removed object files and executables..."

all: $(OBJDIR) server client

cmake:
	mkdir -p build
	cd build
	cmake -GNinja  -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES ..
	ninja -C build/

cmake_release:
	mkdir -p build
	cd build
	cmake -GNinja  -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=YES ..
	ninja

.PHONY: run clean
