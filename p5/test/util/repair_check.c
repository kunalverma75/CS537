#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include "fs.h"

void ASSERT(int cond, char *msg) {
	if (!cond) {
		fprintf(stderr, "%s\n", msg);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_RDONLY);
	ASSERT(fd > 0, "image not found.");	// Nonexistant

	struct stat sbuf;
	ASSERT(fstat(fd, &sbuf) == 0, "image not found.");

	void *img_ptr = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	ASSERT(img_ptr != NULL, "mmap failed.");

	struct superblock *sb = (struct superblock *) (img_ptr + BSIZE);
	struct dinode *dip = (struct dinode *) (img_ptr + 2 * BSIZE);

	ushort lostfound = 21;
	struct dirent *entry = (struct dirent *)(img_ptr + dip[lostfound].addrs[0] * BSIZE) + 2;

	ushort lostinum[sb->ninodes];
	memset(lostinum, 0, sizeof(lostinum));
	lostinum[3] = 1;
	lostinum[5] = 1;
	lostinum[8] = 1;
	lostinum[9] = 1;
	lostinum[10] = 1;
	lostinum[16] = 1;
	lostinum[17] = 1;
	lostinum[18] = 1;
	lostinum[19] = 1;
	lostinum[20] = 1;
	lostinum[22] = 1;
	lostinum[23] = 1;
	lostinum[24] = 1;
	lostinum[26] = 1;
	lostinum[27] = 1;
	lostinum[28] = 1;
	lostinum[30] = 1;
	lostinum[32] = 1;
	lostinum[35] = 1;
	lostinum[36] = 1;
	lostinum[37] = 1;

	for (int k = 0; k < BSIZE / (DIRSIZ + 2) - 2; k++, entry++) {
		if (entry->inum == 0)	continue;
		lostinum[entry->inum]--;
	}

	for (int i = 0; i < sb->ninodes; i++) {
		ASSERT(lostinum[i] == 0, ""); // ERROR
	}

	entry = (struct dirent *)(img_ptr + dip[20].addrs[0] * BSIZE);
	ASSERT((entry + 1)->inum == lostfound, "ERROR");
	printf("PASSED\n");
	return 0;
}