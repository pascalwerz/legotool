/* by xvi */
#include "mappedFile.h"

#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>



// on success, returns the data pointer
// on failure, returns NULL and errno is set
// file must exist at path
void *mapFileAtPath(const char *path, mappedfile_t *mappedFile, mapFileFlag_t flags)
{
	int file;
	off_t fileSize;
	ssize_t readSize;
	void *fileData;


	mappedFile->data = NULL;
	mappedFile->size = 0;
	mappedFile->allocatedSize = 0;
	mappedFile->flags = 0;
	mappedFile->file = -1;

	file = open(path, (flags & MAPPEDFILE_RDWR) ? O_RDWR : O_RDONLY, 0644);
	if (file != -1)
		{
		fileSize = lseek(file, 0, SEEK_END);
		if (fileSize != -1)
			{
			fileData = malloc(fileSize);
			if (fileData)
				{
				lseek(file, 0, SEEK_SET); // ignore failure
				readSize = read(file, fileData, fileSize);
				if (readSize != -1)
					{
					if (readSize < fileSize)
						fileSize = readSize; // for whatever reason, less bytes than expected were read. Remember the lesser size.

					mappedFile->data = fileData;
					mappedFile->size = fileSize;
					mappedFile->allocatedSize = fileSize;
					mappedFile->flags = flags;
					mappedFile->file = file;

					return fileData;
					}
				}
			free(fileData);
			}
		close(file);
		}
	return NULL;
}



// on success, returns the new data pointer and mappedFile is updated, data is either truncated, or extended as requested
//             on extension, additional memory content is unspecified
// on failure, returns NULL, errno is set, mappedFile is untouched and pointed data is still valid
void *resizeMappedFile(mappedfile_t *mappedFile, size_t newSize)
{
	void *newData;


	if (newSize <= mappedFile->allocatedSize)
		{
		mappedFile->size = newSize;
		return mappedFile->data;
		}
	else
		{
		newData = realloc(mappedFile->data, newSize);
		if (newData)
			{
			mappedFile->data = newData;
			mappedFile->size = newSize;
			mappedFile->allocatedSize = newSize;

			return newData;
			}
		}

	return NULL;
}



// on success, returns 0
// on failure, returns -1 and errno is set. mappedFile is left unchanged but file on disk may have been damaged. retrying later is allowed
int unmapFile(mappedfile_t *mappedFile, unmapFileFlag_t flags)
{
	ssize_t writtenSize;
	off_t filePos;


	if ((flags & MAPPEDFILE_WRITE) && (mappedFile->flags & MAPPEDFILE_RDWR))
		{
		filePos = lseek(mappedFile->file, 0, SEEK_SET);
		if (filePos == -1)
			return -1;

		writtenSize = write(mappedFile->file, mappedFile->data, mappedFile->size);
		if ((writtenSize == -1) || (writtenSize < mappedFile->size))
			return -1;
		}

	close(mappedFile->file);	// if close() fails, ignore

	free(mappedFile->data);
	mappedFile->data = NULL;
	mappedFile->size = 0;
	mappedFile->flags = 0;
	mappedFile->file = -1;

	return 0;
}



