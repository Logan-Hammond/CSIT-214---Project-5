// Author	  : Logan Hammond; hamm32637@email.ccbcmd.edu
// Source file: checkRegister.cpp
// Description: Generates a list containing the transaction history of a single checking account.
// IDE        : Microsoft Visual Studio 2017	

#include <iostream> // cout().
#include <iomanip>	// setf(), precision(), setw().
#include <fstream>	// ifstream and ofstream classes.
using namespace std;

// Structure declarations.
struct transactionRecord { 
	int code;
	double amount;
};
struct transactionCounter {
	int numTransactions;
	int numCredits;
	int numDebits; 
	int numServiceCharges;
};

// Function declarations.
void displayTitle();
double getBegBal(ifstream &stream);
transactionRecord getData(ifstream &stream);
void displayBal(double);
double processCheck(double, double);
double processDeposit(double, double);
double processATM(double, double);
double processSvcChg(double);
void displayTotals(transactionCounter c);

// Global constants.
const double CHARGE = 10, ATMFEE = 2;
const string filePath = "C:\\Users\\sonof\\source\\repos\\CSIT 214 - Project 5\\checkIn.dat";	// Bad practice. Should retrieve path from config file, 
																								// rather than hard-code directory.

int main() {
	int transCode;
	double balance, transAmt;
	ifstream recordStream;		// Input stream for data file.
	ofstream outputStream;		// Output stream to store in data file.
	transactionRecord record;	// Record of transaction.
	transactionCounter counter = {0, 0, 0, 0};	// Counter for number of transactions (individually and summed).

	// Output formatting.
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);

	displayTitle();
	cout << "\n";

	// File processing.
	recordStream.open(filePath);
	if (recordStream.fail()) {
		cout << "\n\tError: Data file open failure.";
		cout << "\n\t";
		system("pause");
		exit(1);
	}
	else {
		balance = getBegBal(recordStream);
		displayBal(balance);
		
		while (!recordStream.eof()) {
			record = getData(recordStream);
			transCode = record.code;
			transAmt = record.amount;

			switch (transCode) {
			case 1:
				balance = processCheck(balance, transAmt);
				counter.numCredits++;
				break;
			case 2:
				balance = processDeposit(balance, transAmt);
				counter.numDebits++;
				break;
			case 3:
				balance = processATM(balance, transAmt);
				counter.numDebits++;
				counter.numServiceCharges++;
				break;
			}
			if (balance < 0) {
				balance = processSvcChg(balance);
				counter.numServiceCharges++;
			}
			counter.numTransactions++;
			cout << "\n";
			displayBal(balance);
		}
		cout << "\n\n";
		displayTotals(counter);
		recordStream.close();

		outputStream.open("checkOut.dat");
		if (outputStream.fail()) {
			cout << "\n\tOutput file failed to open. Output data not saved.";
		}
		else {
			outputStream << balance;
			cout << "\n\n\tFinal balance saved to checkOut.dat in working directory.";
			cout << "\n\tValue stored: " << balance;
			outputStream.close();
		}
	}
	cout << "\n\n\t";
	system("pause");
	return 0;
}

/*
	Displays title for output.
	@param none.
	@return void.
*/
void displayTitle() {
	cout << "\n\t\tCheck Register";
	cout << "\n\t      ------------------";
}

/*
	Obtain beginning balance from checkIn.dat. Display beginning balance.
	@param none.
	@return double bal Beginning balance.
*/
double getBegBal(ifstream &stream) {
	double bal = 0;
	stream >> bal;
	return bal;
}

/*
	Returns record object with attributes int code and double amount.
	@param ifstream &stream Reference to input stream object.
	@return transactionRecord record.
*/
transactionRecord getData(ifstream &stream) {
	transactionRecord record;
	stream >> record.code >> record.amount;
	return record;
}

/*
	Displays formatted output of passed double.
	@param int x Double to be formatted
	@return void.
*/
void displayBal(double x) {
	cout << "\n\tBalance     = $" << setw(10) << x;
}

/*
	Displays balance after deduction.
	@param double bal Balance to be deducted from.
	@param double amt Amount to be deducted from balance.
	@return double Balance after deduction.
*/
double processCheck(double bal, double amt) {
	cout << "\n\tCheck       = $" << setw(10) << amt;
	return (bal - amt);
}

/*
	Displays balance after deposit.
	@param double bal Balance to be added to.
	@param double amt Amount to be added.
	@return double Balance after deposit.
*/
double processDeposit(double bal, double amt) {
	cout << "\n\tDeposit     = $" << setw(10) << amt;
	return (bal + amt);
}

/*
	Displays balance before, and after deduction from ATM (inlcudes ATM fee).
	@param double bal Balance to be deducted from.
	@param double amt Amount to be deducted from balance.
	@return double Balance after deduction, and ATM fee.
*/
double processATM(double bal, double amt) {
	cout << "\n\tATM         = $" << setw(10) << amt;
	bal = bal - amt - ATMFEE;
	cout << "\n\t  ATM Fee   = $" << setw(10) << ATMFEE;
	return (bal);
}

/*
	Displays service charge, and balance after the charge is deducted.
	@param double bal Balance to be deducted from.
	@return double bal Balance after deduction.
*/
double processSvcChg(double bal) {
	cout << "\n\tService Chg = $" << setw(10) << CHARGE;
	bal = bal - CHARGE;
	return (bal);
}

/*
	Displays number of credit, and debit transactions (summed and individual).
	@param counter c Counter structure.
	@return none.
*/
void displayTotals(transactionCounter c) {
	cout << "\n\t\t Totals";
	cout << "\n\t\t--------";
	cout << "\n\tTransactions: " << setw(5) << c.numTransactions << "\n\t  Credits   : " << setw(5)
		<< c.numCredits << "\n\t  Debits    : " << setw(5) << c.numDebits << "\n\tSrvc Charges: " << setw(5) << c.numServiceCharges;
}