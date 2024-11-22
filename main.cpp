#include <iostream>
#include <string>

// Defining namespace
using namespace std;

// --------------- Constants ---------------
// Query can contain maximum of 100 words
const int MAX_QUERY_WORDS = 100;
// total number of allowed commands these are also defined
const int NUMBER_OF_ALLOWED_COMMANDS = 20;
// Maximum limitations of tables columns rows and databases
const int MAX_COLUMNS_PER_TABLE = 10;
const int MAX_ROWS_PER_TABLE = 10;
const int MAX_TABLES_PER_DATABASE = 10;
const int MAX_DATABASES = 5;

// All the allowed commands
string ALLOWED_COMMANDS[NUMBER_OF_ALLOWED_COMMANDS] = {
    "use",
    "show",
    "create",
    "createtable",
    "showtables",
    "showtabledata",
    "insertinto",
    "updateinto",
    "deletefrom",
    "droptable",
    "closedb",
    "dropdatabase",
    "exit",
};

// --------------- Constants End Here ---------------

// --------------- All the Structures ---------------

struct Table
{
    string name;
    int columnCount = 0;
    int rowCount = 0;
    string columns[MAX_COLUMNS_PER_TABLE];
    string columnsDataTypes[MAX_COLUMNS_PER_TABLE];
    // string rows[MAX_ROWS_PER_TABLE];
    string data[MAX_ROWS_PER_TABLE][MAX_COLUMNS_PER_TABLE];
};

struct Database
{
    Table tables[MAX_TABLES_PER_DATABASE];
    string name;
    int tableCount = 0;
};

// --------------- All the Structures end here ---------------

// --------------- Global Variables ---------------

// Track of database which is being used initialized with empty database indicating no database is being used
Database databaseBeingUsed;
// defining the databases
Database allDatabases[MAX_DATABASES];
// tracking how many databases have been created
int databaseCount = 0;

// --------------- Global Variables End Here ---------------

// --------------- All The Functions Declerations ---------------
// Database Specific Functions
string switchDatabase(string queryWordsArray[]);
void showAllDatabases();
string createDatabase(string queryWordsArray[]);
bool dropDatabase(string nameOfDb);
void showTablesOfDatabase(string queryWordsArray[]);
string dropTable(string queryWordsArray[]);
string insertDataIntoTable(string queryWordsArray[]);
string createTable(string queryWordsArray[]);
void showTableData(string queryWordsArray[]);
string deleteRow(string queryWrodsArray[]);
string updateDataIntoTable(string queryWordsArray[]);

// Utility Functions
// This function breaks query by spaces and alters the queryWordsArray array stores the query words.
bool splitString(string strToBeSplited, char by, string arrayToBeModified[]);
void takeQuery(string queryWordsArray[]);
void handleCommand(string command, string queryWordsArray[], bool *terminateProgram);
void showError(string error);
bool checkArrayContainsAnElement(string arrayToBeChecked[], int sizeOfArray, string stringToCheck);
bool databaseExists(string nameOfDb);
bool tableExists(string nameOfDb, string nameOfTable);
string convertToLowerCase(string stringToBeConverted);
string substring(string fullString, int startingIndex, int endingIndex);
string extractDatabaseNameFromQuery(string queryWordsArray[]);
Database getDatabaseByName(string nameOfDb);
void updateTheDatabaseInArrayWithDatabaseBeingUsed();
int queryTotalWords(string queryWordsArray[]);
void maxLengthsOfTableColumns(Table table, int arrayContainsLengths[]);
void showRow(string rowValues[], int maxLengthsOfValues[], int columnCount, bool keys, int idNo);
// --------------- All The Functions Declerations End Here ---------------

