OUT          = fos.syntax
OUT_BEGIN    = fos.begin.syntax
OUT_END      = fos.end.syntax

OBJDIR       = lib
SO_CLIENT    = $(OBJDIR)/$(OUT).client.so
SO_MAPPER    = $(OBJDIR)/$(OUT).mapper.so
SO_SERVER    = $(OBJDIR)/$(OUT).server.so

SRC          = $(OUT).cpp

REQUIREMENTS = ASCompiler AngelScript fonline.h

INCLUDE_DIRS = .

CXX        ?= g++
CXXFLAGS    = -std=c++11 $(addprefix -I, $(INCLUDE_DIRS)) -MD
CXXFLAGS_SO = -rdynamic -shared -fPIC

OBJ_CLIENT = $(addprefix $(OBJDIR)/client/, $(notdir $(SRC:.cpp=.o)))
OBJ_MAPPER = $(addprefix $(OBJDIR)/mapper/, $(notdir $(SRC:.cpp=.o)))
OBJ_SERVER = $(addprefix $(OBJDIR)/server/, $(notdir $(SRC:.cpp=.o)))

DEP_CLIENT = $(OBJ_CLIENT:.o=.d)
DEP_MAPPER = $(OBJ_CLIENT:.o=.d)
DEP_SERVER = $(OBJ_CLIENT:.o=.d)

.PHONY: $(OUT)

all: $(REQUIREMENTS) $(OBJDIR) $(SO_CLIENT) $(SO_MAPPER) $(SO_SERVER) $(OUT)

-include $(DEP_CLIENT)
-include $(DEP_MAPPER)
-include $(DEP_SERVER)

$(REQUIREMENTS):
	@echo Missing: $@
	@echo You need to copy/create symlink to this file/directory first
	@echo Default locations:
	@echo  ASCompiler.... [FOnline SDK repository]/Tools/ASCompiler
	@echo  AngelScript... [FOnline SDK repository]/Server/scripts/AngelScript
	@echo  fonline.h..... [FOnline SDK repository]/Server/scripts/fonline.h
	@false

$(OUT):
	cp $(OUT_BEGIN) $@
	./ASCompiler $(OUT).fos -client -D "OUTFILE \"$@\"" -run main
	./ASCompiler $(OUT).fos -mapper -D "OUTFILE \"$@\"" -run main
	./ASCompiler $(OUT).fos         -D "OUTFILE \"$@\"" -run main
	cat $(OUT_END) >> $@

$(OBJDIR):
	mkdir -p $(OBJDIR)/client
	mkdir -p $(OBJDIR)/mapper
	mkdir -p $(OBJDIR)/server

$(SO_CLIENT): $(OBJ_CLIENT)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(SO_MAPPER): $(OBJ_MAPPER)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(SO_SERVER): $(OBJ_SERVER)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(OBJDIR)/client/%.o: %.cpp
	@echo $< ...
	$(CXX) -D __CLIENT $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/mapper/%.o: %.cpp
	@echo $< ...
	$(CXX) -D __MAPPER $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/server/%.o: %.cpp
	@echo $< ...
	$(CXX) -D __SERVER $(CXXFLAGS) -c $< -o $@
