// Cooper Urich (4518739)
// Assignment 3
// COP 4600

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <sys/wait.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

class mysh
{
private:
    string currentDir;
    vector<string> commandHistory;
    vector<pid_t> pid_history;
    pid_t pid;

public:
    mysh(string dir)
    {
        currentDir = dir;
        commandHistory.clear();
        pid_history.clear();
    };

    void setPID(pid_t PID)
    {
        pid = PID;
    }

    pid_t getPID()
    {
        return pid;
    }

    // returns current directory
    string getShellDirectory()
    {
        return currentDir;
    }

    // sets currently directory (exact path)
    void setShellDir(string dir)
    {
        currentDir = dir;
    }

    // clears PID history vector
    void clearPIDHistory()
    {
        pid_history.clear();
    }
    // Changes the current directory by adding the next path (relative path)
    void appendShellDir(string toAppend)
    {
        currentDir.append(toAppend);
    }

    // adds the command to the history
    void recordCommand(string command)
    {
        commandHistory.insert(commandHistory.begin(), command);
    }

    // adds the pid to the pid_history vector
    void pushPID(pid_t pid)
    {
        pid_history.push_back(pid);
    }

    // reverse cycles through command history vector and prints it to the console
    void printCommandHistory()
    {

        int size = commandHistory.size();

        if (size == 0)
            return;

        cout << "Command Shell History\n";
        for (int i = 0; i < size; i++)
        {
            cout << i << ": " << commandHistory.at(i) << "\n";
        }
    }
    // writes to file
    void writeToFile()
    {

        // loads in History.txt file
        ofstream MyFile("History.txt");

        int size = commandHistory.size();

        if (size == 0)
            return;

        // writes the commands put in from the command history vector
        cout << "Command Shell History\n";
        for (int i = 0; i < size - 1; i++)
        {
            MyFile << (i) << ": " << commandHistory.at(i) << "\n";
        }
        MyFile.close();
    }

    // Loads in previous commands from history file so we keep track of all previous commands.
    void readFromFile()
    {
        string line;
        string insert;
        ifstream MyFile("History.txt");
        vector<string> commands;

        if (MyFile.is_open())
        {
            while (MyFile)
            {
                getline(MyFile, line);
                if (line.length() != 0)
                {
                    insert = line.substr(3, line.length());
                    commands.insert(commands.begin(), insert);
                    // recordCommand(insert);
                }
            }
            for (int i = 0; i < commands.size(); i++)
            {
                recordCommand(commands.at(i));
            }
        }
    }

    string getReplay(int num)
    {
        return commandHistory.at(num);
    }

    // Need to make a separate function for this because
    void eraseCommandHistory()
    {
        commandHistory.clear();
    }

    int PIDHistorySize()
    {
        return pid_history.size();
    }
    pid_t getPIDAt(int i)
    {
        return pid_history.at(i);
    }
    int getCmndHstrySize()
    {
        return commandHistory.size();
    }
};

// returns current directory
mysh WhereAmI(mysh shell)
{
    cout << shell.getShellDirectory();
    return shell;
}

// Function to convert a stirng of numbers into an actual integer
// when castinf=g to pid_t, you have to cast FROM an int
int stringToInt(string inputString)
{
    double exp = 0;
    int val = 0;

    for (int size = inputString.size() - 1; size >= 0; size--)
    {
        val += ((int)inputString[size] - 48) * pow(10, exp);
        exp++;
    }

    return val;
}

// code inspried from stack overflow
char **stringToVectorToCharArray(string inputString)
{
    int i, j;
    stringstream ss(inputString);
    vector<string> tokens;

    int size = tokens.size();
    int newSize = 0;
    char **args = new char *[size + 1];
    tokens.clear();

    // Turns the input into a vecotr
    while (ss >> inputString)
    {
        tokens.push_back(inputString);
    }

    // combintion of using arrays and .at() methods to conver
    args[0] = new char[tokens.at(0).size()];
    strcpy(args[0], tokens.at(0).c_str());

    j = 0;
    for (i = 1; i < size; i++)
    {
        j++;
        args[i] = new char[tokens.at(i).size()];
        strcpy(args[i], tokens.at(i).c_str());
    }

    args[j + 1] = (char *)0;

    return args;
}