// Main Entry Point
int main()
{
    // Logo string
    string logo = "\n _______                 _______                 _______ \n(  ___  )   |\\     /|   (  ____ \\   |\\     /|   (  ____ )\n| (   ) |   | )   ( |   | (    \\/   | )   ( |   | (    )|\n| (___) |   | |   | |   | |         | (___) |   | (____)|\n|  ___  |   | |   | |   | | ____    |  ___  |   |     __)\n| (   ) |   | |   | |   | | \\_  )   | (   ) |   | (\\ (   \n| )   ( |   | (___) |   | (___) |   | )   ( |   | ) \\ \\__\n|/     \\|   (_______)   (_______)   |/     \\|   |/   \\__/\n\n---------------------------------------------------------\n\n";
    // printing the logo as program starts
    cout << logo << endl;

    // Variable to end the program on exit command.
    bool terminateProgram = false;

    // Current path indicates which database is being used it shows up before entering the query

    // Starting program with infinite iterations will end on exit command
    while (!terminateProgram)
    {
        // the current path
        string currentPath = databaseBeingUsed.name + ">";
        // The array that holds the query words
        string queryWordsArray[MAX_QUERY_WORDS];
        // Prompting the query
        cout << currentPath;
        // taking the query as input
        takeQuery(queryWordsArray);

        // The first word of query
        string command = queryWordsArray[0];

        // check if the command is correct
        bool checkCommandExistance = checkArrayContainsAnElement(ALLOWED_COMMANDS, sizeof(ALLOWED_COMMANDS) / sizeof(ALLOWED_COMMANDS[0]), command);
        if (!checkCommandExistance)
        {
            // means command is not correct
            showError("Entered wrong query");
            continue;
        }
        handleCommand(command, queryWordsArray, &terminateProgram);
    }
}

// Database Specific Functions Definitions
string switchDatabase(string queryWordsArray[])
{
    string dbName = extractDatabaseNameFromQuery(queryWordsArray);
    // checking if user changes the database to the currently using database
    if (convertToLowerCase(dbName) == convertToLowerCase(databaseBeingUsed.name))
    {
        return "Database " + dbName + " is already in use ";
    }
    // first checking if the database exists which is requested
    bool databaseDoesExists = databaseExists(queryWordsArray[1]);
    if (databaseDoesExists)
    {
        Database db = getDatabaseByName(dbName);
        databaseBeingUsed = db;
        return "Using database " + databaseBeingUsed.name;
    }
    else
    {
        return "Database " + dbName + " is not created";
    }
}
// To show all the databases that have been created
void showAllDatabases()
{
    if (databaseCount == 0)
    {
        cout << "No databases available" << endl;
        return;
    }
    for (int database = 0; database < databaseCount; database++)
    {
        string nameOfDb = allDatabases[database].name;
        cout << database + 1 << " " << nameOfDb << endl;
    }
}
// Creates a new database
string createDatabase(string queryWordsArray[])
{
    string dbName = extractDatabaseNameFromQuery(queryWordsArray);
    if (dbName == "")
    {
        return "Enter a valid database name";
    }
    // first checking if the database is already created
    bool databaseExistance = databaseExists(dbName);
    if (databaseExistance)
    {
        char choice;
        cout << "Database already exists" << endl;
        // asking for choice until enters  y or n
        while (true)
        {
            cout << "Would you like to remove it and create new empty database? (y/n)";
            cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
                bool resultOfDatabaseDropping = dropDatabase(dbName);
                if (resultOfDatabaseDropping)
                {
                    Database db;
                    db.name = dbName;
                    allDatabases[databaseCount] = db;
                    databaseCount++;
                    return "Database " + dbName + " created";
                }
            }
            else if (choice == 'n' || choice == 'N')
            {
                return "Database not created";
            }
            showError("Enter valid choice");
        }
    }
    else
    {
        Database db;
        db.name = dbName;
        allDatabases[databaseCount] = db;
        databaseCount++;
        return "Database " + dbName + " created";
    }
}

