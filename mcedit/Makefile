OUT          = fos.syntax
OUT_BEGIN    = $(OUT:.syntax=.begin.syntax)
OUT_END      = $(OUT:.syntax=.end.syntax)

OBJDIR       = lib
SO_CLIENT    = $(OBJDIR)/$(OUT).client.so
SO_MAPPER    = $(OBJDIR)/$(OUT).mapper.so
SO_SERVER    = $(OBJDIR)/$(OUT).server.so
SO           = $(SO_CLIENT) $(SO_MAPPER) $(SO_SERVER)

SRC          = $(OUT).cpp

REQUIREMENTS = ASCompiler AngelScript fonline.h

INCLUDE_DIRS = .

CXX        ?= g++
CXXFLAGS    = -std=c++11 $(addprefix -I, $(INCLUDE_DIRS)) -MD
CXXFLAGS_SO = -rdynamic -shared -fPIC

OBJ_CLIENT = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.client.o))
OBJ_MAPPER = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.mapper.o))
OBJ_SERVER = $(addprefix $(OBJDIR)/, $(SRC:.cpp=.server.o))
OBJ        = $(OBJ_CLIENT) $(OBJ_MAPPER) $(OBJ_SERVER)


DEP_CLIENT = $(OBJ_CLIENT:.o=.d)
DEP_MAPPER = $(OBJ_MAPPER:.o=.d)
DEP_SERVER = $(OBJ_SERVER:.o=.d)
DEP        = $(DEP_CLIENT) $(DEP_MAPPER) $(DEP_SERVER)


.PHONY: $(OUT) clean again

all: $(REQUIREMENTS) $(OBJDIR) $(SO) $(OUT)

clean:
	rm -f $(DEP) $(OBJ) $(SO)

again: clean all

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

$(OUT): $(OUT_BEGIN) $(OUT_END)
	cp $(OUT_BEGIN) $@
	./ASCompiler $(OUT).fos -client -D "OUTFILE \"$@\"" -run main -D ADDONS
	./ASCompiler $(OUT).fos -mapper -D "OUTFILE \"$@\"" -run main
	./ASCompiler $(OUT).fos         -D "OUTFILE \"$@\"" -run main
	cat $(OUT_END) >> $@

$(OUT_BEGIN) $(OUT_END):
	@echo Missing: $@
	@echo Cannot continue
	@false

$(OBJDIR):
	mkdir -p $@

$(SO_CLIENT): $(OBJ_CLIENT)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(SO_MAPPER): $(OBJ_MAPPER)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(SO_SERVER): $(OBJ_SERVER)
	$(CXX) $(CXXFLAGS) $(CXXFLAGS_SO) -o $@ $<

$(OBJDIR)/%.client.o: %.cpp
	@echo $< ...
	$(CXX) -D __CLIENT $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.mapper.o: %.cpp
	@echo $< ...
	$(CXX) -D __MAPPER $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/%.server.o: %.cpp
	@echo $< ...
	$(CXX) -D __SERVER $(CXXFLAGS) -c $< -o $@
