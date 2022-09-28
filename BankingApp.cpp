/*This is a minor project (learning based project) on banking system that emulates like an online banking system. The project is developed using C++ Programming language concepts and Object Oriented Programming concepts and the objective is to learn the implementation of programming concepts in solving real world problems.*/


// necessary header files inclusion
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <ctime>
#include <cmath>
#include <regex>


using namespace std;


// some global declarations for regular expression match
const regex string_expr("[A-Za-z_]*");
const regex integer_expr("[0-9]*");
const regex alphanum("[A-Za-z0-9]*");


// forward declaration of classes to prevent undefined reference error
class Transaction;
class Account;
class UserRecordSavingAccount;
class UserRecordCurrentAccount;
class Bank;


// Date class - to handle the account creation dates or transaction dates.
class Date{
    private:
        int dd,mm,yy;
    public:
        
        // default constructor
        Date(){
            dd = 0;
            mm = 0;
            yy = 0;
        }

        // parameterized constructor
        Date(int dd, int mm, int yy){
            this->dd = dd;
            this->mm = mm;
            this->yy = yy;
        }

        // overloaded operators prototypes
        int operator - (Date& );
        
        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& , const Date& );
        friend istream& operator >> (istream& , Date& );

    // declaration of friend class
    friend class Account;
    friend class Transaction;
};

// overloaded - operator to calculate difference of dates (specially for interest calculation)
int Date :: operator-(Date& date){
    int tdd, tmm, tyy;
    tyy = yy - date.yy;
    tmm = mm - date.mm;

    tmm < 0 ? tyy -= 1 : tyy;
    tdd = dd - date.dd;

    return abs((tyy*365)-((tmm<0)?(tmm+11)*30:tmm*30)-((tdd<0)?(tdd+30):tdd));
}

// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Date& dat){
    out<<dat.dd<<" "<<dat.mm<<" "<<dat.yy;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Date& dat){
    in>>dat.dd>>dat.mm>>dat.yy;
    return in;
}

// Balance class - to deal with the account balance
class Balance{
    private:
        float balance;
    public:
        // default constructor
        Balance(){
            balance = 0;
        }

        // parameterized constructor
        Balance(float balance){
            this->balance = balance;
        }
        
        // member functions declarations
        inline void showBalance(){
            cout<<left<<setw(50)<<"Show Balance "<<left<<setw(4)<<" : "<<balance<<endl;
        }

        Balance calInterest(float, int);

        // overloaded operators prototypes
        Balance operator + (Balance& );
        Balance operator - (Balance& );
        bool operator <= (Balance& );

        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& , const Balance& );
        friend istream& operator >> (istream& , Balance& );

    friend class Account;
    friend class Transaction;
};

// overloaded + operator to sum up the balance
Balance Balance :: operator +(Balance& bal){
    
    return balance+bal.balance;
}

// overloaded - operator to calculate the balance difference
Balance Balance :: operator -(Balance& bal){
    
    return balance-bal.balance;
}

// method to calculate interest rate
Balance Balance :: calInterest(float rate, int time){
    this->balance=(this->balance*rate*time)/100;
    return *this;
}

// overloaded <= operator to compare the balances
bool Balance :: operator <=(Balance& bal){
    if(balance <= bal.balance){
        return true;
    }
    return false;
}

// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Balance& bal){
    out<<bal.balance;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Balance& bal){
    in>>bal.balance;
    return in;
}


// Transaction class - to handle the bank transactions
class Transaction{
    protected:
        int transactionId;
        float amount;
        Balance balance;
        string transactionType;
        Date date;
        string defaultpath;
    public:

        // default constructor
        Transaction(){
            transactionId = 0;
            amount        = 0.0;
        }

        // pure virtual function declaration (Transaction class - abstract class)
        virtual void storeTransaction(string, string&)=0;
        virtual void retrieveTransactions(string, string& )=0;

        // member function
        inline void showAllTransactions(){
            cout<<left<<setw(15)<<transactionId<<left<<setw(15)<<amount<<left<<setw(15)<<transactionType<<left<<setw(15)<<balance<<left<<setw(15)<<" - "<<date<<endl;
        }

};

// inherited class to deal with bank deposits (Particular Transaction)
class Deposit : public Transaction{
    public:

        // default constructor
        Deposit(){}

        // parameterized constructor
        Deposit(int transactionId, float amount, Balance balance, Date date){
            this->transactionId   = transactionId;
            this->amount          = amount;
            this->balance         = balance;
            this->transactionType = "Credit";
            this->date            = date;
        }

        // member function declaration
        void storeTransaction(string, string&);
        void retrieveTransactions(string, string&);

        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& out, const Deposit& dr);
        friend istream& operator >> (istream& in, Deposit& dr);
    
    // friend classes
    friend class SavingAccount;
    friend class CurrentAccount;

};

// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Deposit& dr){
    out<<dr.transactionId<<" "<<dr.amount<<" "<<dr.balance<<" "<<dr.transactionType<<" "<<dr.date;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Deposit& dr){
    in>>dr.transactionId>>dr.amount>>dr.balance>>dr.transactionType>>dr.date;
    return in;
}

// method to store user transactions - deposits only (for both current and savings accounts)
void Deposit :: storeTransaction(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Deposits.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<date<<endl;
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
    }
    else if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Deposits.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<date<<endl;
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
    }
}

// method to retrieve user transactions - deposits only (for both current and savings accounts)
void Deposit :: retrieveTransactions(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Deposits.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>date;
            }
        }
        else{
            cout<<left<<setw(50)<<"No Deposit Records Found!!"<<endl;
        }
    }
    else if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Deposits.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>date;
            }
        }
        else{
            cout<<left<<setw(50)<<"No Deposit Records Found !!"<<endl;
        }
    }
}


// inherited class to deal with bank withdrawls (Particular Transaction)
class Withdrawl : public Transaction{
    public:

        // default constructor
        Withdrawl(){}

        // parameterized constructor
        Withdrawl(int transactionId, float amount, Balance balance, Date date){
            this->transactionId   = transactionId;
            this->amount          = amount;
            this->balance         = balance;
            this->transactionType = "Debit";
            this->date            = date;
        }
        
        // member function declarations
        void storeTransaction(string, string& );
        void retrieveTransactions(string, string& );
        
    friend class SavingAccount;
    // overloaded operators prototypes with the help of friend function
    friend ostream& operator << (ostream& out, const Withdrawl& cr);
    friend istream& operator >> (istream& in, Withdrawl& cr);

};

// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Withdrawl& cr){
    out<<cr.transactionId<<" "<<cr.amount<<" "<<cr.balance<<" "<<cr.transactionType<<" "<<cr.date;
    out<<endl;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Withdrawl& cr){
    in>>cr.transactionId>>cr.amount>>cr.balance>>cr.transactionType>>cr.date;
    return in;
}

// method to store user transactions - withdrawls only (for both current and savings accounts)
void Withdrawl :: storeTransaction(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Withdrawls.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<date<<endl;
            fout.close();
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
    }
    else if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Withdrawls.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<date<<endl;
            fout.close();
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
    }
}

// method to retrieve user transactions - withdrawls only (for both current and savings accounts)
void Withdrawl :: retrieveTransactions(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Withdrawls.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>date;
            }
            
            fin.close();
        }
        else{
            cout<<left<<setw(50)<<"No Withdrawl Records Found !!"<<endl;
        }
    }
    else if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Withdrawls.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>date;
            }
            
            fin.close();
        }
        else{
            cout<<left<<setw(50)<<"No Withdrawl Records Found !!"<<endl;
        }
    }
}

// inherited class to deal with bank transfers (Particular Transaction)
class Transfer : public Transaction{
    private:
        long long receiverAccountNo;
    public:

