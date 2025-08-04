APP = xfce-tour
SRC = xfce-tour.c
CC = gcc

CFLAGS = `pkg-config --cflags gtk+-3.0 libxfce4ui-2 librsvg-2.0`
LIBS = `pkg-config --libs gtk+-3.0 libxfce4ui-2 librsvg-2.0`

.PHONY: all clean install

all: $(APP)

$(APP): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

install:
	install -Dm755 $(APP) /usr/local/bin/$(APP)
	install -d /usr/local/share/xfce-tour/
	cp page1.svg page2.svg page3.svg /usr/local/share/xfce-tour/

clean:
	rm -f $(APP)
