#include <iostream>
#include<string>
#include<vector>
#include<limits>
#include<fstream>  //文件读取流，用与文件的写入写出，继承自<iostream>
#include<sstream>  //字符串流，用于拆分字段

struct Transaction {
	int id;
	double amount;
	int type;
	std::string category;
	std::string date;
	std::string note;
};

void saveTransactions(const std::vector<Transaction>& transactions);     //保存交易函数
void loadTransactions(std::vector<Transaction>& transactions);  //读取函数：恢复历史交易

void saveTransactions(const std::vector<Transaction>& transactions) {
	//1.使用ofstream打开transactions.txt
	std::ofstream outFile("transactions.txt");      //`ofstream` 是类，`outFile` 是这个类创建出来的一个实例对象。

	if (!outFile.is_open()) {
		std::cout << "错误！无法打开文件进行保存！\n";
		return;
	}
	for (const auto& tx : transactions) {
		//将六个字段写入文件
		outFile << tx.id << "|" << tx.amount << "|" << tx.type << "|" << tx.category << "|" << tx.date << "|" << tx.note << "\n";
	}
	if (!outFile) {
		std::cout << "写入文件发生错误！\n";
	}
	outFile.close();
}

void loadTransactions(std::vector<Transaction>& transactions) {
	
	std::ifstream inFile("transactions.txt");

	//如果文件不存在或大不开，直接返回
	if (!inFile.is_open()) {
		std::cout << "未找到文件，文件打开失败!\n";
		return;
	}
	std::string line;
	
	transactions.clear();   //确保文件打开后，再清空容器

	//按行读取
	while (std::getline(inFile, line)) {
		if (line.empty()) {
			continue;  //跳过空行
		}

		std::stringstream ss(line);
		std::string filed;
		Transaction tx;

		try {
			if(!std::getline(ss, filed, '|')) throw std::runtime_error("字段缺失");   //如果filed是"abc"，这里会抛出异常并跳转到catch
			tx.id = std::stoi(filed);         //拆分后是字符串，转换为整数

			if(!std::getline(ss, filed, '|')) throw std::runtime_error("字段缺失");
			tx.amount = std::stod(filed);         //转换为double

			if(!std::getline(ss, filed, '|')) throw std::runtime_error("字段缺失");
			tx.type = std::stoi(filed);         //转换为int

			if(!std::getline(ss, tx.category, '|')) throw std::runtime_error("字段缺失");

			if(!std::getline(ss, tx.date, '|')) throw std::runtime_error("字段缺失");


			std::getline(ss, tx.note);  //最后一个字段不用检查'|'，直接走到行尾

			transactions.push_back(tx);
		}
		catch (const std::exception& e) {
			//如果上面任何一步抛出了异常，就会跳到这里，且这一行数据不会被添加，程序也不会崩溃
			std::cout << "警告：发现损坏的数据行，已跳过。" << line<<") 错误原因："<<e.what() << "\n";
		}

	}
	inFile.close();   //关闭文件
}

