CC=gcc
CFLAGS=-Wextra -Wall -Werror -pedantic -std=c99 -pthread

BINDIR=bin
OBJDIR=obj
SRCDIR=src

CHECK=check
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
	@$(CC) $(SRCDIR)/main.c -o $(BINDIR)/$(EXEC) $^ $(CFLAGS)

# compile les dépendences dans obj/
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "$(SRCC)$<$(RSTC) Integrated"
	@if [ -f "$<" ]; then \
	mkdir -p $(shell dirname $@); \
	$(CC) -c $< -o $@ $(CFLAGS); \
	fi

$(CHECK): $(OBJDIR)/utils/sha256/sha256.o $(OBJDIR)/utils/sha256/sha256_utils.o
	@$(MAKE) dirs
	@$(CC) $(SRCDIR)/$(CHECK).c $^ -o $(BINDIR)/$(CHECK) $(CFLAGS)

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
		mkdir -p $(BINDIR)/$(2); \
		$(CC) $(TSTDIR)/$(2)test_$(1).c $(3) -o $(BINDIR)/$(2)t$(1) $(CFLAGS) -g; \
		echo "$(EXECC)Executing tests on $(TSTC)$(1).c$(RSTC)"; \
		if valgrind --leak-check=full $(BINDIR)/$(2)t$(1); then \
			echo "\n$(BOLD)$(SUCCC)|>-------------------------------= Tests Passed =- $(RSTC)\n"; \
		else \
			echo "\n$(BOLD)$(FAILC)|>-------------------------------= Tests Failed =- (xcode: $$?) $(RSTC)\n"; \
		fi; \
		rm $(BINDIR)/$(2)t$(1); \
	else \
		echo "$(BOLD)$(ERRC)fatal error$(RSTC): $(TSTDIR)/$(2)test_$(1).c doesn't exist$(RSTC)"; \
	fi; \
	rm -rf $(OBJDIR)

# TODO règles modules

 $(OBJDIR)/test_utils.o:
	@$(CC) -c $(TSTDIR)/test_utils.c -o $@ $(CFLAGS)

 $(OBJDIR)/structure/label_test_set.o:
	@$(CC) -c  $(TSTDIR)/structure/label_test_set.c -o $@ $(CFLAGS)

tlogger: $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,logger,,$^)

tinterpreter: $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,interpreter,,$^)

tbale: $(OBJDIR)/structure/data_struct_utils.o $(OBJDIR)/structure/list.o  $(OBJDIR)/structure/matrix.o $(OBJDIR)/structure/genericlist.o $(OBJDIR)/structure/bale.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o $(OBJDIR)/structure/label_test_set.o
	@$(call run_test,bale,structure/,$^)

tduel: $(OBJDIR)/structure/duel.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,duel,structure/,$^)

tgenericlinkedlist: $(OBJDIR)/structure/genericlinkedlist.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,genericlinkedlist,structure/,$^)

tgraph: $(OBJDIR)/structure/graph.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,graph,structure/,$^)

tlist: $(OBJDIR)/structure/list.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,list,structure/,$^)

tmatrix: $(OBJDIR)/structure/matrix.o $(OBJDIR)/structure/data_struct_utils.o $(OBJDIR)/structure/genericlist.o $(OBJDIR)/structure/list.o $(OBJDIR)/logger.o $(OBJDIR)/test_utils.o
	@$(call run_test,matrix,structure/,$^)

tsha256: $(OBJDIR)/utils/sha256/sha256.o $(OBJDIR)/test_utils.o
	@$(call run_test,sha256,utils/sha256/,$^)

tcsv_reader: $(OBJDIR)/utils/csv_reader.o $(OBJDIR)/test_utils.o $(OBJDIR)/structure/genericlist.o $(OBJDIR)/structure/bale.o $(OBJDIR)/logger.o $(OBJDIR)/structure/matrix.o $(OBJDIR)/test_utils.o $(OBJDIR)/structure/data_struct_utils.o $(OBJDIR)/structure/list.o
	@$(call run_test,csv_reader,utils/,$^)

tutils_sd: $(OBJDIR)/utils/utils_sd.o $(OBJDIR)/test_utils.o
	@$(call run_test,utils_sd,utils/,$^)



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