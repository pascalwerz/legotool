Lego games file ending in *savegamedata* seem to share the same basic structure:

Version 0, without loadVersionChecksum nor loadSize: (*Movie 1*)

|      | …0 …1 …2 …3 | …4 …5 …6 …7 |    …8 …9 …a …b     | …c …d …e …f  |
| :--: | :---------: | :---------: | :----------------: | :----------: |
|  0…  | loadVersion | percentage  | percentageChecksum | loadChecksum |
|  1…  |      …      |             |                    |              |



| **Offset**    | **Size**    | **Type** | **Name**           | **Comment**                                                  |
| ------------- | ----------- | -------- | ------------------ | ------------------------------------------------------------ |
| 0x00          | 4 bytes     | integer  | loadVersion        | load version                                                 |
| 0x04          | 4 bytes     | float    | percentage         | completion percentage or, for Lego Worlds, discoveries count |
| 0x08          | 4 bytes     | integer  | percentageChecksum | checksum of the four preceding bytes                         |
| 0x0c          | 4 bytes     | integer  | loadChecksum       | checksum of the *loadSize* bytes starting at offset 0x14     |
| 0x10          | loadSize    | free     | load               | load data. As there is no loadSize field in header, load data must be parsed to compute loadSize and checksum |
| loadSize+0x10 | unspecified |          |                    | extraneous data allowed                                      |



Version 1, without loadSize: (*Batman 3* and *Jurassic World*)

|      | …0 …1 …2 …3  |     …4 …5 …6 …7     | …8 …9 …a …b |    …c …d …e …f     |
| :--: | :----------: | :-----------------: | :---------: | :----------------: |
|  0…  | loadVersion  | loadVersionChecksum | percentage  | percentageChecksum |
|  1…  | loadChecksum |          …          |             |                    |



| **Offset**    | **Size**    | **Type** | **Name**            | **Comment**                                                  |
| ------------- | ----------- | -------- | ------------------- | ------------------------------------------------------------ |
| 0x00          | 4 bytes     | integer  | loadVersion         | load version                                                 |
| 0x04          | 4 bytes     | integer  | loadVersionChecksum | checksum of the four preceding bytes                         |
| 0x08          | 4 bytes     | float    | percentage          | completion percentage or, for Lego Worlds, discoveries count |
| 0x0c          | 4 bytes     | integer  | percentageChecksum  | checksum of the four preceding bytes                         |
| 0x10          | 4 bytes     | integer  | loadChecksum        | checksum of the *loadSize* bytes starting at offset 0x14     |
| 0x14          | loadSize    | free     | load                | load data. As there is no loadSize field in header, load data must be parsed to compute loadSize and checksum |
| loadSize+0x14 | unspecified |          |                     | extraneous data allowed                                      |



Version 2, with loadSize:

|      | …0 …1 …2 …3  |     …4 …5 …6 …7     | …8 …9 …a …b |    …c …d …e …f     |
| :--: | :----------: | :-----------------: | :---------: | :----------------: |
|  0…  | loadVersion  | loadVersionChecksum | percentage  | percentageChecksum |
|  1…  | loadChecksum |      loadSize       |      …      |                    |



| **Offset**    | **Size**    | **Type** | **Name**            | **Comment**                                                  |
| ------------- | ----------- | -------- | ------------------- | ------------------------------------------------------------ |
| 0x00          | 4 bytes     | integer  | loadVersion         | load version                                                 |
| 0x04          | 4 bytes     | integer  | loadVersionChecksum | checksum of the four preceding bytes                         |
| 0x08          | 4 bytes     | float    | percentage          | completion percentage or, for Lego Worlds, discoveries count |
| 0x0c          | 4 bytes     | integer  | percentageChecksum  | checksum of the four preceding bytes                         |
| 0x10          | 4 bytes     | integer  | loadChecksum        | checksum of the *loadSize* bytes starting at offset 0x18     |
| 0x14          | 4 bytes     | integer  | loadSize            | size of load data starting at offset 0x18                    |
| 0x18          | loadSize    | free     | load                | load data                                                    |
| loadSize+0x18 | unspecified |          |                     | extraneous data allowed                                      |



Each game may store whatever load it needs in such files, e.g. dlc* and game* file share the saveItem structure (and sometimes also includes a raw data block) , but opts* and feopts* are also based on this file format.

The loadVersion is the version of the *game's load*, not the shell's one. The shell's field names and version numbering (1 and 2) is a personal choice and in no way official.

You may also interpret a version 2 shell as a version 1 shell were the load starts with its own length-4.

What the "*percentage*" expresses depends on game and file. While it is specified here as a float and while Lego Worlds indeed stores the discoveries count here as a float, nothing prevents using it as a 4 bytes integer, or any 4 bytes format, as its checksum does not depend on the interpretation of the data, only on its bytes. It looks like this float field is used to present the completion percentage/discoveries count at a game slot selection dialog without analysing the load.

It is frequent to have some data present *after* the load, presumably for padding purpose.