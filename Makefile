CXX = g++
CXX17FLAGS = -std=c++17 -pedantic-errors -Werror -Wall -Weffc++ -Wextra \
	-Wconversion -Wsign-conversion -DNDEBUG -O2
CXX20FLAGS = -std=c++20 -pedantic-errors -Werror -Wall -Weffc++ -Wextra \
	-Wconversion -Wsign-conversion -DNDEBUG -O2
CXX23FLAGS = -std=c++2b -pedantic-errors -Werror -Wall -Weffc++ -Wextra \
	-Wconversion -Wsign-conversion -DNDEBUG -O2

all: main

debug: CXX20FLAGS += -DDEBUG -Og -ggdb
debug: main

main: main.cc camera.h colour.h  hittable.h hittable_list.h interval.h \
	material.h ray.h sphere.h utility.h vec3.h
	${CXX} ${CXX20FLAGS} -o main main.cc

clean:
	rm -f main
	rm -rf *.dSYM/
