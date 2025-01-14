 CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -pthread

SRCDIR = .
INCDIR = .
BINDIR = bin
OBJDIR = obj

SRCS = $(SRCDIR)/main.c $(SRCDIR)/client/client_socket.c $(SRCDIR)/server/server_socket.c
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

EXEC = $(BINDIR)/p2pchat

all: $(EXEC)

$(EXEC): $(OBJS)
	@mkdir -p $(BINDIR)  # Cria o diretório bin, caso não exista
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)
	@echo "Compilação concluída!"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)/client  # Cria o diretório obj/client, caso não exista
	@mkdir -p $(OBJDIR)/server  # Cria o diretório obj/server, caso não exista
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@
	@echo "Compilando $<..."

clean:
	@rm -rf $(OBJDIR) $(BINDIR)
	@echo "Arquivos objeto e binários removidos!"

rebuild: clean all
