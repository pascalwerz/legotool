Lego games file with *.savegamedata* extension share the same basic structure:

Version 2, with loadSize:

| **Offset**    | **Size**    | **Type** | **Name**            | **Comment**                                                  |
| ------------- | ----------- | -------- | ------------------- | ------------------------------------------------------------ |
| 0x00          | 4 bytes     | integer  | loadVersion         | load version                                                 |
| 0x04          | 4 bytes     | integer  | loadVersionChecksum | checksum of the four preceding bytes                         |
| 0x08          | 4 bytes     | float    | percentage          | completion percentage or, for Lego Worlds, discoveries count |
| 0x0c          | 4 bytes     | integer  | percentageChecksum  | checksum of the four preceding bytes                         |
| 0x10          | 4 bytes     | integer  | loadChecksum        | checksum of the *loadSize* bytes starting at offset 0x18     |
| 0x14          | 4 bytes     | integer  | loadSize            | size of load data starting at offset 0x18                    |
| 0x18          | loadSize    |          | load                | load data, format is free.                                   |
| loadSize+0x18 | unspecified |          |                     | extraneous data allowed                                      |



Version 1, without loadSize: (Batman 3, Jurassic World)

| **Offset**    | **Size**    | **Type** | **Name**            | **Comment**                                                  |
| ------------- | ----------- | -------- | ------------------- | ------------------------------------------------------------ |
| 0x00          | 4 bytes     | integer  | loadVersion         | load version                                                 |
| 0x04          | 4 bytes     | integer  | loadVersionChecksum | checksum of the four preceding bytes                         |
| 0x08          | 4 bytes     | float    | percentage          | completion percentage or, for Lego Worlds, discoveries count |
| 0x0c          | 4 bytes     | integer  | percentageChecksum  | checksum of the four preceding bytes                         |
| 0x10          | 4 bytes     | integer  | loadChecksum        | checksum of the *loadSize* bytes starting at offset 0x14     |
| 0x14          | loadSize    |          | load                | load data, format is free. as loadSize is not specified in header, load data must be parsed to compute loadSize and checksum |
| loadSize+0x14 | unspecified |          |                     | extraneous data allowed                                      |



Each game may store whatever load it needs in such files, e.g. dlc* and game* file share the saveItem structure (and sometimes also includes a raw data block) , but opts* and feopts* are also based on this file format.

The loadVersion is the version of the game's load, not the shell's one.

What the "percentage" really expresses in fact depends on game and file. While it is specified here as a float and while Lego Worlds stores the discoveries count here as a float, nothing prevents using it as an integer as its checksum does not depends on the interpretation of the data, only on its bytes. It seems this float field is there to help present the completion percentage/discoveries count at a slot selection dialog without analysing the load.

It is frequent to have some data present *after* the load, presumably for padding purpose.