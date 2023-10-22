CC=gcc
CFLAGS=-Wextra -Wall -Werror -pedantic

BINDIR=bin
OBJDIR=obj
SRCDIR=src

EXEC=rev
HDR= $(shell find $(SRCDIR) -name "*.h")
SRC= $(shell find $(SRCDIR) -name "*.c")
COMMON_SRC= $(filter $(basename $(HDR)), $(basename $(SRC)))
OBJ= $(addsuffix .o, $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(COMMON_SRC)))

# couleurs générales
RSTC= \033[0m
BOLD= \033[1m
SRCC= \033[38;5;189m
TSTC= \033[38;5;228m
ERRC= \033[38;5;160m

# couleurs liés à l'exécution
EXECC= \033[38;5;117m
SUCCC= \033[48;5;2m
FAILC= \033[48;5;124m

# prise en charge du mode DEBUG
ifeq ($(DEBUG),yes)
	CFLAGS+=-g -DDEBUG
else
	CFLAGS=-DNDEBUG
endif

#################################
#            COMPILE            #
#################################

# créer le dossier bin si il n'existe pas
# créer l'exécutable
# retire tout les objets auxiliaires
all: dirs $(EXEC)
	@if [ "$(DEBUG)" = "yes" ]; then \
		echo "Code generated in DEBUG mode"; \
	else \
		echo "Code generated in release mode"; \
	fi

# compile l'exécutable dans bin/
$(EXEC): $(OBJ)
	@$(CC) $(SRCDIR)/main.c -o $(BINDIR)/$(EXEC) $^ $(LDFLAGS)

# compile les dépendences dans obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "$(SRCC)$<$(RSTC) Integrated"
	@if [ -f "$<" ]; then \
	mkdir -p $(shell dirname $@); \
	$(CC) -c $< -o $@ $(CFLAGS); \
	fi

# s'assure que les dossier récepteurs sont crées
dirs:
	@if [ ! -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi
	@if [ ! -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi

#################################
#             TESTS             #
#################################

TSTDIR=test

# procédure de test générique
run_test= if [ -f $(TSTDIR)/$(2)test_$(1).c ]; then \
		$(CC) $(TSTDIR)/$(2)test_$(1).c $(3) -o $(TSTDIR)/$(2)t$(1).c $(CFLAGS); \
		echo "$(EXECC)Executing tests on $(TSTC)$(1).c$(RSTC)"; \
		if $(TSTDIR)/$(2)t$(1).c; then \
			echo "$(BOLD)$(SUCCC)|>-------------------------------= Tests Passed =- $(RSTC)\n"; \
		else \
			echo "$(BOLD)$(FAILC)|>-------------------------------= Tests Failed =- $(RSTC)\n"; \
		fi; \
		rm $(TSTDIR)/$(2)t$(1).c; \
	else \
		echo "$(BOLD)$(ERRC)fatal error$(RSTC): $(TSTDIR)/$(2)test_$(1).c doesn't exist$(RSTC)"; \
	fi; \
	rm -rf $(OBJDIR)

# TODO règles modules

tlogger: $(OBJDIR)/logger.o
	@$(call run_test,logger,,$^)

tbale: $(OBJDIR)/structure/bale.o $(OBJDIR)/logger.o
	@$(call run_test,bale,structure/,$^)

tgenericlinkedlist: $(OBJDIR)/structure/genericlinkedlist.o $(OBJDIR)/logger.o
	@$(call run_test,genericlinkedlist,structure/,$^)

tgraph: $(OBJDIR)/structure/graph.o $(OBJDIR)/logger.o
	@$(call run_test,graph,structure/,$^)

tlist: $(OBJDIR)/structure/list.o $(OBJDIR)/logger.o
	@$(call run_test,list,structure/,$^)

tmatrix: $(OBJDIR)/structure/matrix.o $(OBJDIR)/logger.o
	@$(call run_test,matrix,structure/,$^)

tsha256: $(OBJDIR)/utils/sha256/sha256.o $(OBJDIR)/logger.o
	@$(call run_test,sha256,utils/sha256/,$^)

tutils_sd: $(OBJDIR)/utils/utils_sd.o $(OBJDIR)/logger.o
	@$(call run_test,utils_sd,utils/,$^)

tstrlist: $(OBJDIR)/structure/strlist.o $(OBJDIR)/logger.o
	@$(call run_test,strlist,structure/,$^)

################################
#             MISC             #
################################

docs:
	@doxygen doc/config

# supprime les résultats de compilation
clean:
	@rm -rf $(BINDIR)
	@rm -rf $(OBJDIR)

# clean + supprime la doc
mrproper: clean
	@rm -rf doc/html