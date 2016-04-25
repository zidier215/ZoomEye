a.out:zoomeyeapi.c zlogin.c zmem.c zcurl.c cJSON.c zinfo.c zsearch.c
	gcc $^ -Wall -lc -lcurl -o $@
clean:
	rm a.out
