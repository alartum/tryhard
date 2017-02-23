#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "storage.h"


int print_help(char* name){
	char* help = "\n\nThis program for count word in file.\n\nFor example ~ $ %s [-h] file \n\n";
	printf(help, name);
	return 0;
}

int IS_WORD(char c){
	int ret = 0;
	if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9'))
		ret = 1;
	return ret;
}

int getword(int fd, char* str){
	int size = 0;

	char ch = 0;

	if(!(size = read(fd, &ch, 1)))
		return size;
		
	while(!IS_WORD(ch)){
		if(!(size = read(fd, &ch, 1)))
			return size;
	}

	size_t i = 0;
	str[i++] = ch;

	while((size = read(fd, &ch, 1))){
		if(IS_WORD(ch)){
			str[i++] = ch;
		}else{
			goto out;	
		}
	}
out:
	str[i++] = 0;
	return 1;
}

int main(int argc, char** argv){
	int opt = 0;
	size_t x = 0;
	char str[1024];
	size_t count = 0;

	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch(opt){
			case 'h':
				print_help(argv[0]);
				return 0;
			default:
				fprintf(stderr, "Bad argumets, try -h for help");
				return 0;
		}
	}



	if((argc - optind) != 1){
		fprintf(stderr, "To many argument's: use -h for help\n");
		return 0;
	}

	storage_t* in = init_hash(sizeof(size_t), 1024, 64, NULL, NULL);

	int fin = open(argv[1], O_RDONLY);

	if(fin < 0){
		perror(argv[1]);
		goto out;
	}


	while(getword(fin, str)){
		if(in->find(in, str)){
			x = *((size_t*)(in->find(in, str))) + 1;
			in->drop(in, str);
		}else{
			x = 1;
		}
		in->add(in, str, &x);
	}

	void* begin = in->begin(in);
	void* end = in->end(in);

	while(!in->iterEquel(begin, end)){
		printf("%s %zu\n", (const char*)(in->getKey(begin)), *((size_t*)in->getElem(begin)));
		in->next(begin);
	}

	free(begin);
	free(end);

	printf("Count of all word: %zu\n", in->size(in));

out:
	if(fin >= 0)
		close(fin);

	if(in != NULL){
		in->destroy(in);
		in = NULL;
	}

	return 0;
}
