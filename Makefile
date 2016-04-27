a.out:zoomeyeapi.c zlogin.c zcurl.c cJSON.c zweb.c
	gcc $^ -Wall -lc -lcurl -o $@
clean:
	rm a.out
