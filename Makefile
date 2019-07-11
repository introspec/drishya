PROG= a

OBJS= a.o toolkit/window.o protocol/xdg-shell-unstable-v6-protocol.o protocol/ivi-application-protocol.o protocol/relative-pointer-unstable-v1-protocol.o protocol/pointer-constraints-unstable-v1-protocol.o protocol/text-cursor-position-protocol.o config-parser.o option-parser.o shared/xalloc.o toolkit/frame.o toolkit/cairo-util.o toolkit/image-loader.o shared/os-compatibility.o

CFLAGS= -g -I/usr/include/pixman-1 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0  -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/cairo -I. -DLIBEXECDIR="\"/home/rohit/local/lib\"" -DDATADIR="\"/usr/share/weston\"" -Iprotocol -Ishared -Itoolkit

LDFLAGS= -lwayland-client -lm -lxkbcommon -lpixman-1 -lpng -lpangocairo-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lcairo -ljpeg -lwebp -lwayland-cursor -lutil

all : ${PROG}

${PROG} : ${OBJS}

clean :
	@rm -f ${OBJS}