string createTable(string queryWrodsArray[])
{
    // Extracting the table name
    string tableName = queryWrodsArray[1];
    // first checking if using any database
    if (databaseBeingUsed.name == "")
    {
        return "First start using a database";
    }
    // first checking if that table already exists in the database
    bool tableExistanceCheck = tableExists(databaseBeingUsed.name, tableName);
    if (!tableExistanceCheck)
    {
        // Extracting column name and type of it according to design of query first word is command second is table name and all others are columnName:type.

        // no of total words query contains
        int numberOfQueryWords = queryTotalWords(queryWrodsArray);
        // if no of query words is less than 3 means just command and name is provided so return that command is incorrect
        if (numberOfQueryWords < 3)
        {
            return "Table should have atleast a name, and a single column";
        }
        // creating all the columns schema e.g {{name: string}, {age, int}}
        string columnsSchema[numberOfQueryWords - 2][2];
        // breaking the name of column and type that's why starting from 3 element first two are command and tablename
        for (int i = 2; i < numberOfQueryWords; i++)
        {
            bool result = splitString(queryWrodsArray[i], ':', columnsSchema[i - 2]);
            if (!result)
            {
                // means there wern't colon in the string so the command is incorrect
                return "Entered wrong query";
            }
        }
        // Everything is now broken up so creating table
        Table table;
        table.name = tableName;
        table.columnCount = numberOfQueryWords - 2;
        for (int i = 0; i < numberOfQueryWords - 2; i++)
        {
            string columnName = columnsSchema[i][0];
            string dataType = columnsSchema[i][1];

            table.columns[i] = columnName;
            table.columnsDataTypes[i] = dataType;
        }
        // Saving table in database
        databaseBeingUsed.tables[databaseBeingUsed.tableCount] = table;
        databaseBeingUsed.tableCount++;
        // updating database as well in the array
        updateTheDatabaseInArrayWithDatabaseBeingUsed();
        return "Table created successfully";
    }
    else
    {
        return "Table with this name alreay exists";
    }
    return "hello";
}

bool dropDatabase(string nameOfDb)
{
    if (convertToLowerCase(nameOfDb) == convertToLowerCase(databaseBeingUsed.name))
    {
        cout << "Database is in use first close it." << endl;
    }
    for (int database = 0; database < databaseCount; database++)
    {
        if (convertToLowerCase(allDatabases[database].name) == convertToLowerCase(nameOfDb))
        {
            // removing the particular database by shifting the elements of array
            for (int i = database; i < databaseCount - 1; i++)
            {
                allDatabases[i] = allDatabases[i + 1];
            }
            databaseCount--;
            return true;
        }
    }
    return false;
}

void showTablesOfDatabase(string queryWordsArray[])
{
    if (databaseBeingUsed.name != "")
    {
        if (databaseBeingUsed.tableCount == 0)
        {
            cout << "No tables created yet." << endl;
        }
        for (int table = 0; table < databaseBeingUsed.tableCount; table++)
        {
            cout << table + 1 << " " << databaseBeingUsed.tables[table].name << endl;
        }
    }
    else
    {
        cout << "No database is being used, switch to one of the database." << endl;
    }
}

string dropTable(string queryWordsArray[])
{
    // second word of query should be the name of the table
    string tableName = queryWordsArray[1];
    int tableCount = databaseBeingUsed.tableCount;
    int tableExistsAtIndex = -1;
    // checking if the database has the table which is requested to drop
    for (int table = 0; table < tableCount; table++)
    {
        if (convertToLowerCase(tableName) == convertToLowerCase(databaseBeingUsed.tables[table].name))
        {
            tableExistsAtIndex = table;
            break;
        }
    }
    // if table exists removing it
    if (tableExistsAtIndex != -1)
    {
        for (int table = tableExistsAtIndex; table < tableCount - 1; table++)
        {
            Table currenTable = databaseBeingUsed.tables[table];
            Table nextTable = databaseBeingUsed.tables[table + 1];
            currenTable = nextTable;
        }
        databaseBeingUsed.tableCount -= 1;
        // Updating the database in the array as well
        updateTheDatabaseInArrayWithDatabaseBeingUsed();
        return "Table dropped successfully";
    }
    else
    {
        return "Table does not exists in the database " + databaseBeingUsed.name;
    }
}

