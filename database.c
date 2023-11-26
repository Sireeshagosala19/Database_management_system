// Importing required libraries
 #include<stdio.h>
 #include<conio.h>
 #include<stdlib.h>
 #include<windows.h>
 #include<string.h>
// Defining variables for global usage
 #define MAX_TABLES 10
 #define MAX_FIELDS 10
 #define MAX_RECORDS 100
// creating structures for both records and table schema
typedef struct {
 int id;
 char name[50];
 int age;
} Record;
typedef struct {
 char name[50];
 char fields[MAX_FIELDS][50];
 int numFields;
} TableSchema;
// Declaration of global variables and menu function
TableSchema tables[MAX_TABLES];
int numTables = 0;
void menu();
// saveTables() saves tables at tables.dat file
void saveTables() {
 FILE *tablesFile = fopen("tables.dat", "wb");
 if (tablesFile == NULL) {
 printf("Error creating tables file.\n");

 return;
 }
 fwrite(&numTables, sizeof(int), 1, tablesFile);
 fwrite(tables, sizeof(TableSchema), numTables, tablesFile);
 fclose(tablesFile);
}
// loadTables() loads table schemas from tables.dat file
void loadTables() {
 FILE *tablesFile = fopen("tables.dat", "rb");
 if (tablesFile == NULL) {
 printf("Tables file not found.\n");
 return;
 }
 fread(&numTables, sizeof(int), 1, tablesFile);
 fread(tables, sizeof(TableSchema), numTables, tablesFile);
 fclose(tablesFile);
}
// the gotoxy() function is a custom function used to position the cursor in a console
// window to a specific row and column.
void gotoxy(int x,int y)
{
 COORD c;
 c.X=x;
 c.Y=y;
 SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}
