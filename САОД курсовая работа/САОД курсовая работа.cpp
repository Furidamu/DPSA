#include "pch.h"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <clocale>
#include <string>
#include <cmath>
#include <windows.h>

#pragma warning(disable : 4996);

using namespace std;

struct Databank {//Структура БД
	char author[12];
	char title[32];
	char publisher[16];
	short int year;
	short int num_of_page;
};

struct list {
	list* next;
	Databank *copydata;
};


struct node {
	Databank* data;
	node* left;
	node* right;
	node* next;
};

struct weightstruct {
	Databank* data;
	int weight;
};

struct codeFano {

	unsigned char symbol;
	double frequency;
	double lenght;
	
};

codeFano A[256];
double mediumlong;
std::string codes[256];

weightstruct* createWeights(list* head, int &size);
void sortWeights(weightstruct*& arr, int size);
node* createTree(weightstruct* arr, int size);
node* addInTree(node* root, weightstruct d);
void treeTraversal(node* root);
void quick_sort(struct Databank *elements[], int left, int right);
void quick_sort_struct(struct Databank *elements[], int count);
void addQueue(list*&, Databank*);
list* binarysearch(Databank **, short int);
void TreeSearch(node* root, int key);
double Medium(codeFano*& symbolData, int Left, int Right);
void Fano(codeFano*& symbolData, int Left, int Right);
codeFano* readFile();
void sortFano(codeFano*& symbolData);

weightstruct* createWeights(list* head, int& size) {//Создание массива данных с весом
	list* now = head;
	size = 0;
	while (now != nullptr) {
		size += 1;
		now = now->next;
	}

	now = head;
	weightstruct* arr = new weightstruct[size];
	int i = 0;
	while (now != nullptr) {
		arr[i].data = now->copydata;
		arr[i].weight = rand() % size * 3 + 10;
		i += 1;
		now = now->next;
	}
	return arr;
}

void sortWeights(weightstruct*& arr, int size) {//Сортировка весов вершин
	for (int i = 0; i < size - 1; i++) {
		for (int j = size - 1; j > i; j--) {
			if (arr[j].weight > arr[j - 1].weight) {
				weightstruct temp = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = temp;
			}
		}
	}
}

node* createTree(weightstruct* arr, int size) {//Создание дерева
	node* root = nullptr;
	sortWeights(arr, size);
	for (int i = 0; i < size; i++) {
		root = addInTree(root, arr[i]);
	}

	return root;
}

node* addInTree(node* root, weightstruct d) {//Добавление в дерево
	if (root == nullptr) {
		root = new node;
		if (root == nullptr) {
			exit(1);
		}
		root->data = d.data;
		root->left = root->right = nullptr;
		root->next = NULL;
	}
	else
	if (d.data->num_of_page == root->data->num_of_page) {
		root->next = addInTree(root->next, d);
	} else if (d.data->num_of_page < root->data->num_of_page) {
		root->left = addInTree(root->left, d);
	} else if (d.data->num_of_page > root->data->num_of_page) {
		root->right = addInTree(root->right, d);
	}
	return root;
}

void TreeSearch(node* root, int key) {
	static int count = 0;
	if (root != NULL)
	{
		if (key < root->data->num_of_page) {
			TreeSearch(root->left, key);
		}
		else {
			if (key > root->data->num_of_page) {
				TreeSearch(root->right, key);
			}
			else {
				cout
					<< ++count << "\t"
					<< root->data->author << "\t"
					<< root->data->title << "\t"
					<< root->data->publisher << "\t"
					<< root->data->year << "\t"
					<< root->data->num_of_page
					<< endl
					;
				TreeSearch(root->next, key);
			}
		}
	}
}

void treeTraversal(node* root) {//Вывод дерева
	static int count = 0;
	if (root != nullptr) {
		treeTraversal(root->left);
		cout
			<< ++count<<"\t"
			<<	root->data->author <<"\t"
			<< root->data->title <<"\t"
			<< root->data->publisher<< "\t"
			<< root->data->year<<"\t"
			<< root->data->num_of_page
			<< endl
		;
		treeTraversal(root->next);
		treeTraversal(root->right);
	}

}

void addQueue(list*& head, Databank* data) {//Добавление в очередь
	if (head == nullptr) {
		head = new list;
		head->copydata = data;
		head->next = nullptr;
		return;
	}
	list* now = head;
	while (now->next != nullptr) {
		now = now->next;
	}

	now->next = new list;
	now->next->copydata = data;
	now->next->next = nullptr;
}

void quick_sort_struct(struct Databank *elements[], int count)//Быстрая сортировка
{
	quick_sort(elements, 0, count - 1);
}