        // default constructor
        Transfer(){}

        // parameterized constructor
        Transfer(int transactionId, float amount, Balance balance, long long receiverAccountNo, Date date){
            this->transactionId     = transactionId;
            this->amount            = amount;
            this->balance           = balance;
            this->transactionType   = "Debit";
            this->receiverAccountNo = receiverAccountNo;
            this->date              = date;
        }

        // member function declarations
        void storeTransaction(string, string&);
        void retrieveTransactions(string, string& );
                
        inline void showAllTransactions(){
            cout<<left<<setw(15)<<transactionId<<left<<setw(15)<<amount<<left<<setw(15)<<transactionType<<left<<setw(15)<<balance<<left<<setw(15)<<receiverAccountNo<<date<<endl;
        }

        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& out, const Transfer& tr);
        friend istream& operator >> (istream& in, Transfer& tr);
    
    friend class SavingAccount;
    friend class CurrentAccount;
    
    
};


// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Transfer& tr){
    out<<tr.transactionId<<" "<<tr.amount<<" "<<tr.balance<<" "<<tr.transactionType<<" "<<tr.receiverAccountNo<<" "<<tr.date;
    out<<endl;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Transfer& tr){
    in>>tr.transactionId>>tr.amount>>tr.balance>>tr.transactionType>>tr.receiverAccountNo>>tr.date;
    return in;
}

// method to store user transactions - transfers only for both current and savings accounts
void Transfer :: storeTransaction(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Transfers.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<receiverAccountNo<<" "<<date<<endl;
            fout.close();
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
        
    }
    if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Transfers.txt";
        ofstream fout(defaultpath, ios :: app);
        if(fout){
            fout<<transactionId<<" "<<amount<<" "<<balance<<" "<<transactionType<<" "<<receiverAccountNo<<" "<<date<<endl;
            fout.close();
        }
        else{
            cout<<left<<setw(50)<<"Unable to process right now !!"<<endl;
        }
    }
}
// method to retrieve user transactions - transfers only (for both current and savings accounts)
void Transfer :: retrieveTransactions(string accountType, string& userName){
    if(accountType == "Savings_Account"){
        defaultpath = "SavingAccounts/Transactions/" + userName + "_Transfers.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>receiverAccountNo>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>receiverAccountNo>>date;
            }
            
            fin.close();
        }
        else{
            cout<<left<<setw(50)<<"No Transfer Records Found !!"<<endl;
        }
    }
    else if(accountType == "Current_Account"){
        defaultpath = "CurrentAccounts/Transactions/" + userName + "_Transfers.txt";
        ifstream fin(defaultpath, ios :: in);
        if(fin){
            fin>>transactionId>>amount>>balance>>transactionType>>receiverAccountNo>>date;
            while(!fin.eof()){
                showAllTransactions();
                fin>>transactionId>>amount>>balance>>transactionType>>receiverAccountNo>>date;
            }
            
            fin.close();
        }
        else{
            cout<<left<<setw(50)<<"No Transfer Records Found !!"<<endl;
        }
    }
}

// Account class - to handle user account details
class Account{
    protected:
        long long accountNo;
        Balance balance;
        string ifscCode;
        string userName;
        Deposit dr;
        Withdrawl cr;
        Transfer tr; 
        Date date;
        string defaultpath;
    public:

        // default constructor
        Account(){
            accountNo  = 0;
            ifscCode   = "BANK01234";
        }
        
        // pure virtual function declaration - (Account - abstract class)
        virtual void storeAccountDetails()=0;
        virtual void retrieveAccountDetails()=0;
        virtual void showAccountDetails()=0;

};

// inherited class 'SavingsAccount' to handle user's saving account 
class SavingAccount : public Account{
    private:
        string accountType;
        static float ROI;   
        static Balance minBalance; 
        static int transactionLimit; 
    public:
        // default constructor
        SavingAccount(){}

        // parameterized constructor
        SavingAccount(string& userName){
            this->userName = userName;
        }

        // parameterized constructor 
        SavingAccount(long long accountNo, Balance balance, string userName, Date date){
            this->accountNo   = accountNo;
            this->balance     = balance;
            this->userName    = userName;
            this->date        = date;
            this->accountType = "Savings_Account";
        }

        // member function declarations
        SavingAccount& updateBalanceDeposit(int, float);
        SavingAccount& updateBalanceWithdrawn(int, float);
        SavingAccount& updateBalanceTransfer(int, float, long long);
        
        void accountTransactions();
        void showAccountDetails();
        Balance interestEarned();

        void storeAccountDetails();
        void retrieveAccountDetails();

        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& out, const SavingAccount& acc);
        friend istream& operator >> (istream& in, SavingAccount& acc);

    // friend class
    friend class UserRecordSavingAccount;    
    
};

// static member definition
float SavingAccount :: ROI = 8.5;
Balance SavingAccount :: minBalance = 500.0; 
int SavingAccount :: transactionLimit = 5;

// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const SavingAccount& acc){
    out<<acc.ifscCode<<endl<<acc.accountNo<<endl<<acc.balance<<endl<<acc.userName<<endl<<acc.date<<endl<<acc.accountType;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, SavingAccount& acc){
    in>>acc.ifscCode>>acc.accountNo>>acc.balance>>acc.userName>>acc.date>>acc.accountType;
    return in;
}

// method to handle the updation of balance whenever a user deposits amount
SavingAccount& SavingAccount :: updateBalanceDeposit(int transactionId, float amount){
    Balance tempBal(amount);

    if(transactionLimit){
        balance = balance + tempBal;
        cout<<left<<setw(50)<<"Amount deposited Successfully."<<endl;
        
        time_t now = time(0);
        tm* locTime = gmtime(&now);

        Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);

        Deposit tempDr(transactionId, amount, balance, dt);
        dr=tempDr;
        dr.storeTransaction(accountType, userName);
        transactionLimit--;
    }
    else{
        cout<<left<<setw(50)<<"Transaction Limit Reached !"<<endl;
    }
        
    
    return (*this);
}
// method to handle the updation of balance whenever a user withdraws amount
SavingAccount& SavingAccount :: updateBalanceWithdrawn(int transactionId, float amount){
    Balance tempBal(amount);
    
    if(transactionLimit){
                
        try{

            Balance leftAmount = balance-tempBal;
            if(leftAmount <= minBalance){
                throw minBalance;
            }
            else{
                balance=balance-tempBal;          
            
                time_t now = time(0);
                tm* locTime = gmtime(&now);

                Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);

                Withdrawl tempCr(transactionId, amount, balance, dt);
                cr=tempCr;
                cr.storeTransaction(accountType, userName);
                
                cout<<left<<setw(50)<<"Amount Withdrawn Successfully."<<endl;

                transactionLimit--;
            }

        }
        catch(Balance bal){
            cout<<left<<setw(50)<<"Cannot withdraw amount!! Minimum balance reached."<<endl;
            return (*this);
        }

    }
    else{
        cout<<left<<setw(50)<<"Transaction Limit Reached !"<<endl;
    }
    
    return (*this);
}

// method to handle the updation of balance whenever a user transfers amount
SavingAccount& SavingAccount :: updateBalanceTransfer(int transactionId, float amount, long long receiverAccountNo){
    Balance tempBal(amount);
    
    if(transactionLimit){
        
        try{
            Balance leftAmount = balance-tempBal;
        
            if(leftAmount <= minBalance){
                throw minBalance;
            }
            else{
                balance=balance-tempBal;
                
                time_t now = time(0);
                tm* locTime = gmtime(&now);

                Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);
                
                Transfer tempTr(transactionId, amount, balance, receiverAccountNo, dt);
                tr=tempTr;
                tr.storeTransaction(accountType, userName);
                cout<<left<<setw(50)<<"Amount transferred Successfully."<<endl;

                transactionLimit--;
            }

        }
        catch(Balance bal){
            cout<<left<<setw(50)<<"Cannot transfer amount!! Minimum balance reached."<<endl;
            return (*this);
        }
    }
    else{
        cout<<left<<setw(50)<<"Transaction Limit Reached !"<<endl;
    }

    return (*this);
}

