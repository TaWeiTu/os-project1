all:
	mkdir -p bin
	make -C src
	mv ./src/main ./bin
	mv ./src/job ./bin

clean:
	$(RM) output/*
	$(RM) -r bin
