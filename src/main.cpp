#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#include<iostream>
#include"mysql.h"
#include"function.h"
using namespace std;
#pragma warning(disable:4996)

MYSQL mysql;

void test(void) {
	mysql_real_connect(&mysql, "localhost", "root", "123", "", 3306, 0, 0);
	mysql_query(&mysql, "CREATE DATABASE IF NOT EXISTS  `BOOKSHOP`;");
	MYSQL_RES* result;
	MYSQL_ROW row;
	mysql_query(&mysql, "SELECT * FROM jxgl.course;");
	result = mysql_store_result(&mysql);
	int num = mysql_num_fields(result);
	while ((row = mysql_fetch_row(result))) {
		for (int i = 0; i < num; i++) {
			if (row[i] != NULL)
				cout << row[i] << "\t";
			else
				cout << "NULL\t";
		}
		cout << endl;
	}
}
int main(void) {
	GUI();
//	test();
	getchar();
}