// method to fetch all transactions made by a user
void SavingAccount :: accountTransactions(){
    dr.retrieveTransactions(accountType, userName);
    cr.retrieveTransactions(accountType, userName);
    tr.retrieveTransactions(accountType, userName);
}

// method to show account details of a user
void SavingAccount :: showAccountDetails(){
    cout<<endl;
    cout<<left<<setw(50)<<"Account Details "<<endl;
    cout<<left<<setw(50)<<"Account Type "<<left<<setw(4)<<" : "<<accountType<<endl;
    cout<<left<<setw(50)<<"Account Number "<<left<<setw(4)<<" : "<<accountNo<<endl;
    cout<<left<<setw(50)<<"IFSC Code "<<left<<setw(4)<<" : "<<ifscCode<<endl;
    
    balance.showBalance();
    cout<<left<<setw(50)<<"Total Intrest Earned (Yearly) "<<left<<setw(4)<<" : "<<interestEarned()<<endl;
}

// method to calculate the interest earned on savings by a user (per annum)
Balance SavingAccount :: interestEarned(){
    int dd,mm,yy;

    time_t now = time(0);
    
    tm* locTime = gmtime(&now);

    dd = locTime->tm_mday;
    mm = 1+locTime->tm_mon;
    yy = 1900+locTime->tm_year;

    Date dt(dd,mm,yy);

    int period = dt - date;

    Balance interest(0);    
    
    if(period>=360){
        interest = balance;
        interest = interest.calInterest(ROI,period);
        balance  = balance + interest;
    }
    
    return interest;   
}


// method to store account details of a user (Saving Account)
void SavingAccount :: storeAccountDetails(){ 
    defaultpath = "SavingAccounts/Accounts/" + userName + "_Account_Details.txt";

    ofstream fout(defaultpath,ios::out);
    if(fout){
        fout<<ifscCode<<endl<<accountNo<<endl<<balance<<endl<<userName<<endl<<date<<endl<<accountType;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !!"<<endl;
    }
    
}

// method to retrieve account details of a user (Saving Account)
void SavingAccount :: retrieveAccountDetails(){
    defaultpath = "SavingAccounts/Accounts/" + userName + "_Account_Details.txt";

    ifstream fin(defaultpath,ios::in);
    if(fin){
        fin>>ifscCode>>accountNo>>balance>>userName>>date>>accountType;
        showAccountDetails();
    }
    else{
        cout<<left<<setw(50)<<"No record Exists"<<endl;
    }
}


// inherited class 'CurrentAccount' to handle user's current account 
class CurrentAccount : public Account{
    private:
        string accountType; 
        static Balance minBalance;
    public:

        // default constructor
        CurrentAccount(){}

        // parameterized constructor
        CurrentAccount(string& userName){
            this->userName = userName;
        }

        // parameterized constructor
        CurrentAccount(long long accountNo, Balance balance, string userName, Date date){
            this->accountNo   = accountNo;
            this->balance     = balance;
            this->userName    = userName;
            this->date        = date;
            this->accountType = "Current_Account";
        }

        // member function declarations
        CurrentAccount& updateBalanceDeposit(int, float);
        CurrentAccount& updateBalanceWithdrawn(int, float);
        CurrentAccount& updateBalanceTransfer(int, float, long long);
        void accountTransactions();
        void showAccountDetails();

        void storeAccountDetails();
        void retrieveAccountDetails();

        // overloaded operators prototypes with the help of friend function
        friend ostream& operator << (ostream& out, const CurrentAccount& acc);
        friend istream& operator >> (istream& in, CurrentAccount& acc);
        
    // friend class
    friend class UserRecordCurrentAccount;
   
};

// definition of static member variables
Balance CurrentAccount :: minBalance = 5000.0;


// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const CurrentAccount& acc){
    out<<acc.ifscCode<<endl<<acc.accountNo<<endl<<acc.balance<<endl<<acc.userName<<endl<<acc.date<<endl<<acc.accountType;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, CurrentAccount& acc){
    in>>acc.ifscCode>>acc.accountNo>>acc.balance>>acc.userName>>acc.date>>acc.accountType;
    return in;
}

// method to handle the updation of balance whenever a user deposits amount
CurrentAccount& CurrentAccount :: updateBalanceDeposit(int transactionId, float amount){
    Balance tempBal(amount);

    balance = balance + tempBal;

    time_t now = time(0);
    tm* locTime = gmtime(&now);

    Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);

    Deposit tempDr(transactionId, amount, balance, dt);
    dr=tempDr;
    dr.storeTransaction(accountType, userName);
    
    return (*this);
}

// method to handle the updation of balance whenever a user withdraws amount
CurrentAccount& CurrentAccount :: updateBalanceWithdrawn(int transactionId, float amount){
    Balance tempBal(amount);
    
    Balance leftAmount = balance-tempBal;
        
    if(leftAmount <= minBalance){
        cout<<"Cannot withdraw amount!! Minimum balance reached."<<endl;
            return (*this);
        }
        else{
            balance=balance-tempBal;
        }

        time_t now = time(0);
        tm* locTime = gmtime(&now);

        Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);
        
        Withdrawl tempCr(transactionId, amount, balance, dt);
        cr=tempCr;
        cr.storeTransaction(accountType, userName);

    
    return (*this);
}


// method to handle the updation of balance whenever a user transfers amount
CurrentAccount& CurrentAccount :: updateBalanceTransfer(int transactionId, float amount, long long receiverAccountNo){
    Balance tempBal(amount);
    Balance leftAmount = balance-tempBal;
    
    if(leftAmount <= minBalance){
        cout<<"Cannot transfer amount!! Minimum balance reached."<<endl;
        return (*this);
    }
    else{
        balance=balance-tempBal;
    }
    
    time_t now = time(0);
    tm* locTime = gmtime(&now);

    Date dt(locTime->tm_mday,1+locTime->tm_mon,1900+locTime->tm_year);
    
    Transfer tempTr(transactionId, amount, balance, receiverAccountNo, dt);
    tr=tempTr;
    tr.storeTransaction(accountType, userName);

    return (*this);
}


// method to fetch all transactions made by a user
void CurrentAccount :: accountTransactions(){
    dr.retrieveTransactions(accountType, userName);
    cr.retrieveTransactions(accountType, userName);
    tr.retrieveTransactions(accountType, userName);
}

// method to show account details of a user
void CurrentAccount :: showAccountDetails(){
    cout<<left<<setw(50)<<"Account Details "<<endl;
    cout<<left<<setw(50)<<"Account Type "<<left<<setw(4)<<" : "<<accountType<<endl;
    cout<<left<<setw(50)<<"Account Number "<<left<<setw(4)<<" : "<<accountNo<<endl;
    cout<<left<<setw(50)<<"IFSC Code "<<left<<setw(4)<<" : "<<ifscCode<<endl;
    balance.showBalance();
}

// method to store account details of a user (Current Account)
void CurrentAccount :: storeAccountDetails(){
    defaultpath = "CurrentAccounts/Accounts/" + userName + "_Account_Details.txt";

    ofstream fout(defaultpath,ios::out);
    if(fout){
        fout<<ifscCode<<endl<<accountNo<<endl<<balance<<endl<<date<<endl<<accountType;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !! "<<endl;
    }
    
}
// method to retrieve account details of a user (Current Account)
void CurrentAccount :: retrieveAccountDetails(){
    defaultpath = "CurrentAccounts/Accounts/" + userName + "_Account_Details.txt";

    ifstream fin(defaultpath,ios::in);
    if(fin){
        fin>>ifscCode>>accountNo>>balance>>date>>accountType;
        showAccountDetails();
    }
    else{
        cout<<left<<setw(50)<<"No Record Exists !!"<<endl;
    }
}


