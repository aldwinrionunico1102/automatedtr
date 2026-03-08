/*

Group 10 (O1A)

Bathan, Andrea Gwen
Unico, Aldwin Rion
Sato, Troy

Date: Dec. 20, 2023 - Jan. 09, 2024

PROGLAD FINAL PROJECT (CAPSTONE): AutomateDTR

*/

#include<iostream>
#include<fstream> //for file operations
#include<ctime> //for time-related functions
#include<sstream> //for parsing and string manipulation
#include<algorithm> //for capitalization
#include<string> //for modifying strings
#include<iomanip> // for spacing and width
#include<stdlib.h> //system cls
#include<windows.h> //for pause
#include <limits> //for error handling

using namespace std;

//functions
int menu(); //displays main menu
int studentMenu(); //displays student menu
bool processStudentLogin(string, fstream&); //verifies student login
int timeInOutMenu(); //displays time in or time out
string extract(istringstream&); //extract student info
string getStudentInfo(int, string, fstream&); //get student name, office assignment, office head
void timeIn(string, string, string, string, bool, fstream&, ifstream&); //function to process time in
void recordTimeIn(string, string, string, bool, fstream&); //records time in
string computeHours(fstream&); //compute hours rendered after every duty
void timeOut(string, string, string, string, fstream&, ifstream&); //compressed na dito ay check last time in, missed time in, at normal record time out
void recordTimeOut(string, string, string, fstream&); //records time out
bool isFileBlank(fstream&); //detects if a file is blank
string getMonth(); //gets the current month
int getYear(); //gets the current year
string getLastLine(fstream&); //gets the last line in a txt file which will be used in other functions
int countWords(string); //counts words in a line
string getDate(); // gets the current date
string getLastDate(fstream&); // gets the date from the last line in the txt file
string getCurrentTime(); //gets the current time
int getCurrentHour(); //gets the current hour
bool overLunchTime(); //determines if the time is over lunch time (12 pm)
string getFirstName(string); //gets the scholar's first name
string getMissedTime(); //prompts users to manually enter
int getLastTimeInHour(fstream& file); //gets the last time in hour of a scholar
bool officeHeadVerify(string, ifstream&); //office head verification for login
bool adminLogin(); //lets the admin to login
int displayAdminMenu(); //displays admin menu
void viewStudentRecords(); //view the record of any student
void addNewStudent(fstream&); //add " " upon entering ng info before ilagay sa txt file??
void viewMyRecords(string); //let students view their record

