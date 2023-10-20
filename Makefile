docs:
	@doxygen doc/config

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf doc/html