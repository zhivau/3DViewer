SRC = model/*.h \
    model/*.cpp \
    view/*.h \
    view/*.cpp \
    main.cpp

TEST_FLAGS= -lgtest_main -pthread -lgtest

all: install dist dvi

install: uninstall
	@mkdir build
	@cd build/ && qmake ../3DViewer_2.pro && make
	@rm -rf build/*.cpp build/*.h  build/*.o build/Makefile build/.qmake.stash

run:
	./build/3DViewer_2

uninstall:
	@rm -rf build

dvi:
	@doxygen
	@open html/index.html 

dist: 
	@mkdir -p 3D_Viewer_temp/src
	@cp -R tests 3D_Viewer_temp/src
	@cp -R model 3D_Viewer_temp/src
	@cp -R QtGifImage 3D_Viewer_temp/src
	@cp -R view 3D_Viewer_temp/src
	@cp -R resources 3D_Viewer_temp/src
	@cp -R main.cpp 3D_Viewer_temp/src
	@cp -R Makefile 3D_Viewer_temp/src
	@cp -R 3DViewer_2.pro 3D_Viewer_temp/src
	@cp -R Doxyfile 3D_Viewer_temp/src
	tar cvzf 3D_Viewer.tgz 3D_Viewer_temp/
	@rm -rf 3D_Viewer_temp

tests: clean
	@mkdir tests/build
	@cp resources/cube.obj tests/build/
	@cd tests/build/ && qmake ../Tests.pro && make
	@rm -rf tests/build/*.cpp tests/build/*.h  tests/build/*.o tests/build/Makefile tests/build/.qmake.stash
	@cd tests/build/ && ./Tests


clean:
	rm -rf build 3D_Viewer.tgz html tests/build
	
check_style: $(SRC)
	@clang-format -style=Google -n  $^
	
style: $(SRC)
	@clang-format -style=Google -i  $^