int main()
{
	int menuSel = 0;
	do
	{		
		menuSel = menu(); // select who is using the system
		string studentId;
		
		//open student accounts file
		fstream studentLogin("StudentAccounts.txt", ios::in | ios::out | ios::app);
		
		if(!studentLogin.is_open())
		{
			cout << "Error opening the student accounts file." << endl;
	        return 1;
		}
		
		if(menuSel == -1)
		{
			continue;
		}
		else if(menuSel == 1)
		{
			system("CLS");
			
			bool studentLoginResult = false;
			cout << "\n======== SCHOLAR LOGIN ========\n\n";
			do
			{
				//student
				int studIdDigits = 0;
				do
				{
					cout << "Enter student ID (10 digits): ";
	            	cin >> studentId;
					
					studIdDigits = studentId.length();
				}
				while(studIdDigits != 10);
	            
	            studentLoginResult = processStudentLogin(studentId, studentLogin);
	            
	            if(studentLoginResult == true) 
				{               
	            	system("CLS");
	            	
					cout << "\nWelcome Scholar!";
	            	int userChoice;
			
					//open and set up file
					string monthForFilename = getMonth();
	
					transform(monthForFilename.begin(), monthForFilename.end(), monthForFilename.begin(), ::tolower);
					
					string filename = studentId + "_" + monthForFilename + ".txt";
					
					//get name, office assignment, and officeHead
					string studentName = getStudentInfo(1, studentId, studentLogin);
				    string officeAssignment = getStudentInfo(2, studentId, studentLogin);
				    string officeHead = getStudentInfo(3, studentId, studentLogin);
					
					fstream studentRecord(filename.c_str(), ios::in | ios::out | ios::app);
					ifstream officeHeadPass("officeHeadLogin.txt", ios::in);
					
					//check if files are opened
					if(!studentRecord.is_open())
					{
						cout<<"Error opening the student record file.\n";
						return 1;
					}
					
					if(!officeHeadPass.is_open())
					{
						cout << "Error opening the office head login file." << endl;
				        return 1;
					}
					
					bool isBlank = isFileBlank(studentRecord);
					bool dateFirstPrevious = false;
					
					do
					{
						userChoice = studentMenu();
						
						if(userChoice == 1)
						{
							system("CLS");
							
							cout<<"\n======== SCHOLAR TIME IN/TIME OUT ========\n";
							//student chose to time in/time out
							if(isBlank == true) //means the file is new and blank
							{
								dateFirstPrevious = true;
								
								char ans;
								cout<<"\nWelcome! This is your first time logging in. You only have the option to time in for now. ";
								do
								{
									cout<<"Would you like to set the current time as your time in (Y/N)? ";
									cin>>ans;
									ans = toupper(ans);
									
									if(ans == 'Y')
									{
										system("CLS");
						                studentRecord<<studentId<<endl<<studentName<<endl<<officeAssignment<<endl<<officeHead<<endl;
										
										//get month for printing to txt file
										string month = getMonth();
										transform(month.begin(), month.end(), month.begin(), ::toupper);
										
										//get year for printing to txt file
										int year = getYear();
										
										//print info to txt file
										studentRecord<<endl<<month<<" "<<year<<endl<<endl<<"Date"<<"\t\t\tTime In"<<"\t\tTime Out"<<"\tHours of Duty";
										
										//set current time as time in
										recordTimeIn(studentName, studentId, officeAssignment, dateFirstPrevious, studentRecord);
									}
									else if(ans == 'N')
									{
										cout<<"\nNo time in was recorded. Have a good day!\n";
									}
								 	else
						            {   
						                cout<<"\nInvalid input. Please try again.\n\n";
						                cin.clear();
        								cin.ignore(numeric_limits<streamsize>::max(), '\n');
						            }
								}
								while(ans != 'Y' && ans != 'N');
							}
							else
							{
								dateFirstPrevious = false;
								
								int n = 0;
						        do
						        {
									n = timeInOutMenu();
							        
							        if(n == 1)
							        {
							        	system("CLS");							        	
							        	cout<<"\n======== SCHOLAR TIME IN ========\n";
							        	timeIn(studentName, studentId, officeHead, officeAssignment, dateFirstPrevious, studentRecord, officeHeadPass); //function for time in
									}
									else if(n == 2)
									{
										system("CLS");
										cout<<"\n======== SCHOLAR TIME OUT ========\n";
										timeOut(studentName, studentId, officeHead, officeAssignment, studentRecord, officeHeadPass); //function for time out
									}
									else
									{
										cout<<"\nInvalid input. Please try again.\n";
										cin.clear();
        								cin.ignore(numeric_limits<streamsize>::max(), '\n');
										Sleep(400);
										system("CLS");
									}
								}
								while(n != 1 && n != 2); //loop
							}
						}
						else if(userChoice == 2)
						{
							system("CLS");
							
							cout<<"\n======== VIEW MY RECORDS ========\n";
							//student chose to view their records
							viewMyRecords(studentId);
							cout<<"\n";
						}
						else
						{
							//invalid input
							cout<<"\nInvalid input. Please try again.\n";
							cin.clear();
        					cin.ignore(numeric_limits<streamsize>::max(), '\n');
							Sleep(400);
							system("CLS");
						}
					}
					while(userChoice != 1 && userChoice != 2);
					
					//close txt files
					studentRecord.close();
			    	officeHeadPass.close();
				}
				else
				{
					//incorrect student id/student password
					cout<<"\nSorry. The scholar was not found or the password was incorrect. Please try again.\n\n";
				}	
			}
			while(studentLoginResult == false);
			
			//clear the console after delay
			system("pause"); 
			system("CLS");
		}
		else if(menuSel == 2)
		{
			system("CLS");
			
			//admin
			cout << "\n======== ADMIN LOGIN ========\n";
			bool adminLoginResult = false;
			do
			{
		        adminLoginResult = adminLogin();
		
		        if(adminLoginResult == true) //valid login
		        {
		        	system("CLS");
		        	
		            // Display Login Success
		            cout<<"\nWelcome, Admin!";
					
					int adminMenuSel;
					do
					{
						adminMenuSel = displayAdminMenu();
		        		
						if(adminMenuSel == 1)
		            	{	            		
		                	//admin chose to view students' records
							char answer1;
		    	            do
		        	        {
		        	        	system("CLS");
		        	        	
								cout<<"\n======== VIEW SCHOLAR RECORDS ========\n\n";                                
		        	            //function to view a student's record
								viewStudentRecords();
		                    
		                    	do
		                    	{                                
		                        	cout<<"\nDo you want to view another record (Y/N)? ";
		                        	cin>>answer1;
		                    	}
		                    	while(answer1 != 'Y' && answer1 != 'y' && answer1 != 'N' && answer1 != 'n');
		                    	
		                    	cout<<"\n";
		                    	
								if(answer1 == 'N' || answer1 == 'n')
								{
									cout<<"\nGoing back to main menu. ";
								}
								else
								{
									system("CLS");
								}
		                	}
		                	while(answer1 == 'Y' || answer1 == 'y');
		            	}
		            	else if(adminMenuSel == 2)
		            	{
		                	//admin chose to add new student
		                	char answer2;
		                	do
		                	{
		                		system("CLS");
		                		
		                		cout<<"\n======== ADD NEW SCHOLAR TO THE RECORD ========\n\n";
		                    	//function to add new student info and credentials
								addNewStudent(studentLogin);
		                    
								do
		                    	{                                
		                        	cout<<endl<<"Do you want to add another scholar's information (Y/N)? ";
		                        	cin>>answer2;
		                    	}
		                    	while(answer2 != 'Y' && answer2 != 'y' && answer2 != 'N' && answer2 != 'n');
		                    	
		                    	if(answer2 == 'N' || answer2 == 'n')
								{
									cout<<"\nGoing back to main menu. ";
								}
								else
								{
									system("CLS");
								}
		                	}
		                	while (answer2 == 'Y' || answer2 == 'y');
		            	}
		            	else
		            	{
		            		//invalid input
		            		cout<<"\nInvalid input. Please try again.";
		            		cin.clear();
        					cin.ignore(numeric_limits<streamsize>::max(), '\n');
		            		Sleep(400);
							system("CLS");
						}
		        	}
		        	while(adminMenuSel != 1 && adminMenuSel != 2);
		        }
		        else
		        {
		        	//invalid admin login
		            cout<<"\nSorry, Admin. The username or password that you entered was incorrect or invalid. Please try again.\n\n";
		        }                    
		    }
		    while(adminLoginResult == false);
		    
		    //clear the console after delay
			system("pause"); 
			system("CLS");
		}
		else
		{
			cout<<"\nInvalid input. Please try again.\n"<<endl;
			cin.clear(); // Clear the error state to allow further input
        	cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore any remaining characters in the input buffer
			Sleep(450);
			system("CLS");
		}
		
		//close txt file
		studentLogin.close(); //close student login with info txt file
	}
	while(menuSel != -1);
	
	if(menuSel == -1)
	{
		cout<<"\nProgram Terminated.\n";
	}
	
	return 0;	
} 

//-------------------------------------------------FUNCTIONS-------------------------------------------------//

int menu()
{	
	int choice;

    cout << "\n====== AutomateDTR ======" << endl;
    cout << "\nWho is using the system?" << endl;
    cout << "\n1 - Scholar" << endl;
    cout << "2 - Admin" << endl;
    cout << "\nEnter your choice: ";
    cin >> choice;

    return choice;
}

bool processStudentLogin(string studentId, fstream& studentLogin)
{
	//move file pointer to the beginning
	studentLogin.clear();
	studentLogin.seekg(0, ios::beg);
	
	string password;
    cout << "Enter password: ";
    cin >> password;

    bool found = false;
    string line;

    while (getline(studentLogin, line)) 
	{
        istringstream iss(line);
        string fileStudentId;
        string filePassword;

        if (iss >> fileStudentId >> filePassword) 
		{
            if (studentId == fileStudentId && password == filePassword) 
			{
                found = true;
                break;
            }
        }
    }

    return found;
}

int studentMenu()
{
	int choice;
	
    cout<<"\n\n----- SCHOLAR MENU -----\n";
    cout<<"\n1 - Time In/Time Out\n";
    cout<<"2 - View Records\n";
    cout<<"\n------------------------\n\n";
    cout<<"Enter your choice: ";
    cin>>choice;

    return choice;
}

int timeInOutMenu()
{
	int choice;

    cout << "\nWelcome, scholar! Do you want to time in (1) or time out (2)? \n";
    cout << "Enter your choice: ";
    cin >> choice;

    return choice;
}

string extract(istringstream& info) 
{
    string result;
    if (info >> ws && info.peek() == '"') //skip the leading whitespaces and check if the next character is an opening double quote '"'
	{
        info.ignore(); // ignore the opening quote and move the stream to the next character
        getline(info, result, '"'); // read the quoted string and store it in result until it encounters the closing '"'
    }
    return result;
}