mysh repeat(mysh shell, int num, string str)
{
    int temp = 0;
    stringstream ss(str);
    string command;
    string argument = "";
    string parameters;
    vector<pid_t> pidList;
    bool relative = false;

    // essentially the same code as start()

    ss >> argument;
    string executeString = "";
    const char *executeChar;
    int childEscapeStatus;

    if (argument[0] == '/')
    {
        //Absolute
        executeString = argument;
    }

    else
    {
        //Relative
        executeString = shell.getShellDirectory() + '/' + argument;
    }

    executeChar = executeString.c_str();

    // fork() the files "num" times
    for (int i = 0; i < num; i++)
    {

        // access() returns '0' if path exists
        if (access(executeChar, F_OK) != 0)
        {
            cout << "File does not exist\n";
            return shell;
        }

        pid_t PID = fork();

        //If its a child process, run the program
        if (PID == 0)
        {

            // read in the parameters
            while (ss >> parameters)
            {
                argument.append(" ");
                argument.append(parameters);
            }

            char **args = stringToVectorToCharArray(argument);
            execv(executeChar, args);
        }

        //If its an error, return
        else if (PID < 0)
        {
            cout << "Failed to fork, returning \n";
            return shell;
        }

        //If its a parent process, wait for the child process to finish
        else
        {
            temp = 1;
            waitpid(PID, &childEscapeStatus, WUNTRACED);
        }

        if (temp == 1)
        {
            pidList.push_back(PID);
        }
    }

    cout << "PIDs: ";
    int size = pidList.size();

    // print out the PIDS that you just ran
    for (int i = 0; i < size; i++)
    {
        if (i == (size - 1))
        {
            cout << pidList.at(i) << endl;
        }
        else
        {
            cout << pidList.at(i) << ", ";
        }
    }
    return shell;
}
mysh start(mysh shell, string argument)
{
    stringstream ss("");
    string parameters = "";
    string executeString = "";
    const char *executeChar;
    int childEscapeStatus;

    // executeString = shell.getShellDirectory() + "/" + argument;

    if (argument[0] == '/')
    {
        //Absolute
        executeString = argument;
    }

    else
    {
        //Relative
        executeString = shell.getShellDirectory() + '/' + argument;
    }

    executeChar = executeString.c_str();

    // access() returns '0' if path exists
    // used F_OK to check if that file exists
    if (access(executeChar, F_OK) != 0)
    {
        cout << "File does not exist\n";
        return shell;
    }

    pid_t PID = fork();

    //If its a child process, run the program
    if (PID == 0)
    {

        // read in the parameters
        while (ss >> parameters)
        {
            argument.append(" ");
            argument.append(parameters);
        }

        char **args = stringToVectorToCharArray(argument);
        execv(executeChar, args);
    }

    //If its an error, return
    else if (PID < 0)
    {
        cout << "Failed to fork, returning \n";
        return shell;
    }

    //If its a parent process, wait for the child process to finish
    else
        waitpid(PID, &childEscapeStatus, WUNTRACED);

    return shell;
}

