#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <boost/tokenizer.hpp>
#include <string>

using namespace std;
using namespace boost;

class rdbms
{
public:
    vector<set<vector<string>>> database;
    vector<vector<string>> metadata;
    rdbms()
    {
    }
};

vector<string> getTableNames(rdbms &obj)
{
    vector<string> tableName;
    tableName = obj.metadata[1];
    return tableName;
}

void checkTable(rdbms &obj, string name)
{
    vector<string> tableNames;
    tableNames = obj.metadata[1];
    for (vector<string>::iterator begin = tableNames.begin(); begin != tableNames.end(); ++begin)
    {
        if (name == *begin)
        {
            cout << "Error: Table name already exists";

            return;
        }
        else
        {
            continue;
        }
    }
}

void setTableNames(rdbms &obj, vector<string> names)
{
    obj.metadata[1] = names;
}

int tableIndex(rdbms &obj, string tableName)
{
    vector<string> tableNames;
    int tableIndex;
    tableNames = obj.metadata[1];
    for (vector<string>::iterator begin = tableNames.begin(); begin != tableNames.end(); ++begin)
    {
        if (tableName == *begin)
        {
            tableIndex = begin - tableNames.begin();
        }
    }
    return tableIndex;
}

void createDatabase(rdbms &obj, vector<string> tokens)
{
    vector<string> name;
    vector<string> tbName;
    string dbname = tokens[2];
    obj.metadata.push_back(name);
    obj.metadata.push_back((tbName));
    cout << "Database created successfully!!!" << endl;
}
void createTable(rdbms &obj, vector<string> tokens)
{
    vector<string> tbName;
    vector<string> columns;
    set<vector<string>> table;
    int index = 0;
    string tableName = tokens[2];
    checkTable(obj, tableName);
    /*
        Add input table name validation using the function checkTable to check whether the table name is unique;
    */
    tbName = getTableNames(obj); //get the vector containing the table names from the metadata
    tbName.push_back(tableName);
    setTableNames(obj, tbName); // sets the newly added table names vector into the metadata
    for (vector<string>::iterator i = tokens.begin() + 3; i != tokens.end(); ++i)
    { //extract the columns from the tokens
        columns.push_back(*i);
    }
    table.insert(columns); //insert the columns as the first tuple of the table
    index = tableIndex(obj, tableName);

    if (obj.database.size() >= index + 1) //if the size of the vector containing the tables is greater that the index + 1
    {                                     //it means that there is already an entry regarding that table name
        cout << "Error table already exists";
    }
    else
    {
        obj.database.push_back(table);
    }
    cout << "Table created successfully!!!" << endl;
}

vector<string> parse(string s)
{
    vector<string> parsedText;
    typedef tokenizer<char_separator<char>> tokenizer;
    tokenizer tok(s);
    for (tokenizer::iterator itr = tok.begin(); itr != tok.end(); ++itr)
    {
        parsedText.push_back(*itr);
    }
    return parsedText;
}

void insert(rdbms &obj, vector<string> tokens)
{
    vector<string> columnOrder;
    vector<string> tuples;
    int index = 0;
    set<vector<string>>::iterator itr;
    string tableName = tokens[2];

    index = tableIndex(obj, tableName);

    itr = obj.database[index].end();
    --itr;              //Iterator to fetch the last tuple from the selected table
    columnOrder = *itr; // Fetched the tuple containing the column names in order
    for (vector<string>::iterator i = columnOrder.begin(); i != columnOrder.end(); ++i)
    { //Input the tuples in the column order
        for (vector<string>::iterator t = tokens.begin(); t != tokens.end(); ++t)
        {
            if (*i == *t)
            {
                tuples.push_back(*(t + 1));
            }
        }
    }
    obj.database[index].insert(tuples);
    cout << "Insert successful!!!" << endl;
}

void insert1(rdbms &obj, vector<string> tokens)
{
    string tbName = tokens[2];
    vector<string> columns;
    vector<string> tuple;
    int index = tableIndex(obj, tbName);
    columns = *obj.database[index].begin();

    for (int i = 0; i < columns.size(); ++i)
    {
        for (int j = 0; j < tokens.size(); ++j)
        {
            if (columns[i] == tokens[j])
            {
                tuple.push_back(tokens[j + 1]);
            }
        }
    }
    obj.database[index];
}

void display(set<vector<string>> table)
{
    set<vector<string>>::iterator a = table.end();
    --a;
    set<vector<string>>::iterator b = table.begin();
    --b;
    for (a; a != b; --a)
    {
        vector<string> tuple;
        tuple = *a;
        for (int j = 0; j < tuple.size(); ++j)
        {
            cout << tuple[j] << "\t";
        }
        cout << endl;
    }
}

void select(rdbms &obj, vector<string> token)
{
    string tableName = token[token.size() - 1];
    int fromIndex = token.size() - 2;
    int index = tableIndex(obj, tableName);
    vector<string> selectedCols;
    vector<string> columns;
    vector<int> selectedColsIndex;
    set<vector<string>> table;

    set<vector<string>>::iterator it = obj.database[index].end();
    --it;
    columns = *it;

    if (token[1] == "*")
    {
        selectedCols = columns;
    }
    else
    {
        for (int i = 1; i < fromIndex; ++i)
        {
            selectedCols.push_back(token[i]);
        }
    }
    for (int j = 0; j < selectedCols.size(); ++j)
    {
        vector<string>::iterator it = find(columns.begin(), columns.end(), selectedCols[j]);
        selectedColsIndex.push_back(distance(columns.begin(), it));
    }
    for (set<vector<string>>::iterator i = obj.database[index].begin(); i != obj.database[index].end(); ++i)
    { //Iterate over the tuples
        vector<string> tuple = *i;
        vector<string> selectedTuple;
        for (int j = 0; j < selectedColsIndex.size(); ++j)
        {
            selectedTuple.push_back(tuple[selectedColsIndex[j]]);
        }
        table.insert(selectedTuple);
    }
    display(table);
}