// Address class - to deal with the address of a user.
class Address{
    private:
        string houseNo;
        string street,city,state;
        int pincode;
    public:

        // default constructor
        Address(){}

        // parameterized constructor
        Address(string houseNo, string street, string city, string state, int pincode){
            this->houseNo    = houseNo;
            this->street     = street;
            this->city       = city;
            this->state      = state;
            this->pincode    = pincode;
        }

        // member function declarations
        void showAddress();

        // overloaded operators prototypes// overloaded operators prototypes
        friend ostream& operator << (ostream& out, const Address& addr);
        friend istream& operator >> (istream& in, Address& addr);


    // friend class
    friend class User;
};


// overloaded << operator to redirect the stream for writing the data in file
ostream& operator << (ostream& out, const Address& addr){
    out<<addr.houseNo<<" "<<addr.street<<endl<<addr.city<<" "<<addr.state<<" ";
    out<<addr.pincode;
    return out;
}

// overloaded >> operator to redirect the stream for reading the data from file
istream& operator >> (istream& in, Address& addr){
    in>>addr.houseNo>>addr.street>>addr.city>>addr.state;
    in>>addr.pincode;
    return in;
}

// method to show the address of a user
void Address :: showAddress(){
    cout<<left<<setw(50)<<"Address Details"<<endl;
    cout<<left<<setw(50)<<"House No. "<<left<<setw(4)<<" : "<<houseNo<<endl;
    cout<<left<<setw(50)<<"Street "<<left<<setw(4)<<" : "<<street<<endl;
    cout<<left<<setw(50)<<"City "<<left<<setw(4)<<" : "<<city<<endl;
    cout<<left<<setw(50)<<"State "<<left<<setw(4)<<" : "<<state<<endl;
    cout<<left<<setw(50)<<"Pincode "<<left<<setw(4)<<" : "<<pincode<<endl;
}


// User class - to handle the details of a user
class User{
    protected:
        string userFirstName,userLastName;
        string userOccupation;
        string userName, password;
        long long userPhoneNo;
        char userGender;
        
        Address userAddress;
        string defaultpath;
    public:

        // default constructor
        User(){}

        // parametrized constructor
        User(string userFirstName, string userLastName, string userName, string password, string userOccupation, long long userPhoneNo, char userGender, Address userAddress){
            this->userFirstName  = userFirstName;
            this->userLastName   = userLastName;
            this->userName       = userName;
            this->password       = password;
            this->userOccupation = userOccupation;
            this->userPhoneNo    = userPhoneNo;
            this->userGender     = userGender;
            this->userAddress    = userAddress;            
        }

        // pure virtual function declarations - (User class - abstract class)
        virtual void storeUserDetails()=0;
        virtual void retrieveUserDetails()=0;
        virtual void updateUserDetails(string, string, long long)=0;

        // member function declarations
        void showUserDetails();
        
};

// method to show the details of a user
void User :: showUserDetails(){
    cout<<left<<setw(50)<<"Account Holder Name "<<left<<setw(4)<<" : "<<userFirstName<<" "<<userLastName<<endl;
    cout<<left<<setw(50)<<"Gender "<<left<<setw(4)<<" : "<<userGender<<endl;
    cout<<left<<setw(50)<<"Phone Number "<<left<<setw(4)<<" : "<<userPhoneNo<<endl;
    userAddress.showAddress();
}

// UserRecordSavingAccount class - to handle the savings account records associated to a bank user. 
class UserRecordSavingAccount : public User{
    protected :
        SavingAccount userSavingsAccount;
    public :

        // default constructor
        UserRecordSavingAccount(){}

        // parameterized constructor
        UserRecordSavingAccount(string& userName){
            this->defaultpath = "SavingAccounts/Users/" + userName + "_Details.txt";
        }

        // overloaded parameterized constructor
        UserRecordSavingAccount(string userFirstName, string userLastName, string userName, string password, string userOccupation, long long userPhoneNo, char userGender, Address userAddress, SavingAccount userSavingsAccount): User(userFirstName, userLastName, userName, password, userOccupation, userPhoneNo, userGender, userAddress){
            this->userSavingsAccount = userSavingsAccount;
            this->defaultpath = "SavingAccounts/Users/" + userName + "_Details.txt";
        }

        // member function declarations
        void storeUserDetails();
        void retrieveUserDetails();
        void updateUserDetails(string, string, long long);
        void retrieveUserAccountDetails();
        
        UserRecordSavingAccount& userTransactions(int , float, int, long long);
        void userAccountTransactions();

    // friend class
    friend class Bank;    

};

// method to store the user record and the saving account record associated to a user.
void UserRecordSavingAccount :: storeUserDetails(){

    ofstream fout(defaultpath, ios::out);
    if(fout){
        fout<<userFirstName<<" "<<userLastName<<endl<<userName<<endl<<password<<endl<<userOccupation<<endl<<userPhoneNo<<endl<<userGender<<endl<<userAddress<<endl<<userSavingsAccount;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !!"<<endl;
    }
}

// method to retrieve the user record and the saving account record associated to a user.
void UserRecordSavingAccount :: retrieveUserDetails(){

    ifstream fin(defaultpath, ios::in);
    if(fin){
        fin>>userFirstName>>userLastName>>userName>>password>>userOccupation>>userPhoneNo>>userGender>>userAddress>>userSavingsAccount;
        showUserDetails();  
    }
    else{
        cout<<left<<setw(50)<<"No Record Exists !!"<<endl;
    }  
}


// method to update the user record associated to an accounts.
void UserRecordSavingAccount :: updateUserDetails(string updatedFname = "", string updatedLname = "", long long updatedPhoneNo = 0){

    if(updatedPhoneNo){
        userPhoneNo = updatedPhoneNo;
    }
    else if(updatedFname != ""){
        userFirstName = updatedFname;
    }
    else{
        userLastName = updatedLname;
    }

    ofstream fout(defaultpath, ios::out);
    if(fout){
        fout<<userFirstName<<" "<<userLastName<<endl<<userName<<endl<<password<<endl<<userOccupation<<endl<<userPhoneNo<<endl<<userGender<<endl<<userAddress<<endl<<userSavingsAccount;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !!"<<endl;
    }
    
}

// method to show the account of a user (user account view)
void UserRecordSavingAccount :: retrieveUserAccountDetails(){
    userSavingsAccount.showAccountDetails();
}

// method to handle the various transactions made by a user (Deposits / Withdrawls/ Transfers)
UserRecordSavingAccount& UserRecordSavingAccount :: userTransactions(int transactionId, float amount, int transactionCode, long long recAccountNo=0){
    
    if(transactionCode == 1){
        userSavingsAccount = userSavingsAccount.updateBalanceDeposit(transactionId, amount);
        userSavingsAccount.showAccountDetails();
    }
    else if(transactionCode == 2){
        userSavingsAccount = userSavingsAccount.updateBalanceWithdrawn(transactionId, amount);
        userSavingsAccount.showAccountDetails();
    }
    else if(transactionCode == 3){
        userSavingsAccount = userSavingsAccount.updateBalanceTransfer(transactionId, amount, recAccountNo);
        userSavingsAccount.showAccountDetails();
    }
    // cout<<userSavingsAccount;
    userSavingsAccount.storeAccountDetails();

    return (*this);
}

