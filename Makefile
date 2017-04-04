export PKG_CONFIG_PATH = /usr/local/lib/pkgconfig/


IMGUI_PATH = ../imgui
OGL3_PATH = ../imgui/examples/opengl3_example
IMGUI_LIBS_PATH = ../imgui/examples/libs

TARGET = YAouRT
OBJS += main.o
# OBJS += tracer.o
OBJS += $(OGL3_PATH)/imgui_impl_glfw_gl3.o
OBJS += $(IMGUI_PATH)/imgui.o $(IMGUI_PATH)/imgui_demo.o $(IMGUI_PATH)/imgui_draw.o
OBJS += $(IMGUI_LIBS_PATH)/gl3w/GL/gl3w.o


#CXX = g++

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS = -lGL `pkg-config --static --libs glfw3`

	CXXFLAGS = -I$(IMGUI_PATH)/ -I$(IMGUI_LIBS_PATH)/gl3w `pkg-config --cflags glfw3`
	#CXXFLAGS += -I/usr/local/include
	CXXFLAGS += -Wall -Wformat
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS = -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	#LIBS += -L/usr/local/lib -lglfw3
	LIBS += -L/usr/local/lib -lglfw

	CXXFLAGS = -I$(IMGUI_PATH)/ -I$(IMGUI_LIBS_PATH)/gl3w -I/usr/local/include
	CXXFLAGS += -Wall -Wformat
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), MINGW64_NT-6.3)
   ECHO_MESSAGE = "Windows"
   LIBS = -lglfw3 -lgdi32 -lopengl32 -limm32

   CXXFLAGS = -I../../ -I../libs/gl3w `pkg-config --cflags glfw3`
   CXXFLAGS += -Wall -Wformat
   CFLAGS = $(CXXFLAGS)
endif


.cpp.o:
	@echo "*** C++: $@"
	$(CXX) $(CXXFLAGS) $(INCS) -c -o $@ $<

all: $(TARGET)
	@echo Build complete for $(ECHO_MESSAGE)

$(TARGET): $(OBJS)
	@echo "*** LINK: $(OBJS)"
	$(CXX) -o $(TARGET) $(OBJS) $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(TARGET) $(filter %.o, $(OBJS))
clean_main:
	rm -f $(TARGET) main.o

# header dependencies
main.o: tracer.hpp
