#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <regex.h>
#include <time.h>

int checkMathId(char *s) {
	regex_t regex;
	regcomp(&regex, "^[[:digit:]]{4}-[[:digit:]]{5}-[[:digit:]]{5}$", REG_EXTENDED);
	return !regexec(&regex, s, 0, NULL, 0);
}

char *genActivationKey() {
	char *s = (char *)malloc(17 * sizeof(char));
	for (int i = 0; i < 16; i ++) s[i] = rand() % 10 + 0x30;
	s[4] = '-'; s[10] = '-'; s[16] = '\0';
	return s;
}

int f1(int n, int b, int c) {
	for (int bitIndex = 0; bitIndex < 8; bitIndex ++) {
		int bit = (b >> bitIndex) & 1;
		if (bit + ((n - bit) & ~1) == n) n = (n - bit) >> 1;
		else n = ((c - bit) ^ n) >> 1;
	}
	return n;
}

char *genPassword(char *s, int h) {
	for (int i = strlen(s) - 1; i >= 0; i --) h = f1(h, s[i], 0x105C3);
	int n1 = 0, n2 = 0;
	char n1str[6], n2str[6];
	while (f1(f1(h, n1 & 0xFF, 0x105C3), n1 >> 8, 0x105C3) != 0xA5B6)
		if (++n1 >= 0xFFFF) return "Error";
	n1 = ((n1 + 0x72FA) & 0xFFFF) * 99999.0 / 0xFFFF;
	sprintf(n1str, "%.05d", n1);
	int temp = n1str[0]*10000 + n1str[1]*1000 + n1str[3]*100 + n1str[4]*10 + n1str[2] - 533328;
	temp =  temp / 99999.0 * 0xFFFF + 1;
	temp = f1(f1(0, temp & 0xFF, 0x1064B), temp >> 8, 0x1064B);
	for (int i = strlen(s) - 1; i >= 0; i --) temp = f1(temp, s[i], 0x1064B);
	while (f1(f1(temp, n2 & 0xFF, 0x1064B), n2 >> 8, 0x1064B) != 0xA5B6)
		if (++n2 >= 0xFFFF) return "Error";
	n2 = (n2 & 0xFFFF) * 99999.0 / 0xFFFF;
	sprintf(n2str, "%.05d", n2);
	
	char tmp[16] = {n2str[3], n1str[3], n1str[1], n1str[0], '-', n2str[4], n1str[2], n2str[0], '-', n2str[2], n1str[4], n2str[1], ':', ':', '1', '\0'};
	char *passwd = (char *)malloc(16 * sizeof(char));
	memccpy(passwd, tmp, 1, 16);
	return passwd;
}

void generate(char *mathId) {
	if (!checkMathId(mathId)) printf("This is not a valid MathID !");
	else {
		char *activationKey = genActivationKey();
		int magicNumbers[15] = {10690, 12251, 17649, 24816, 33360, 35944, 36412, 42041, 42635, 44011, 53799, 56181, 58536, 59222, 61041};
		char *hash = (char *)malloc(36 * sizeof(char));
		sprintf(hash, "%s$1&%s", mathId, activationKey);
		char *password = genPassword(hash, magicNumbers[rand() % 15]);
		printf("Activation Key: %s\n", activationKey);
		printf("Password: %s", password);
	}
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
	char mathId[17];
	printf("Input your MathID (xxxx-xxxxx-xxxxx) and press ENTER: ");
	fgets(mathId, 17, stdin);
	mathId[16] = '\0';
	generate(mathId);
}