// method to show the transactions made by a user (user transactions view)
void UserRecordSavingAccount :: userAccountTransactions(){
    userSavingsAccount.accountTransactions();
}

// UserRecordSavingAccount class - to handle the current account records associated to a bank user. 
class UserRecordCurrentAccount : public User{
    protected :
        CurrentAccount userCurrAccount;
    public :

        // default constructor
        UserRecordCurrentAccount(){}

        // parameterized constructor
        UserRecordCurrentAccount(string& userName){
            this->defaultpath = "CurrentAccounts/Users/" + userName + "_Details.txt";
        }

        // overloaded parameterized constructor
        UserRecordCurrentAccount(string userFirstName, string userLastName, string userName, string password, string userOccupation, long long userPhoneNo, char userGender, Address userAddress, CurrentAccount userCurrAccount): User(userFirstName, userLastName, userName, password, userOccupation, userPhoneNo, userGender, userAddress){
            this->userCurrAccount = userCurrAccount;
            this->defaultpath = "CurrentAccounts/Users/" + userName + "_Details.txt";
        }

        // member function declarations
        void storeUserDetails();
        void retrieveUserDetails();
        void updateUserDetails(string, string, long long);
        void retrieveUserAccountDetails();
                
        UserRecordCurrentAccount& userTransactions(int , float, int, long long);
        void userAccountTransactions();
    
    // friend class
    friend class Bank;    
};

// method to store the user record and the current account record associated to a user.
void UserRecordCurrentAccount :: storeUserDetails(){
    ofstream fout(defaultpath,ios::out);
    if(fout){
        fout<<userFirstName<<" "<<userLastName<<endl<<userName<<endl<<password<<endl<<userOccupation<<endl<<userPhoneNo<<endl<<userGender<<endl<<userAddress<<endl<<userCurrAccount;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !!"<<endl;
    }
}

// method to retrieve the user record and the current account record associated to a user.
void UserRecordCurrentAccount :: retrieveUserDetails(){
    ifstream fin(defaultpath,ios::in);
    if(fin){
        fin>>userFirstName>>userLastName>>userName>>password>>userOccupation>>userPhoneNo>>userGender>>userAddress>>userCurrAccount;
        showUserDetails();    
    }
    else{
        cout<<left<<setw(50)<<"No Record Exists !!"<<endl;
    }
    
}

// method to update the user records associated to an account.
void UserRecordCurrentAccount :: updateUserDetails(string updatedFname = "", string updatedLname = "" , long long updatedPhoneNo = 0){

    if(updatedPhoneNo){
        userPhoneNo = updatedPhoneNo;
    }
    else if(updatedFname != ""){
        userFirstName = updatedFname;
    }
    else{
        userLastName = updatedLname;
    }

    ofstream fout(defaultpath, ios::out);
    if(fout){
        fout<<userFirstName<<" "<<userLastName<<endl<<userName<<endl<<password<<endl<<userOccupation<<endl<<userPhoneNo<<endl<<userGender<<endl<<userAddress<<endl<<userCurrAccount;
    }
    else{
        cout<<left<<setw(50)<<"Unable to Process Right Now !!"<<endl;
    }
    
}

// method to show the account of a user (user account view)
void UserRecordCurrentAccount :: retrieveUserAccountDetails(){
    userCurrAccount.showAccountDetails();
}

// method to handle the various transactions made by a user (Deposits / Withdrawls/ Transfers)
UserRecordCurrentAccount& UserRecordCurrentAccount :: userTransactions(int transactionId, float amount, int transactionCode, long long recAccountNo=0){
    
    if(transactionCode == 1){
        userCurrAccount = userCurrAccount.updateBalanceDeposit(transactionId, amount);
        userCurrAccount.showAccountDetails();
    }
    else if(transactionCode == 2){
        userCurrAccount = userCurrAccount.updateBalanceWithdrawn(transactionId, amount);
        userCurrAccount.showAccountDetails();
    }
    else if(transactionCode == 3){
        userCurrAccount = userCurrAccount.updateBalanceTransfer(transactionId, amount, recAccountNo);
        userCurrAccount.showAccountDetails();
    }
    userCurrAccount.storeAccountDetails();

    return (*this);
}

// method to show the transactions of a user (user transactions view)
void UserRecordCurrentAccount :: userAccountTransactions(){
    userCurrAccount.accountTransactions();
}


// Bank class - to carry out banking operaions (providing banking facilities to a user)
class Bank
{
    private:
        UserRecordSavingAccount userSavAccount;
        UserRecordCurrentAccount userCurrAccount;
        SavingAccount savAccount;
        CurrentAccount currAccount;
    
        int transactionCode;
        int accountCode;
        string userName, password;
    public:

        // default constructor
        Bank(){
            transactionCode = 0;
            accountCode     = 0;
            userName        = "";
            password        = "";
        }     

        // member function declarations
        void bankMenu();
        void newUserForm();
        Address newUserCommunicationDetails();
        
        void createSavingAccount(string , string , string , string , string , long long , char , Address);
        void createCurrentAccount(string , string , string , string , string , long long , char , Address);

        void loginAccount();
        void depositForm();
        void withdrawlForm();
        void transferAmountForm();
        void updateForm(string&, string&, long long&);

        void showUserAccount();
        void showUserTransactions();
        void showUserProfile();
        void updateUserProfile();
        void deleteAccount();

        long long generateAccountNumber();
        int generateTransactionNumber();

        void storeUserName(string&, string&, int& );
        bool validateUser(string&, string&, int& );
        bool retrieveUserName(string& );
        void updateUserPassword(string&, string&, int&);
        
};

// method to generate 11 digit account number 
long long Bank :: generateAccountNumber(){
    vector <int> trNo;
    int temp = 0;
    
    srand(time(0));
    
    for(int i=0; i<11; i++){
        temp = 1 + (rand()%9);
        trNo.push_back(temp);
    }

    reverse(trNo.begin(),trNo.end());

    long long sum=0;
    for(int i=0; i<trNo.size(); i++){
        sum = sum + (trNo[i]*pow(10,i));
    }
    
    return sum;
}


// method to generate 6 digit transaction number
int Bank :: generateTransactionNumber(){
    vector <int> trNo;
    int temp = 0;
    
    srand(time(0));
    
    for(int i=0; i<6; i++){
        temp = 1 + (rand()%9);
        trNo.push_back(temp);
    }

    reverse(trNo.begin(),trNo.end());

    int sum=0;
    for(int i=0; i<trNo.size(); i++){
        sum = sum + (trNo[i]*pow(10,i));
    }
    
    return sum;
}


// bank menu to provide various banking features to a user
void Bank :: bankMenu(){
    int choice;
    bool flag = true;

    do{
        system("cls");
        cout<<left<<setw(50)<<"1. Deposit"<<endl;
        cout<<left<<setw(50)<<"2. Withdraw"<<endl;
        cout<<left<<setw(50)<<"3. Transfer Amount"<<endl;
        cout<<left<<setw(50)<<"4. Show Balance"<<endl;
        cout<<left<<setw(50)<<"5. Show All Transactions"<<endl;
        cout<<left<<setw(50)<<"6. Update Profile"<<endl;
        cout<<left<<setw(50)<<"7. Profile"<<endl;
        cout<<left<<setw(50)<<"8. Delete Account"<<endl;
        cout<<left<<setw(50)<<"0. Logout"<<endl;
        cin>>choice;
        switch(choice){
            case 1 : depositForm();
                     cin.get();
            break;
            case 2 : withdrawlForm();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 3 : transferAmountForm();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 4 : showUserAccount();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 5 : showUserTransactions();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 6 : updateUserProfile();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 7 : showUserProfile();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
                     cin.get();
            break;
            case 8 : deleteAccount();
                     flag = false;
            break;
            case 0 : flag = false;
        }
        
    }while(flag);

}


