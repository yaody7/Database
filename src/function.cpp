#include"function.h"
#include<queue>
#include<time.h>
#include<string>
#include<stdlib.h>
void GUI(void) {
	//初始化mysql
	mysql_init(&mysql);
	//连接数据库
	if (mysql_real_connect(&mysql, "localhost", "root", "123", "", 3306, 0, 0)) {
		mysql_query(&mysql, "SET NAMES GB2312");
		mysql_query(&mysql, "CREATE DATABASE IF NOT EXISTS  `BOOKSHOP`;");
		mysql_query(&mysql, "use BOOKSHOP;");
		mysql_query(&mysql, "CREATE TABLE table_store (bno char(8),bco int, bna char(10), bda char(10),bnu int) ENGINE = InnoDB;");
		mysql_query(&mysql, "CREATE VIEW user_view AS SELECT table_store.bno, table_store.bco, table_store.bna, table_store.bda test FROM table_store;");
		mysql_query(&mysql, "CREATE INDEX book_name ON bookshop.table_store(bna);");
		while (1) {
			//功能提示符
			cout << "=======================================================" << endl;
			cout << "|                                                     |" << endl;
			cout << "|      请选择需要的功能：                             |" << endl;
			cout << "|                                                     |" << endl;
			cout << "|                0 -- 退出                            |" << endl;
			cout << "|                1 -- 进货                            |" << endl;
			cout << "|                2 -- 退货                            |" << endl;
			cout << "|                3 -- 显示藏书                        |" << endl;
			cout << "|                4 -- 销售                            |" << endl;
			cout << "|                5 -- 查询书目                        |" << endl;
			cout << "|                6 -- 显示进货库情况                  |" << endl;
			cout << "|                7 -- 显示退货库情况                  |" << endl;
			cout << "|                8 -- 显示销售情况                    |" << endl;
			cout << "|                9 -- 显示统计情况                    |" << endl;
			cout << "|                                                     |" << endl;
			cout << "=======================================================" << endl;
			char choose;
			cin >> choose;
			system("cls");
			switch (choose)
			{
			case '0': //退出
				break;
			case '1': //进货
				purchase_book();
				break;
			case '2': //退货
				return_book();
				break;
			case '3': //统计
				request_book_all();
				break;
			case '4': //销售
				sell_book();
				break;
			case '5': //通过书名查询书本
				request_book_byname();
				break;
			case '6': //显示进货库情况
				show_in();
				break;
			case '7': //显示退货库情况
				show_return();
				break;
			case '8': //显示销售情况
				show_sell();
				break;
			case '9': //显示销售统计
				show_statistical();
				break;
			default: //输入其他的编号则报错
				cout << "请输入正确编号!" << endl;
				break;
			}
			if (choose == '0')
				break;
			cout << "按任意键返回！" << endl;
			getchar();
			getchar();
			system("cls");
		}
	}
	else
		cout << "数据库不存在!" << endl;
	//将关掉mysql
	mysql_close(&mysql);
}
void purchase_book(void) {
	cout << "输入需要购买图书的编号" << endl;
	string tempbno;
	cin >> tempbno;
	if (tempbno.size() != 8) {
		cout << "请输入正确图书编号" << endl;
		return;
	}
	else {
		cout << "输入需要购买图书的名字" << endl;
		string temp_bna;
		cin >> temp_bna;
		cout << "输入需要购买图书的出版社" << endl;
		string temp_bda;
		cin >> temp_bda;
		cout << "经销商价格:" << endl;
		srand(time(0));
		int temp1 = rand() % 30 + 10;
		int temp2 = rand() % 30 + 10;
		int temp3 = rand() % 30 + 10;
		cout << "晶东：" << temp1 << endl << "桃宝：" << temp2 << endl << "亚马孙：" << temp3 << endl;
		priority_queue<int, vector<int>, greater<int>>choose;
		choose.push(temp1);
		choose.push(temp2);
		choose.push(temp3);
		cout << "输入进货数量" << endl;
		int inu;
		cin >> inu;
		cout << "输入图书定价" << endl;
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
	cout << "进货成功！";

}
void return_book(void) {
	cout << "输入销售小票秘钥" << endl;
	string xiaopiao;
	cin >> xiaopiao;
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT * FROM bookshop.table_sell where sindex=\""+xiaopiao+"\";";
	mysql_query(&mysql, statement.c_str());
	result = mysql_store_result(&mysql);
	row = mysql_fetch_row(result);
	if (row == 0) {
		cout << "小票秘钥有误" << endl;
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
	cout << "退货成功！";
}
void sell_book(void) {
	request_book();
	cout << "输入需要销售图书的编号" << endl;
	string tempbno;
	cin >> tempbno;
	if (tempbno.size() != 8) {
		cout << "请输入正确图书编号" << endl;
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
			cout << "没有这种书可以卖" << endl;
		}
		else {
			int count = 0;
			for (int i = 0; i < strlen(row[4]); i++) {
				count *= 10;
				count += row[4][i] - '0';
			}
			cout << "书本库存量：" << count << endl;
			cout << "输入销售数量" << endl;
			int snu;
			cin >> snu;
			if (count < snu) {
				cout << "没有那么多书可以卖了" << endl;
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
				cout << "销售小票秘钥：" << xiaopiao << endl;
				cout << "请谨慎保存，凭此退货" << endl;
			}
		}
	}
}
void request_book(void) {
	MYSQL_RES* result;
	MYSQL_ROW row;
	mysql_query(&mysql, "SELECT * FROM bookshop.user_view;");
	cout << "图书编号      图书价格      图书名称      出版社" << endl;
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
	cout << "图书编号      图书价格      图书名称      出版社      数量" << endl;
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
	cout << "输入需要查找图书的名字" << endl;
	string book_name;
	cin >> book_name;
	string statement = "SELECT * FROM bookshop.table_store where bna=\"" + book_name + "\";";
	mysql_query(&mysql, statement.c_str());
	cout << "图书编号      图书价格      图书名称      出版社" << endl;
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
	cout << "ID      图书编号      图书数量      价格" << endl;
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
	cout << "ID      图书编号      图书数量" << endl;
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
	cout << "ID      图书编号      图书数量" << endl;
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
	cout << "销售排行榜：" << endl;
	MYSQL_RES* result;
	MYSQL_ROW row;
	string statement = "SELECT bno,sum(snu)as book_sum FROM bookshop.table_sell GROUP BY bno ORDER BY book_sum desc;";
	mysql_query(&mysql, statement.c_str());
	result = mysql_store_result(&mysql);
	for (int i = 0; i < 3; i++) {
		row = mysql_fetch_row(result);
		if (i == 0)
			cout << "销售冠军：  " << endl;
		if (i == 1)
			cout << "销售亚军：  " << endl;
		if (i == 2)
			cout << "销售季军：  " << endl;
		cout << "图书编号：" << row[0] << "  销量：" << row[1] << endl;
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
	cout << "销售书本总数：" << totalcount << endl;
	cout << "销售书本总金额：" << totalprice << endl;
}