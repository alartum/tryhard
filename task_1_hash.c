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

	storage_t* in = init_hash(sizeof(size_t), 1024, 0, NULL);
	storage_t* out = init_hash(sizeof(size_t), 1024, 0, NULL);

	FILE* fin = fopen(argv[1], "r");
	if(fin == NULL){
		perror(argv[1]);
		goto out;
	}


	while(fscanf(fin, "%s", str) != -1){
		in->add(in, str, &x);
	}



	FILE* fout = fopen(argv[2], "r");
	if(fout == NULL){
		perror(argv[2]);
		goto out;
	}

	

	while(fscanf(fout, "%s", str) != -1){
		if(in->find(in, str) && !out->find(out, str)){
			count++;		
		}
		out->add(out, str, &x);
	}

	printf("%zu\n", count);

out:
	if(fin != NULL)
		fclose(fin);
	if(fout != NULL)
		fclose(fout);

	if(in != NULL){
		in->delete(in);
		in = NULL;
	}
	if(out != NULL){
		out->delete(out);
		out = NULL;
	}

	return 0;
}