int main()
{
	std::vector<Transaction> T;
	loadTransactions(T);

	int choice;
	do {
		//1.打印菜单
		std::cout << "============ 个人财务系统 ============\n";
		std::cout << "1.添加交易：\n";
		std::cout << "2.查看交易：\n";
		std::cout << "3.删除交易：\n";
		std::cout << "4.查看统计：\n";
		std::cout << "0.退出查询\n";
		std::cout << "请输入你的选择：";

		//2.接收用户输入
		std::cin >> choice;

		//3.根据输入执行相应操作
		switch (choice) {
		case 1: {
			Transaction newTx;

			std::cout << "请输入交易编号：";
			std::cin >> newTx.id;

			std::cout << "请输入交易金额：";
			std::cin >> newTx.amount;

			std::cout << "请输入交易类型（1=收入，2=支出）：";
			std::cin >> newTx.type;

			while (newTx.type != 1 && newTx.type != 2) {
				std::cout << "输入错啦，类型只能是1或2哦^-^，请重新输入：";

				if (std::cin.fail()) {
					std::cin.clear(); //清除错误标志，如用户输入字母时，输入非法，会触发错误标志
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cin >> newTx.type;

				}
			}

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');   //清空缓冲区

			std::cout << "请输入交易类别：";
			getline(std::cin, newTx.category);

			std::cout << "请输入交易日期：";
			getline(std::cin, newTx.date);

			std::cout << "请输入备注：";
			getline(std::cin, newTx.note);

			//T.clear();   //用于清空之前的数据，要使用的话就去掉注释
			T.push_back(newTx);
			saveTransactions(T); //调用保存函数

			std::cout << "添加交易成功！\n";
			break;
		}
		case 2: {
			int viewChoice;
			std::cout << "请选择查看方式：（1=查看所有交易，2=按照编号查找交易）";
			std::cin >> viewChoice;
			std::cout<<std::endl;
			if (viewChoice == 1) {
				if (T.empty()) {
					std::cout << "当前没有任何交易。\n";
				}
				else {
					for (const auto& i : T) {                                  //for (int i = 0;i < T.size();i++)
						std::cout << "===========交易详情==========\n";
						std::cout << "交易编号：" << i.id << std::endl;
						std::cout << "交易金额：" << i.amount << std::endl;
						if (i.type == 1) {
							std::cout << "交易类型：收入" << std::endl;
						}
						else if (i.type == 2) {
							std::cout << "交易类型：支出" << std::endl;
						}
						else {
							std::cout << "未知类型!" << std::endl;
						}
						std::cout << "交易分类：" << i.category << std::endl;
						std::cout << "交易时间：" << i.date << std::endl;
						std::cout << "备注信息：" << i.note << std::endl;
						std::cout << "=============================" << std::endl;
						std::cout << std::endl;
					}
				}
			}
			else if (viewChoice == 2) {
				int searchId;
				std::cout << "请输入要查找交易的编号：";
				std::cin >> searchId;
				std::cout << std::endl;

				bool found = false;
				for (const auto& i : T) {                                  //for (int i = 0;i < T.size();i++)
					if (i.id == searchId) {
						std::cout << "===========交易详情==========\n";
						std::cout << "交易编号：" << i.id << std::endl;
						std::cout << "交易金额：" << i.amount << std::endl;
						if (i.type == 1) {
							std::cout << "交易类型：收入" << std::endl;
						}
						else if (i.type == 2) {
							std::cout << "交易类型：支出" << std::endl;
						}
						else {
							std::cout << "未知类型!" << std::endl;
						}
						std::cout << "交易分类：" << i.category << std::endl;
						std::cout << "交易时间：" << i.date << std::endl;
						std::cout << "备注信息：" << i.note << std::endl;
						std::cout << "=============================" << std::endl;
						std::cout << std::endl;
						found = true;
						break;   //因为编号的唯一性，输出完目标标号直接退出循环，提高效率
					}
					
				}
				if (!found) {
					std::cout << "\n未找到编号！\n";
				}
			}
			else {
				std::cout << "请选择1或2！\n";
			}
			break;
		}
		case 3: {
			if (T.empty()) {
				std::cout << "当前没有任何交易。\n";
				break;
			}
			int deleteId;
			bool found = false;
			std::cout << "请输入要删除的交易编号：";
			std::cin >> deleteId;
			for (auto it = T.begin();it != T.end();) {
				if (it->id == deleteId) {
					it=T.erase(it);
					std::cout << "删除交易成功！\n";
					found = true;
				}
				else {
					it++;
				}
			}
			if (!found) {
				std::cout << "未找到交易，无法删除！\n";
			}
			if (found) {
				saveTransactions(T);
			}
			break;
		}
		case 4: {
			if (T.empty()) {
				std::cout << "当前没有任何交易。\n";
				break;
			}
			double totalIncome = 0.0;
			double totalExpense = 0.0;
			for (const auto& x : T) {
				if (x.type == 1) {
					totalIncome += x.amount;
				}
				else {
					totalExpense += x.amount;
				}
			}
			std::cout << std::endl;
			std::cout << "==========统计交易==========\n";
			std::cout << "总收入：" << totalIncome << std::endl;
			std::cout << "总支出：" << totalExpense<< std::endl;
			std::cout << "当前结余：" << totalIncome-totalExpense << std::endl;
			
			break;
		}
		case 0:
			//输入0时，退出
			std::cout << "退出程序，再见！\n";
			break;
		default:
			//输入其他不符的选项时
			std::cout << "无效的选择，请重试！\n";
			break;
		}
		std::cout << "\n";
	} while (choice != 0);

}