string getStudentInfo(int set, string studentId, fstream& studentLogin) 
{    
	// move file cursor to the beginning to avoid errors
    studentLogin.clear();
    studentLogin.seekg(0, ios::beg);

    string line;
	while (getline(studentLogin, line)) 
	{
        istringstream info(line);
        string id, password, studentName, officeAssignment, officeHead;

        info >> id >> password; // extract first two info on the line

        // Extract quoted strings manually
        studentName = extract(info);
        officeAssignment = extract(info);
        officeHead = extract(info);

        if (id == studentId) 
		{
            switch (set) 
			{
                case 1:
                    return studentName;
                    break;
                case 2:
                    return officeAssignment;
                    break;
                case 3:
                    return officeHead;
                    break;
            }
        }
    }
    
    // if the student was not found
    return "";
}

bool isFileBlank(fstream& file)
{
	bool isBlank = false;
	
	//detect first if txt file is blank. if blank, lagyan ng info. if hindi, continue.
    file.seekg(0, ios::end); //put cursor at the end of the file
    int fileLength = file.tellg(); //find the cursor's position

    //reset cursor
    file.seekg(0, ios::beg);
    
    if(fileLength == 0)
    {
    	isBlank = true;
	}
	else
	{
		isBlank = false;
	}
	
	return isBlank;
}

string getMonth()
{
	//get the current time (in seconds since the epoch which is Jan 1, 1970)
    time_t currentTime = time(0);

    //converts current time to a local time structure and stores the address to the pointer localTime
    //this can be used to access individual components of the current time, like year, month, and so on
    struct tm *localTime = localtime(&currentTime);

    //get current month from the local time structure
    int currentMonth = localTime->tm_mon + 1; //+1 kasi 0 based daw ang tm_mon
    //if walang +1 november ang nagpiprint, eh december ngayon, so add 1

    string month = " ";
    switch(currentMonth)
    {
        case 1:
            month = "January";
            break;
        case 2:
            month = "February";
            break;
        case 3:
            month = "March";
            break;
        case 4:
            month = "April";
            break;
        case 5:
            month = "May";
            break;
        case 6:
            month = "June";
            break;
        case 7:
            month = "July";
            break;
        case 8:
            month = "August";
            break;
        case 9:
            month = "September";
            break;
        case 10:
            month = "October";
            break;
        case 11:
            month = "November";
            break;
        case 12:
            month = "December";
            break;
    }

    return month;
}

int getYear()
{
	//get the current time (in seconds since the epoch which is Jan 1, 1970)
    time_t currentTime = time(0);

    //converts current time to a local time structure and stores the address to the pointer localTime
    //this can be used to access individual components of the current time, like year, month, and so on
    struct tm *localTime = localtime(&currentTime);

    //get current month from the local time structure
    int year = 1900 + localTime->tm_year;
    
	return year;
}

string getLastLine(fstream& file)
{
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);
	
	int numberOfLines = 0;
	string line = " ";
	
	//read and count the no. of lines
	while(getline(file, line))
	{
		numberOfLines++;
	}
	
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);

	string lastLine = " ";
	//get the last line
	for(int i = 1; i <= numberOfLines; i++)
	{
		getline(file, line);
		if(i == numberOfLines)
		{
			lastLine = line;
		}
	}
	
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::end);
	
	return lastLine;
}

int countWords(string line)
{
	//iterate over the words in the line and count the no. of words
	int wordCount = 0;
	stringstream word(line); //extract words from the string lastLine
	string holder = " ";
	
	while(word >> holder) //transfer each word into string holder until the end of line
	{
		wordCount++;
	}
	
	return wordCount;
}

string getDate()
{
	string currentDate = " ";
	currentDate = __DATE__;
	
	if (currentDate[4] == ' ') 
	{
        // if single digit ang date, shift characters, remove leading space
        currentDate.erase(4, 1);
    }
	
	return currentDate;
}

string getLastDate(fstream& file)
{
	string lastLine = getLastLine(file);
	
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);
	
	//get the first three words in the lastLine and store it in the string
	string word1, word2, word3;
	
	istringstream word(lastLine); //istringstream to read from lastLine string
	word>>word1>>word2>>word3; //read the first three words and store them in individual strings
	
	//concatenate the words
	string lastDate = word1 + " " + word2 + " " + word3;
	
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);
	
	return lastDate;
}

string getCurrentTime()
{
	//get the current time (in seconds since the epoch which is Jan 1, 1970)
    time_t currentTime = time(0);

    //converts current time to a local time structure and stores the address to the pointer localTime
    //this can be used to access individual components of the current time, like year, month, and so on
    struct tm *localTime = localtime(&currentTime);

    //get current hour from the local time structure
    int currentHour = localTime->tm_hour;
    int currentMins = localTime->tm_min;
    
    //convert to string
    stringstream time;
    time << currentHour;
	string currentHourStr = time.str();
	time.str(""); //clear the stringstream to remove past vlaues
	time << currentMins;
	string currentMinsStr = time.str();
	
	//manipulate single-digit mins
	if(currentMinsStr == "0")
    {
    	currentMinsStr = "00";
	}
	else if(currentMinsStr == "1" || currentMinsStr == "2" || currentMinsStr == "3" || currentMinsStr == "4" || currentMinsStr == "5" || currentMinsStr == "6" || currentMinsStr == "7" || currentMinsStr == "8" || currentMinsStr == "9")
	{
		currentMinsStr = "0" + currentMinsStr;
	}
    
    //manipulate single digit hours
    if(currentHourStr == "0")
    {
    	currentHourStr = "00";
	}
	else if(currentHourStr == "1" || currentHourStr == "2" || currentHourStr == "3" || currentHourStr == "4" || currentHourStr == "5" || currentHourStr == "6" || currentHourStr == "7" || currentHourStr == "8" || currentHourStr == "9")
	{
		currentHourStr = "0" + currentHourStr;
	}
	
	//construct time string
	string timeStr = currentHourStr + ":" + currentMinsStr;
	
	return timeStr;
}

int getCurrentHour()
{
	//get the current time (in seconds since the epoch which is Jan 1, 1970)
    time_t currentTime = time(0);

    //converts current time to a local time structure and stores the address to the pointer localTime
    //this can be used to access individual components of the current time, like year, month, and so on
    struct tm *localTime = localtime(&currentTime);

    //get current hour from the local time structure
    int currentHour = localTime->tm_hour;
    	
	return currentHour;
}

bool overLunchTime()
{
	bool isTimeOverLunch = false;
	int currentHour = getCurrentHour();
		
	if(currentHour >= 12)
	{
		isTimeOverLunch = true;
	}
	else
	{
		isTimeOverLunch = false;
	}
	
	return isTimeOverLunch;
}