// Inserting data into table
string insertDataIntoTable(string queryWordsArray[])
{
    // first word of query is command and second word is table name extracting it
    string tableName = queryWordsArray[1];
    // index of table in which data is being inserted
    int indexOfTable = -1;
    // if no database is being used so returning here
    if (databaseBeingUsed.name == "")
    {
        return "Try to use a database first and then insert data into tables";
    }

    for (int i = 0; i < databaseBeingUsed.tableCount; i++)
    {
        if (convertToLowerCase(databaseBeingUsed.tables[i].name) == convertToLowerCase(tableName))
        {
            indexOfTable = i;
        }
    }
    if (indexOfTable != -1)
    {
        // Extracting data that should be entered from query
        int numberOfWordsInQuery = queryTotalWords(queryWordsArray);
        int numberOfColumns = databaseBeingUsed.tables[indexOfTable].columnCount;
        int rowCount = databaseBeingUsed.tables[indexOfTable].rowCount;
        // Tracking how data is entered
        bool dataWithKeyValuePairs = 0;
        bool dataWithValues = 0;
        string data[numberOfColumns][2];
        string dataWithoutKeys[numberOfWordsInQuery - 2];
        // checking if the data is entered with key:value pair or with just values
        for (int i = 2; i < numberOfWordsInQuery; i++)
        {
            // creating a single column for key value pairs
            string singleFieldData[2];
            bool result = splitString(queryWordsArray[i], ':', singleFieldData);
            // if result is true means data is entered in the form of key:values pairs
            if (result)
            {
                dataWithKeyValuePairs = 1;
                // storing key and value in data array which is 2d
                data[i - 2][0] = singleFieldData[0];
                data[i - 2][1] = singleFieldData[1];
                // if last were just value and now is key value pair
                if (dataWithValues)
                {
                    return "Entered wrong query";
                }
            }
            else
            {
                dataWithValues = 1;
                // if last were key value pair and now is just value
                if (dataWithKeyValuePairs)
                {
                    return "Entered wrong query";
                }
                // making an array of entered values
                dataWithoutKeys[i - 2] = queryWordsArray[i];
            }
        }
        // if only values were provided in the query
        if (dataWithValues)
        {
            // Adding the array dataWithoutKeys elements to actual structure array
            for (int i = 0; i < databaseBeingUsed.tables[indexOfTable].columnCount; i++)
            {
                databaseBeingUsed.tables[indexOfTable].data[rowCount][i] = dataWithoutKeys[i];
            }
            databaseBeingUsed.tables[indexOfTable].rowCount++; // row has been inserted so incrementing the rowCount.
            updateTheDatabaseInArrayWithDatabaseBeingUsed();
            return "Data inserted successfully";
        }

        // if key value pairs were provided in the query which have been extracted and stored in data array.
        if (dataWithKeyValuePairs)
        {
            // storing the indexes of entered keys
            int indexesOfKeys[numberOfColumns];
            int count = 0; // tracking how many memebers have been entered in indexesOfKeys.
            for (int index = 0; index < numberOfWordsInQuery - 2; index++)
            {
                string key = data[index][0]; // key entered in query
                if (convertToLowerCase(key) == convertToLowerCase(databaseBeingUsed.tables[indexOfTable].columns[index]))
                {
                    indexesOfKeys[count] = index;
                    count++;
                }
            }
            // Inserting values
            for (int i = 0; i < numberOfWordsInQuery - 2; i++)
            {
                if (indexesOfKeys[i] < 0 || indexesOfKeys[i] > 9)
                {
                    cout << "Warning: Spotted a key-value pair which doesn't relate to table" << endl;
                    continue;
                }
                string fieldValue = data[i][1]; // extracting the value enterd by user in query
                databaseBeingUsed.tables[indexOfTable].data[rowCount][indexesOfKeys[i]] = fieldValue;
            }
            databaseBeingUsed.tables[indexOfTable].rowCount++; // row has been inserted so incrementing the rowCount.
            updateTheDatabaseInArrayWithDatabaseBeingUsed();
            return "Data inserted successfully";
        }
    }
    else
    {
        return "No table is created with name " + tableName + " in database " + databaseBeingUsed.name;
    }
    return "Unknown error occurred";
}

