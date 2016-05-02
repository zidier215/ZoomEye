MAINC += zoomeyeapi.c 
SRC += zlogin.c 
SRC += zcurl.c 
SRC += cJSON.c 
SRC += zweb.c
SRC += zxml.c
SRC += utils.c
OBJ = $(subst .c,.o, $(SRC))

CFLAGS = -fPIC -Wall -g -O2 -I /usr/include/libxml2 
LDFLAGS = -lc -lcurl -lxml2

LIB_PATH=$(PWD)/../libs
EXE_PATH=$(PWD)/../bin
a.out:$(OBJ) $(MAINC)
	gcc $^  $(LDFLAGS) -o $@
	cp -r $@ $(EXE_PATH)

%.o:%.c
	gcc $(CFLAGS) -c $^ -o $@ 
so:$(OBJ)
	ar rcsv libZoomEye.a $(OBJ)
	gcc -shared $(LDFLAGS) -o libZoomEye.so $(OBJ)
	mv libZoomEye.a  $(LIB_PATH)
	mv libZoomEye.so $(LIB_PATH)

clean:
	rm a.out *.o