// method to accept new user details for opening account (New User Account Opening Form)
void Bank :: newUserForm(){
    system("cls");
    string tempUserFirstName, tempUserLastName, tempUserName, tempUserOccupation, tempUserPassword;
    long long tempUserPhoneNo;
    char tempUserGender;
    
    string input;
    stringstream s;
    
    cout<<left<<setw(50)<<"Use '_' instead of space "<<endl;
    cout<<endl;

    cout<<left<<setw(50)<<"Applicant's Personal Details "<<endl;
    
    while(true){
        cout<<left<<setw(50)<<"Applicant's First Name "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            tempUserFirstName = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(50)<<"Applicant's Last Name "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            tempUserLastName = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
    
    regex gender_expr("(M|F)|(m|f){1}");
    

    while(true){
        cout<<left<<setw(50)<<"Applicant's Gender (M|F) or (m|f)"<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,gender_expr)){
            s<<input;
            s>>tempUserGender;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain a single character."<<endl;
        }
    }

    Address tempUserAddress = newUserCommunicationDetails();

    regex phone_expr("[6-9][0-9]{9}");
    
    while(true){
        cout<<left<<setw(50)<<"Applicant's Phone Number "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,integer_expr)){
            s<<input;
            s>>tempUserPhoneNo;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain 10 digit number."<<endl;
        }
    }

    while(true){
        cout<<left<<setw(50)<<"Applicant's Occupation "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            tempUserOccupation = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
                         
    regex username_expr("[^0-9_][A-Za-z0-9_]{6,10}");

    while(true){
        cout<<left<<setw(50)<<"Choose Username "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,username_expr)){
            if(retrieveUserName(input)){
                cout<<left<<setw(50)<<"Username Aleardy Exists !!"<<endl;
                continue;
            }
            else{
                tempUserName = input;
                break;
            }
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain 6 alphanumeric characters."<<endl;
           
        }
    }

    cout<<left<<setw(50)<<"Set Password "<<setw(4)<<" : ";
    cin>>tempUserPassword;  
    
    int choice;
    cout<<left<<setw(50)<<"Account Type (1.Savings | 2.Current)"<<setw(4)<<" : ";
    cin>>choice;  

    if(choice == 1){
        createSavingAccount(tempUserFirstName, tempUserLastName, tempUserName, tempUserPassword, tempUserOccupation, tempUserPhoneNo, tempUserGender, tempUserAddress);
        storeUserName(tempUserName,tempUserPassword, choice);
    }
    else if(choice == 2){
        createCurrentAccount(tempUserFirstName, tempUserLastName, tempUserName, tempUserPassword, tempUserOccupation, tempUserPhoneNo, tempUserGender, tempUserAddress);
        storeUserName(tempUserName,tempUserPassword, choice);
    }
    else{
        cout<<left<<setw(50)<<"Invalid Choice !!"<<endl;
        exit(0);
    }
    
}

// method to accept user communication details (New User Form for Address)
Address Bank :: newUserCommunicationDetails(){
    string houseNo,street,city,state;
    int pincode;

    string input;
    stringstream s;

    regex house_expr("[A-za-z0-9-/]*");
    
    cout<<endl;
    cout<<left<<setw(50)<<"Applicant's Address "<<endl<<endl;   
    
    while(true){
        cout<<left<<setw(50)<<"Only '/' or '-' are allowed in 'House Name' "<<endl<<endl;
        cout<<left<<setw(50)<<"House Number "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,house_expr)){
            houseNo = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain mentioned characters."<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(50)<<"Street "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            street = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(50)<<"City "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            city = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
    
    while(true){
        cout<<left<<setw(50)<<"State "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,string_expr)){
            state = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphabets."<<endl;
        }
    }
    
    regex pincode_expr("[0-9]{6}");

    while(true){
        cout<<left<<setw(50)<<"Pincode "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,pincode_expr)){
            s<<input;
            s>>pincode;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain 6 digit number."<<endl;
        }
    }
    
    Address tempAddress(houseNo,street,city,state,pincode);
    
    return tempAddress;
}

// method accepts user details and creates the new user's savings account
void Bank :: createSavingAccount(string userFirstName, string userLastName, string userName, string password, string userOccupation, long long userPhoneNo, char userGender, Address userAddress){
    
    long long accountNo;
    float amount;

    int dd,mm,yy;

    time_t now = time(0);
    
    tm* locTime = gmtime(&now);

    dd = locTime->tm_mday;
    mm = 1+locTime->tm_mon;
    yy = 1900+locTime->tm_year;

    Date date(dd,mm,yy);
    

    accountNo = generateAccountNumber();
    
    Balance tempBalance(0.0);
    
    SavingAccount tempAccount(accountNo, tempBalance, userName, date);
    
    UserRecordSavingAccount tempUser(userFirstName, userLastName, userName, password, userOccupation, userPhoneNo, userGender, userAddress, tempAccount); 
        
    userSavAccount = tempUser;
    savAccount     = tempAccount;
    
    system("cls");
    userSavAccount.showUserDetails();
    
    userSavAccount.storeUserDetails();
    savAccount.storeAccountDetails();

    cout<<left<<setw(50)<<"New Account Opened Successfully "<<endl;
    cout<<endl;
    cout<<left<<setw(50)<<"Press Enter."<<endl;
    cin.get();
    cin.get();

    
    accountCode = 1;
    depositForm();

}

// method accepts user details and creates the new user's current account
void Bank :: createCurrentAccount(string userFirstName, string userLastName, string userName, string password, string userOccupation, long long userPhoneNo, char userGender, Address userAddress){
    
    long long accountNo;
    float amount;

    int dd,mm,yy;

    time_t now = time(0);
    
    tm* locTime = gmtime(&now);

    dd = locTime->tm_mday;
    mm = 1+locTime->tm_mon;
    yy = 1900+locTime->tm_year;

    Date date(dd,mm,yy);
    
    accountNo = generateAccountNumber();
    
    Balance tempBalance(0.0);
    
    CurrentAccount tempAccount(accountNo, tempBalance, userName, date);
    
    UserRecordCurrentAccount tempUser(userFirstName, userLastName, userName, password, userOccupation, userPhoneNo, userGender, userAddress, tempAccount); 

    
    userCurrAccount = tempUser;
    currAccount     = tempAccount;
    
    system("cls");

    userCurrAccount.showUserDetails();
    
    userCurrAccount.storeUserDetails();
    currAccount.storeAccountDetails();

    cout<<left<<setw(50)<<"New Account Opened Successfully "<<endl;
    cout<<endl;
    cout<<left<<setw(50)<<"Press Enter."<<endl;
    cin.get();
    cin.get();



    accountCode = 2;
    depositForm();

}

