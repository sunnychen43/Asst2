detector: Asst2.c analyzer.c datastructs.c reader.c
	gcc Asst2.c analyzer.c datastructs.c reader.c -o detector -lpthread -lm

clean:
	rm -rf detector