void quick_sort(struct Databank *elements[], int left, int right)
{
	int i, j;
	int y;
	char *x;
	Databank* temp;

	i = left;
	j = right;
	y = (elements[(left + right) / 2]->year);
	x = (elements[(left + right) / 2]->author);

	while (i < j) {

		while ((elements[i]->year < y) && (i < right)) {//Сортировка по полю ГОД
			i++;
		}
		while ((elements[j]->year > y) && (j > left)) {
			j--;
		}

		if (elements[i]->year == elements[j]->year) {
			while ((strcmp(elements[i]->author, x) < 0) && (i < right)) {//Сортировка по полю АВТОР
				i++;
			}
		}
		
		if (elements[i]->year == elements[j]->year) {
			while ((strcmp(elements[j]->author, x) > 0) && (j > left)) {
				j--;
			}
		}

		if (i <= j) {
			temp = elements[i];
			elements[i] = elements[j];
			elements[j] = temp;
			i++;
			j--;
		}

	}
	if (left < j) quick_sort(elements, left, j);
	if (i < right) quick_sort(elements, i, right);
}

list* binarysearch(Databank **elements, short int key) {
	int left = 0;
	int right = 4000 - 1;
	int middle = 0;
	list* queue = nullptr;

	while (left < right) {
		middle = (left + right) / 2;
		if (elements[middle]->year < key) {
			left = middle + 1;
		}
		else {
			right = middle;
		}
	}

	int i = 1;
	while (elements[right]->year == key) {
		addQueue(queue, elements[right]);
		cout << i << "\t" << elements[right]->author << "\t" << elements[right]->title << elements[right]->publisher << "\t" << elements[right]->year << "\t" << elements[right]->num_of_page << "\n";
		right += 1;
		i += 1;
	}
	return queue;
}

void Menu(int &turn) {//Текстовое меню
	cout << "1. The output records are consistently\n";
	cout << "2. Output 20 records\n";
	cout << "3. Sorted database\n";
	cout << "4. Sorted 20 records\n";
	cout << "5. Binary search\n";
	cout << "6. Creat tree\n";
	cout << "7. Fano code\n";
	scanf_s("%d", &turn);
}

int Check_open(fstream &Database) {//Функция проверки на считывание БД

	Database.open("testBase1.dat", ios::in | ios::binary);
	if (!Database.is_open()) {
		return 0;
	}
	else {
		return 1;
	}
}

void print_database() {
	Databank author;
	fstream Database;
	int size = 4000;
	int index = 0, page = 20, j = 0, turn = -1, next;
	short int key = 0;
	Databank* Adress = new Databank[size];
	Databank** IndexArray = new Databank *[size];
	list* queueHead = nullptr;

	setlocale(LC_ALL, "Russian");

	if (Check_open(Database) == 0) {//Проверка на доступность файла
		cout << "No data file found";
		system("pause");
		exit(1);
	}

	while (!Database.read((char*)& author, sizeof(author)).eof() && index < size) {//Считывание базы данных в память
		Adress[index] = author;
		index++;
	}

	for (int i = 0; i < size; i++) {
		IndexArray[i] = &Adress[i];
	}

	Database.close();
	index = 0;

	while (turn != 0) {
		Menu(turn);
		switch (turn) {

		case (1): {
			/* Вывод 4000 записей*/
			for (index = 0; index < size; index++) {
				if (_kbhit()) {
					if (_getch() == 13) {
						system("Pause");
					}
				}
				printf_s("%2d) %-12s %-32s %-16s %2d %2d\n", index + 1, IndexArray[index]->author, IndexArray[index]->title, IndexArray[index]->publisher, IndexArray[index]->year, IndexArray[index]->num_of_page);
			}
			system("Pause");
			break;
		}

		case (2): {
			/*Вывод 20 записей*/
			while (index < size) {
				index += page;
				for (; j < index; j++) {
					if (_kbhit()) {
						if (_getch() == 13) {
							system("Pause");
						}
					}
					printf_s("%2d) %-12s %-32s %-16s %2d %2d\n", index + 1, IndexArray[index]->author, IndexArray[index]->title, IndexArray[index]->publisher, IndexArray[index]->year, IndexArray[index]->num_of_page);
				}
				cout << "To print the next page?\n";
				scanf_s("%d", &next);
				if (next != 1) {
					return;
				}
			}
			system("Pause");
			break;
		}

		case (3): {
			//Вывод 4000 записей уже отсортированных
			for (index = 0; index < size; index++) {
				if (_kbhit()) {
					if (_getch() == 13) {
						system("Pause");
					}
				}
				quick_sort_struct(IndexArray, size);
				printf_s("%2d) %-12s %-32s %-16s %2d %2d\n", index + 1, IndexArray[index]->author, IndexArray[index]->title, IndexArray[index]->publisher, IndexArray[index]->year, IndexArray[index]->num_of_page);
			}
			system("Pause");
			break;
		}

		case (4): {
			//Вывод 20 записей уже отсортированных
			for (index = 0; index < size; index++) {
				index += page;
				for (; j < index; j++) {
					if (_kbhit()) {
						if (_getch() == 13) {
							system("Pause");
						}
					}
					quick_sort_struct(IndexArray, size);
					printf_s("%2d) %-12s %-32s %-16s %2d %2d\n", index + 1, IndexArray[index]->author, IndexArray[index]->title, IndexArray[index]->publisher, IndexArray[index]->year, IndexArray[index]->num_of_page);
				}
				cout << "To print the next page?\n";
				scanf_s("%d", &next);
				if (next != 1) {
					return;
				}
			}
			system("Pause");
			break;
		}

		case (5): {
			//Вывод поиска по ключу
			cout << "Input key search\n";
			quick_sort_struct(IndexArray, size);
			scanf_s("%d", &key);
			queueHead = binarysearch(IndexArray, key);
			system("Pause");
			break;
		}

		case (6): {

			if (queueHead != nullptr) {
				int size = 0, s = 0;
				weightstruct* arr = createWeights(queueHead, size);
				node* root = createTree(arr, size);
				treeTraversal(root);
				cout << "Input key for search in tree\n";
				cin >> s;
				cout << "\n";
				TreeSearch(root, s);
			}
			break;
		}
		case (7): {
			codeFano* symbolData = readFile();
			sortFano(symbolData);
			Fano(symbolData, 0, 255);
			double Entropy = 0;
			for (int i = 0; i < 256; i++){
			Entropy += symbolData[i].frequency * abs(log(symbolData[i].frequency)) / log(2);
			mediumlong += symbolData[i].frequency * A[i].lenght;
			}
			cout <<"Medium lenght = " <<  mediumlong<<endl;
			cout << "Entropy = " << Entropy << endl;
		}
		}
	}
}

