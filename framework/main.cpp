#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "sha256.h"

using namespace std;

bool isInit = false;
bool isRunning = true;
bool passInserted = false;

string salt(string a)
{
	string Salt = "AJFrqmcp174-!!!#$!@#";
	a.append(Salt);
	return a;
}

bool check(string x)
{
	ifstream in("encryptedPasswords.txt");
	string aux;
	while (in >> aux)
	{
		if (aux == x)
		{
			in.close();
			return true;
		}
	}
	in.close();
	return false;
}


void insertPassword()
{
	string pass;
	cout << "Insert the password : ";
	cin >> pass;
	pass = salt(pass);
	pass = sha256(pass);
	bool ok = check(pass);
	if (ok)
	{
		cout << "OK\n";
		passInserted = true;
	}
	else
	{
		cout << "DENIED\n";
	}
	system("pause");
}

void createPassword()
{
	if (isInit && passInserted)
	{
		string pass;
		cout << "Enter the new password : ";
		cin >> pass;
		pass = salt(pass);
		pass = sha256(pass);
		ofstream out("encryptedPasswords.txt", ios::app);
		out << pass << endl;
		out.close();
	}
	else if (isInit && !passInserted)
	{
		cout << "Password not inserted\n";
	}
	else if (!isInit && !passInserted)
	{
		string pass;
		cout << "First Time Insert Password\n";
		cout << "Enter the password : ";
		cin >> pass;
		pass = salt(pass);
		pass = sha256(pass);
		ofstream out("encryptedPasswords.txt", ios::app);
		out << pass << endl;
		out.close();
	}
	system("pause");
}

void printEncryptList()
{
	ifstream in("encryptedFiles.txt");
	string aux;
	int i = 1;
	while (in >> aux)
	{
		cout << i << " " << aux<<endl;
		i++;
	}
	in.close();
}

void encryptFile(string filename)
{
	string filename1 = filename + ".txt";
	ifstream in(filename1.c_str());
	
	string filename2 = filename + "1.txt";
	ofstream out(filename2.c_str());
	string line;

	while (getline(in,line))
	{
		for (size_t i = 0; i < line.size(); i++)
		{
				line[i] += 4;
		}
		out << line;
	}
	in.close();
	out.close();
	ifstream fin(filename2.c_str());
	ofstream fout(filename1.c_str());

	while (getline(fin, line))
	{
		fout << line;
	}
	fin.close();
	fout.close();

	remove(filename2.c_str());
	cout << "File crypted\n";
}

void encryptMenu()
{
	if (passInserted)
	{
		cout << "Enter the file name : ";
		string filename;
		cin >> filename;
		encryptFile(filename);
		ofstream out("encryptedFiles.txt", ios::app);
		out << filename + ".txt" << endl;
		out.close();
	}
	else
	{
		cout << "Insert the password first\n";
	}
	system("pause");
}

void decryptFile(string filename)
{
	string filename1 = filename + ".txt";
	ifstream in(filename1.c_str());

	string filename2 = filename + "1.txt";
	ofstream out(filename2.c_str());

	string line;

	in >> line;

	int space = 32;

	for (size_t i = 0; i < line.size(); i++)
	{
		if (line[i] == '$')
		{
			line.insert(i + 1, "\xA0");
			line.erase(i, 1);
		}
		else
		{
			line[i] -= 4;
		}
	}

	out << line;

	in.close();
	out.close(); 
	remove(filename1.c_str());
	rename(filename2.c_str(),filename1.c_str());

	cout << "File decrypted!\n";
}

void deleteFromList(string filename)
{
	string name = "encryptedFiles";

	string filename1 = name + ".txt";
	ifstream in(filename1.c_str());

	string filename2 = name + "1.txt";
	ofstream out(filename2.c_str());

	filename += ".txt";

	string aux;
	while (in >> aux)
	{
		if (aux != filename)
			out << aux << endl;

	}

	ifstream fin(filename2.c_str());
	ofstream fout(filename1.c_str());

	while (fin>>aux)
	{
		fout << aux << endl;
	}
	fin.close();
	fout.close();
	
	remove(filename2.c_str());

}

void decryptMenu()
{
	if (passInserted)
	{
		printEncryptList();
		cout << "Enter the file name : ";
		string filename;
		cin >> filename;
		decryptFile(filename);
		deleteFromList("filename");
	}
	else
	{
		cout << "Insert the password first\n";
	}
	system("pause");
}

void menu()
{
	system("cls");
	cout << "Menu" << endl;
	cout << "1.Create a new password\n";
	cout << "2.Insert password to access the secure content\n";
	cout << "3.Encrypt a file\n";
	cout << "4.Decrypt a file\n";
	cout << "5.Exit\n";

	int opt;
	cin >> opt;
	switch (opt)
	{
	case 1:
		createPassword();
		break;
	case 2:
		insertPassword();
		break;
	case 3:
		encryptMenu();
		break;
	case 4:
		decryptMenu();
		break;
	case 5:
		isRunning = false;
		break;
	default:
		break;
	}
}

void loop()
{
	while (isRunning)
	{
		menu();
	}
}

void init()
{
	ifstream in("encryptedPasswords.txt");
	string aux;
	in >> aux;
	if (aux.empty())
	{
		isInit = false;
	}
	else
	{
		isInit = true;
	}
}

int main()
{
	init();
	loop();

	return 0;
}