/*void innerJoin(rdbms &obj, vector<string> token)
{
    int fromIndex = 0;
    vector<string> selectedCols, selectedTables, selectedColsT1, selectedColsT2, t1constraint, t2constraint, t1cols, t2cols, commonConstraint;
    vector<int> selectedTablesIndex;
    set<vector<string>> table1, table2, selectedTable;
    string constraint;

    vector<string>::iterator it = find(token.begin(), token.end(), "from");
    fromIndex = (distance(token.begin(), it));

    for (int i = 0; i < fromIndex; ++i)
    {
        selectedCols.push_back(token[i]);
    }

    selectedTables.push_back(token[fromIndex + 1]);
    selectedTables.push_back(token[fromIndex + 4]);

    constraint = token[fromIndex + 6];
    for (int i = 0; i < selectedTables.size(); ++i)
    {
        for (vector<string>::iterator j = obj.metadata[1].begin(); j != obj.metadata[1].end(); ++j)
        {
            if (selectedTables[i] == *j)
            {
                selectedTablesIndex.push_back(distance(obj.metadata[1].begin(), j));
            }
        }
    }
    table1 = obj.database[selectedTablesIndex[0]];
    table2 = obj.database[selectedTablesIndex[1]];

    set<vector<string>>::iterator a = table1.end();
    --a;
    t1cols = *a;
    --a;
    set<vector<string>>::iterator t = table2.end();
    --t;
    t2cols = *t;
    --t;
    set<vector<string>>::iterator b = table1.begin();
    --b;
    set<vector<string>>::iterator c = table2.begin();
    --c;

    for (a; a != b; --a)
    {
        vector<string>::iterator it = find(t1cols.begin(), t1cols.end(), constraint);
        int constraintIndex = (distance(t1cols.begin(), it));
        vector<string> tuple = *a;
        t1constraint.push_back(tuple[constraintIndex]);
    }

    for (t; t != c; --t)
    {
        vector<string>::iterator it = find(t2cols.begin(), t2cols.end(), constraint);
        int constraintIndex = (distance(t1cols.begin(), it));
        vector<string> tuple = *t;
        t2constraint.push_back(tuple[constraintIndex]);
    }
    for (int i = 0; i < t1constraint.size(); ++i)
    {
        for (int j = 0; j < t2constraint.size(); ++j)
        {
            if (t1constraint[i] == t2constraint[j])
            {
                commonConstraint.push_back(t1constraint[i]);
            }
        }
    }

    ++a;
    t1cols = *a;
    selectedTable.insert(t1cols);
    for (a; a != b; --a)
    {
        vector<string>::iterator it = find(t1cols.begin(), t1cols.end(), constraint);
        int constraintIndex = (distance(t1cols.begin(), it));
        vector<string> tuple = *a;
        for (int i = 0; i < commonConstraint.size(); ++i)
        {
            if (commonConstraint[i] == tuple[constraintIndex])
            {
                selectedTable.insert(tuple);
            }
        }
    }

    vector<string> table2SelectedCols;
    string table2 = obj.metadata[1][selectedTablesIndex[1]];
}*/

void showTables(rdbms &obj)
{
    vector<string> tableNames;
    tableNames = obj.metadata[1];
    cout << "Table Names" << endl;
    cout << "---------------------------" << endl;
    for (int i = 0; i < tableNames.size(); ++i)
    {
        cout << tableNames[i] << endl;
    }
}

/*void deleteTable(rdbms &obj, vector<string> token)
{
    string tableName = token[2];
    string constraint = token[4];
    string constraintValue = token[5];
    int constraintIndex = 0;
    int index = tableIndex(obj, tableName);
    vector<string> columns;

    set<vector<string>>::iterator it = obj.database[index].end();
    --it;
    columns = *it;

    for(int i=0; i < columns.size(); ++i) {
        if(columns[i] == constraint){
            constraintIndex =
        }
    }
    for (set<vector<string>>::iterator i = obj.database[index].begin(); i != obj.database[index].end(); ++i)
    {
        vector<string> tuple = *i;
        for (int j = 0; j < tuple.size(); ++j)
        {
            if ()
        }
    }
}*/

void parseSyntax(rdbms &obj, string input)
{
    vector<string> token;
    typedef tokenizer<char_separator<char>> tokenizer;
    char_separator<char> sep("=, ");
    tokenizer tokens(input, sep);
    for (tokenizer::iterator itr = tokens.begin(); itr != tokens.end(); ++itr)
    {
        token.push_back(*itr);
    }
    if (token[0] == "insert" && token[1] == "into")
    {
        insert(obj, token);

        /*
            Action to perform when insert query is selected
        */
    }
    else if (token[0] == "create" && token[1] == "table")
    {
        createTable(obj, token);
        /*
            Action to perfom when create table query is selected
        */
    }
    else if (token[0] == "create" && token[1] == "database")
    {
        createDatabase(obj, token);
        /*
            Action to perform when create database query is selected
        */
    }
    else if (token[0] == "select")
    {
        select(obj, token);
    }
    else if (token[0] == "show" && token[1] == "tables")
    {
        showTables(obj);
    }
        /*else if (token[0] == "delete")
        {
            deleteTable(obj, token);
        }*/
    else if(token[0] == "exit"){
        exit(0);
    }
    else
    {
        cout << "Incorrect INPUT!!!" << endl;
    }
}