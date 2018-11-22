# Not the best Makefile, but good enough for now.

ifdef DEBUG
 DEBUGFLAGS=-g2 -O0
else
 DEBUGFLAGS=
endif

TARGETS=legotool hash rhash checksum

LEGOTOOL_SRC=$(SRC)/legotool.c $(SRC)/saveItems.c $(SRC)/objects.c $(SRC)/mappings.c $(SRC)/ids.c $(SRC)/memory.c \
    $(SRC)/dumpSaveItem.c $(SRC)/refillProps.c $(SRC)/zap.c $(SRC)/setDebugSaveItem.c $(SRC)/setCoins.c $(SRC)/games.c \
    $(SRC)/ids/globalKnownIDs.c \
    $(SRC)/mappedFile.c $(SRC)/fnv.c $(SRC)/keywords.c
LEGOTOOL_OBJ=$(patsubst $(SRC)/%,$(OBJ)/%,$(LEGOTOOL_SRC:.c=.o))

.PHONY : all clean cleanall distrib basicdistrib

SRC=src
OBJ=obj

CFLAGS=-Wall -Werror
LDFLAGS=
CC=cc
RM=rm
MV=mv
MKDIR=mkdir
DEPFLAGS=-MT $@ -MMD -MP -MF $(OBJ)/$(@F:%.o=%).Td

$(shell $(MKDIR) -p $(OBJ) $(OBJ)/ids >/dev/null)

all: $(TARGETS)

clean:
	-$(RM) -rf $(OBJ) *.dSYM

cleanall: clean
	-$(RM) -rf $(TARGETS) legotool.tgz

basicdistrib: $(SRC) Makefile README.md LICENSE 00-do.sh 01-hash-pics.sh "savegame shell file format.md"
	-tar -cvzf legotool.tgz $^

distrib:      $(SRC) Makefile README.md LICENSE 00-do.sh 01-hash-pics.sh "savegame shell file format.md" dictionary.c
	-tar -cvzf legotool.tgz $^

include $(wildcard $(OBJ)/*.d)
include $(wildcard $(OBJ)/ids/*.d)

# $@	The file name of the target of the rule
# $<	The name of the first prerequisite
# $^	The names of all the prerequisites
# $*	The stem with which an implicit rule matches

$(OBJ)/%.d: ;
.PRECIOUS: $(OBJ)/%.d $(OBJ)/ids/%.d

$(OBJ)/%.o: $(SRC)/%.c $(OBJ)/%.d
	$(CC) $(DEBUGFLAGS) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
	@$(MV) -f $(OBJ)/$(@F:%.o=%).Td $(OBJ)/$(@F:%.o=%).d && touch $@

$(OBJ)/ids/globalKnownIDs.o: $(SRC)/ids/globalKnownIDs.c $(OBJ)/ids/globalKnownIDs.d
	$(CC) $(DEBUGFLAGS) -MT $@ -MMD -MP -MF $(OBJ)/ids/$(@F:%.o=%).Td $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
	-@$(MV) -f $(OBJ)/ids/$(@F:%.o=%).Td $(OBJ)/ids/$(@F:%.o=%).d && touch $@

$(OBJ)/wordHash.o: $(SRC)/wordHash.c $(OBJ)/wordHash.d
	$(CC) $(DEBUGFLAGS) -MT $@ -MMD -MP -MF $(OBJ)/$(@F:%.o=%.Td) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<
	-@$(MV) -f $(OBJ)/$(@F:%.o=%.Td) $(OBJ)/$(@F:%.o=%.d) && touch $@

%: $(SRC)/%.c $(OBJ)/%.d
	$(CC) $(DEBUGFLAGS) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $<
	-@$(MV) -f $(OBJ)/$(@F:%=%.Td) $(OBJ)/$(@F:%=%.d) && touch $@

legotool: $(LEGOTOOL_OBJ) $(OBJ)/legotool.d
	$(CC) $(DEBUGFLAGS) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(filter-out %.d,$(filter-out %.h,$^))

rhash: $(SRC)/rhash.c $(OBJ)/wordHash.o $(OBJ)/rhash.d
	$(CC) $(DEBUGFLAGS) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(filter-out %.d,$(filter-out %.h,$^))
	-@$(MV) -f $(OBJ)/rhash.Td $(OBJ)/rhash.d && touch $@

checksum: $(SRC)/checksum.c $(OBJ)/fnv.o $(OBJ)/mappedFile.o $(OBJ)/checksum.d
	$(CC) $(DEBUGFLAGS) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@ $(filter-out %.d,$(filter-out %.h,$^))
	-@$(MV) -f $(OBJ)/$(@F:%=%.Td) $(OBJ)/$(@F:%=%.d) && touch $@
