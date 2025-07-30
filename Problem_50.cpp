#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;
#define str string
void ShowMainMenue();

const str ClientsFileName = "Clients.txt";

struct sClient
{
	str AccountNumber;
	str PinCode;
	str Name;
	str Phone;
	double AccountBalance=0.0;
	bool MarkForDelete = false;
};

sClient ReadNewClient();
str ReadClientAccountNumber();
vector<sClient> LoadClientsDataFromFile(str FileName);
bool FindClientByAccountNumber(str AccountNumber, vector<sClient> vClients, sClient& Client);
void ShowTransactionsMenue();
void GoBackToMainMenue();
void GoBackToTransactiosMenue();


sClient ReadNewClient()
{
	sClient Client;
	
	Client.AccountNumber = ReadClientAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	while (FindClientByAccountNumber(Client.AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << Client.AccountNumber << "]already exists, Enter another Account Number ? ";
			getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin>>ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin>>ws, Client.Name);
	cout << "Enter Phone? ";
	getline(cin>>ws, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}



vector<str> SplitString(str s1, str Delim)
{
	vector<str>vString;
	size_t pos = 0;
	str sWord;

	while ((pos = s1.find(Delim)) != std::string::npos)
	{
		sWord = s1.substr(0, pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}

		s1.erase(0, pos + Delim.length());
	}

	if (s1 != " ")
	{
		vString.push_back(s1);
	}

	return vString;

}

str ConvertRecordToLine(sClient Client)
{
	str seperator="#//#";
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + seperator;
	stClientRecord += Client.PinCode + seperator;
	stClientRecord += Client.Name + seperator;
	stClientRecord += Client.Phone + seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

sClient ConvertLineTORecord(str line)
{
	vector<str>vClient;
	vClient = SplitString(line, "#//#");
	sClient Client;

	Client.AccountNumber = vClient[0];
	Client.PinCode = vClient[1];
	Client.Name = vClient[2];
	Client.Phone = vClient[3];
	Client.AccountBalance = stod(vClient[4]);

	return Client;
}

void PrintClient(sClient Client)
{
	cout << "\n\nThe following is the extracted client record :\n";
	cout << "-------------------------------------------------";
	cout << "\nAccount Number  : " << Client.AccountNumber;
	cout << "\nPin Code        : " << Client.PinCode;
	cout << "\nName            : " << Client.Name;
	cout << "\nPhone           : " << Client.Phone;
	cout << "\nAccount Balance : " << Client.AccountBalance;
}


vector<sClient> LoadClientsDataFromFile(str FileName)
{
	vector<sClient>vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in); // read mode

	if (MyFile.is_open())
	{
		str Line;
		sClient Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertLineTORecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}

void PrintLineRecord(sClient Client)
{
	cout << "|  " << setw(18) << left << Client.AccountNumber;
	cout << "|  " << setw(10) << left << Client.PinCode;
	cout << "|  " << setw(33) << left << Client.Name;
	cout << "|  " << setw(12) << left << Client.Phone;
	cout << "|  " << setw(12) << left << Client.AccountBalance;
}


void ShowAllClientsScreen()
{
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
	cout <<"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(19) << "Accout Number";
	cout << "| " << left << setw(11) << "Pin Code";
	cout << "| " << left << setw(34) << "Client Name";
	cout << "| " << left << setw(13) << "Phone";
	cout << "| " << left << setw(12) << "Balance";

	cout <<"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
	{
		for (sClient Client : vClients)
		{
			PrintLineRecord(Client);
			cout << endl;
		}
	}

	cout <<"\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}



bool FindClientByAccountNumber(str AccountNumber, vector < sClient > vClients,sClient &Client)
{
	 
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}


sClient ChangeClientRecord(str AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;
	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}

bool MarkClientForDeleteByAccountNumber(str AccountNumber, vector<sClient>& vClients)
{
	for (sClient& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient> SaveClientsDataToFile(str FileName, vector<sClient> vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out); //overwrite

	str DataLine;

	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				//we only write records that are not marked for delete.
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;

}

void AddDataLineToFile(str FileName, str stLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stLine << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	//system("cls");
	cout << "Do you want to add a new client? Yes/No? ";
	char AddMore = 'n';
	cin >> AddMore;
	
	if (AddMore == 'y' || AddMore == 'Y')
	{
		AddNewClient();
		cout << "\nClient Added Successfully." << endl;
	}
}

bool DeleteClientByAccountNumber(str AccountNumber, vector<sClient>& vClients)
{
	sClient Client;
	char Answer;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClient(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;

		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);

			//Refresh Clients
			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

bool UpdateClientByAccountNumber(str AccountNumber, vector<sClient> vClients)
{
	sClient Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClients,Client))
	{
		PrintClient(Client);
		cout << "\n\nAre you sure you want update this client? y/n?";
		cin >> Answer;

		if (Answer == 'Y' || Answer == 'y')
		{
			for (sClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Update Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber<< ") is Not Found!";
		return false;
	}
}

str ReadClientAccountNumber()
{
	str AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}

void ShowAddNewClientsScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";
	AddNewClients();
}

void ShowDeleteClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	str AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);
}

void ShowUpdateClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";
	
	vector <sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	vector <sClient> vClients =LoadClientsDataFromFile(ClientsFileName);
	sClient Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients,Client))
	{
		PrintClient(Client);
	}
	else
	{
		cout << "\nClient with Account Number[" << AccountNumber<< "] is not found!";
	}
}



void ShowEndScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

void ShowDepositScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";

	sClient Client;
	str AccountNumber = ReadClientAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClient(Client);
		double nDeposit;
		cout << "\n\nPlease enter deposit amount ? ";
		cin >> nDeposit;
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += nDeposit;
				char Anwser = 'n';
				cout << "\n\nAre you sure you want prefrom this transaction? y/n? ";
				cin >> Anwser;
				if (Anwser == 'Y' || Anwser == 'y')
				{
					SaveClientsDataToFile(ClientsFileName, vClients);
					vClients = LoadClientsDataFromFile(ClientsFileName);
					for (sClient& updatedClient : vClients)
					{
						if (updatedClient.AccountNumber == AccountNumber)
						{
							PrintClient(updatedClient);  
						}
					}
		    	}
				GoBackToTransactiosMenue();
			}
		}
	}
	cout << "\nClient with[" << AccountNumber << "] does not exist.\n";
}

void ShowWithdrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	sClient Client;
	str AccountNumber = ReadClientAccountNumber();
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClient(Client);
		double nWithdraw;
		cout << "\n\nPlease enter withdraw amount ? ";
		cin >> nWithdraw;
		for (sClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				
				char Anwser = 'n';
				cout << "\n\nAre you sure you want prefrom this transaction? y/n? ";
				cin >> Anwser;
				if (Anwser == 'Y' || Anwser == 'y')
				{
					if (nWithdraw <= C.AccountBalance)
					{
						C.AccountBalance += nWithdraw * -1;
						if (nWithdraw <= C.AccountBalance)
						{
							SaveClientsDataToFile(ClientsFileName, vClients);
							vClients = LoadClientsDataFromFile(ClientsFileName);
							for (sClient& updatedClient : vClients)
							{
								if (updatedClient.AccountNumber == AccountNumber)
								{
									PrintClient(updatedClient);
								}
							}
						}
					}
					else
					{
						cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << C.AccountBalance << endl;
					}
				}
				GoBackToTransactiosMenue();
			}
		}
	}
	cout << "\nClient with[" << AccountNumber << "] does not exist.\n";
}

