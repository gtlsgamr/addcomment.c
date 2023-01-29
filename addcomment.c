/*
######################################################################
First version written by Hitarth Thummar <hittarth91@gmail.com>.
It used javascript to dynamically inject content into webpages.

This is version two: It only creates a file, it is up to the website owner
to insert the comments into the pages however they wish. I inject them during build time
so no javascript is needed on user end to see comments.

The GPLv3 License (GPLv3)

Copyright (c) 2023 Hitarth Thummar

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	fprintf(fp,"\n{\"alias\": \"%s\"], \"url\": [\"%s\"], \"time\": [\"%s\"], \"body\": [\"%s\"]}\n",udcd(alias),udcd(url),udcd(time),udcd(body));

	fclose(fp); 

	printf("Your comment has been recorded for approval. <a href='javascript:history.back()'>Go Back</a>");
	return 0;
}
