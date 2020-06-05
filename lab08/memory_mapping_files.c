#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	const char *first_file = argv[1];
	const char *second_file = argv[2];
	int f_file = open(first_file, O_RDWR, 0600);
	int s_file = open(second_file, O_RDWR|O_CREAT, 0600);
	struct stat st;
	stat(first_file, &st);
	int second_file_size = st.st_size;
	char *file_data = (char *) mmap (NULL, second_file_size, PROT_WRITE|PROT_READ, MAP_SHARED, f_file, 0);
	lseek (s_file, 0, SEEK_SET);
	write (s_file, file_data, second_file_size);
	ftruncate (s_file, second_file_size);
	close(f_file);
	close(s_file);
}
