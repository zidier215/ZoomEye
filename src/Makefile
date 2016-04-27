MAINC += zoomeyeapi.c 
SRC += zlogin.c 
SRC += zcurl.c 
SRC += cJSON.c 
SRC += zweb.c
OBJ = $(subst .c,.o, $(SRC))

CFLAGS = -Wall -g -O2
LDFLAGS = -lc -lcurl

SO_PATH=$(PWD)/../libs
EXE_PATH=$(PWD)/../bin
a.out:$(OBJ) $(MAINC)
	gcc $^  $(LDFLAGS) -o $@
	cp -r $@ $(EXE_PATH)

%.o:%.c
	gcc $(CFLAGS) -c $^ -o $@ 
so:$(OBJ)
	ar rcsv libZoomEye.a $(OBJ)
	mv libZoomEye.a  $(SO_PATH)

clean:
	rm a.out *.o