// createTable() function creates table schema by receiving table details
void createTable(char *tableName, int numFields, char fields[][50]) {
 if (numTables < MAX_TABLES) {
 TableSchema newTable;
 strcpy(newTable.name, tableName);
 newTable.numFields = numFields;
 for (int i = 0; i < numFields; i++) {
    strcpy(newTable.fields[i], fields[i]);
 }
 tables[numTables++] = newTable;
 FILE *schemaFile = fopen(strcat(tableName, ".schema"), "wb");
 if (schemaFile == NULL) {
 printf("Error creating schema file for table '%s'\n", tableName);
 return;
 }
 fwrite(&newTable, sizeof(TableSchema), 1, schemaFile);
 fclose(schemaFile);
 gotoxy(11,18);
 printf("Table '%s' created successfully.\n", tableName);
 } else {
 printf("Max limit of tables reached.\n");
 }
}
// dropTable() removes table schema and data permanently
void dropTable(char *tableName) {
 char schemaFileName[60], dataFileName[60];
 sprintf(schemaFileName, "%s.schema", tableName);
 sprintf(dataFileName, "%s.data", tableName);
 int tableIndex = -1; // Initialize to an invalid index
 for (int i = 0; i < numTables; i++) {
 if (strcmp(tables[i].name, tableName) == 0) {
 tableIndex = i; // Store the index of the table to be dropped
 break;
 }
 }
 if (tableIndex != -1) {

 // Remove the schema and data files
 if (remove(schemaFileName) == 0 && remove(dataFileName) == 0) {
 // Shift the elements in the tables array to remove the table
 for (int i = tableIndex; i < numTables - 1; i++) {
 tables[i] = tables[i + 1];
 }
 numTables--;
 gotoxy(11,18);
 printf("Table '%s' dropped successfully.\n", tableName);
 } else {
 printf("Error deleting schema or data files for table '%s'.\n", tableName);
 }
 } else {
 gotoxy(11,18);
 printf("Table '%s' not found.\n", tableName);
 }
}
// displayTable() displays particular table data
void displayTable(char *tableName) {
 for (int i = 0; i < numTables; i++) {
 if (strcmp(tables[i].name, tableName) == 0) {
 char schemaTableName[50];
 char dataTableName[50];
 strcpy(schemaTableName,tableName);
 strcpy(dataTableName,tableName);
 FILE *schemaFile = fopen(strcat(schemaTableName, ".schema"), "rb");
 if (schemaFile == NULL) {
 printf("Table '%s' not found.\n", tableName);
 return;
 }
 TableSchema schema;
 fread(&schema, sizeof(TableSchema), 1, schemaFile);
 fclose(schemaFile);
 gotoxy(15,19);
 printf("Records in table '%s':\n", tableName);
 gotoxy(15,21);
 printf("%-5s %-20s %-5s\n", "ID", "Name", "Age");
 gotoxy(15,22);
 printf("--------------------------------");
 FILE *dataFile = fopen(strcat(dataTableName, ".data"), "rb");
 if (dataFile == NULL) {
 printf("Error reading data file for table '%s'\n", tableName);
 return;
 }
 //printf("\n");
int j=23;
 Record record;
 while (fread(&record, sizeof(Record), 1, dataFile)) {
 gotoxy(15,j);
 printf("%-5d %-20s %-5d\n", record.id, record.name, record.age);
 j++;
 }
 printf("\n");
 fclose(dataFile);
 return;
 }
 }
 printf("Table '%s' not found.\n", tableName);
}
// displayAllTables() displays all table schema and data
void displayAllTables() {
 printf("Available tables: %d\n",numTables);
 for (int i = 0; i < numTables; i++) {
 printf("%s\n", tables[i].name);

}
 for (int i = 0; i < numTables; i++) {
 displayTable(tables[i].name);
 }
}
// insertRecord() inserts record into particular table
void insertRecord(char *tableName, int id, char *name, int age) {
 for (int i = 0; i < numTables; i++) {
 if (strcmp(tables[i].name, tableName) == 0) {
 FILE *dataFile = fopen(strcat(tableName, ".data"), "ab");
 if (dataFile == NULL) {
 printf("Error opening data file for table '%s'\n", tableName);
 return;
 }
 Record newRecord;
 newRecord.id = id;
 strcpy(newRecord.name, name);
 newRecord.age = age;
 fwrite(&newRecord, sizeof(Record), 1, dataFile);
 fclose(dataFile);
 gotoxy(11,18);
 printf("Record inserted into '%s' successfully.\n", tableName);
 return;
 }
 }
 gotoxy(11,18);
 printf("Table '%s' not found.\n", tableName);
}
// updateRecord() updates records in the table
void updateRecord(char *tableName, int id, char *name, int age) {
 char dataFileName[60];
 sprintf(dataFileName, "%s.data", tableName);
 for (int i = 0; i < numTables; i++) {
 if (strcmp(tables[i].name, tableName) == 0) {
 FILE *dataFile = fopen(dataFileName, "rb+");
 if (dataFile == NULL) {
 printf("Table '%s' not found.\n", tableName);
 return;
 }
 Record record;
 while (fread(&record, sizeof(Record), 1, dataFile)) {
 if (record.id == id) {
 strcpy(record.name, name);
 record.age = age;
 fseek(dataFile, -sizeof(Record), SEEK_CUR);
 fwrite(&record, sizeof(Record), 1, dataFile);
 fclose(dataFile);
 gotoxy(11,18);
 printf("Record with ID %d in table '%s' updated successfully.\n", id, tableName);
 return;
 }
 }
 gotoxy(11,18);
 fclose(dataFile);
 printf("Record with ID %d not found in table '%s'.\n", id, tableName);
 return;
 }
 }
 gotoxy(11,18);
 printf("Table '%s' not found.\n", tableName);
}

