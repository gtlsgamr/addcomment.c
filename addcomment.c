#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define FILEPATH "comments.js.bkp"

/* Converts a hex character to its integer value */
char from_hex(char ch) {
	return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *udcd(char *str) {
	char *pstr = str, *buf = malloc(strlen(str) + 1), *pbuf = buf;
	while (*pstr) {
		if (*pstr == '%') {
			if (pstr[1] && pstr[2]) {
				*pbuf++ = from_hex(pstr[1]) << 4 | from_hex(pstr[2]);
				pstr += 2;
			}
		} else if (*pstr == '+') { 
			*pbuf++ = ' ';
		} else {
			*pbuf++ = *pstr;
		}
		pstr++;
	}
	*pbuf = '\0';
	return buf;
}

int main(void)
{
	char Buffer[5500]={0};
	char time[100], url[500], alias[25], body[5000];
	int InputLength = atoi(getenv("CONTENT_LENGTH") );
	int r;

	/*InputLength = min( InputLength, sizeof(Buffer)-1 );*/

	fread( Buffer, InputLength, 1, stdin );

	r = sscanf(Buffer, "alias=%[^&]&url=%[^&]&time=%[^&]&body=%[^&]", alias, url, time, body);

	if(r!=4){
		printf("ERROR");
		return -1;
	}

	puts("Status: 200 OK\r");
	puts("Content-Type: text/html\r");
	puts("\r");

	FILE *fp;

	fp = fopen(FILEPATH,"a+");

	if(fp==NULL){
		printf("NO FILE");
		return -1;
	}

	fprintf(fp,"\n{\"alias\": \"%s\", \"url\": \"%s\", \"time\": \"%s\", \"body\": \"%s\"},\n",udcd(alias),udcd(url),udcd(time),udcd(body));

	fclose(fp); 

	printf("Your comment has been recorded for approval. <a href='javascript:history.back()'>Go Back</a>");
	return 0;
}