string getFirstName(string studentName)
{
	//get student first name for messages
	string firstName = " ";
	
	istringstream word(studentName); //istringstream to read from studentName string
	word>>firstName; //read the first word and then store it in another string
	
	return firstName;
}

string getMissedDate()
{
	//applicable for missed time in
	int month = 0;
	int day = 0;
	int year = 0;
	bool correct = false;
	//month
	do
	{
		cout<<"\tMonth (1-12): ";
		cin>>month;
		
		if(cin.fail() || month < 1 || month > 12)
		{
			cout<<"\n\tInvalid input. Try again.\n\n";
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	correct = false;
		}
		else
		{
			correct = true;
		}
	}
	while(correct == false);
	//day
	if(month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
	{
		do
		{
			cout<<"\tDay (1-31): ";
			cin>>day;
			
			if(cin.fail() || day < 1 || day > 31)
			{
				cout<<"\n\tInvalid input. Try again.\n\n";
				cin.clear();
	        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        	correct = false;
			}
			else
			{
				correct = true;
			}
		}
		while(correct == false);
	}
	else if(month == 4 || month == 6 || month == 9 || month == 11)
	{
		do
		{
			cout<<"\tDay (1-30): ";
			cin>>day;
			
			if(cin.fail() || day < 1 || day > 30)
			{
				cout<<"\n\tInvalid input. Try again.\n\n";
				cin.clear();
	        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        	correct = false;
			}
			else
			{
				correct = true;
			}
		}
		while(correct == false);
	}
	else // if(month == 2) aka february
	{
		do
		{
			cout<<"\tDay (1-29): ";
			cin>>day;
			
			if(cin.fail() || day < 1 || day > 29)
			{
				cout<<"\n\tInvalid input. Try again.\n\n";
				cin.clear();
	        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	        	correct = false;
			}
			else
			{
				correct = true;
			}
		}
		while(correct == false);
	}
	//year
	do
	{
		cout<<"\tYear (ex. 2024): ";
		cin>>year;
		
		if(cin.fail() || year < 2023 || year > 9999)
		{
			cout<<"\n\tInvalid input. Try again.\n\n";
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	correct = false;
		}
		else
		{
			correct = true;
		}
	}
	while(correct == false);
	
	//get missedDate string
	//month
	string monthStr;
	switch(month)
	{
		case 1:
            monthStr = "Jan";
            break;
        case 2:
            monthStr = "Feb";
            break;
        case 3:
            monthStr = "Mar";
            break;
        case 4:
            monthStr = "Apr";
            break;
        case 5:
            monthStr = "May";
            break;
        case 6:
            monthStr = "Jun";
            break;
        case 7:
            monthStr = "Jul";
            break;
        case 8:
            monthStr = "Aug";
            break;
        case 9:
            monthStr = "Sep";
            break;
        case 10:
            monthStr = "Oct";
            break;
        case 11:
            monthStr = "Nov";
            break;
        case 12:
            monthStr = "Dec";
            break;
	}
	//day and year
	stringstream date;
	date<<day;
	string dayStr = date.str();
	date.str("");
	date<<year;
	string yearStr = date.str();
	
	string missedDate = monthStr + " " + dayStr + " " + yearStr;
	
	return missedDate;
}

string getMissedTime()
{
	int hour = 0;
	int mins = 0;
	string period = " ";
	bool correct = false;
	do
	{
		cout<<"\tHour (1-12): ";
		cin>>hour;
		
		if(cin.fail() || hour < 1 || hour > 12)
		{
			cout<<"\n\tInvalid input. Try again.\n\n";
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	correct = false;
		}
		else
		{
			correct = true;
		}
	}
	while(correct == false);
	do
	{
		cout<<"\tMinutes (00-59): ";
		cin>>mins;
		
		if(cin.fail() || mins < 0 || mins > 59)
		{
			cout<<"\n\tInvalid input. Try again.\n\n";
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	correct = false;
		}
		else
		{
			correct = true;
		}
	}
	while(correct == false);
	do
	{
		cout<<"\tPeriod (AM/PM): ";
		cin>>period;
		transform(period.begin(), period.end(), period.begin(), ::toupper);
		
		if(cin.fail() || period != "AM" && period != "PM")
		{
			cout<<"\n\tInvalid input. Try again.\n\n";
			cin.clear();
        	cin.ignore(numeric_limits<streamsize>::max(), '\n');
        	correct = false;
		}
		else
		{
			correct = true;
		}
	}
	while(correct == false);
	
	//change time to military time for recording to txt file
	int inputHour = hour;
	if(period == "PM" && hour != 12)
	{
		inputHour = hour + 12;
	}
	
	//convert int to string for manipulation and return
	stringstream time;
	time << inputHour;
	string inputHourStr = time.str();
	time.str(""); //clear the stringstream to remove past vlaues
	time << mins;
	string minsStr = time.str();
	
	//manipulate single-digit mins
	if(minsStr == "0")
    {
    	minsStr = "00";
	}
	else if(minsStr == "1" || minsStr == "2" || minsStr == "3" || minsStr == "4" || minsStr == "5" || minsStr == "6" || minsStr == "7" || minsStr == "8" || minsStr == "9")
	{
		minsStr = "0" + minsStr;
	}
    
    //manipulate single digit hours
    if(inputHourStr == "0")
    {
    	inputHourStr = "00";
	}
	else if(inputHourStr == "1" || inputHourStr == "2" || inputHourStr == "3" || inputHourStr == "4" || inputHourStr == "5" || inputHourStr == "6" || inputHourStr == "7" || inputHourStr == "8" || inputHourStr == "9")
	{
		inputHourStr = "0" + inputHourStr;
	}
	
	string missedTime = inputHourStr + ":" + minsStr;
	
	return missedTime;
}

int getLastTimeInHour(fstream& file)
{
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);
	
	string lastLine = getLastLine(file);
	
	//get the time in and time out
	string date1, date2, date3, timeIn, timeOut;
	istringstream dutyStream(lastLine);
	dutyStream>>date1>>date2>>date3>>timeIn>>timeOut;
	
	//get the time in hour, time in mins, time out hour, time out mins
	int timeInHour, timeInMins, timeOutHour, timeOutMins;
	char colon; //to get the colon between hours and mins
	
	istringstream timeInStream(timeIn);
	timeInStream>>timeInHour>>colon>>timeInMins; //time in parsing
	
	return timeInHour;
}

