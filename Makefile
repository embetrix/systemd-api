CFLAGS := -Wall -g
LIBS := 
LDFLAGS := -lsystemd

TARGET= systemd-api

all:  clean $(TARGET)

clean:
	$(RM) $(TARGET) *.o

$(TARGET):
	${CC} *.c  -o $@ $(LDFLAGS) $(LIBS) $(CFLAGS)$<

.PHONY: all clean