void print(pid_t PID)
{
    cout << "PID for the program ran is: " << PID << endl;
}
mysh background(mysh shell, string argument)
{

    stringstream ss("");
    int temp = 0;
    string executeString = "";
    const char *executeChar;
    int childEscapeStatus;
    string parameters = "";
    ss >> argument;
    // go to thatspecific directory
    if (argument[0] == '/')
    {
        //Absolute
        executeString = argument;
    }

    // the directory is inside of the directory that you are already in
    else
    {
        //Relative
        executeString = shell.getShellDirectory() + '/' + argument;
    }

    executeChar = executeString.c_str();

    // Checks to see if file exists
    if (access(executeChar, F_OK) != 0)
    {
        cout << "File does not exist\n";
        return shell;
    }

    pid_t PID = fork();

    // //If its a child process, use execv to execute the program at the path specified by argument
    // //With any parameters given by user passed as one array of args, with arg[0] being the path
    if (PID == 0)
    {

        while (ss >> parameters)
        {
            argument.append(" ");
            argument.append(parameters);
        }

        char **args = stringToVectorToCharArray(argument);
        execv(executeChar, args);
    }

    //If its an error, return
    else if (PID < 0)
    {
        cout << "Failed to fork, returning \n";
        return shell;
    }

    // //If its a parent process, wait for the child process to finish
    else
    {
        waitpid(PID, &childEscapeStatus, WNOHANG);
    }

    // temp = 1;
    shell.pushPID(PID);
    shell.setPID(PID);
    return shell;
}

mysh movetodir(mysh shell, string argument)
{
    stringstream ss(argument);
    string command;
    // string argument = "";
    string parameters;
    bool relative = false;

    const char *testChar;
    //Grab the target directory

    //Is it a relative path or full path?
    if (argument[0] == '/')
    {
        //Full Path
        testChar = argument.c_str();
    }

    else
    {
        //Relative Path
        string testString = shell.getShellDirectory() + "/" + argument;
        testChar = testString.c_str();
        relative = true;
    }
    //Make sure the file exists
    int result = access(testChar, F_OK);
    if (result == 0)
    {
        //Format the working directory based on relative path
        if (relative == true)
        {
            shell.appendShellDir("/");
            shell.appendShellDir(argument);
        }

        //Replace the working directory based on absolute path
        else
            shell.setShellDir(argument);

        //Print new directory
        cout << shell.getShellDirectory();
        return shell;
    }

    else
        cout << "Error " << testChar << " is not a valid path.";

    return shell;
}

mysh history(mysh shell, string argument)
{
    if (argument.compare("-c") == 0)
        shell.eraseCommandHistory();

    // if not argument after history, then print the history
    else if (argument == "")
        shell.printCommandHistory();
    // otherwise, invalid command
    else
        cout << "Invalid argument(s) for history command\n";
    return shell;
}
mysh dalekall(mysh shell)
{
    stringstream ss("");
    int PIDSize = shell.PIDHistorySize();
    pid_t tempPID;
    int killed;
    vector<pid_t> exterminated;
    int vectorSize = 0;

    // for loop to terminate all of the PIDs
    for (int i = 0; i < PIDSize; i++)
    {
        tempPID = shell.getPIDAt(i);

        killed = kill((tempPID), SIGKILL);

        if (killed == -1)
        {
            cout << "PID " << tempPID << "failed to terminate\n";
            return shell;
        }
        exterminated.push_back(tempPID);
    }
    vectorSize = exterminated.size();

    if (vectorSize == 1)
    {
        cout << "Exterminating 1 process ";
    }

    if (vectorSize > 1)
    {
        cout << "Exterminating " << vectorSize << " processes ";
    }

    for (int i = 0; i < vectorSize; i++)
    {
        cout << exterminated.at(i) << " ";
    }

    cout << endl;

    return shell;
}
mysh dalek(mysh shell, string argument)
{

    pid_t newPID = (pid_t)stringToInt(argument);
    // use kill command to terminate program with that PID
    int ifKilled = kill((newPID), SIGKILL);

    // if it fails to kill, print an error and return
    if (ifKilled == -1)
    {
        cout << "Process failed to terminate\n";
        return shell;
    }
    return shell;
}