bool officeHeadVerify(string officeHead, ifstream& officeHeadFile)
{
	bool isVerified = false;
	
	string password;
	cout<<endl<<"\tOffice Head's Password: ";
	cin>>password;	
	
	//find the office head name in officeHeadLogin file
	officeHead = '"' + officeHead + '"';
		
	//send file cursor back to the beginning
    officeHeadFile.clear();
    officeHeadFile.seekg(0, ios::beg);

    string line;
    while(getline(officeHeadFile, line))
    {
        //find the beginning of the line
        size_t startPosition = line.find(officeHead);

        //if the beginning is found, find and store the first set of words inside " "
        if(startPosition != string::npos)
        {
            size_t endPosition = startPosition;

            //find the first set of words inside " "
            if((startPosition = line.find('"', endPosition)) != string::npos)
            {
                //find the closing " "
                endPosition = line.find('"', startPosition + 1);

                //if both " " are found, extract and store the words inside " "
                if(endPosition != string::npos)
                {
                    // Extract and store the words inside " "
                    string extractedWord = line.substr(startPosition + 1, endPosition - startPosition - 1);

                    // Get the next word
                    stringstream wordStream(line.substr(endPosition + 1));
                    string nextWord;
                    wordStream >> nextWord;

                    // Check if the next word is the same as the inputted password
                    if (nextWord == password)
                    {
                        isVerified = true;
                    }
                    else
                    {
                        isVerified = false;
                    }
                }
            }
        }
    }
	
	return isVerified;	
}

void timeIn(string studentName, string studentId, string officeHead, string officeAssignment, bool dateFirstPrevious, fstream& file, ifstream& officeHeadFile)
{
	string lastLine = getLastLine(file);
	int wordCount = countWords(lastLine);
	
	file.seekg(0, ios::beg);
	
	string lastDate = getLastDate(file);
	
	string firstName = getFirstName(studentName);
	string missedTimeOut;
	
	//check last time out first
	if(wordCount == 9)
	{
		//may last time out
		recordTimeIn(studentName, studentId, officeAssignment, dateFirstPrevious, file);
	}
	else
	{
		char check;
		cout<<"\nOh no, "<<firstName<<"! It seems like you did not time out last time ("<<lastDate<<"). "; 
		do
		{		
			//missed time out - prompt student to enter missed time out
			cout<<"Please record your previous time out by entering the needed details below.\n\n";
			missedTimeOut = getMissedTime();
			
			do
			{
				//verify with office head
				cout<<"\nFor the office head, is the information above correct (Y/N)? ";
				cin>>check;
				
				check = toupper(check);
				
				if(check == 'N')
				{
					cout<<"\nResetting...\n\n";
				}
			}
			while(check != 'Y' && check != 'N');
		}
		while(check == 'N');
		
		string password;
		cout<<"\nTo record the previous time out, which is "<<missedTimeOut<<", the respective office head ("<<officeHead<<") needs to verify it by entering their password.\n";
		bool isVerified;
		do
		{		
			isVerified = officeHeadVerify(officeHead, officeHeadFile);
			if(isVerified == true)
			{
				system("CLS");
				cout<<endl<<"\nThank you. The password that you entered was correct.\n\n";
				cout<<"Processing...\n\n";
			}
			else
			{
				cout<<endl<<"\tIncorrect password. Please try again.\n";
			}
		}
		while(isVerified == false);
		
		//print missed time out into txt file
		file<<"\t\t"<<missedTimeOut;
		
		//compute no. of hours
		string dutyHours = computeHours(file);
		
		Sleep(400);
		system("CLS");
		
		cout<<"\n======== MISSED SCHOLAR TIME OUT ========\n";
		
		//print missed time out messages
		cout<<"\nHello "<<firstName<<"! Your previous time out has been recorded.\n\n";
		cout<<"Student's ID: "<<studentId<<endl;
		cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
		cout<<"Missed Time of Out: "<<missedTimeOut<<endl;
		cout<<"Hours of Duty: "<<dutyHours<<endl;
		cout<<"Don't forget to time out next time!\n";
		
		//after missed time out, ask
		char recordCurrentTimeInSel;
        cout<<"\n***\n\nWould you like to record the current time as your time in (Y/N)? ";
        cin>>recordCurrentTimeInSel;
        recordCurrentTimeInSel = toupper(recordCurrentTimeInSel);

        if(recordCurrentTimeInSel == 'Y')
        {
        	system("CLS");
        	cout<<"\n======== SCHOLAR TIME IN ========\n";
        	recordTimeIn(studentName, studentId, officeAssignment, dateFirstPrevious, file);
        }
        else if(recordCurrentTimeInSel == 'N')
        {
            cout<<"\nThank you, Scholar. Have a nice Day!\n\n";
        }
        else
        {
            cout<<"\nInvalid Input. Going Back to Main Menu. Missing time out has now been recorded. No new time-ins recorded.\n\n";
        }		
	}
}

void recordTimeIn(string studentName, string studentId, string officeAssignment, bool dateFirstPrevious, fstream& file)
{
	//move file pointer to the beginning
	file.clear();
	file.seekg(0, ios::beg);
	
	string dateToday = getDate();
	string lastDate = getLastDate(file);
	string timeIn = getCurrentTime();
	
	string firstName = getFirstName(studentName);
	
	//record time in
	if(dateToday != lastDate)
	{
		if(dateFirstPrevious == true)
		{
			//means nadetect na not same date pero possibly ay "DATE" string ang nadetect so blank file talaga
			file<<"\n";
		}
		else
		{
			//means diff day
			file<<"\n\n";
		}		
	}
	else 
	{
		//means same day
		file<<"\n";
	}
	
	//print current date and time in onto txt file
	file<<dateToday<<"\t\t"<<timeIn;
	
	//print time in messages
	cout<<"\nHi "<<firstName<<"! You have timed in.\n\n";
	cout<<"Student ID: "<<studentId<<endl;
	cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
	cout<<"Time of In: "<<timeIn<<endl;
	cout<<"Happy Working!\n\n";		
}