void ShowTotalBalances()
{
	sClient Client;
	vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
	double tBalances = 0.0;
	for (sClient C : vClients)
	{
		tBalances += C.AccountBalance;
	}
	cout << right << setw(175) << "Balances List ("<<vClients.size() << ") Client(s)" << endl;
	cout << "------------------------------------------------------------------------------------------------------------------------\n";
	cout << "| " << left << setw(25) << "Accout Number";
	cout << "| " << left << setw(50) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n------------------------------------------------------------------------------------------------------------------------\n";

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
	{
		for (sClient Client : vClients)
		{
			cout << "|  " << setw(24) << left << Client.AccountNumber;
			cout << "|  " << setw(49) << left << Client.Name;
			cout << "|  " << setw(12) << left << Client.AccountBalance;
			cout << endl;
		}
	}

	cout << "\n------------------------------------------------------------------------------------------------------------------------\n\n";
	cout << right << setw(202) << "Total Balances : " << tBalances << endl;

}

enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2,
	eDeleteClient = 3, eUpdateClient = 4,
	eFindClient = 5, eTranszctions = 6,
	eExit = 7
};

enum enTransactionsOptions
{
	eDeposit = 1, eWithdraw = 2,
	eTotalBalances = 3, eMainMenume = 4

};

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back Main Menue....";
	system("pause>0");
	ShowMainMenue();
}

void GoBackToMainMenueFromTransactionsMenue()
{
	ShowMainMenue();
}

void GoBackToTransactiosMenue()
{
	cout << "\n\nPress any key to go back Transactions Menue....";
	system("pause>0");
	ShowTransactionsMenue();
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 7]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

short ReadTransactionOption()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;
	return Choice;
}

void PrefromMainMenueOption(enMainMenueOptions MainMenueOption)
{
	switch (MainMenueOption)
	{
	case enMainMenueOptions::eListClients:
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eAddNewClient:
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eDeleteClient:
		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eUpdateClient:
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eFindClient:
		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eTranszctions:
		system("cls");
		ShowTransactionsMenue();
		GoBackToMainMenue();
		break;
	case enMainMenueOptions::eExit:
		system("cls");
		ShowEndScreen();
		break;
	}
}

void PrefromTransactionsMenueOption(enTransactionsOptions TransactionsOptions)
{
	
	switch (TransactionsOptions)
	{
	case enTransactionsOptions::eDeposit:
		system("cls");
		ShowDepositScreen();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsOptions::eWithdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsOptions::eTotalBalances:
		system("cls");
		ShowTotalBalances();
		GoBackToTransactiosMenue();
		break;
	case enTransactionsOptions::eMainMenume:
		system("cls");
		GoBackToMainMenueFromTransactionsMenue();
		break;
	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transzctions.\n";
	cout << "\t[7] Exit.\n";
	cout << "===========================================\n";

	PrefromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

void ShowTransactionsMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";
	PrefromTransactionsMenueOption((enTransactionsOptions)ReadTransactionOption());
}


int main()
{
	ShowMainMenue();
	system("pause>0");
	return 0;
}



/*str ReverseWordsInString(str s1)
{
	vector<str>vString;
	str s2 = "";

	vString = SplitString(s1, " ");

	vector<str>::iterator iter = vString.end();

	while (iter != vString.begin())
	{
		--iter;
		s2 += *iter + " ";
	}

	return s2;
}*/

/*str ReplaceWord(str s1, str word,str ReWord)
{
	vector<str>vString;
	vString = SplitString(s1," ");
	transform(word.begin(), word.end(), word.begin(), ::toupper);
	str s2 = "";

	for (int i = 0; i < vString.size(); i++)
	{
		str temp = vString[i];
		transform(vString[i].begin(), vString[i].end(), vString[i].begin(), ::toupper);
		if (vString[i] == word)
		{
			s2 += ReWord + " ";
		}
		else
		{
			s2 += temp + " ";
		}
		
	}

	return s2;

}*/

/*str RemovePunctuationFromString(str s)
{
	str s2 = "";
	for (int i = 0; i < s.size(); i++)
	{
		if (!ispunct(s[i]))
		{
			s2 += s[i];
		}
	}
	return s2;
}*/