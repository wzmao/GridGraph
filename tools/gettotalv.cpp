#include <stdio.h>
#include <stdlib.h>

#define MAXREAD 1000000
int main(int argc, char ** argv){
	if (argc<=1){
		printf("usage: gettotalv [file]\n");
	}
	FILE *fp=fopen(argv[1],"rb");
	int maxvalue=0,ilist[MAXREAD],i,temp;
	while (1){
		temp=fread(&ilist,sizeof(int),MAXREAD,fp);
		if (temp==MAXREAD)
			for (i=0;i<MAXREAD;i++)
				maxvalue=maxvalue>ilist[i]?maxvalue:ilist[i];
		else{
			for (i=0;i<temp;i++)
				maxvalue=maxvalue>ilist[i]?maxvalue:ilist[i];
			break;
		}
	}
	fclose(fp);
	printf("The max vetical id is %d.\n",maxvalue);
	printf("Use %d for -v.\n",maxvalue+1);
	return 0;
}