string computeHours(fstream& file)
{
	file.seekg(0, ios::beg);
	
	string lastLine = getLastLine(file);
	
	//get the time in and time out
	string date1, date2, date3, timeIn, timeOut;
	istringstream dutyStream(lastLine);
	dutyStream>>date1>>date2>>date3>>timeIn>>timeOut;
	
	//get the time in hour, time in mins, time out hour, time out mins
	int timeInHour, timeInMins, timeOutHour, timeOutMins;
	char colon; //to get the colon between hours and mins
	
	istringstream timeInStream(timeIn);
	timeInStream>>timeInHour>>colon>>timeInMins; //time in parsing
	
	istringstream timeOutStream(timeOut);
	timeOutStream>>timeOutHour>>colon>>timeOutMins; //time out parsing
	
	//compute the hours of duty	
	int totalMins = (timeOutHour * 60 + timeOutMins) - (timeInHour * 60 + timeInMins);
	int dutyHour = totalMins / 60;
	int dutyMins = totalMins % 60;
	
	//convert to string
	stringstream convert;
	convert<<dutyHour;
	string dutyHourStr = convert.str();
	convert.str("");
	convert<<dutyMins;
	string dutyMinsStr = convert.str();
	
	//get string
	string dutyHours;
	//hour string
	if(dutyHour > 1)
	{
		dutyHours = dutyHourStr + " hours, ";
	}
	else if(dutyHour == 1 || dutyHour == 0)
	{
		dutyHours = dutyHourStr + " hour, ";	
	}
	//mins string
	if(dutyMins > 1)
	{
		dutyHours += dutyMinsStr + " minutes";
	}
	else if(dutyMins == 1 || dutyMins == 0)
	{
		dutyHours += dutyMinsStr + " minute";
	}
	
	//print hours onto txt file
	file<<"\t\t"<<dutyHours;
	
	return dutyHours;	
}

