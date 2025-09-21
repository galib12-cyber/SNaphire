#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>

using namespace std;

// Check if file exists (cross-platform)
bool fileExists(const string &filename)
{
#ifdef _WIN32
    struct _stat buffer;
    return (_stat(filename.c_str(), &buffer) == 0);
#else
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
#endif
}

// Function to clear the console screen cross-platform
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Prints a line of text centered with newline
void printCentered(const string &text, int consoleWidth = 80, int contentWidth = 40)
{
    int padding = (consoleWidth - contentWidth) / 2;
    string pad(padding, ' ');
    cout << pad << setw(contentWidth) << left << text << endl;
}

// Prints a prompt centered on the line without newline, so input cursor stays on the same line
void printCenteredPrompt(const string &text, int consoleWidth = 80, int contentWidth = 40)
{
    int padding = (consoleWidth - contentWidth) / 2;
    string pad(padding, ' ');
    cout << pad << text;
    cout.flush();
}

// Menu class handles menu display and input
class Menu
{
    int consoleWidth;
    int menuWidth;
    int padding;
    string pad;

public:
    Menu(int cWidth = 80, int mWidth = 40)
        : consoleWidth(cWidth), menuWidth(mWidth)
    {
        padding = (consoleWidth - menuWidth) / 2;
        pad = string(padding, ' ');
    }

    int show()
    {
        int m;
        cout << pad << string(menuWidth, '=') << endl;
        cout << pad << setw((menuWidth + 8) / 2) << "MAIN MENU" << endl;
        cout << pad << string(menuWidth, '=') << endl;

        cout << pad << left << setw(menuWidth) << "1. User login" << endl;
        cout << pad << left << setw(menuWidth) << "2. User signup" << endl;
        cout << pad << left << setw(menuWidth) << "3. Quit" << endl;

        cout << pad << string(menuWidth, '=') << endl;

        printCenteredPrompt("Choose option (1-3): ", consoleWidth, menuWidth);
        while (!(cin >> m) || m < 1 || m > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printCenteredPrompt("Invalid input. Please enter a number 1-3: ", consoleWidth, menuWidth);
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline for next getline
        return m;
    }
};

// SecondMenu class for the secondary menu
class SecondMenu
{
    int consoleWidth;
    int menuWidth;
    int padding;
    string pad;

public:
    SecondMenu(int cWidth = 80, int mWidth = 40)
        : consoleWidth(cWidth), menuWidth(mWidth)
    {
        padding = (consoleWidth - menuWidth) / 2;
        pad = string(padding, ' ');
    }

    int show()
    {
        int m;
        cout << pad << string(menuWidth, '=') << endl;
        cout << pad << setw((menuWidth + 10) / 2) << "USER MENU" << endl;
        cout << pad << string(menuWidth, '=') << endl;

        cout << pad << left << setw(menuWidth) << "1. Services" << endl;
        cout << pad << left << setw(menuWidth) << "2. Support" << endl;
        cout << pad << left << setw(menuWidth) << "3. About Us" << endl;
        cout << pad << left << setw(menuWidth) << "4. Logout" << endl;

        cout << pad << string(menuWidth, '=') << endl;
        printCenteredPrompt("Choose option (1-4): ", consoleWidth, menuWidth);
        while (!(cin >> m) || m < 1 || m > 4)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printCenteredPrompt("Invalid input. Please enter a number 1-4: ", consoleWidth, menuWidth);
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear newline
        return m;
    }
};

// ServicesMenu class for the services submenu
class ServicesMenu
{
    int consoleWidth;
    int menuWidth;
    int padding;
    string pad;

public:
    ServicesMenu(int cWidth = 80, int mWidth = 40)
        : consoleWidth(cWidth), menuWidth(mWidth)
    {
        padding = (consoleWidth - menuWidth) / 2;
        pad = string(padding, ' ');
    }