double Medium(codeFano*& symbolData, int Left, int Right) {//Подсчет суммы для медианы
	double summ = 0;
	for (int i = Left; i <= Right; i++)
		summ += symbolData[i].frequency;
	summ /= 2;
	double s = 0;
	int i;
	for (i = Left; s <= summ; i++) {
		s += symbolData[i].frequency;
	}

	return i;
}

void Fano(codeFano*& symbolData, int Left, int Right) {
	static int Temp = -1;
	int i =0 ;


	if (Left == Right) {
		return;
	}

	if ((Right - Left) == 1) {
		codes[Left] += "0";
		codes[Right] += "1";
		
	}

	int Med = Medium(symbolData, Left, Right);
	if (Temp == Med) {
		return;
	}

	Temp = Med;

	for (i = Left; i <= Right; i++) {
		if (i <= Med) {
			codes[i] += "0";
			A[i].lenght++;
			cout << "Code words = ["<<codes[i]<<"]\t" <<"Lenght code words = "<<A[i].lenght << endl;
			

		}
		else {
			codes[i] += "1";
			A[i].lenght++;
		
			cout << "Code words = [" << codes[i] << "]\t" << "Lenght code words = " << A[i].lenght << endl;
		}
	}
	if (Left < Right){
	Fano(symbolData, Left, Med-1);
	Fano(symbolData, Med, Right);
	}
}

void sortFano(codeFano*& symbolData) {
	codeFano Temp;
	for (int i = 0; i < 256; ++i) {
		for (int j = 255; j > i; --j) {
			if (symbolData[j - 1].frequency < symbolData[j].frequency) {
				Temp = symbolData[j];
				symbolData[j] = symbolData[j - 1];
				symbolData[j - 1] = Temp;
			}
		}
	}

}

codeFano* readFile() {
	
	FILE *file = fopen("testBase1.dat", "rb");;
	int *freq;
	int size = 256;
	double counter = 0;
	double summary = 0;
	unsigned char ch;
	SetConsoleCP(1251);
	freq = new int[size];

	for (int i = 0; i < size; i++) {
		freq[i] = 0;
	}

	while (true) {
		ch = fgetc(file);
		if (!feof(file)) {
			freq[ch] += 1;
			counter += 1;
		}
		else {
			break;
		}
	}

	fclose(file);
	double Entropy = 0;
	
	codeFano* symbolfreq = new codeFano[size];
	float s= 0;
	int count = 0;
	double lenght = 0;
	printf("Num\tSymbol\tFrequency\n");
	for (int i = 0; i < size; i++) {
		symbolfreq[i].symbol = i;
		symbolfreq[i].frequency = (double)freq[i] / counter;
		Entropy += symbolfreq[i].frequency * abs(log(symbolfreq[i].frequency)) / log(2);
		
		count++;
		printf("%d)\t", count);
		printf("%c\t", symbolfreq[i].symbol);
		printf("%4.8f\n", symbolfreq[i].frequency);
		
	}
	printf("Entropy = %f\n", Entropy);
	return symbolfreq;
}

int main()
{
	srand(time(NULL));
	print_database();
	return 0;
}
