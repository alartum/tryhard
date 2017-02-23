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
	char* help = "\n\nThis program for count the same word in 2 files.\n\nFor example ~ $ %s first.file second.file\n\n";
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



	if((argc - optind) != 2){
		fprintf(stderr, "To many argument's: use -h for help\n");
		return 0;
	}

	storage_t* in = init_hash(sizeof(size_t), 1024, 64, NULL, NULL);
	storage_t* out = init_hash(sizeof(size_t), 1024, 64, NULL, NULL);

	int fin = open(argv[1], O_RDONLY);

	if(fin < 0){
		perror(argv[1]);
		goto out;
	}


	while(getword(fin, str)){
		in->add(in, str, &x);
	}


	int fout = open(argv[2], O_RDONLY);

	if(out < 0){
		perror(argv[2]);
		goto out;
	}

	

	while(getword(fout, str)){
		if(in->find(in, str) && !out->find(out, str)){
			count++;		
		}
		out->add(out, str, &x);
	}

	printf("%zu\n", count);

out:
	if(fin >= 0)
		close(fin);
	if(fout >= 0)
		close(fout);

	if(in != NULL){
		in->destroy(in);
		in = NULL;
	}
	if(out != NULL){
		out->destroy(out);
		out = NULL;
	}

	return 0;
}