// Printing data of table
void showTableData(string queryWordsArray[])
{
    string tableName = queryWordsArray[1];

    for (int i = 0; i < databaseBeingUsed.tableCount; i++)
    {
        if (convertToLowerCase(databaseBeingUsed.tables[i].name) == convertToLowerCase(tableName))
        {
            int rowCount = databaseBeingUsed.tables[i].rowCount;
            int columnCount = databaseBeingUsed.tables[i].columnCount;
            int lengthsOfColumns[columnCount];
            // first initializing all the elements with column names lengths
            for (int element = 0; element < columnCount; element++)
            {
                lengthsOfColumns[element] = databaseBeingUsed.tables[i].columns[element].length();
            }
            maxLengthsOfTableColumns(databaseBeingUsed.tables[i], lengthsOfColumns);
            // before showing the actual data showing column names
            showRow(databaseBeingUsed.tables[i].columns, lengthsOfColumns, columnCount, true, 0);
            for (int row = 0; row < rowCount; row++) // Iterating over rows
            {
                showRow(databaseBeingUsed.tables[i].data[row], lengthsOfColumns, columnCount, false, row);
            }
        }
    }
}

// deleting data from table
string deleteRow(string queryWordsArray[])
{
    // if no database is being used so returning here
    if (databaseBeingUsed.name == "")
    {
        return "Try to use a database first and then delete data from it.";
    }
    // Query must have a command, tableName, and id of record so should have 3 words
    int numberOfWordsInQuery = queryTotalWords(queryWordsArray);
    if (numberOfWordsInQuery != 3)
    {
        return "Invalid query";
    }
    // Checking if the table exists
    bool tableExistance = tableExists(databaseBeingUsed.name, queryWordsArray[1]);
    if (!tableExistance)
    {
        return "Table does not exists in " + databaseBeingUsed.name + " database";
    }

    string tbName = queryWordsArray[1];
    int rowId = stoi(queryWordsArray[2]) - 1;
    // Deleting the row from table || making every column empty
    for (int i = 0; i < databaseBeingUsed.tableCount; i++)
    {
        int rowCount = databaseBeingUsed.tables[i].rowCount;
        int columnCount = databaseBeingUsed.tables[i].columnCount;
        if (convertToLowerCase(tbName) == convertToLowerCase(databaseBeingUsed.tables[i].name))
        {
            for (int j = 0; j < columnCount; j++)
            {
                databaseBeingUsed.tables[i].data[rowId][j] = "";
            }
            updateTheDatabaseInArrayWithDatabaseBeingUsed();
            return "Row deleted";
        }
    }
    return "Unknown error occured";
}