// method to take user credentials and login to their bank account
void Bank :: loginAccount(){
    
    int tempCode=0; 
    cout<<left<<setw(50)<<"Account type (1-Saving | 2-Current) "<<left<<setw(4)<<" : ";
    cin>>tempCode;

    regex username_expr("[^0-9][A-Za-z0-9]{5,10}");
    string input;

    while(true){
        cout<<left<<setw(50)<<"Username "<<setw(4)<<" : ";
        cin>>input;
        
        if(regex_match(input,username_expr)){
            userName = input;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain alphanumeric characters."<<endl;
        }
            
    }
    
    cout<<left<<setw(50)<<"Password "<<setw(4)<<" : ";
    cin>>password;
        
    if(validateUser(userName, password, tempCode)){   
        system("cls");
        cout<<left<<setw(50)<<userName<<endl;
        cout<<left<<setw(50)<<"Welcome to our banking services."<<endl<<endl;
        cout<<left<<setw(50)<<"User Profile. "<<endl<<endl;
        if(tempCode == 1){ 
            accountCode = tempCode;
            UserRecordSavingAccount tempUser(userName);
            userSavAccount = tempUser;
            userSavAccount.retrieveUserDetails();
            cout<<endl;
            cout<<left<<setw(50)<<"Press Enter."<<endl;
            cin.get();
            cin.get();

            bankMenu();
        }
        else if(tempCode == 2){
            accountCode = tempCode;
            UserRecordCurrentAccount tempUser(userName);
            userCurrAccount = tempUser;
            userCurrAccount.retrieveUserDetails();
            cout<<endl;
            cout<<left<<setw(50)<<"Press Enter."<<endl;
            cin.get();
            cin.get();

            bankMenu();
        }
        else{
            cout<<left<<setw(50)<<"Invalid Option !!"<<endl;
            cout<<endl;
            cout<<left<<setw(50)<<"Press Enter."<<endl;
            cin.get();
            cin.get();

    }
    }
    else{
        cout<<"Invalid Username or Password"<<endl;
        cout<<endl;
        cout<<left<<setw(50)<<"Press Enter."<<endl;
        cin.get();
        cin.get();

    }
    
}


// method to provide the functionality of deposit form to the user
void Bank :: depositForm(){
    system("cls");
    int transactionId = generateTransactionNumber();
    float amount;

    stringstream s;
    string input;

    cout<<left<<setw(50)<<"DEPOSIT FORM"<<endl;
    while(true){
        cout<<left<<setw(50)<<"Enter the amount to be deposited "<<left<<setw(4)<<" : ";
        cin>>input;

        if(regex_match(input,integer_expr)){
            s<<input;
            s>>amount;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain 11 digits."<<endl;
        }

    }
       
    // code for amount deposits
    transactionCode = 1;

    if(accountCode == 1){
        userSavAccount = userSavAccount.userTransactions(transactionId, amount, transactionCode);
        userSavAccount.storeUserDetails();
        cin.get();
    }
    else if(accountCode == 2){
        userCurrAccount = userCurrAccount.userTransactions(transactionId, amount, transactionCode);
        userCurrAccount.storeUserDetails();
        cin.get();
    }
    
    
}

// method to provide the functionality of withdrawl form to the user
void Bank :: withdrawlForm(){
    system("cls");
    int transactionId = generateTransactionNumber();
    float amount;

    stringstream s;
    string input;

    cout<<left<<setw(50)<<"WITHDRAWL FORM"<<endl;
    while(true){
        cout<<left<<setw(50)<<"Enter the amount to be withdrawn "<<left<<setw(4)<<" : ";
        cin>>input;

        if(regex_match(input,integer_expr)){
            s<<input;
            s>>amount;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain digits."<<endl;
        }

    }
    
    // code for amount withdrawl
    transactionCode = 2;
    
    if(accountCode == 1){
        userSavAccount = userSavAccount.userTransactions(transactionId, amount, transactionCode);
        userSavAccount.storeUserDetails();
       
    }
    else if(accountCode == 2){
        userCurrAccount = userCurrAccount.userTransactions(transactionId, amount, transactionCode);
        userCurrAccount.storeUserDetails();
        
    }
        
}

// method to provide the functionaity of transfer form to the user
void Bank :: transferAmountForm(){
    system("cls");
    int transactionId = generateTransactionNumber();
    long long accountNo;

    float amount;

    stringstream s;
    string input;
    regex accno_expr("[0-9]{11}");

    cout<<left<<setw(50)<<"TRANSFER FORM"<<endl;
    while(true){
        cout<<left<<setw(50)<<"Enter the reciever account number "<<left<<setw(4)<<" : ";
        cin>>input;

        if(regex_match(input,accno_expr)){
            s<<input;
            s>>accountNo;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain digits."<<endl;
        }

    }
    
    s.clear();
    
    while(true){
        cout<<left<<setw(50)<<"Enter the amount to be transferred "<<left<<setw(4)<<" : ";
        cin>>input;

        if(regex_match(input,integer_expr)){
            s<<input;
            s>>amount;
            break;
        }
        else{
            cout<<left<<setw(50)<<"Invalid Input !! "<<endl;
            cout<<left<<setw(50)<<"Can only contain digits."<<endl;
        }

    }
    
    // code for amount transfer  
    transactionCode = 3;

    if(accountCode == 1){
        userSavAccount = userSavAccount.userTransactions(transactionId, amount, transactionCode, accountNo);
        userSavAccount.storeUserDetails();
        
    }
    else if(accountCode == 2){
        userCurrAccount = userCurrAccount.userTransactions(transactionId, amount, transactionCode, accountNo);
        userCurrAccount.storeUserDetails();
        
    }
}


// method to show the records of transactions of a particular user 
void Bank :: showUserTransactions(){
    system("cls");
    cout<<left<<setw(15)<<"Id"<<left<<setw(15)<<"Amount"<<left<<setw(15)<<"T. Type"<<left<<setw(15)<<"Balance"<<left<<setw(15)<<"Account No"<<left<<"Date"<<endl;
    
    if(accountCode == 1){
        userSavAccount.userAccountTransactions();
    }
    else if(accountCode == 2){
        userCurrAccount.userAccountTransactions();
    }
        
}

// method to handle the view of user's account details
void Bank:: showUserAccount(){
    system("cls");
    if(accountCode == 1){
        userSavAccount.retrieveUserAccountDetails();
    }
    else if(accountCode == 2){
        userCurrAccount.retrieveUserAccountDetails();
    }
    else{
        cout<<left<<setw(50)<<"Sorry Cannot process at the moment !!"<<endl;
    }
}

// method to provide a form to user to take user details to be updated
void Bank :: updateForm(string& inputFName, string& inputLName, long long& inputNumber){
    int choice;
    
    regex phone_expr("[6-9][0-9]{9}");
    
    cout<<left<<setw(50)<<"Select the option you wish to update."<<endl;
        cout<<left<<setw(50)<<"1. First Name"<<endl;
        cout<<left<<setw(50)<<"2. Last Name"<<endl;
        cout<<left<<setw(50)<<"3. Phone Number"<<endl;
        cout<<left<<setw(50)<<"0. Previous Menu"<<endl;
        cin>>choice;
        switch(choice){
            
            case 1 : cout<<left<<setw(50)<<"Use '_' instead of spaces. "<<endl<<endl;
                     cout<<left<<setw(50)<<"Enter New First Name"<<left<<setw(4)<<" : "<<endl;
                     cin>>inputFName;
                     if(!regex_match(inputFName, string_expr)){
                        cout<<left<<setw(50)<<"Invalid Input."<<endl<<endl;
                        inputFName = "";
                     }
            break;
            case 2 : cout<<left<<setw(50)<<"Use '_' instead of spaces."<<endl<<endl;
                     cout<<left<<setw(50)<<"Enter New Last Name"<<left<<setw(4)<<" : "<<endl;
                     cin>>inputLName;
                     if(!regex_match(inputLName, string_expr)){
                        cout<<left<<setw(50)<<"Invalid Input."<<endl<<endl;
                        inputLName = "";
                     }
            break;
            case 3 : cout<<left<<setw(50)<<"Input only 10 digit numbers."<<endl<<endl;
                     cout<<left<<setw(50)<<"Enter New Phone Number"<<left<<setw(4)<<" : "<<endl;
                     cin>>inputNumber;
                     if(!regex_match(to_string(inputNumber), phone_expr)){
                        cout<<left<<setw(50)<<"Invalid Input."<<endl<<endl;
                        inputNumber = 0;
                     }
            break;
            
            case 0 : bankMenu();
    }
}