    int show()
    {
        int m;
        cout << pad << string(menuWidth, '=') << endl;
        cout << pad << setw((menuWidth + 12) / 2) << "SERVICES MENU" << endl;
        cout << pad << string(menuWidth, '=') << endl;

        cout << pad << left << setw(menuWidth) << "1. Browse jobs" << endl;
        cout << pad << left << setw(menuWidth) << "2. Post a Job" << endl;
        cout << pad << left << setw(menuWidth) << "3. Back" << endl;

        cout << pad << string(menuWidth, '=') << endl;
        printCenteredPrompt("Choose option (1-3): ", consoleWidth, menuWidth);
        while (!(cin >> m) || m < 1 || m > 3)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            printCenteredPrompt("Invalid input. Please enter a number 1-3: ", consoleWidth, menuWidth);
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return m;
    }
};

// UserManager handles users storage, login, and signup - now with file system
class UserManager
{
    int consoleWidth = 80;
    int contentWidth = 40;

    void printUserMsg(const string &msg)
    {
        int padding = (consoleWidth - contentWidth) / 2;
        string pad(padding, ' ');
        cout << pad << setw(contentWidth) << left << msg << endl;
    }

    void printUserPrompt(const string &prompt)
    {
        int padding = (consoleWidth - contentWidth) / 2;
        string pad(padding, ' ');
        cout << pad << prompt;
        cout.flush();
    }

    string sanitizeFilename(const string &id)
    {
        string filename = id;
        for (char &c : filename)
        {
            if (!isalnum(c))
                c = '_';
        }
        return filename + ".txt";
    }

public:
    UserManager() {}