// updating data into table
string updateDataIntoTable(string queryWordsArray[])
{
    // first word of query is command and second word is table name extracting it
    string tableName = queryWordsArray[1];
    // index of table in which data is being inserted
    int indexOfTable = -1;
    // if no database is being used so returning here
    if (databaseBeingUsed.name == "")
    {
        return "Try to use a database first and then insert data into tables";
    }

    for (int i = 0; i < databaseBeingUsed.tableCount; i++)
    {
        if (convertToLowerCase(databaseBeingUsed.tables[i].name) == convertToLowerCase(tableName))
        {
            indexOfTable = i;
        }
    }
    if (indexOfTable != -1)
    {
        // Extracting data that should be entered from query
        int numberOfWordsInQuery = queryTotalWords(queryWordsArray);
        int numberOfColumns = databaseBeingUsed.tables[indexOfTable].columnCount;
        int rowId = stoi(queryWordsArray[2]) - 1;
        string data[numberOfColumns][2];
        string singleFieldData[2];
        // checking if the data is entered with key:value pairs.
        for (int i = 3; i < numberOfWordsInQuery; i++)
        {
            bool result = splitString(queryWordsArray[i], ':', singleFieldData);
            // if result is true means data is entered in the form of key:values pairs
            if (result)
            {
                // storing key and value in data array which is 2d
                data[i - 3][0] = singleFieldData[0];
                data[i - 3][1] = singleFieldData[1];
            }
            else
            {
                return "Entered wrong query";
            }
        }

        // checking all the entered keys relate to columns
        for (int i = 0; i < numberOfWordsInQuery - 3; i++)
        {
            bool keyExists = false;
            for (int j = 0; j < numberOfColumns; j++)
            {
                if (convertToLowerCase(databaseBeingUsed.tables[indexOfTable].columns[j]) == convertToLowerCase(singleFieldData[0]))
                {
                    keyExists = true;
                }
            }
            if (!keyExists)
            {
                return "Entered values doesn't relate to table schema.";
            }
        }

        // storing the indexes of entered keys
        int indexesOfKeys[numberOfColumns];
        int count = 0; // tracking how many memebers have been entered in indexesOfKeys.
        for (int index = 0; index < numberOfWordsInQuery - 3; index++)
        {
            string key = data[index][0]; // key entered in query
            if (convertToLowerCase(key) == convertToLowerCase(databaseBeingUsed.tables[indexOfTable].columns[index]))
            {
                indexesOfKeys[count] = index;
                count++;
            }
            else
            {
                indexesOfKeys[count] = -1;
            }
        }

        // Updating values
        for (int i = 0; i < numberOfWordsInQuery - 3; i++)
        {
            string fieldValue = data[i][1]; // extracting the value enterd by user in query
            databaseBeingUsed.tables[indexOfTable].data[rowId][indexesOfKeys[i]] = fieldValue;
        }
        updateTheDatabaseInArrayWithDatabaseBeingUsed();
        return "Data updated successfully";
    }
    else
    {
        return "No table is created with name " + tableName + " in database " + databaseBeingUsed.name;
    }
    return "Unknown error occurred";
}

// Utility functions definitions

// This function breaks the query by spaces like extracting the words from string and making an array of them
bool splitString(string strToBeSplited, char by, string arrayToBeModified[])
{
    // Getting the length of query string will iterate over each character.
    int const LENGTH_OF_QUERY = strToBeSplited.length();
    // Making each time a new word and storing in the array
    string word;
    int wordCount = 0;
    bool containsSplitter = false;
    for (int character = 0; character < LENGTH_OF_QUERY; character++)
    {
        // Handling if this is the last word
        if (character == LENGTH_OF_QUERY - 1)
        {
            word = word + strToBeSplited[character];
            arrayToBeModified[wordCount] = word;
            break;
        }
        // Handling if word ends here and making sure that the next character is not a character by which spliting like multiple consectively.
        if (strToBeSplited[character] == by && (strToBeSplited[character + 1] != by))
        {
            // character identified means the word ends here
            arrayToBeModified[wordCount] = word;
            // increasing wordCount so the next will be inserted ahead of last.
            wordCount++;
            // making word an empty string so that can make new word
            word = "";
            // incrementing as it will not include the separator in the next word
            character++;
            containsSplitter = true;
        }
        // if word is not ending adding the character to the word
        word = word + strToBeSplited[character];
    }
    return containsSplitter;
}

