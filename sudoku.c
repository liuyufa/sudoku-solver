#include <stdio.h>
#include <time.h>
int outer = 0;
void show(int a[9][9])
{
	int i, j;
	for(i = 0; i < 9; i ++) {
		if(!(i % 3)) {
			printf("\n");
		}
		for(j = 0; j < 9; j ++) {
			if(!(j % 3)) {
				printf(" ");
			}
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	printf("---------------------\n");
}
int count(int a[9][9])
{
	int i, *ad;
	ad = * a;
	for(i = 0; i < 81; i ++) {
		if(!ad[i]) {
			break;
		}
	}
	return i;
}
void turnReal(int a[9][9])
{
	int i, j, k;
	for(i = 0; i < 9; i ++) {
		for(j = 0; j < 9; j ++) {
			k = 1;
			while(a[i][j] >> k) {
				k ++;
			}
			a[i][j] = k;
		}
	}
}
int nChoice(int n)
{
	int i, flag = 0;
	for(i = 0; i < 9; i++) {
		if(((n >> i) & 1) == 0) {
			flag ++;
		}
	}
	return flag;
}
void newB(int b[9][9], int i, int j, int x)
{
	int k;
	for(k = 0; k < 9; k ++) {
		b[i][k] |= x;
		b[k][j] |= x;
		b[i / 3 * 3 + k / 3][j / 3 * 3 + k % 3] |= x;
	}
}
int one(int a[9][9], int b[9][9]) {
	int i, j;
	for(i = 0; i < 9; i ++) {
		for(j = 0; j < 9; j ++) {
			if(a[i][j] == 0) {
				if(b[i][j] == 511) {
					return -1;
				}
				if(nChoice(b[i][j]) == 1) {
					a[i][j] = (511 ^ b[i][j]);
					newB(b, i, j, a[i][j]);
					return 1;
				}
			}
		}
	}
	return 0;
}
int guess(int a[9][9], int b[9][9])
{
	int i, j, tmp, ii, jj, bx[9][9], ax[9][9], k, bxx[9][9];
	for(ii = 0; ii < 9; ii ++) {
		for(jj = 0; jj < 9; jj ++) {
			ax[ii][jj] = a[ii][jj];
			bx[ii][jj] = b[ii][jj];
		}
	}
	while((tmp = one(ax, bx)) == 1);
	if(tmp == -1) {
		return 0;
	}
	i = count(ax);
	if(i == 81) {
		turnReal(ax);
		show(ax);
		return 1;
	}
	outer ++;
	j = i % 9;
	i /= 9;
	for(tmp = 0; tmp < 9; tmp ++) {
		if(((bx[i][j] >> tmp) & 1) == 0) {
			for(ii = 0; ii < 9; ii++) {
				for(jj = 0; jj < 9; jj ++) {
					bxx[ii][jj] = bx[ii][jj];
				}
			}
			ax[i][j] = (1 << tmp);
			bxx[i][j] = 511;
			newB(bxx, i, j, ax[i][j]);
			k = guess(ax, bxx);
			if(k) {
				return 1;
			}
		}
	}
	return 0;
}
int prepare(int a[9][9])
{
	int i, j, ax[9][9], row[9], col[9], cube[9], k, bx[9][9];
	for(i = 0; i < 9; i ++) {
		for(j = 0; j < 9; j ++) {
			if(a[i][j]) {
				ax[i][j] = 1 << (a[i][j] - 1);
			} else {
				ax[i][j] = 0;
			}
		}
	}
	for(k = 0; k < 9; k ++) {
		col[k] = row[k] = cube[k] = 0;
	}
	for(k = 0; k < 9; k ++) {
		for(i = 0; i < 9; i ++) {
			row[k] |= ax[k][i];
			col[k] |= ax[i][k];
			cube[k] |= ax[k / 3 * 3 + i / 3][k % 3 * 3 + i % 3];
		}
	}
	for(i = 0; i < 9; i ++) {
		for(j = 0; j < 9; j ++) {
			if(ax[i][j]) {
				bx[i][j] = 511;
			} else {
				bx[i][j] = row[i] | col[j] | cube[i / 3 * 3 + j / 3];
			}
		}
	}
	return guess(ax, bx);
}
int main(int args, char *argv[])
{
	int i, j, a[9][9];
	FILE *fp;
	clock_t start, end;
	start = clock();
	if(args != 2) {
		printf("Usage: %s filename\n", argv[0]);
		return 73;
	}
	if(!(fp = fopen(argv[1], "r"))) {
		printf("No such file\n");
		return 73;
	}
	for(i = 0; i < 9; i ++) {
		for(j = 0; j < 9; j ++) {
			a[i][j] = fgetc(fp) - 0x30;
		}
		fgetc(fp);
	}
	fclose(fp);
	show(a);
	i = prepare(a);
	if(!i) {
		printf("!!This puzzle is contradictory!\n");
	}
	printf(">%d guesses\n", outer);
	end = clock();
	printf(">%d ns it takes\n", end - start);
	return 0;
}
