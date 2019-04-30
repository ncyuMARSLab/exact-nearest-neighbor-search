#include <windows.h>
#include <vector>
#include <thread>
#include <algorithm>
#include "myfunction\time_spend.h"
#include "myfunction\file_to_array.h"
#include "myfunction\open_file_chick_buffer.h"
#include "myfunction\create_delete_largeSize_array.h"

#define DATA_NUM 1000000000
#define DATA_DIM 128
#define QUERY_NUM 10000
#define THREAD_NUM 10
#define GROUNDTRUTH_NUM 1000
#define QUERY_PER_TIME 5
char *g_input_folder = "D:\\Input\\SIFT1B\\";//10.16.173.110/Public/output_1B_for_65536_class/"; // "..//..//input//";//smae folder for this project
char *g_output_folder = "D:\\Output\\";
string g_file_name = "";

double **g_query = NEW2D(QUERY_NUM, DATA_DIM, double);

class myclass {
public:
	//myclass(int a, int b) :Distance(a), ID(b) {}
	double Distance;
	unsigned __int32 ID;
	bool operator < (const myclass &m)const {
		return Distance < m.Distance;
	}
};

void LoadFile() {
	FILE *input_file;
	g_file_name = "Top2Data.txt";//*
	ChickOpenFile(input_file, g_input_folder, g_file_name, "rb");
	FileToArray2D_double(input_file, g_query, QUERY_NUM, DATA_DIM);
	fclose(input_file);
}
double Distance(double data[], double codebook[]) {
	double dist = 0;
	double res;
	for (int j = 0; j < DATA_DIM; j++) {
		res = data[j] - codebook[j];
		dist += res * res;
	}
	return dist;
}

