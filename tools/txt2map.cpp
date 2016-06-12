#include <stdio.h>
#include <stdlib.h>


int main(int argc, char ** argv){
	if (argc<=2){
		printf("usage: txt2map [infile] [outfile]\n");
		return -1;
	}
	FILE *fp=fopen(argv[1],"r");
	FILE *fo=fopen(argv[2],"wb");
	int i,j;
	float k;
	while (fscanf(fp,"%d %d %f",&i,&j,&k)!=-1){
		fwrite(&i,sizeof(int),1,fo);
		fwrite(&j,sizeof(int),1,fo);
		fwrite(&k,sizeof(float),1,fo);
	}
	fclose(fp);
	fclose(fo);
	return 0;
}