void timeOut(string studentName, string studentId, string officeHead, string officeAssignment, fstream& file, ifstream& officeHeadFile)
{
	string lastLine = getLastLine(file);
	int wordCount = countWords(lastLine);
	
	int lastTimeInHour = getLastTimeInHour(file);
	bool isTimeOverLunch = overLunchTime();
	string dateToday = getDate();
	file.seekg(0, ios::beg);
	string lastDate = getLastDate(file);
	
	string firstName = getFirstName(studentName);
	string missedDate, missedTimeIn, missedTimeOut;
	
	//check last time in first
	if(wordCount == 4)
	{
		if(dateToday == lastDate)
		{
			if(lastTimeInHour < 12)
			{
				if(isTimeOverLunch == true)
				{
					//confirmation if they worked over lunch
					char ans = ' ';
					do
					{
						cout<<"\nYour last time in hour was before 12:00 pm. Did you continued rendering your duty during/after lunch time (12:00 pm - 1:00 pm) and only just finished now (Y/N)? ";
						cin>>ans;
						ans = toupper(ans);
						
						if(ans == 'Y')
						{
							//record time out since nagduty daw after lunch
							cout<<"\nThank you for the confirmation. Your time out will now be recorded.\n\nProcessing...\n";
							
							Sleep(400);
							system("CLS");
							
							cout<<"\n======== SCHOLAR TIME OUT ========\n";							
							recordTimeOut(studentName, studentId, officeAssignment, file);
						}
						else if(ans == 'N')
						{
							char check;
							//means missed time out for previous session and missed time in for current session
							cout<<"\nOh no, "<<firstName<<"! It seems like you did not time out last time ("<<lastDate<<"). ";
							do
							{
								cout<<"Please record your previous time out by entering the needed details below.\n\n";
								missedTimeOut = getMissedTime();
								
								//missed time in - prompt student to enter missed time in
								cout<<"\nYou selected the option to time out, but it seems like you also forgot to time in for your current duty session. Please record your missed time in by entering the needed details below.\n\n";
								missedTimeIn = getMissedTime();
								
								do
								{
									//verify with office head
									cout<<"\nFor the office head, is the information above correct (Y/N)? ";
									cin>>check;
									
									check = toupper(check);
									
									if(check == 'N')
									{
										cout<<"\nResetting...\n\n";
									}
								}
								while(check != 'Y' && check != 'N');
							}
							while(check == 'N');
							
							//verify with office head
							string password;
							cout<<"\nTo record the missed time out which is "<<missedTimeOut<<" on "<<lastDate<<", as well as the missed time in today which is "<<missedTimeIn<<", the respective office head ("<<officeHead<<") needs to verify it by entering their password.\n";
							bool isVerified;
							do
							{
								isVerified = officeHeadVerify(officeHead, officeHeadFile);
								if(isVerified == true)
								{
									system("CLS");
									cout<<endl<<"\nThank you. The password that you entered was correct.\n\n";
									cout<<"Processing...\n\n";
								}
								else
								{
									cout<<endl<<"\tIncorrect password. Please try again.\n";
								}
							}
							while(isVerified == false);
							
							//record/print missed time out into txt file
							file<<"\t\t"<<missedTimeOut;
							
							//compute no. of hours
							string dutyHours = computeHours(file);
							
							//print time in onto txt file						
							file<<"\n"<<dateToday<<"\t\t"<<missedTimeIn;
							
							Sleep(400);
							system("CLS");
							
							cout<<"\n======== MISSED SCHOLAR TIME OUT & TIME IN ========\n";
							
							//print messages
							cout<<"\nHello "<<firstName<<"! Your previous time out has been recorded.\n\n";
							cout<<"Student's ID: "<<studentId<<endl;
							cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
							cout<<"Missed Time of Out: "<<missedTimeOut<<endl;
							cout<<"Hours of Duty: "<<dutyHours<<endl;
							cout<<"\nYour latest time in has also been recorded.\n\n";
							cout<<"Student's ID: "<<studentId<<endl;
							cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
							cout<<"Missed Time of In: "<<missedTimeIn<<endl;
							cout<<"Don't forget to time in and time out next time!\n";
							
							//after missed time in, ask
							char recordCurrentTimeOutSel;
					        cout<<"\n***\n\nWould you like to record the current time as your time out (Y/N)? ";
					        cin>>recordCurrentTimeOutSel;
					        recordCurrentTimeOutSel = toupper(recordCurrentTimeOutSel);
					
					        if(recordCurrentTimeOutSel == 'Y')
					        {
					        	system("CLS");
        						cout<<"\n======== SCHOLAR TIME OUT ========\n";
					        	recordTimeOut(studentName, studentId, officeAssignment, file);
					        }
					        else if(recordCurrentTimeOutSel == 'N')
					        {
					            cout<<"\nThank you, Scholar. Have a nice Day!\n\n";
					        }
					        else
					        {
					            cout<<"\nInvalid Input. Going Back to Main Menu. Missing time in has now been recorded. No new time-outs recorded.\n\n";
					        }	
						}
						else
						{
							cout<<"Invalid input. Please try again.\n";
							cin.clear();
        					cin.ignore(numeric_limits<streamsize>::max(), '\n');
						}
					}
					while(ans != 'Y' && ans != 'N');
				}
				else
				{
					//last time in was umaga pero hindi pa naman lunch time nag-time out
					recordTimeOut(studentName, studentId, officeAssignment, file);
				}
			}
			else
			{
				//last time in was hapon na or after lunch so record time out na lang
				recordTimeOut(studentName, studentId, officeAssignment, file);
				
			}			
		}
		else
		{
			//may last time in but diff day, so missed time out noong last date then missed time in ngayong current date
			//missed time out - prompt student to enter missed time out
			cout<<"\nOh no, "<<firstName<<"! It seems like you did not time out last time ("<<lastDate<<"). Please record your previous time out by entering the needed details below.\n\n";
			missedTimeOut = getMissedTime();
			
			//missed time in - prompt student to enter missed time in
			cout<<"\nYou selected the option to time out, but it seems like you also forgot to time in for your current duty session. Please record your missed time in by entering the needed details below.\n\n";
			missedDate = getMissedDate();
			missedTimeIn = getMissedTime();
			
			//verify with office head
			string password;
			cout<<"\nTo record your missed time out which is "<<missedTimeOut<<" on "<<lastDate<<", as well as your missed time on "<<missedDate<<" which is "<<missedTimeIn<<", your respective office head ("<<officeHead<<") needs to verify it by entering their password.\n";
			bool isVerified;	
			do
			{
				isVerified = officeHeadVerify(officeHead, officeHeadFile);
				if(isVerified == true)
				{
					system("CLS");
					cout<<endl<<"\nThank you. The password that you entered was correct.\n\n";
					cout<<"Processing...\n\n";
				}
				else
				{
					cout<<endl<<"\tIncorrect password. Please try again.\n\n";
				}
			}
			while(isVerified == false);
			
			//record/print missed time out into txt file
			file<<"\t\t"<<missedTimeOut;
			
			//compute no. of hours
			string dutyHours = computeHours(file);
					
			//print time in onto txt file
			file<<"\n\n"<<missedDate<<"\t\t"<<missedTimeIn;
			
			Sleep(400);
			system("CLS");
			
			cout<<"\n======== MISSED SCHOLAR TIME OUT & TIME IN ========\n";
			
			//print messages
			cout<<"\nHello "<<firstName<<"! Your previous time out has been recorded.\n\n";
			cout<<"Student's ID: "<<studentId<<endl;
			cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
			cout<<"Missed Time of Out: "<<missedTimeOut<<endl;
			cout<<"Hours of Duty: "<<dutyHours<<endl;
			cout<<"\nYour latest time in has also been recorded.\n\n";
			cout<<"Student's ID: "<<studentId<<endl;
			cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
			cout<<"Missed Time of In: "<<missedTimeIn<<endl;
			cout<<"Don't forget to time in and time out next time!\n";
			
			//after missed time in, ask
			char recordCurrentTimeOutSel;
	        cout<<"\n***\n\nWould you like to record the current time as your time out (Y/N)? ";
	        cin>>recordCurrentTimeOutSel;
	        recordCurrentTimeOutSel = toupper(recordCurrentTimeOutSel);
	
	        if(recordCurrentTimeOutSel == 'Y')
	        {
	        	system("CLS");
        		cout<<"\n======== SCHOLAR TIME OUT ========\n";
	        	recordTimeOut(studentName, studentId, officeAssignment, file);
	        }
	        else if(recordCurrentTimeOutSel == 'N')
	        {
	            cout<<"\nThank you, Scholar. Have a nice Day!\n\n";
	        }
	        else
	        {
	            cout<<"\nInvalid Input. Going Back to Main Menu. Missing time in has now been recorded. No new time-outs recorded.\n\n";
	        }	
		}
	}
	else
	{
		char check;
		//missed time in - prompt student to enter missed time in
		cout<<"\nOh no, "<<firstName<<"! It seems like you did not time in last time. ";
		do
		{
			cout<<"Please record your previous time in by entering the needed details below.\n\n";
			//get missed date, and missed time in
			missedDate = getMissedDate();
			missedTimeIn = getMissedTime();
			
			do
			{
				//verify with office head
				cout<<"\nFor the office head, is the information above correct (Y/N)? ";
				cin>>check;
				
				check = toupper(check);
				
				if(check == 'N')
				{
					cout<<"\nResetting...\n\n";
				}
			}
			while(check != 'Y' && check != 'N');
		}
		while(check == 'N');
	
		//verify with office head
		string password;
		cout<<"\nTo record the previous time in, which is "<<missedTimeIn<<" on "<<missedDate<<", the respective office head ("<<officeHead<<") needs to verify it by entering their password.\n";
		bool isVerified;
		do
		{
			isVerified = officeHeadVerify(officeHead, officeHeadFile);
			if(isVerified == true)
			{
				system("CLS");
				cout<<endl<<"\nThank you. The password that you entered was correct.\n\n";
				cout<<"Processing...\n\n";
			}
			else
			{
				cout<<endl<<"\tIncorrect password. Please try again.\n";
			}
		}
		while(isVerified == false);
		
		//record missed time in
		if(missedDate != lastDate)
		{
			//means diff days
			file<<"\n\n";
		}
			else
		{
			//means same day
			file<<"\n";
		}
		
		//print missed date and missed time in onto txt file
		file<<missedDate<<"\t\t"<<missedTimeIn;
		
		Sleep(400);
		system("CLS");
		
		cout<<"\n======== MISSED SCHOLAR TIME IN ========\n";
		
		//print missed time in messages
		cout<<"\nHello "<<firstName<<"! Your previous time in has been recorded.\n\n";
		cout<<"Student's ID: "<<studentId<<endl;
		cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
		cout<<"Missed Time of In: "<<missedTimeIn<<endl;
		cout<<"Don't forget to time in next time!\n";
		
		//after missed time out, ask
		char recordCurrentTimeOutSel;
        cout<<"\n***\n\nWould you like to record the current time as your time out (Y/N)? ";
        cin>>recordCurrentTimeOutSel;
        recordCurrentTimeOutSel = toupper(recordCurrentTimeOutSel);

        if(recordCurrentTimeOutSel == 'Y')
        {
        	system("CLS");
        	cout<<"\n======== SCHOLAR TIME OUT ========\n";
        	recordTimeOut(studentName, studentId, officeAssignment, file);
        }
        else if(recordCurrentTimeOutSel == 'N')
        {
            cout<<"\nThank you, Scholar. Have a nice Day!\n\n";
        }
        else
        {
            cout<<"\nInvalid Input. Going Back to Main Menu. Missing time in has now been recorded. No new time-outs recorded.\n\n";
        }	
	}
}

void recordTimeOut(string studentName, string studentId, string officeAssignment, fstream& file)
{	
	string timeOut = getCurrentTime();
	string firstName = getFirstName(studentName);
	
	//print time out onto txt file
	file<<"\t\t"<<timeOut;
	
	//compute no. of hours
	string dutyHours = computeHours(file);  
	
	//print missed time out messages
	cout<<"\nThank you "<<firstName<<"! You have timed out.\n\n";
	cout<<"Student ID: "<<studentId<<endl;
	cout<<"Office/Area Assignment: "<<officeAssignment<<endl;
	cout<<"Time of Out: "<<timeOut<<endl;
	cout<<"Hours of Duty: "<<dutyHours<<endl;
	cout<<"Good work!\n\n";
}

