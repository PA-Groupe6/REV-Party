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

# prise en charge du mode DEBUG
ifeq ($(DEBUG),yes)
	CFLAGS+=-g -DDEBUG
else
	CFLAGS=-DNDEBUG
endif

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
	@echo $< Integrated!
	@if [ -f "$<" ]; then \
	mkdir -p $(shell dirname $@); \
	$(CC) -c $< -o $@ $(CFLAGS); \
	fi

# s'assure que les dossier récepteurs sont crées
dirs:
	@if [ ! -d "$(BINDIR)" ]; then mkdir $(BINDIR); fi
	@if [ ! -d "$(OBJDIR)" ]; then mkdir $(OBJDIR); fi

# tests
# TODO

# génère la doc
docs:
	@doxygen doc/config

# supprime les résultats de compilation
clean:
	@rm -rf $(BINDIR)
	@rm -rf $(OBJDIR)

# clean + supprime la doc
mrproper: clean
	@rm -rf doc/html