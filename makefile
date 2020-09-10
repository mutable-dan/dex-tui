CC=g++-8

INSTALL_DIR = ../bin
INCLUDE_DIR = -I. -Iinclude/ -I../dexplex/include -I../cpr/include/ -I../json/include/ -I../spdlog/include/


EXE      = dex-cli
SOURCE   = src/main.cpp src/dexcom.cpp src/bg_cache.cpp
LINKLIBS = -L../cpr/lib/ -L/usr/lib/libdaemon/ -lspdlog -lcpr -lcurl -lmut -lstdc++fs -lboost_date_time -lpthread 

OBJS     = $(SOURCE:.cpp=.o) 
DEPS     = $(SOURCE:.cpp=.d) 

-include $(DEPS)

CFLAGSALL     = -std=c++17 -Wall -Wextra -Werror -Wshadow -march=native -fno-default-inline -fno-stack-protector -pthread -Wall -Werror -pedantic -Wextra -Waddress -Warray-bounds -Wno-builtin-macro-redefined -Wundef
CFLAGSALL     = -std=c++17 
CFLAGSALL    += -DSPDLOG_COMPILED_LIB
CFLAGSRELEASE = -O2 -DNDEBUG $(CFLAGSALL)
CFLAGSDEBUG   = -ggdb3 -DDEBUG $(CFLAGSALL)

.PHONY: release
release: CFLAGS = $(CFLAGSRELEASE)
release: all

.PHONY: debug
debug: CFLAGS = $(CFLAGSDEBUG)
debug: all


# compile and link

all : $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LINKLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE_DIR) -MMD -MP -c $< -o $@

install : all
	install -d $(INSTALL_DIR)
	install -m 750 $(EXE) $(INSTALL_DIR)

uninstall :
	/bin/rm -rf $(INSTALL_DIR)

clean :
	rm -f src/*.o src/*.d $(EXE) 
