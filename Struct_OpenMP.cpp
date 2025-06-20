#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define NUM 2
#define QUAN 21000
#define ADDRESS "D:/dots.txt"

struct Point {
	double x;
	double y;
	double z;
};

void strFill(Point arr[]) {
	int i;
#pragma omp parallel for private(i)
	for (int i = 0; i < QUAN; i += 1) {
		arr[i].x = rand();
		arr[i].y = rand();
		arr[i].z = rand();
	}
}

Point strCenter(Point arr_temp[]) {
	struct Point Center;
	int i;
	double x_sum = 0;
	double y_sum = 0;
	double z_sum = 0;
#pragma omp parallel for private(i) reduction(+:x_sum,y_sum,z_sum)
	for (i = 0; i < QUAN; i += 1) {
		x_sum += arr_temp[i].x;
		y_sum += arr_temp[i].y;
		z_sum += arr_temp[i].z;
	}
	Center.x = x_sum / QUAN;
	Center.y = y_sum / QUAN;
	Center.z = z_sum / QUAN;
	return Center;
}

int main() {
	//=====================
	double t1, t2;
	t1 = omp_get_wtime();
	//=====================
	omp_set_num_threads(NUM);
	srand(time(NULL));
	Point arr[QUAN];
	strFill(arr);
	printf("x=%f y=%f z=%f\n", arr[0].x, arr[0].y, arr[0].z);
	FILE* f = fopen(ADDRESS, "wb");
	if (f == NULL) {
		printf("file open error: %s\n", ADDRESS);
		exit(1);
	}
	else {
		fwrite(arr, sizeof(Point), QUAN, f);
		fclose(f);
	}
	FILE* fi = fopen(ADDRESS, "rb");
	rewind(fi);
	Point arr_temp[QUAN];
	fread(arr_temp, sizeof(Point), QUAN, fi);
	printf("x=%f y=%f z=%f\n", arr_temp[0].x, arr_temp[0].y, arr_temp[0].z);
	fclose(f);
	Point Center = strCenter(arr_temp);
	printf("Center is x=%f y=%f z=%f\n", Center.x, Center.y, Center.z);
	//=====================
	t2 = omp_get_wtime();
	//=====================
	printf("Spent time is %f\n", t2 - t1);
	system("pause");
	return 0;
}