// Determine what the command is
mysh parseString(mysh shell, string str)
{
    stringstream ss(str);
    string command;
    string argument = "";
    string parameters;
    bool relative = false;

    //Parse command to compare to shell command
    ss >> command;

    //Change Directory
    if (command.compare("movetodir") == 0)
    {

        ss >> argument;
        // call movetodir function
        movetodir(shell, argument);

        // record command
        shell.recordCommand(str);
        return shell;
    }

    // recursivly calls parse string
    else if (command.compare("replay") == 0)
    {
        int size = shell.getCmndHstrySize();
        int num;
        string str = "";
        ss >> argument;
        argument += 1;

        // get command from that certain index in the command history
        str = shell.getReplay(num);

        cout << "Replaying: " << str << endl;
        // recursivey call parseString() function again
        shell = parseString(shell, str);

        // do not store replay command to avoid infinite recursion
        return shell;
    }
    // calls repeat function
    else if (command.compare("repeat") == 0)
    {
        int number;
        string str;

        ss >> number;
        ss >> str;
        // call repeat() after recording the number of times you have to call it
        shell = repeat(shell, number, str);

        // add command to history
        shell.recordCommand(str);
        return shell;
    }

    //Print Working Directory
    else if (command.compare("whereami") == 0)
    {

        // print out currentDir
        cout << shell.getShellDirectory();

        // add command to history
        shell.recordCommand(str);
        return shell;
    }

    //Print Command History
    else if (command.compare("history") == 0)
    {
        ss >> argument;
        shell.recordCommand(str);

        // call history with argument either being  "-c" or ""
        shell = history(shell, argument);
        return shell;
    }

    // figure out start and background
    else if (command.compare("start") == 0)
    {
        ss >> argument;
        shell.recordCommand(str);

        // call
        shell = start(shell, argument);
        return shell;
    }

    else if (command.compare("background") == 0)
    {
        ss >> argument;

        // call background with the file stored in argument
        shell = background(shell, argument);
        shell.recordCommand(str);
        cout << "PID for the program ran is: " << shell.getPID() << endl;
        return shell;
    }

    // Kill the process of the current PID
    else if (command.compare("dalek") == 0)
    {
        // argument stores the PID that you want to terminate
        ss >> argument;

        // call dalek() with the shell and argument
        shell = dalek(shell, argument);
        shell.recordCommand(str);
        return shell;
    }

    else if (command.compare("dalekall") == 0)
    {
        // call dalekall()
        shell = dalekall(shell);

        // clears clear current PID
        shell.clearPIDHistory();
        shell.recordCommand(str);
        return shell;
    }

    // Terminates the Shell and writes to the history file
    else if (command.compare("byebye") == 0)
    {
        shell.recordCommand(str);
        // write the command history to history.txt
        shell.writeToFile();
        cout << "Exiting mysh" << endl;
        exit(0);
    }

    // Else, it is an invalid command and you must retry that command
    else
    {
        // even though command is invalid, you still store it in command history
        shell.recordCommand(str);
        cout << "Error! Unrecognized command. Please try again.";
        return shell;
    }
}

int main()
{
    char currentDir[500];
    string init_Directory = getcwd(currentDir, sizeof(currentDir));
    string inputString;
    mysh shell(init_Directory);

    // print out menu
    cout << "\n\n";
    cout << "                       Cooper Urich Operating Systems Assignment 3\n";
    cout << "                                  List of Commands\n";
    cout << "--------------------------------------------------------------------------------------------\n";
    cout << "- movetodir (directory)                -> move to that directory\n";
    cout << "- whereami                             -> prints current director\n";
    cout << "- history [-c]                         -> prints shell history\n";
    cout << "- replay (number)                      -> call a certain numnber from the shell history\n";
    cout << "- repeat [n] \"command\"                 -> repeat the command n number of  \n";
    cout << "- start \"program\" [parameters]         -> run program with parameters\n";
    cout << "- background \"program\"[parameters]     -> prints the PID of the program it started\n";
    cout << "- dalek \"PID\"                          -> terminate program based off of PID\n";
    cout << "- dalekall                             -> terminates all current active PIDs in shell\n";
    cout << "- byebye                               -> close shell\n\n\n";
    cout << "# ";

    // reads in the previous commands from the history file
    shell.readFromFile();

    getline(cin, inputString);

    // cycle through so you are always reading in commands
    while (true)
    {
        shell = parseString(shell, inputString);
        cout << "\n# ";
        getline(cin, inputString);
    }
}