// deleteRecord() deletes specified record identified with id(primary key)
void deleteRecord(char *tableName, int id) {
 char dataFileName[60];
 sprintf(dataFileName, "%s.data", tableName);
 for (int i = 0; i < numTables; i++) {
 if (strcmp(tables[i].name, tableName) == 0) {
 FILE *dataFile = fopen(dataFileName, "rb+");
 if (dataFile == NULL) {
 printf("Table '%s' not found.\n", tableName);
 return;
 }
 Record record;
 while (fread(&record, sizeof(Record), 1, dataFile)) {
 if (record.id == id) {
 Record emptyRecord = {0}; // Fill with zeros to mark as deleted
 fseek(dataFile, -sizeof(Record), SEEK_CUR);
 fwrite(&emptyRecord, sizeof(Record), 1, dataFile);
 fclose(dataFile);
 gotoxy(11,18);
 printf("Record with ID %d deleted from table '%s' successfully.\n", id,
tableName);
 return;
 }
 }
 gotoxy(11,18);
 fclose(dataFile);
 printf("Record with ID %d not found in table '%s'.\n", id, tableName);
 return;
 }
 }
 gotoxy(11,18);
 printf("Table '%s' not found.\n", tableName);
}
// another() function calls menu function to perform different operation on tables
void another() {
 printf("Would you like to perform another action (choose 'y' or 'n'): ");
 char another_operation;
 scanf(" %c", &another_operation); // Add a space before %c to consume any whitespace
 if (another_operation == 'y') {
 menu();
 } else if (another_operation == 'n') {
 //printf("Press any key to continue....");
 //getch();
 } else {
 printf("Invalid choice.");
 getch();
 }
}
// menu() displays different options for performing different operations on tables based
// on user choice
void menu() {
 int choice;
 char tableName[50];
 int id, age;
 char name[50];
 char fields[][50] = {"id", "name", "age"};
 char alterFields[][50] = {"id", "name", "age", "new_field"};

 system("cls");
 loadTables(); // Load existing tables (if any)



 gotoxy(10,3);
 printf("\nDatabase Management System Menu:\n");
 gotoxy(10,5);
 printf("1. Create Table\n");
 gotoxy(10,6);
 printf("2. Drop Table\n");
 gotoxy(10,7);
 printf("3. Display All Tables\n");
 gotoxy(10,8);
 printf("4. Display Specific Table\n");
 gotoxy(10,9);
 printf("5. Insert Record\n");
 gotoxy(10,10);
 printf("6. Update Record\n");
 gotoxy(10,11);
 printf("7. Delete Record\n");
 gotoxy(10,12);
 printf("8. Exit\n");
 gotoxy(11,15);
 printf("Enter your choice: ");
 scanf("%d", &choice);
switch (choice) {
 case 1:
gotoxy(11,17);
 printf("Enter table name: ");
 scanf("%s",tableName);
 createTable(tableName, 3, fields);
 break;
 case 2:
 gotoxy(11,17);
 printf("Enter table name to drop: ");
 scanf("%s", tableName);
 dropTable(tableName);
 break;
 case 3:
 gotoxy(11,17);
 displayAllTables();
 break;
 case 4:
 gotoxy(11,17);
 printf("Enter table name to display: ");
 scanf("%s", tableName);
 displayTable(tableName);
 break;
 case 5:
 gotoxy(11,17);
 printf("Enter table name, id, name, and age: ");
 scanf("%s %d %s %d", tableName, &id, name, &age);
 insertRecord(tableName, id, name, age);
 break;
 case 6:
 gotoxy(11,17);
 printf("Enter table name, id, new name, and new age: ");
 scanf("%s %d %s %d", tableName, &id, name, &age);
 updateRecord(tableName, id, name, age);
 break;
 case 7:
 gotoxy(11,17);
 printf("Enter table name and id to delete: ");
 scanf("%s %d", tableName, &id);
 deleteRecord(tableName, id);
 break;
 case 8:
 //printf("Exiting...\n");
 break;
 default:
 printf("Invalid choice. Please enter a valid option.\n");

 }


 saveTables(); // Save tables before exiting
 if(choice!=8)
 another();
}


// program execution starts with main(). It calls menu() function
int main()
{
 gotoxy(42,8);
 printf("<--:Database Management System:-->");
 gotoxy(46,15);
 printf("Press any key to continue.");
 getch();
 menu();
 return 0;
}