void viewMyRecords(string studentId)
{
	bool isFileOpen = false;
	do
	{
		// File and variable initializations 
		ifstream myRecFile;
		string month, fileName, line;
		
		// Ask user to input month
		cout<<"\nEnter the month of record that you want to retrieve (ex. january): ";
		cin>>month;
		
		// Create the filename
		fileName = studentId + "_" + month + ".txt";
		
		// Open the file
		myRecFile.open(fileName.c_str());
		
		// Check if file has opened successfully
		if (!myRecFile.is_open())
		{
			// Display unsuccessful file opening and repeat loop
			cout<<"\nYour record file cannot be found. Your input might be incorrect or the file does not exits yet. Please try again.\n";
			isFileOpen = false;
		}
		else
		{
			isFileOpen = true;
			// Read all student's record in the file
			cout<<"\n____________________________________________________________________________________\n\n";
			while (!myRecFile.eof())
			{
				getline(myRecFile, line);
				cout<<line<<endl;
			}
			cout<<"\n____________________________________________________________________________________\n\n";
		}
		
		// Close the file
		myRecFile.close();
	}
	while(isFileOpen == false);
}

bool adminLogin()
{
	// File and variable initializations
	ifstream adminAccFile;
	string adminUnameRef, adminPassRef, adminUname, adminPass;
	bool Login = false;
	
	// Ask user to input username and password
	cout<<"\nEnter admin username: ";
	cin>>adminUnameRef;
	cout<<"Enter admin password: ";
	cin>>adminPassRef;
	
	// Open the file
	adminAccFile.open("AdminAccounts.txt");
	
	// Check if file has opened successfully
	if (!adminAccFile.is_open()) 
	{
		cout<<"\nThere is an ERROR opening the file.\n";
	}
	
	// Read username and password infomation from txt file
	while (adminAccFile>>adminUname>>adminPass)
	{
		if (adminUname == adminUnameRef)
		{
			if (adminPass == adminPassRef)
			{
				Login = true;
			}
		}
	}
	
	// Close the file
	adminAccFile.close();
	
	// Return value
	return Login;	
}

int displayAdminMenu()
{
	// Variable Initialization
	int menuSel;
	
	// Display Admin Menu Selection
	cout<<"\n\n------------------ ADMIN MENU ------------------\n";
	cout<<"\n1 - View a specific student's record\n";
	cout<<"2 - Add a new student and their information\n";
	cout<<"\n------------------------------------------------\n\n";
	
	// Ask user to input selection
	cout<<"Enter your selection: ";
	cin>>menuSel;
	
	// Return value
	return menuSel;
}

void viewStudentRecords()
{
	bool isFileOpen = false;
	do
	{
		// File and variable initializations
		ifstream studRecFile;
		string studentId, monthDuty, filename, line;
		
		// Ask user to input username and password
		int studIdDigits = 0;
		do
		{
			cout<<"Enter student ID (10 digits): ";
			cin>>studentId;
			
			studIdDigits = studentId.length();
		}
		while(studIdDigits != 10);
		
		cout<<"Enter student's month of duty: ";
		cin>>monthDuty;
		
		transform(monthDuty.begin(), monthDuty.end(), monthDuty.begin(), ::tolower);
		
		// Create the filename
		filename = studentId + "_" + monthDuty + ".txt";
		
		// Open the file
		studRecFile.open(filename.c_str());
		
		// Check if file has opened successfully
		if (!studRecFile.is_open())
		{
			// Display unsuccessful file opening and repeat loop
			cout<<"\nThe student's record file cannot be found. Please try again\n\n";
			isFileOpen = false;
		}
		else
		{
			isFileOpen = true;
			// Read all student's record in the file then print
			cout<<"\n____________________________________________________________________________________\n\n";
			while (!studRecFile.eof())
			{
				getline(studRecFile, line);
				cout<<line<<endl;
			}
			cout<<"\n____________________________________________________________________________________\n\n";
		}
		
		// Close the file
		studRecFile.close();
	}
	while(isFileOpen == false);
}

void addNewStudent(fstream& newStudInfoFile)
{
	char check;
	do
	{
		// Variable initializations
		string studId, studPass, studName, studOffAss, studOffHead, month, fileInfo, fileRecName;
		int studIdDigits = 0;
		int studPassDigits = 0;
		
		// Ask user to input all information
		do
		{
			cout<<"Enter student's ID number (10 digits): ";
			cin.sync();
			getline(cin, studId, '\n');
			
			studIdDigits = studId.length();
		}
		while(studIdDigits != 10);
		do
		{
			cout<<"Enter student's password (13-15 digits): ";
			cin.sync();
			getline(cin, studPass, '\n');
			
			studPassDigits = studPass.length();
		}
		while(studPassDigits != 13 && studPassDigits != 14 && studPassDigits != 15);	
		cout<<"Enter student's full name: ";
		cin.sync();
		getline(cin, studName, '\n');
		cout<<"Enter student's office/area assignment: ";
		cin.sync();
		getline(cin, studOffAss, '\n');
		cout<<"Enter student's office head: ";
		cin.sync();
		getline(cin, studOffHead, '\n');
		
		// Display all inputs for user confirmation
		cout<<"\n\tStudent ID: "<<studId<<"\n\tPassword: "<<studPass<<"\n\tStudent Name: "<<studName<<"\n\tOffice/Area Assignment: "<<studOffAss<<"\n\tOffice Head: "<<studOffHead<<"\n";
		
		do
		{
			cout<<"\nIs this information correct (Y/N)? ";
			cin>>check;
			
			check = toupper(check);
		}
		while(check != 'Y' && check != 'N');
		
		// Checking student information
		if(check == 'Y')
		{	
			studName = '"' + studName + '"';
			studOffAss = '"' + studOffAss + '"';
			studOffHead = '"' + studOffHead + '"';
		
			// Add student information to the student info file
			newStudInfoFile<<setw(12)<<left<<studId<<"    ";
			newStudInfoFile<<setw(17)<<left<<studPass<<"    ";
			newStudInfoFile<<setw(33)<<left<<studName<<"    ";
			newStudInfoFile<<setw(33)<<left<<studOffAss<<"    ";
			newStudInfoFile<<setw(33)<<left<<studOffHead<<"    \n";
	
			// Display student info is added
			cout<<"\nThe new student's information was successfully added to the system.\n";
		}
		else
		{
			// Display unsuccessful file opening and repeat loop
			cout<<"\nPlease input the student's information again.\n\n";
		}
	}
	while(check == 'N');
}
