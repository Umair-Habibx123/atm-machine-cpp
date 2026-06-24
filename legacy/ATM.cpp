// ============================================================================
//  ORIGINAL 1st-SEMESTER VERSION -- preserved as-is for nostalgia.
//  The modern, modular rewrite lives in ../src/. Do not "fix" this file;
//  it's a snapshot of where the project began. See ../README.md.
// ============================================================================
#include<iostream>
#include<cstdlib>
#include<string.h>
int balance=0;        // GLOBAL VARIABLE
using namespace std;
void show_menu()
{
	cout<<"\n\t\t\t\t*****************************************************\n";
	cout<<"\t\t\t\t*            * WELCOME TO MENU *                    *\n";
	cout<<"\t\t\t\t*                                                   *\n";
	cout<<"\t\t\t\t*               1. CHECK BALANCE.                   *\n";
	cout<<"\t\t\t\t*               2. DEPOSIT MONEY.                   *\n";
	cout<<"\t\t\t\t*               3. WITHDRAWL MONEY.                 *\n";
	cout<<"\t\t\t\t*               4. SEND MONEY.                      *\n";
	cout<<"\t\t\t\t*               5. RECIEVE MONEY.                   *\n";
	cout<<"\t\t\t\t*               6. PAY BILLS.                       *\n";
	cout<<"\t\t\t\t*               7. EXIT.                            *\n";
	cout<<"\t\t\t\t*                                                   *\n";
	cout<<"\t\t\t\t*****************************************************\n";
}
void PAY_BILL()
{
	int BC=0,P=0,EB=0,WB=0,GB=0;
	cout<<"\n\t\t\t\t*****************************************************\n";
	cout<<"\t\t\t\t*                                                   *\n";
	cout<<"\t\t\t\t*               1. ELECTRICITY.                     *\n";
	cout<<"\t\t\t\t*               2. WATER.                           *\n";
	cout<<"\t\t\t\t*               3. GAS.                             *\n";
	cout<<"\t\t\t\t*                                                   *\n";
	cout<<"\t\t\t\t*****************************************************\n";
	cin>>BC;
		system("cls");
if(balance!=0)
{
	if(BC==1)
	{
	EB=(rand() % 5000) +1 ;
	cout<<"\t\tELECTRICITY BILL IS "<<EB<<" WITH ALL TAXES \n";
	cout<<"\t\tPRESS 1 TO PAY OR 0 TO EXIT \n"; 
	cin>>P;
		system("cls");
		if(P==1)
		{
			balance-=EB;
			cout<<"\t\t\tYOU HAVE SUCCESSFULLY PAY ELECTRICITY BILL \n";
		}
	}
	else if(BC==2)
	{
	WB=(rand() % 5000) +1 ;
	cout<<"\t\tWATER BILL IS "<<WB<<" WITH ALL TAXES \n";
	cout<<"\t\tPRESS 1 TO PAY OR 0 TO EXIT \n"; 
	cin>>P;
		system("cls");
		if(P==1)
		{
			balance-=WB;
			cout<<"\t\t\tYOU HAVE SUCCESSFULLY PAY WATER BILL \n";
		}
	}
	else if(BC==3)
		{
		GB=(rand() % 5000) +1 ;
		cout<<"\t\tGAS BILL IS "<<GB<<" WITH ALL TAXES \n";
		cout<<"\t\tPRESS 1 TO PAY OR 0 TO EXIT \n"; 
		cin>>P;
			system("cls");
		if(P==1)
		{
			balance-=GB;
			cout<<"\t\t\tYOU HAVE SUCCESSFULLY PAY GAS BILL \n";
		}	
		}
	else
		{
			cout<<"\n\n\n\t\t\t\tINVALID ENTRY \n";
		}
	}
if(balance==0)
{
	cout<<"\n\n\t\t\t\"YOU CANNOT PAY BILL  \"\n\t\t\t\" YOUR BALANCE IS 0 \"\n";
}
}
void DEP_AM()
{
	cout<<"\n\n\n\t\t\t\tDEPOSIT AMMOUNT : \n\t\t\t\t";	
	double DepAmount; 
	cin>>DepAmount; 
	balance+=DepAmount;
}
void WIT_AM()
{
	cout<<"\n\n\n\t\t\t\tWITHDRAWL MONEY : \n\t\t\t\t"; 
	double WidAmount; 
	cin>>WidAmount; 
	if(WidAmount<=balance)
	balance-=WidAmount;
	else
	cout<<"\t\t\"ERROR\" WITHDRAWING AMOUNT BIGGER THAN SAVE AMOUNT IN YOUR ACCOUNT \n";
}
int main()
{
	int a,b,counter=0;  char ch;
	string name,NAME;  int pin,PIN;
			cout<<"\t\t\t\t                 * NEW USER *                       \n";
			cout<<"\t\t\t\t        * PLEASE SIGN IN YOUR ACCOUNT FIRST *       \n\n";
			cout<<"\n\n\t\t\t\t              PLEASE MEMORIZE IT                    \n";
			cout<<"\t\t\t\t           1. >> ENTER YOUR USERNAME : ";  cin>>NAME;
			cout<<"\t\t\t\t           2. >> ENTER YOUR PASSWORD : "; cin>>PIN;system("cls");
			cout<<"\t\t\t\t          * PLEASE LOG IN YOUR ACCOUNT *       \n\n";
			cout<<"\t\t\t\t           1. >> ENTER YOUR USERNAME : ";  cin>>name;
			cout<<"\t\t\t\t           2. >> ENTER YOUR PASSWORD : "; cin>>pin;system("cls");	
	do
		{
			if((NAME==name)&&(PIN==pin))
			{	
			show_menu();
				if(balance==0)
					{
					cout<<"\n\t\t\tYOU HAVE 0 BALANCE SO FIRST OF ALL \"DEPOSIT MONEY\" IN YOUR ACCOUNT \n";
					}
				cout<<"\n\n\t\t\t\tSELECT AN OPTION FROM MENU ??? \n\t\t\t\t";
				cin>>a; 
									string n;  int no,NO;   
				system("cls");
				switch(a)
				{
					case 1: 
							cout<<"\n\n\n\n\t\t\t\tBALANCE IS = "<<balance<<" RS \n";  break;
					case 2: 
							DEP_AM();  break;
					case 3: 
							WIT_AM() ; break;
					case 4: 
							cout<<"\n\n\n\n\t\t\t\tENTER SENDER NAME : ";  cin>>n;
							cout<<"\n\n\n\n\t\t\t\tENTER SENDER ACCOUNT NUMBER : ";  cin>>no;
							cout<<"\n\n\n\n\t\t\t\tENTER AMOUNT TO SENT = " ; 
							double SntAmount;  cin>>SntAmount; 
							if(SntAmount<=balance)
							{
				   				balance-=SntAmount;
				   				cout<<"\n\n\n\\n\n\t\t\t\tYOU HAVE SUCCESSFULLY SEND AMOUNT OF "<<SntAmount<<" TO ACCOUNT NUMBER "<<no;}
				   			 else
				    			cout<<"\t\t\"ERROR\" SENDING AMOUNT BIGGER THAN SAVE AMOUNT IN YOUR ACCOUNT \n"; break;
					case 5: 
							double RecAmount; 
							RecAmount= (rand() % 5000) +1 ;  
							balance +=RecAmount;
							cout<<"\n\n\n\n\t\t\t\tYOU HAVE RECIEVED AMOUNT OF = "<<RecAmount<<" FROM ACCOUNT NUMBER "<<(rand()%10000000)+1;break;
					case 6:
							PAY_BILL();  break;
					case 7: 
							cout<<"\n\n\n\t\t\t\t\t\"GOOD BYE\""; break;
					default:
							cout<<"\n\n\n\t\t\t\tINVALID ENTRY \n";
				}
			}
			else
			{
				counter=counter+1;
				if(counter==3)
				{
					cout<<"\t\t\t\t YOU HAVE TRIED TO LOGIN \""<<counter<<"\" TIMES OUT OF 3 TIMES \n\n";
					cout<<"\n\n\n\t\t\t\t\t LIMIT EXCEED YOU ARE ILLEGAL USER .\n\n";break;
				}
				cout<<"\t\t\t\t YOU HAVE TRIED TO LOGIN \""<<counter<<"\" TIMES OUT OF 3 TIMES \n";
					cout<<"\t\t\t\t          * PLEASE LOG IN YOUR ACCOUNT *       \n\n";
					cout<<"\t\t\t\t           1. >> ENTER YOUR USERNAME : ";  cin>>name;
					cout<<"\t\t\t\t           2. >> ENTER YOUR PASSWORD : "; cin>>pin;system("cls");
			}
		}
		while(a!=7);
}