#include"function.h"
#include<queue>
#include<time.h>
#include<string>
#include<stdlib.h>
void GUI(void) {
	//��ʼ��mysql
	mysql_init(&mysql);
	//�������ݿ�
	if (mysql_real_connect(&mysql, "localhost", "root", "123", "", 3306, 0, 0)) {
		mysql_query(&mysql, "SET NAMES GB2312");
		mysql_query(&mysql, "CREATE DATABASE IF NOT EXISTS  `BOOKSHOP`;");
		mysql_query(&mysql, "use BOOKSHOP;");
		mysql_query(&mysql, "CREATE TABLE table_store (bno char(8),bco int, bna char(10), bda char(10),bnu int) ENGINE = InnoDB;");
		mysql_query(&mysql, "CREATE VIEW user_view AS SELECT table_store.bno, table_store.bco, table_store.bna, table_store.bda test FROM table_store;");
		mysql_query(&mysql, "CREATE INDEX book_name ON bookshop.table_store(bna);");
		while (1) {
			//������ʾ��
			cout << "=======================================================" << endl;
			cout << "|                                                     |" << endl;
			cout << "|      ��ѡ����Ҫ�Ĺ��ܣ�                             |" << endl;
			cout << "|                                                     |" << endl;
			cout << "|                0 -- �˳�                            |" << endl;
			cout << "|                1 -- ����                            |" << endl;
			cout << "|                2 -- �˻�                            |" << endl;
			cout << "|                3 -- ��ʾ����                        |" << endl;
			cout << "|                4 -- ����                            |" << endl;
			cout << "|                5 -- ��ѯ��Ŀ                        |" << endl;
			cout << "|                6 -- ��ʾ���������                  |" << endl;
			cout << "|                7 -- ��ʾ�˻������                  |" << endl;
			cout << "|                8 -- ��ʾ�������                    |" << endl;
			cout << "|                9 -- ��ʾͳ�����                    |" << endl;
			cout << "|                                                     |" << endl;
			cout << "=======================================================" << endl;
			char choose;
			cin >> choose;
			system("cls");
			switch (choose)
			{
			case '0': //�˳�
				break;
			case '1': //����
				purchase_book();
				break;
			case '2': //�˻�
				return_book();
				break;
			case '3': //ͳ��
				request_book_all();
				break;
			case '4': //����
				sell_book();
				break;
			case '5': //ͨ��������ѯ�鱾
				request_book_byname();
				break;
			case '6': //��ʾ���������
				show_in();
				break;
			case '7': //��ʾ�˻������
				show_return();
				break;
			case '8': //��ʾ�������
				show_sell();
				break;
			case '9': //��ʾ����ͳ��
				show_statistical();
				break;
			default: //���������ı���򱨴�
				cout << "��������ȷ���!" << endl;
				break;
			}
			if (choose == '0')
				break;
			cout << "����������أ�" << endl;
			getchar();
			getchar();
			system("cls");
		}
	}
	else
		cout << "���ݿⲻ����!" << endl;
	//���ص�mysql
	mysql_close(&mysql);
}
void purchase_book(void) {
	cout << "������Ҫ����ͼ��ı��" << endl;
	string tempbno;
	cin >> tempbno;
	if (tempbno.size() != 8) {
		cout << "��������ȷͼ����" << endl;
		return;
	}
	else {
		cout << "������Ҫ����ͼ�������" << endl;
		string temp_bna;
		cin >> temp_bna;
		cout << "������Ҫ����ͼ��ĳ�����" << endl;
		string temp_bda;
		cin >> temp_bda;
		cout << "�����̼۸�:" << endl;
		srand(time(0));
		int temp1 = rand() % 30 + 10;
		int temp2 = rand() % 30 + 10;
		int temp3 = rand() % 30 + 10;
		cout << "������" << temp1 << endl << "�ұ���" << temp2 << endl << "�����" << temp3 << endl;
		priority_queue<int, vector<int>, greater<int>>choose;
		choose.push(temp1);
		choose.push(temp2);
		choose.push(temp3);
		cout << "�����������" << endl;
		int inu;
		cin >> inu;
		cout << "����ͼ�鶨��" << endl;
		int cost;
		cin >> cost;
		mysql_query(&mysql, "drop table in_temp;");
		mysql_query(&mysql, "CREATE TABLE in_temp(bno char(8) PRIMARY KEY,inu int,ico int, bna char(10), bda char(10)) ENGINE = InnoDB;");
		mysql_query(&mysql, "CREATE TABLE table_in(iindex int AUTO_INCREMENT PRIMARY KEY,bno char(8),inu int,ico int)	AUTO_INCREMENT = 1 ENGINE = InnoDB;");
		string statement = "insert into bookshop.in_temp values('" + tempbno + "','" + to_string(inu) + "','" + to_string(choose.top()) + "','" + temp_bna + "','" + temp_bda + "');";
		mysql_query(&mysql, statement.c_str());
		
		MYSQL_RES* result;
		MYSQL_ROW row;
		statement = "SELECT * FROM bookshop.table_store as t WHERE t.bno='" + tempbno + "';";
		mysql_query(&mysql, statement.c_str());
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if (row == 0)
			statement = "insert into bookshop.table_store values('" + tempbno + "','" + to_string(cost) + "','" + temp_bna + "','" + temp_bda + "','" + to_string(inu) + "');";
		else {
			statement = "update bookshop.table_store as t SET t.bnu=t.bnu+" + to_string(inu) +",t.bco="+to_string(cost)+ " WHERE t.bno=" + tempbno + ";";
		}
		mysql_query(&mysql, statement.c_str());
		statement = "insert into bookshop.table_in(bno,inu,ico) values('" + tempbno + "','" + to_string(inu) + "','" + to_string(choose.top()) + "');";
		mysql_query(&mysql, statement.c_str());
	}
	cout << "�����ɹ���";

}
void return_book(void) {
	cout << "��������СƱ��Կ" << endl;
	string xiaopiao;
	cin >> xiaopiao;
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT * FROM bookshop.table_sell where sindex=\""+xiaopiao+"\";";
	mysql_query(&mysql, statement.c_str());
	result = mysql_store_result(&mysql);
	row = mysql_fetch_row(result);
	if (row == 0) {
		cout << "СƱ��Կ����" << endl;
		return;
	}
	else {
		int onu = 0;
		for (int i = 0; i < strlen(row[2]); i++) {
			onu *= 10;
			onu += row[2][i] - '0';
		}
		mysql_query(&mysql, "drop table return_temp;");
		mysql_query(&mysql, "CREATE TABLE return_temp(bno char(8) PRIMARY KEY,onu int) ENGINE = InnoDB;");
		mysql_query(&mysql, "CREATE TABLE table_return(oindex int AUTO_INCREMENT PRIMARY KEY,bno char(8),onu int)	AUTO_INCREMENT = 1 ENGINE = InnoDB;");
		string row1 = row[1];
		statement = "insert into bookshop.return_temp values('" + row1 + "','" + to_string(onu) + "');";
		mysql_query(&mysql, statement.c_str());

		statement = "SELECT * FROM bookshop.table_store as t WHERE t.bno='" + row1 + "';";
		mysql_query(&mysql, statement.c_str());
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		statement = "update bookshop.table_store as t SET t.bnu=t.bnu+" + to_string(onu) + " WHERE t.bno=" + row1 + ";";
		mysql_query(&mysql, statement.c_str());
		statement = "insert into bookshop.table_return(bno,onu) values('" + row1 + "','" + to_string(onu) + "');";
		mysql_query(&mysql, statement.c_str());
	}
	cout << "�˻��ɹ���";
}
void sell_book(void) {
	request_book();
	cout << "������Ҫ����ͼ��ı��" << endl;
	string tempbno;
	cin >> tempbno;
	if (tempbno.size() != 8) {
		cout << "��������ȷͼ����" << endl;
		return;
	}
	else {
		mysql_query(&mysql, "drop table sell_temp;");
		mysql_query(&mysql, "CREATE TABLE sell_temp(bno char(8),snu int) ENGINE = InnoDB;");
		mysql_query(&mysql, "CREATE TABLE table_sell(sindex int AUTO_INCREMENT PRIMARY KEY,bno char(8),snu int)	AUTO_INCREMENT = 1 ENGINE = InnoDB;");
		string statement;
		MYSQL_RES* result;
		MYSQL_ROW row;
		statement = "SELECT * FROM bookshop.table_store as t WHERE t.bno='" + tempbno + "';";
		mysql_query(&mysql, statement.c_str());
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if (row == 0) {
			cout << "û�������������" << endl;
		}
		else {
			int count = 0;
			for (int i = 0; i < strlen(row[4]); i++) {
				count *= 10;
				count += row[4][i] - '0';
			}
			cout << "�鱾�������" << count << endl;
			cout << "������������" << endl;
			int snu;
			cin >> snu;
			if (count < snu) {
				cout << "û����ô�����������" << endl;
			}
			else {
				statement = "insert into bookshop.sell_temp values('" + tempbno + "','" + to_string(snu) + "');";
				mysql_query(&mysql, statement.c_str());
				count -= snu;
				statement = "update bookshop.table_store as t SET t.bnu=" + to_string(count) + " WHERE t.bno=\"" + tempbno + "\";";
				mysql_query(&mysql, statement.c_str());
				statement = "insert into bookshop.table_sell(bno,snu) values('" + tempbno + "','" + to_string(snu) + "');";
				mysql_query(&mysql, statement.c_str());

				string xiaopiao;
				statement = "SELECT * FROM bookshop.table_sell;";
				mysql_query(&mysql, statement.c_str());
				result = mysql_store_result(&mysql);
				while ((row = mysql_fetch_row(result))) {
					xiaopiao = row[0];
				}
				cout << "����СƱ��Կ��" << xiaopiao << endl;
				cout << "��������棬ƾ���˻�" << endl;
			}
		}
	}
}
void request_book(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	mysql_query(&mysql, "SELECT * FROM bookshop.user_view;");
	cout << "ͼ����      ͼ��۸�      ͼ������      ������" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0] << "      ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		for (int i = 0; i < 14 - strlen(row[2]); i++)
			cout << " ";
		cout << row[3];
		for (int i = 0; i < 14 - strlen(row[3]); i++)
			cout << " ";
		cout << endl;
	}
}
void request_book_all(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	mysql_query(&mysql, "SELECT * FROM bookshop.table_store;");
	cout << "ͼ����      ͼ��۸�      ͼ������      ������      ����" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0] << "      ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		for (int i = 0; i < 14 - strlen(row[2]); i++)
			cout << " ";
		cout << row[3];
		for (int i = 0; i < 12 - strlen(row[3]); i++)
			cout << " ";
		cout << row[4];
		cout << endl;
	}
}
void request_book_byname(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	cout << "������Ҫ����ͼ�������" << endl;
	string book_name;
	cin >> book_name;
	string statement = "SELECT * FROM bookshop.table_store where bna=\"" + book_name + "\";";
	mysql_query(&mysql, statement.c_str());
	cout << "ͼ����      ͼ��۸�      ͼ������      ������" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0] << "      ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		for (int i = 0; i < 14 - strlen(row[2]); i++)
			cout << " ";
		cout << row[3];
		for (int i = 0; i < 14 - strlen(row[3]); i++)
			cout << " ";
		cout << endl;
	}
}
void show_in(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT * FROM bookshop.table_in;";
	mysql_query(&mysql, statement.c_str());
	cout << "ID      ͼ����      ͼ������      �۸�" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0];
		for (int i = 0; i < 8 - strlen(row[0]); i++)
			cout << " ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		for (int i = 0; i < 14 - strlen(row[2]); i++)
			cout << " ";
		cout << row[3];
		cout << endl;
	}
}
void show_return(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT * FROM bookshop.table_return;";
	mysql_query(&mysql, statement.c_str());
	cout << "ID      ͼ����      ͼ������" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0];
		for (int i = 0; i < 8 - strlen(row[0]); i++)
			cout << " ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		cout << endl;
	}
}
void show_sell(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT * FROM bookshop.table_sell;";
	mysql_query(&mysql, statement.c_str());
	cout << "ID      ͼ����      ͼ������" << endl;
	result = mysql_store_result(&mysql);
	while ((row = mysql_fetch_row(result))) {
		cout << row[0];
		for (int i = 0; i < 8 - strlen(row[0]); i++)
			cout << " ";
		cout << row[1];
		for (int i = 0; i < 14 - strlen(row[1]); i++)
			cout << " ";
		cout << row[2];
		cout << endl;
	}
}
void show_statistical(void) {
	cout << "�������а�" << endl;
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT bno,sum(snu)as book_sum FROM bookshop.table_sell GROUP BY bno ORDER BY book_sum desc;";
	mysql_query(&mysql, statement.c_str());
	result = mysql_store_result(&mysql);
	for (int i = 0; i < 3; i++) {
		row = mysql_fetch_row(result);
		if (i == 0)
			cout << "���۹ھ���  " << endl;
		if (i == 1)
			cout << "�����Ǿ���  " << endl;
		if (i == 2)
			cout << "���ۼ�����  " << endl;
		cout << "ͼ���ţ�" << row[0] << "  ������" << row[1] << endl;
		cout << endl;
	}
	statement = "SELECT table_sell.bno,table_sell.snu,table_store.bco FROM table_sell, table_store	where table_sell.bno = table_store.bno; ";
	mysql_query(&mysql, statement.c_str());
	result = mysql_store_result(&mysql);
	int totalcount = 0;
	int totalprice = 0;
	while (row = mysql_fetch_row(result)) {
		int tempcount = 0;
		for (int i = 0; i < strlen(row[1]); i++) {
			tempcount *= 10;
			tempcount += row[1][i] - '0';
		}
		totalcount += tempcount;
		int tempprice = 0;
		for (int i = 0; i < strlen(row[2]); i++) {
			tempprice *= 10;
			tempprice += row[1][i] - '0';
		}
		totalprice += tempcount * tempprice;
	}
	cout << "�����鱾������" << totalcount << endl;
	cout << "�����鱾�ܽ�" << totalprice << endl;
}