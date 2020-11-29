detector: Asst2.c analyzer.c datastructs.c reader.c
	gcc Asst2.c analyzer.c datastructs.c reader.c -o detector 

clean:
	rm -rf detector