void takeQuery(string queryWordsArray[])
{
    for (int i = 0; i < MAX_QUERY_WORDS; i++)
    {
        cin >> queryWordsArray[i];
        // The current entered query word
        string currentQueryWord = queryWordsArray[i];
        // last character of the current query word
        char currentQueryWordLastChar = currentQueryWord[currentQueryWord.length() - 1];
        // checking if the last entered query word contains the semicolon at the end or enters the semicolon at the end or query
        if (currentQueryWordLastChar == ';')
        {
            // the last word without ;
            string queryWord = substring(queryWordsArray[i], 0, queryWordsArray[i].length() - 2);
            queryWordsArray[i] = queryWord;
            break;
        }
    }
}

void handleCommand(string command, string queryWordsArray[], bool *terminateProgram)
{
    if (command == "use")
    {
        // first checking if the given name of database exists
        string result = switchDatabase(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "show")
    {
        // This shows all the databases have been created
        showAllDatabases();
    }
    else if (command == "create")
    {
        // Creates the new database
        string result = createDatabase(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "createtable")
    {
        string result = createTable(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "showtables")
    {
        // If a database is being used then show the tables that are created in it.
        showTablesOfDatabase(queryWordsArray);
    }
    else if (command == "showtabledata")
    {
        // Show the table data but first checking that given name of table is created in database.
        showTableData(queryWordsArray);
    }
    else if (command == "insertinto")
    {
        // insert the data into table but checking that table exists.
        string result = insertDataIntoTable(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "updateinto")
    {
        // update the table data.
        string result = updateDataIntoTable(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "deletefrom")
    {
        string result = deleteRow(queryWordsArray);
        cout << result << endl;
    }
    else if (command == "droptable")
    {
        dropTable(queryWordsArray);
    }
    else if (command == "closedb")
    {
        if (databaseBeingUsed.name == "")
        {
            cout << "No database is being used" << endl;
        }
        else
        {
            databaseBeingUsed.name = "";
            cout << "Database closed" << endl;
        }
    }
    else if (command == "dropdatabase")
    {
        string dbName = extractDatabaseNameFromQuery(queryWordsArray);
        bool result = dropDatabase(dbName);
        if (result)
        {
            cout << "Database dropped." << endl;
        }
        else
        {
            cout << "An error occured" << endl;
        }
    }
    else if (command == "exit")
    {
        *terminateProgram = true;
    }
}

void showError(string error)
{
    cout << "Error: " << error << endl;
}

// checks if the given string array contains an string
bool checkArrayContainsAnElement(string arrayToBeChecked[], int sizeOfArray, string stringToCheck)
{
    for (int i = 0; i < sizeOfArray; i++)
    {
        if (arrayToBeChecked[i] == stringToCheck)
        {
            return true;
        }
    }
    return false;
}

// checks if database exists
bool databaseExists(string nameOfDb)
{
    for (int i = 0; i < databaseCount; i++)
    {
        // first converting
        if (convertToLowerCase(allDatabases[i].name) == convertToLowerCase(nameOfDb))
        {
            return true;
        }
    }
    return false;
}

// checks if the table exists in a particular database
bool tableExists(string nameOfDb, string nameOfTable)
{
    for (int i = 0; i < databaseCount; i++)
    {
        // first converting strings to lowercase and then checking if the database exists with the given name
        if (convertToLowerCase(allDatabases[i].name) == convertToLowerCase(nameOfDb))
        {
            // now checking if that particular database has that table
            Database db = allDatabases[i];
            // Table allTablesOfDb[] = db.tables;
            int totalTablesCreated = db.tableCount;
            // looping through all the tables that have been created in the database
            for (int table = 0; table < totalTablesCreated; table++)
            {
                if (convertToLowerCase(db.tables[table].name) == convertToLowerCase(nameOfTable))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

string extractDatabaseNameFromQuery(string queryWordsArray[])
{
    string dbName = queryWordsArray[1];
    // Extracting the database name from query
    for (int i = 2; i < MAX_QUERY_WORDS; i++)
    {
        if (queryWordsArray[i] == "")
        {
            break;
        }
        dbName += (" " + queryWordsArray[i]);
    }
    return dbName;
}

Database getDatabaseByName(string nameOfDb)
{
    for (int i = 0; i < databaseCount; i++)
    {
        if (convertToLowerCase(allDatabases[i].name) == convertToLowerCase(nameOfDb))
        {
            return allDatabases[i];
        }
    }
    Database emptyDb;
    return emptyDb;
}

// converts any string to lowercase and return the converted lowercase string
string convertToLowerCase(string stringToBeConverted)
{
    string newConvertedString = stringToBeConverted;
    for (int i = 0; i < stringToBeConverted.length(); i++)
    {
        int asciiOfChar = stringToBeConverted[i];
        if (asciiOfChar <= 90 && asciiOfChar >= 65)
        {
            char upperCaseCharacter = asciiOfChar + 32;
            newConvertedString[i] = upperCaseCharacter;
        }
    }
    return newConvertedString;
}

// takes string and return the sub string from that particular string
string substring(string fullString, int startingIndex, int endingIndex)
{
    string newSubString;
    for (int character = startingIndex; character <= endingIndex; character++)
    {
        newSubString += fullString[character];
    }
    return newSubString;
}

void updateTheDatabaseInArrayWithDatabaseBeingUsed()
{
    for (int db = 0; db < databaseCount; db++)
    {
        if (convertToLowerCase(allDatabases[db].name) == convertToLowerCase((databaseBeingUsed.name)))
        {
            allDatabases[db] = databaseBeingUsed;
            break;
        }
    }
}

int queryTotalWords(string queryWordsArray[])
{
    for (int i = 0; i < MAX_QUERY_WORDS; i++)
    {
        if (queryWordsArray[i] == "")
        {
            // returning i not i+1 because when here condition will fail it will have the incremented value
            return i;
        }
    }
    return MAX_QUERY_WORDS;
}

void maxLengthsOfTableColumns(Table table, int arrayContainsLengths[])
{

    for (int i = 0; i < table.rowCount; i++)
    {
        for (int j = 0; j < table.columnCount; j++)
        {
            // checking if already stored length is less than the new length if yes then replacing with new value
            if ((table.data[i][j]).length() > arrayContainsLengths[j])
            {
                arrayContainsLengths[j] = (table.data[i][j]).length();
            }
        }
    }
}

void showRow(string rowValues[], int maxLengthsOfValues[], int columnCount, bool keys, int idNo)
{
    // checking if all the row values are empty means that row were deleted so will not print anything
    bool wholeRowEmpty = true;
    for (int i = 0; i < columnCount - 1; i++)
    {
        if (rowValues[i] != "")
        {
            wholeRowEmpty = false;
        }
    }
    if (wholeRowEmpty)
    {
        return;
    }
    // caluculating the total characters a row will contain
    int totalCharacters = 0;
    for (int i = 0; i < columnCount; i++)
    {
        totalCharacters += maxLengthsOfValues[i];
    }
    // as there will be 2 spaces for each column and a bar.
    totalCharacters += (columnCount * 3) + 8;

    // printing line
    for (int i = 0; i <= totalCharacters; i++)
    {
        cout << "-";
        if (i == totalCharacters)
        {
            cout << endl;
        }
    }
    if (keys)
        cout << "| id no ";
    else
        cout << "| " << idNo + 1 << "     ";

    // Printing the values and spaces
    for (int i = 0; i < columnCount; i++)
    {
        cout << "| " << rowValues[i] << " ";
        // printing spaces after the value
        for (int j = 0; j < maxLengthsOfValues[i] - rowValues[i].length(); j++)
        {
            cout << " ";
        }
        if (i == columnCount - 1)
        {
            cout << "|" << endl;
        }
    }
    // printing line
    for (int i = 0; i <= totalCharacters; i++)
    {
        cout << "-";
        if (i == totalCharacters)
        {
            cout << endl;
        }
    }
}