    void signup()
    {
        string temp_id, temp_pss;
        bool valid = false;
        do
        {
            printUserPrompt("User ID: ");
            getline(cin, temp_id);

            if (temp_id.length() < 3)
            {
                printUserMsg("User ID must contain at least 3 characters!");
                printUserMsg("Press ENTER to try again!");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            string filename = sanitizeFilename(temp_id);
            if (fileExists(filename))
            {
                printUserMsg("User ID already exists! Try a different User ID!");
                printUserMsg("Press ENTER to try again!");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            valid = true;
        } while (!valid);

        bool valid_pss = false;
        do
        {
            printUserPrompt("Password: ");
            getline(cin, temp_pss);
            if (temp_pss.length() < 4)
            {
                printUserMsg("Password must be at least 4 characters or numbers!");
                printUserMsg("Press ENTER to try again!");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else
            {
                valid_pss = true;
            }
        } while (!valid_pss);

        string filename = sanitizeFilename(temp_id);
        ofstream fout(filename);
        if (fout)
        {
            fout << temp_pss << "\n";
            fout.close();
            printUserMsg("Signup successful! You can now login.");
        }
        else
        {
            printUserMsg("Error: Could not create user file.");
        }
    }

    bool checkId()
    {
        string id1, pss1;
        printUserPrompt("User ID: ");
        getline(cin, id1);
        printUserPrompt("Password: ");
        getline(cin, pss1);

        string filename = sanitizeFilename(id1);
        if (!fileExists(filename))
        {
            return false;
        }

        ifstream fin(filename);
        if (!fin)
        {
            return false;
        }

        string storedPassword;
        getline(fin, storedPassword);
        fin.close();

        return (pss1 == storedPassword);
    }
};

// Function to handle posting a job ( GALIB )
void postAJob()
{
    string profession, name, gender, contact, address, fees, jobdesc;

    clearScreen();
    printCentered("POST A JOB", 80, 40);
    cout << endl;

    printCenteredPrompt("Profession: ");
    getline(cin >> ws, profession);

    printCenteredPrompt("Name: ");
    getline(cin, name);

    printCenteredPrompt("Gender: ");
    getline(cin, gender);

    printCenteredPrompt("Contact: ");
    getline(cin, contact);

    printCenteredPrompt("Address: ");
    getline(cin, address);

    printCenteredPrompt("Fees: ");
    getline(cin, fees);

    printCenteredPrompt("Job Description: ");
    getline(cin, jobdesc);

    string filename = profession;
    transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    for (char &c : filename)
        if (!isalnum(c))
            c = '_';
    filename += ".txt";

    ofstream fout(filename, ios::app);
    if (fout)
    {
        fout << "-----------------------------\n";
        fout << "Profession: " << profession << "\n";
        fout << "Name: " << name << "\n";
        fout << "Gender: " << gender << "\n";
        fout << "Contact: " << contact << "\n";
        fout << "Address: " << address << "\n";
        fout << "Fees: " << fees << "\n";
        fout << "Job Description: " << jobdesc << "\n";
        fout << "-----------------------------\n\n";
        fout.close();
        printCentered("Job posted successfully!", 80, 40);
    }
    else
    {
        printCentered("Error: Could not save job.", 80, 40);
    }
    cout << endl;
    printCenteredPrompt("Press ENTER to continue...");
    cin.get();
}

// Function to handle browsing jobs
void browseJobs()
{
    string profession;
    clearScreen();
    printCentered("BROWSE JOBS", 80, 40);
    cout << endl;

    printCenteredPrompt("Enter profession/job name: ");
    getline(cin >> ws, profession);

    string filename = profession;
    transform(filename.begin(), filename.end(), filename.begin(), ::tolower);
    for (char &c : filename)
        if (!isalnum(c))
            c = '_';
    filename += ".txt";

    ifstream fin(filename);
    if (fin)
    {
        printCentered("Job Listings:", 80, 40);
        cout << endl;
        string line;
        while (getline(fin, line))
        {
            printCentered(line, 80, 40);
        }
        fin.close();
    }
    else
    {
        printCentered("No jobs found for this profession.", 80, 40);
    }
    cout << endl;
    printCenteredPrompt("Press ENTER to continue...");
    cin.get();
}

int main()
{
    Menu menu;
    SecondMenu userMenu;
    ServicesMenu servicesMenu;
    UserManager manager;

    while (true)
    {
        clearScreen();

        int option = menu.show();

        clearScreen();

        if (option == 1)
        {
            if (manager.checkId())
            {
                printCentered("Login successful!");
                cout << endl;
                printCenteredPrompt("Press ENTER to continue...");
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();

                while (true)
                {
                    clearScreen();
                    int userOpt = userMenu.show();

                    clearScreen();

                    if (userOpt == 1)
                    {
                        while (true)
                        {
                            clearScreen();
                            int servOpt = servicesMenu.show();
                            clearScreen();
                            if (servOpt == 1)
                            {
                                browseJobs();
                            }
                            else if (servOpt == 2)
                            {
                                postAJob();
                            }
                            else if (servOpt == 3)
                            {
                                break;
                            }
                        }
                    }
                    else if (userOpt == 2)
                    {
                        clearScreen();
                        printCentered("SUPPORT", 80, 40);
                        cout << endl;
                        printCentered("If you face any kind of problem contact us at---", 80, 40);
                        cout << endl;
                        cout << endl;
                        printCentered("Phone : 018********", 80, 40);
                        printCentered("E-mail : snaphire@gmail.com", 80, 40);
                        printCentered("Address : IIUC Library, Chittagong", 80, 40);
                        cout << endl;
                        printCenteredPrompt("Press ENTER to continue...");
                        cin.get();
                    }
                    else if (userOpt == 3)
                    {
                        clearScreen();
                        printCentered("ABOUT US", 80, 40);
                        cout << endl;
                        printCentered("1. MD SAYEED ABRAR AQIL (C243067)", 80, 40);
                        cout << endl;
                        printCentered("2. SAYED MOHAMMAD OMAR (C243004)", 80, 40);
                        cout << endl;
                        printCentered("3. SHAJIDUL HOQUE GALIB (C243105)", 80, 40);
                        cout << endl;
                        printCentered("4. SAWAD SARWAR TANMOY (C243144)", 80, 40);
                        cout << endl;
                        printCenteredPrompt("Press ENTER to continue...");
                        cin.get();
                    }
                    else if (userOpt == 4)
                    {
                        printCentered("Logging out...");
                        cout << endl;
                        printCenteredPrompt("Press ENTER to continue...");
                        cin.get();
                        break;
                    }
                }
            }
            else
            {
                printCentered("Login failed!");
                cout << endl;
                printCenteredPrompt("Press ENTER to continue...");
                cin.get();
            }
        }
        else if (option == 2)
        {
            manager.signup();
            cout << endl;
            printCenteredPrompt("Press ENTER to continue...");
            cin.get();
        }
        else if (option == 3)
        {
            printCentered("Goodbye!");
            break;
        }
    }
    return 0;
}