void GetGroundtruth(FILE *input_file, vector<myclass> query_dist[], int file_id, int query_id) {
	rewind(input_file);
	char *saveptr = NULL;
	char *buffer = (char*)malloc(bufferSizeForLine * DATA_DIM);
	chickBuffer(buffer);
	int id = file_id *(DATA_NUM / THREAD_NUM);
	double *dist = new double[QUERY_PER_TIME]();
	double res = 0;
	double data = 0;
	for (int i = 0; i < DATA_NUM / THREAD_NUM; i++, id++) {
		fgets(buffer, bufferSizeForLine * DATA_DIM, input_file);
		saveptr = buffer;
		for (int j = 0; j < DATA_DIM; j++) {
			data = CharToDouble(&saveptr);
			for (int q = 0; q < QUERY_PER_TIME; q++) {
				res = data - g_query[query_id + q][j];
				dist[q] += res*res;
			}
		}
		for (int q = 0; q < QUERY_PER_TIME; q++) {
			query_dist[q][id].Distance = dist[q];
			dist[q] = 0;
		}
	}
	free(buffer);
}
void Output(vector<myclass> &ans, FILE *output_file) {
	char str[64];

	for (int i = 0; i < GROUNDTRUTH_NUM; i++) {
		_itoa_s(ans[i].ID, str, sizeof(str), 10);
		fwrite(str, strlen(str), 1, output_file);
		fwrite(" ", 1, 1, output_file);
	}
	fwrite("\n", 1, 1, output_file);
}
/*void SortQueryAns(int query_id, vector<myclass> query_dist[]){
	int n = query_dist[query_id].size();
	int i, j, k, t = 1, Temp, sp;
	for (i = n - 1; i>0; i--)
	{
		sp = 1;
		for (j = 0; j <= i; j++)
			if (query_dist[query_id][j].Distance > query_dist[query_id][j+1].Distance)
			{  //兩數交換位置
				Temp = query_dist[query_id][j].Distance;
				query_dist[query_id][j].Distance = query_dist[query_id][j+1].Distance;
				query_dist[query_id][j+1].Distance = Temp;
				sp = 0;
			}
		if (sp == 1) break;
	}
}*/
/*void SortQueryAns(int query_id, vector<myclass> query_dist[]) {
	int n = query_dist[query_id].size();
	// Start with a big gap, then reduce the gap
	for (int gap = n / 2; gap > 0; gap /= 2)
	{
		// Do a gapped insertion sort for this gap size.
		// The first gap elements a[0..gap-1] are already in gapped order
		// keep adding one more element until the entire array is
		// gap sorted 
		for (int i = gap; i < n; i += 1)
		{
			// add a[i] to the elements that have been gap sorted
			// save a[i] in temp and make a hole at position i
			int temp_dis = query_dist[query_id][i].Distance;

			// shift earlier gap-sorted elements up until the correct 
			// location for a[i] is found
			int j;
			for (j = i; j >= gap && query_dist[query_id][j - gap].Distance > temp_dis; j -= gap)
				swap(query_dist[query_id][j], query_dist[query_id][j - gap]);
		}
	}
}*/
/*void SortQueryAns(int query_id, vector<myclass> query_dist[]){
	int n = DATA_NUM;
	int gap, i, j, temp;
	for (gap = n / 2; gap > 0; gap /= 2)

		for (i = gap; i < n; i++)

			for (j = i - gap; j >= 0 && query_dist[query_id][j].Distance > query_dist[query_id][j + gap].Distance; j -= gap) {

				temp = query_dist[query_id][j].Distance;

				query_dist[query_id][j].Distance = query_dist[query_id][j+gap].Distance;

				query_dist[query_id][j+gap].Distance = temp;

			}

}*/
void SortQueryAns(int query_id, vector<myclass> query_dist[]) {
	stable_sort(query_dist[query_id].begin(), query_dist[query_id].end());
	//sort(query_dist[query_id].begin(), query_dist[query_id].end());
	//partial_sort(query_dist[query_id].begin(), query_dist[query_id].begin() + GROUNDTRUTH_NUM + 1, query_dist[query_id].end());

}
void Processing() {
	vector<myclass> query_dist[QUERY_PER_TIME];

	FILE *output_file;//write
	g_file_name = "Groundtruth.txt";
	ChickOpenFile(output_file, g_output_folder, g_file_name, "wb"); //"wb" write by binary

	thread thread_sort[QUERY_PER_TIME];

	thread thread_proc[THREAD_NUM];
	FILE *input_file[THREAD_NUM];
	for (int i = 0; i < 10; i++) {
		cout << i << endl;
		g_file_name = "SIFT100MRealNumber" + to_string(i) + ".txt";
		ChickOpenFile(input_file[i], g_input_folder, g_file_name, "rb");
	}
	/*g_file_name = "Deep1B_100M_8.txt";
	ChickOpenFile(input_file[8], "C:\\Users\\1050468\\Desktop\\deep\\", g_file_name, "rb");
	g_file_name = "Deep1B_100M_9.txt";
	ChickOpenFile(input_file[9], "G:\\deep\\", g_file_name, "rb");*/

	for (int i = 0; i < QUERY_PER_TIME; i++) {
		query_dist[i].resize(DATA_NUM);
	}
	for (int q = 6415; q < QUERY_NUM; q += QUERY_PER_TIME) {
		double startTime = clock();
		for (int i = 0; i < QUERY_PER_TIME; i++) {
			for (int j = 0; j < DATA_NUM; j++) {
				query_dist[i][j].ID = j;
				query_dist[i][j].Distance = 0;
			}
		}
		for (int i = 0; i < THREAD_NUM; i++) {
			thread_proc[i] = thread(GetGroundtruth, input_file[i], query_dist, i, q);
		}
		for (int i = 0; i < THREAD_NUM; i++) {
			thread_proc[i].join();
		}
		cout << "process end" << endl;
		for (int i = 0; i < QUERY_PER_TIME; i++) {
			thread_sort[i] = thread(SortQueryAns, i, query_dist);
		}
		for (int i = 0; i < QUERY_PER_TIME; i++) {
			thread_sort[i].join();
		}
		cout << "sort end" << endl;
		for (int i = 0; i < QUERY_PER_TIME; i++) {
			Output(query_dist[i], output_file);
		}
		TimeSpend(startTime);
		//system("pause");
		//sort(query_dist.begin(), query_dist.end());
		//partial_sort(query_dist.begin(), query_dist.begin() + GROUNDTRUTH_NUM + 1, query_dist.end());
		//Output(query_dist, output_file);
		//cout << query_dist[0].ID << endl;
	}
	fclose(output_file);
}

void main() {
	CreateDirectory(g_output_folder, NULL);//create Output floder

	LoadFile();
	Processing();

	system("pause");

	/*int x;
	while (cin >> x) {
		for (int i = 0; i < 128; i++) {
			cout << g_query[x][i] << " ";
		}
		cout << endl;
	}*/

}