(**important: legotool is developed on macOS and not tested on any other platform**, but should be compilable on other platforms with little or reasonable effort)

Here are my command line tools to analyse and modify saved game files for following Lego games:

- Lego Batman 3: Beyond Gotham
- Lego Jurassic World
- Lego Marvel's Avengers
- Lego Star Wars: The Force Awakens
- Lego Worlds
- The Lego Ninjago Movie Videogame
- Lego The Incredibles
- Lego Marvel Super Heroes 2
- Lego DC Super-Villains
- The Lego Movie 2

No other game is supported.

More, for all these games, only a limited number of files are supported:

- the *GAME\*.SAVEGAMEDATA* and *DLC\*.SAVEGAMEDATA* inside the *SLOT\** folders. These files do not hold everything about a saved game but hold most progress information.
- for Lego Worlds, CONTENT/S[1-4]-SYSGLOBAL/2500304796/QL/AUTOSAVE.GAMESAVE can be dumped but not modified. If you wonder, 2500304796 is the decimal for 0x95079f9c which is the FNV hash for *"QuestLogs"*.

This tool exists because most recent LEGO games store saved games in a similar way, heavily relying on FNV hashs (see *About Lego's FNV hash.md*)



**Tools**

*legotool* is the tool to read and write the *.savegamedata* files. Other tools are designed to help build it.

*hash* is a tool to convert a string to its FNV1 hash. Invoke with -? option for usage.

*rhash* is a tool to analyse saved games, finding strings matching given FNV1 hash using a *user-built* dictionary. Invoke with -? option for usage. *rhash* is needed to add support for new games to *legotool*.

*src/ids/00-how-to.txt* contains instructions to build the dictionary required to build *rhash* (but not required to build *legotool*).

*checksum* is a simple tool to compute a file's part checksum or to find which part of a file yield given checksum. Invoke with -? option for usage.

*00-do.sh* is a simple shell to automate the handling off all supported files in a slot directory.

*01-hash-pics.sh* is a simple shell to hash picture path used in *PhotoGallery* saveItems of *LEGO Marvel Super Heroes 2*.