obj-m:= myled.o

myled.ko: myled.c
	make -C /usr/src/linux M=`pwd` V=1 modules	#makeと打つと実行される
clean:
	make -C /usr/src/linux M=`pwd` V=1 clean	#make cleanで実行
