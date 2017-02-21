#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "storage.h"
#include <unistd.h>

int print_help(char* name){
	char* help = "\n\nThis program for count the same word in 2 files.\n\nFor example ~ $ %s first.file second.file\n\n";
	printf(help, name);
	return 0;
}

int main(int argc, char** argv){

	int opt;

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
	}

	storage_t* in = init_hash(sizeof(size_t), 1024, 0, NULL);
	storage_t* out = init_hash(sizeof(size_t), 1024, 0, NULL);

	FILE* f = fopen(argv[1], "r");

	if(f == NULL)
		perror("INPUT"); 

	char str[24];

	size_t x = 0;

	while(fscanf(f, "%s", str) != -1){
		in->add(in, str, &x);
	}

	fclose(f);

	f = fopen(argv[2], "r");

	size_t count = 0;	

	while(fscanf(f, "%s", str) != -1){
		if(in->find(in, str) && !out->find(out, str)){
			count++;		
		}
		out->add(out, str, &x);
	}
	fclose(f);
	printf("%zu\n", in->size(in));
	printf("%zu\n", out->size(out));
	printf("%zu\n", count);
	in->delete(in);

	return 0;
}
