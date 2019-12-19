#ifndef FUNCTION_H
#define FUNCTION_H
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#include<iostream>
#include"mysql.h"
using namespace std;
extern MYSQL mysql;
void GUI(void);
void purchase_book(void);
void return_book(void);
void sell_book(void);
void request_book(void);
void request_book_all(void);
void request_book_byname(void);
void show_in(void);
void show_return(void);
void show_sell(void);
void show_statistical(void);
#endif