// method to deal with updation of user details (such as Name, Phone Number)
void Bank :: updateUserProfile(){
    system("cls");

    string inputFName="";
    string inputLName="";
    long long inputNumber=0;
    
    if(accountCode == 1){
        updateForm(inputFName, inputLName, inputNumber);
        if(inputFName != "" || inputLName != "" || inputNumber !=0){
            userSavAccount.updateUserDetails(inputFName, inputLName, inputNumber);
            cout<<left<<setw(50)<<"Profile Updated Successfully !!"<<endl;
        }
    }
    else if(accountCode == 2){
        updateForm(inputFName, inputLName, inputNumber);
        if(inputFName != "" || inputLName != "" || inputNumber !=0){
            userCurrAccount.updateUserDetails(inputFName, inputLName, inputNumber);
            cout<<left<<setw(50)<<"Profile Updated Successfully !!"<<endl;
        }
    }
    else{
        cout<<left<<setw(50)<<"Sorry Cannot process at the moment !!"<<endl;
    }
}


// method to show user profiles (Show User Details)
void Bank :: showUserProfile(){
    system("cls");
    if(accountCode == 1){
        userSavAccount.showUserDetails();
    }
    else if(accountCode == 2){
        userCurrAccount.showUserDetails();
    }
    else{
        cout<<left<<setw(50)<<"Sorry Cannot process at the moment !!"<<endl;
    }
}


// method to provide functionality to delete user Profile and user Account
void Bank :: deleteAccount(){
    system("cls");
    char choice;

    if(accountCode == 1){

        // code to delete user details and account details having saving account
        cout<<left<<setw(50)<<"Are you sure you want to delete your account?"<<endl;
        cout<<left<<setw(50)<<"Select (Y|N) or (y|n) "<<left<<setw(4)<<" : "<<endl;
        cin>>choice;

        if(choice == 'Y' || choice == 'y'){
            
            string defaultUserpath = "SavingAccounts/Users/" + userName + "_Details.txt";
            const char *deleteUserFile = defaultUserpath.c_str();    
            remove(deleteUserFile);

            string defaultAccountpath = "SavingAccounts/Accounts/" + userName + "_Account_Details.txt";
            const char *deleteAccountFile = defaultAccountpath.c_str();    
            remove(deleteAccountFile);

            string defaultTransactionpath = "SavingAccounts/Transactions/" + userName + "_Deposits.txt";
            const char *deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
            
            defaultTransactionpath = "SavingAccounts/Transactions/" + userName + "_Withdrawls.txt";
            deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
            
            defaultTransactionpath = "SavingAccounts/Transactions/" + userName + "_Transfers.txt";
            deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
            
            updateUserPassword(userName, password, accountCode);

            cout<<left<<setw(50)<<"Account deleted successfully."<<endl;
            
            cout<<endl;
            cout<<left<<setw(50)<<"Press Enter."<<endl;
            cin.get();
            cin.get();
        }
        else{
            bankMenu();
        }
        
    }
    else if(accountCode == 2){
        // code to delete user details and account details having current account
        cout<<left<<setw(50)<<"Are you sure you want to delete your account?"<<endl;
        cout<<left<<setw(50)<<"Select (Y|N) or (y|n) "<<left<<setw(4)<<" : "<<endl;
        cin>>choice;

        if(choice == 'Y' || choice == 'y'){
            
            string defaultUserpath = "CurrentAccounts/Users/" + userName + "_Details.txt";
            const char *deleteUserFile = defaultUserpath.c_str();    
            remove(deleteUserFile);

            string defaultAccountpath = "CurrentAccounts/Accounts/" + userName + "_Account_Details.txt";
            const char *deleteAccountFile = defaultAccountpath.c_str();    
            remove(deleteAccountFile);

            string defaultTransactionpath = "CurrentAccounts/Transactions/" + userName + "_Deposits.txt";
            const char *deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
            
            defaultTransactionpath = "CurrentAccounts/Transactions/" + userName + "_Withdrawls.txt";
            deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
            
            defaultTransactionpath = "CurrentAccounts/Transactions/" + userName + "_Transfers.txt";
            deleteTransactionFile = defaultTransactionpath.c_str();    
            remove(deleteTransactionFile);
        
            updateUserPassword(userName, password, accountCode);
            
            cout<<left<<setw(50)<<"Account deleted successfully."<<endl;
            
            cout<<endl;
            cout<<left<<setw(50)<<"Press Enter."<<endl;
            cin.get();
            cin.get();
        }
        else{
            bankMenu();
        }
        
    }
    else{
        cout<<left<<setw(50)<<"Sorry Cannot process at the moment !!"<<endl;
        
    }
}


// method to store the username and password of a new user
void Bank :: storeUserName(string& userName, string& password, int& acctype){
    ofstream fout("UserName_Passwd.txt",ios::app);
    if(fout){
        fout<<userName<<" "<<password<<" "<<acctype<<endl;
        fout.close();
    }    
}


// method to validate a user if he is a valid bank user or not
bool Bank :: validateUser(string& userName, string& password, int& acctype){
    string user,passwd;
    int type;

    ifstream fin("UserName_Passwd.txt",ios::in);
    if(fin){
        fin>>user>>passwd>>type;
        while(!fin.eof()){
            if(userName == user && password == passwd && acctype == type){
                return true;
            }
            fin>>user>>passwd>>type;
        }
    }
    return false;
}


// method to update username password file if a user no more exists in bank records
void Bank :: updateUserPassword(string& userName, string& password, int& acctype){
    string user,passwd;
    int type;

    
    ifstream fin("UserName_Passwd.txt",ios::in);
    if(fin){
        ofstream fout("tempFile.txt",ios::app);
        fin>>user>>passwd>>type;
        while(!fin.eof()){
            if(userName == user && password == passwd && acctype == type){
                fin>>user>>passwd>>type;
                continue;
            }
            else{
                fout<<user<<" "<<passwd<<" "<<type<<endl;
            }
            
            fin>>user>>passwd>>type;
        }
        fout.close();
    }
    fin.close();
    
    
    remove("UserName_Passwd.txt");
    rename("tempFile.txt", "UserName_Passwd.txt");
}

// method to check if a username has already been taken by some user or not
bool Bank :: retrieveUserName(string& tempUserName){
    string userName,password;
    int type;

    ifstream fin("UserName_Passwd.txt",ios::in);
    if(fin){
        fin>>userName>>password>>type;
        while(!fin.eof()){
            if(tempUserName==userName){
                return true;
            }
            fin>>userName>>password>>type;
        }
    }
    return false;
}

// driver code
int main(){
    
    // creating file structure to store data
    system("mkdir CurrentAccounts");
    system("mkdir CurrentAccounts\\Accounts, CurrentAccounts\\Transactions, CurrentAccounts\\Users");
    system("mkdir SavingAccounts");
    system("mkdir SavingAccounts\\Accounts, SavingAccounts\\Transactions, SavingAccounts\\Users");

    system("cls");
    
    // banking operations starts here
    Bank bankOperation;
    int choice;

    do{
        system("cls");
        cout<<left<<setw(50)<<"   WELCOME TO CPLUS BANK "<<endl;
        cout<<left<<setw(50)<<"1. New Account"<<endl;
        cout<<left<<setw(50)<<"2. Login"<<endl;
        cout<<left<<setw(50)<<"3. Exit"<<endl;
        cin>>choice;
        switch(choice){
            case 1 : bankOperation.newUserForm();
                     cout<<endl;
                     cout<<left<<setw(50)<<"Press Enter."<<endl;
                     cin.get();
            break;

            case 2 : bankOperation.loginAccount();
            break;

            case 3 : exit(0);
            break;

            default : cout<<left<<setw(50)<<"Invalid  Input !!"<<endl;


        }
    }while(1);

    return 0;
}

