#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <string>


using namespace std;

int byteoffset = 0;
int byteoffsetD = 0;

struct employee {
    char employee_ID[13];
    char Dept_ID[30];
    char employee_name[50];
    char Employee_Position[50];
};

struct department{
    char Dept_ID[30];
    char Dept_Name[50];
    char Dept_Manger[50];
};

struct ESecIndex{
    string DID;
    string ID;
};

struct DSecIndex{
    string name;
    string ID;
};

string convertToString(char* a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        s += a[i];
    }
    return s;
}


class DepartmentFile {

public:
    department d;
    int count = 0;
    string IDlist[100];
    int OffsetList[100];
    DSecIndex namelist[100];

    void writeToFileDepartment() {

        string buffer, buffer1;
        fstream file;

        cout << "\nDepartment ID : ";
        cin >> d.Dept_ID;
        while (searchIndex(d.Dept_ID) != -1) {
            cout << "\nplease enter a new id, this id is already taken" << endl;
            cout << "\nDepartment ID: ";
            cin >> d.Dept_ID;
        }

        cout << "\nDepartment name: ";
        cin.ignore();
        cin.getline(d.Dept_Name, 50);

        cout << "\nDepartment Manger: ";
        cin.ignore();
        cin.getline(d.Dept_Manger, 50);
        cout << endl;


        buffer =  convertToString(d.Dept_ID, strlen(d.Dept_ID)) + '|' + convertToString(d.Dept_Name, strlen(d.Dept_Name))
            + '|' + convertToString(d.Dept_Manger, strlen(d.Dept_Manger)) + '$';
        string s = to_string(buffer.size()) + '|';
        buffer1 = s + buffer;

        file.open("Department.txt", ios::out | ios::app);
        file << buffer1;
        file.close();

        IDlist[count] = convertToString(d.Dept_ID, strlen(d.Dept_ID));
        OffsetList[count] = byteoffsetD;
        namelist[count].ID = convertToString(d.Dept_ID, strlen(d.Dept_ID));
        namelist[count].name = convertToString(d.Dept_Name, strlen(d.Dept_Name));
        byteoffsetD = byteoffsetD + buffer1.size();

        sortSecIndex();
        sortIndex();
        count++;
//        indexFile.open("Department Primary index.txt", ios::out | ios::app);
//        indexPosition = indexFile.tellp();
//
//        indexFile << convertToString(d.Dept_ID, strlen(d.Dept_ID));
//        indexFile << "|" << byteoffset << "|"<< endl; ;
//        indexFile.close();
    }

    void readFromFile(int offset) {

        int address;
        fstream file;
        char xx[4];

        file.open("Department.txt" , ios::binary | ios::in );

        address = OffsetList[offset];

        file.seekp(address, file.beg);
        char buffer[150];
        file.getline(buffer, 150, '$');

        istringstream stream(buffer);
        stream.getline(xx, 4, '|');
        stream.getline(d.Dept_ID, 29, '|');
        stream.getline(d.Dept_Name, 49, '|');
        stream.getline(d.Dept_Manger, 49, '$');
        file.close();

        cout << endl << "[Found the Record]\n" << endl;
        cout << "Department ID: " << d.Dept_ID << endl;
        cout << "Department Name: " << d.Dept_Name << endl;
        cout << "Department Manger: " << d.Dept_Manger << endl;
        cout << endl;
    }


    void sortIndex() {
        int i, j, tempAddres;
        string tempName;
        fstream indexFile;

        for (i = 0;i < count;i++) {
            for (j = i + 1;j <= count;j++) {
                if (IDlist[i] > IDlist[j]) {
                    tempName = IDlist[i];
                    IDlist[i] = IDlist[j];
                    IDlist[j] = tempName;

                    tempAddres = OffsetList[i];
                    OffsetList[i] = OffsetList[j];
                    OffsetList[j] = tempAddres;
                }
            }
        }

        indexFile.open("Department Primary.txt", ios::out);
        indexFile.clear();
        for(int i = 0; i <= count ;i++){
            indexFile << IDlist[i] << "|" << OffsetList[i] << "|" << endl;
        }
        indexFile.close();
    }

    int searchIndex(string id) {
        int low = 0, high = count, mid = 0;
        bool found = false;

        while (low <= high) {

            mid = (low + high) / 2;

            if (IDlist[mid] == id) {
                found = true;
                break;
            }
            if (IDlist[mid] > id) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        if (found) {
            return mid;
        }

        else {
            return -1;
        }

    }

    bool searchbyid(string id) {
        int pos = 0, temp;

        pos = searchIndex(id);

        if (pos >= 0) {
            readFromFile(pos);

            temp = pos;
            while (IDlist[++temp] == id) {
                readFromFile(temp);
            }

            temp = pos;
            while (IDlist[--temp] == id) {
                readFromFile(temp);
                return true;
            }
        }
        else {
            cout<<"\n[NOT FOUND TRY ANOTHER ID]...... \n"<<endl;
            return false;
        }
    }


    void sortSecIndex() {

        fstream indexFile;
        int in , out;
        string temp, temp1;

        for(out = 1; out <= count; out++)
        {
            temp = namelist[out].name;
            temp1 = namelist[out].ID;
            in = out;
            while(in >= 0 && namelist[in - 1].name > temp )
            {
                namelist[in].name = namelist[in-1].name;
                namelist[in].ID = namelist[in-1].ID;
                in--;
            }
            namelist[in].name = temp;
            namelist[in].ID = temp1;
        }

        indexFile.open("Department secondary.txt", ios::out);
        indexFile.clear();
        for(int i = 0; i <= count ;i++){
            //cout << namelist[i].name << "|" << namelist[i].ID << "|" << endl;
            indexFile << namelist[i].name << "|" << namelist[i].ID << "|" << endl;
        }
        indexFile.close();
    }

    int searchSecIndex(string name) {
        int low = 0, high = count, mid = 0;
        bool found = false;

        while (low <= high) {

            mid = (low + high) / 2;

            if (namelist[mid].name == name) {
                found = true;
                break;
            }
            if (namelist[mid].name > name) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        if (found) {
            return mid;
        }

        else {
            return -1;
        }

    }

    void searchbyName(string name) {
        int pos = 0, temp;
        pos = searchSecIndex(name);

        if (pos >= 0) {

            if(namelist[pos].name == name)
                searchbyid(namelist[pos].ID);

            temp = pos;
            while (namelist[++temp].name == name) {
                searchbyid(namelist[temp].ID);
            }

            temp = pos;

            while (namelist[--temp].name == name) {
                searchbyid(namelist[temp].ID);
            }
        }

        else {
            cout << "\n[Not Found]..... \n";
        }
    }


    void removeEDepartment(string id) {
        int pos = 0, temp;
        string buffer = "";


        pos = searchIndex(id);
        if (pos >= 0) {
            readFromFile(pos);
            deleteFromFile(pos);
            temp = pos;
            while (IDlist[++temp] == id) {
                readFromFile(temp);
                deleteFromFile(temp);
            }
            temp = pos;
            while (IDlist[--temp] == id) {
                readFromFile(temp);
                deleteFromFile(temp);
            }
        }
        else {
            cout << "\n[Not Found]..... \n";
        }

    }

    void deleteFromFile(int byteoffset) {
        fstream indexFile, avialListFile;
        char deletedChar = '*';
        int i, address;
        if (byteoffset >= 0) {
            fstream file;
            file.open("Department.txt");

            address = OffsetList[byteoffset];
            file.seekp(address, ios::beg);
            file.put(deletedChar);
            char buffer[160];
            file.getline(buffer, 160, '$');

            avialListFile.open("Department avialList File.txt", ios::out | ios::app);
            avialListFile <<'*' + convertToString(buffer,strlen(buffer)) + '$';

            cout << endl << "\n[Record deleted successfully]..... \n"<<endl;
            file.close();
        }

        for (int i = byteoffset;i < count;i++) {
            IDlist[i] = IDlist[i + 1];
            OffsetList[i] = OffsetList[i + 1];
            namelist[i].ID = namelist[i + 1].ID;
            namelist[i].name = namelist[i + 1].name;
        }
        count--;

        indexFile.open("Department Primary.txt", ios::out);
        indexFile.clear();
        for(int i = 0; i < count ;i++){
            indexFile << IDlist[i] << "|" << OffsetList[i] << "|" << endl;
        }
        indexFile.close();

        indexFile.open("Department secondary.txt", ios::out);
        indexFile.clear();
        for(int i = 0; i < count ;i++){
            indexFile << namelist[i].name << "|" << namelist[i].ID << "|" << endl;
        }
        indexFile.close();
    }

};


class EmployeeFile {

    employee e;
    int count = 0;
    string IDlist[100];
    ESecIndex DIDlist[100];
    int OffsetList[100];

public:

    void writeToFileEmpolyee() {
        string buffer, buffer1;
        fstream file;

        cout << "\nEmployee ID : ";
        cin >> e.employee_ID;
        while (searchIndex(e.employee_ID) != -1) {
            cout << "\nplease enter a new id, this id is already taken" << endl;
            cout << "\nEmployee ID: ";
            cin >> e.employee_ID;
        }

        cout << "\nEmployee name: ";
        cin.ignore();
        cin.getline(e.employee_name, 50);

        cout << "\ndepartment ID: ";
        cin >> e.Dept_ID;

        cout << "\nEmployee position: ";
        cin >> e.Employee_Position;

        cout << endl;

        buffer =  convertToString(e.employee_ID, strlen(e.employee_ID)) + '|' + convertToString(e.employee_name, strlen(e.employee_name))
            + '|' + convertToString(e.Dept_ID, strlen(e.Dept_ID)) + '|' + convertToString(e.Employee_Position, strlen(e.Employee_Position)) + '$';
        string s = to_string(buffer.size()) + '|';

        buffer1 = s + buffer;
        file.open("Employee.txt", ios::out | ios::app);
        file << buffer1;
        file.close();

        IDlist[count] = convertToString(e.employee_ID, strlen(e.employee_ID));
        OffsetList[count] = byteoffset;
        DIDlist[count].DID = convertToString(e.Dept_ID, strlen(e.Dept_ID));
        DIDlist[count].ID = convertToString(e.employee_ID, strlen(e.employee_ID));
        byteoffset = byteoffset + buffer1.size();

        sortIndex();
        sortSecIndex();
        count++;

//        indexFile.open("Employee Primary index.txt", ios::out | ios::app);
//        indexPosition = indexFile.tellp();
//        indexFile << convertToString(e.employee_ID, strlen(e.employee_ID));
//        indexFile << "|" << byteoffset << "|"<< endl; ;
//
//        indexFile.close();


    }

    void readFromFile(int offset) {

        int address;
        fstream file;
        char xx[4];

        file.open("Employee.txt" , ios::binary | ios::in );
        address = OffsetList[offset];
        file.seekp(address, file.beg);
        char buffer[160];
        file.getline(buffer, 160, '$');



        istringstream stream(buffer);
        stream.getline(xx, 4, '|');
        stream.getline(e.employee_ID, 12, '|');
        stream.getline(e.employee_name, 49, '|');
        stream.getline(e.Dept_ID, 29, '|');
        stream.getline(e.Employee_Position, 49, '$');

        cout << endl << "[Found the Record]\n" << endl;
        cout << "Employee ID: "<<e.employee_ID << endl;
        cout << "Employee Name: " << e.employee_name << endl;
        cout << "Department ID: " << e.Dept_ID << endl;
        cout << "Employee Position: " << e.Employee_Position << endl;
        cout << endl;

        file.close();
    }


    void sortIndex() {
        int i, j, tempAddres, indexPosition;
        string tempName;
        fstream indexFile;

        for (i = 0;i < count; i++) {

            for (j = i + 1;j <= count;j++) {
                if (IDlist[i] > IDlist[j]) {

                    tempName = IDlist[i];
                    IDlist[i] = IDlist[j];
                    IDlist[j] = tempName;

                    tempAddres = OffsetList[i];
                    OffsetList[i] = OffsetList[j];
                    OffsetList[j] = tempAddres;
                }
            }
        }
        indexFile.open("Employee Primary.txt", ios::out);
        indexFile.clear();

        for(int i = 0; i <= count ;i++){
            indexPosition = indexFile.tellp();
            indexFile << IDlist[i] << "|" << OffsetList[i] << "|" << endl;
        }

        indexFile.close();
    }

    int searchIndex(string id) {

        int low = 0, high = count, mid = 0;
        bool found = false;

        while (low <= high) {

            mid = (low + high) / 2;

            if (IDlist[mid] == id) {
                found = true;
                break;
            }
            if (IDlist[mid] > id) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        if (found) {
            return mid;
        }

        else {
            return -1;
        }

    }

    bool searchbyid(string id) {
        int pos = 0, temp;
        string buffer = "";

        pos = searchIndex(id);

        if (pos >= 0) {
            readFromFile(pos);

            temp = pos;
            while (IDlist[++temp] == id) {
                readFromFile(temp);
            }

            temp = pos;
            while (IDlist[--temp] == id) {
                readFromFile(temp);
                return true;
            }
        }
        else {
            cout<<"[NOT FOUND TRY ANOTHER ID]..... "<<endl;
            return false;
        }
    }


    void sortSecIndex() {

        fstream indexFile;
        int in , out, flag = 0;
        string temp, temp1;

        for(out = 1; out <= count; out++)
        {
            temp = DIDlist[out].DID;
            temp1 = DIDlist[out].ID;
            in = out;

            while(in >= 0 && DIDlist[in - 1].DID > temp )
            {
                DIDlist[in].DID = DIDlist[in-1].DID;
                DIDlist[in].ID = DIDlist[in-1].ID;
                in--;
            }

            DIDlist[in].DID = temp;
            DIDlist[in].ID = temp1;
        }

        indexFile.open("Employee secondary.txt", ios::out);
        indexFile.clear();

        for(int i = 0; i <= count ;i++){
            indexFile << DIDlist[i].DID << "|" << DIDlist[i].ID << "|" << endl;
        }

        indexFile.close();
    }

    int searchSecIndex(string did) {

        int low = 0, high = count, mid = 0;
        bool found = false;

        while (low <= high) {

            mid = (low + high) / 2;

            if (DIDlist[mid].DID == did) {
                found = true;
                break;
            }
            if (DIDlist[mid].DID > did) {
                high = mid - 1;
            }
            else {
                low = mid + 1;
            }
        }

        if (found) {
            return mid;
        }

        else {
            return -1;
        }

    }

    void searchbydid(string did) {
        int pos = 0, temp;

        pos = searchSecIndex(did);
        cout<<pos<<endl;

        if (pos >= 0) {
            if(DIDlist[pos].DID == did)
                searchbyid(DIDlist[pos].ID);

            temp = pos;
            while (DIDlist[++temp].DID == did) {
                searchbyid(DIDlist[temp].ID);
            }

            temp = pos;
            while (DIDlist[--temp].DID == did) {
                searchbyid(DIDlist[temp].ID);
            }
        }

        else {
            cout << "\n[Not Found!]..... \n";
        }
    }


    void removeEmp(string id) {
        int pos = 0, temp;
        string buffer = "";


        pos = searchIndex(id);
        if (pos >= 0) {
            readFromFile(pos);
            deleteFromFile(pos);
            temp = pos;
            while (IDlist[++temp] == id) {
                readFromFile(temp);
                deleteFromFile(temp);
            }
            temp = pos;
            while (IDlist[--temp] == id) {
                readFromFile(temp);
                deleteFromFile(temp);
            }
        }
        else {
            cout << "\n[Not Found!]..... \n";
        }

    }

    void deleteFromFile(int byteoffset) {
        fstream indexFile, avialListFile;
        char deletedChar = '*';
        int i, address;
        if (byteoffset >= 0) {
            fstream file;
            file.open("Employee.txt");

            address = OffsetList[byteoffset];
            file.seekp(address, ios::beg);
            file.put(deletedChar);
            char buffer[160];
            file.getline(buffer, 160, '$');
            avialListFile.open("Employee avialList File.txt", ios::out | ios::app);
            avialListFile <<'*' + convertToString(buffer,strlen(buffer)) + '$';

            cout << endl << "\n\n[Record deleted]..... \n" << endl;

            file.close();
        }

        for (int i = byteoffset;i < count;i++) {
            IDlist[i] = IDlist[i + 1];
            OffsetList[i] = OffsetList[i + 1];
            DIDlist[i].ID = DIDlist[i + 1].ID;
            DIDlist[i].DID = DIDlist[i + 1].DID;
        }

        count--;
        indexFile.open("Employee Primary.txt", ios::out);
        indexFile.clear();

        for(int i = 0; i < count ;i++){
            indexFile << IDlist[i] << "|" << OffsetList[i] << "|" << endl;
        }

        indexFile.close();

        indexFile.open("Employee secondary.txt", ios::out);
        indexFile.clear();

        for(int i = 0; i < count ;i++){
            indexFile << DIDlist[i].DID << "|" << DIDlist[i].ID << "|" << endl;
        }

        indexFile.close();
    }

};


void takeQuery(string query, DepartmentFile department, EmployeeFile employee){

    transform(query.begin(), query.end(), query.begin(), ::tolower);
    string word = "";
    string arr[8];
    int j=0;

    for (auto x : query)
    {
        if (x == ' ')
        {
            arr[j] = word;
            j++;
            word = "";
        }
        else {
            word = word + x;
        }
    }

    word.erase(0,1);
    word.pop_back();
    arr[j] = word;


    if(arr[3] == "employee"){

        if(arr[5] == "dept_id"){
            employee.searchbydid(word);
        }
        else if(arr[5] == "employee_id"){
            employee.searchbyid(word);
        }
    }
    else if(arr[3] == "department"){

        if(arr[5] == "dept_id"){
            department.searchbyid(word);
        }
        else if(arr[5] == "dept_name"){
            department.searchbyName(word);
        }
    }
    else
        cout<< "error: wrong query"<<endl;
}


int main() {



        int choice;
        string ID;
        EmployeeFile employee;
        DepartmentFile department;


        do {
            cout << "To Add Employee              -----> Press 1" << endl;
            cout << "To Add Department            -----> Press 2" << endl;
            cout << "To Delete Employee           -----> Press 3" << endl;
            cout << "To Delete Department         -----> Press 4" << endl;
            cout << "To Print Employee by ID      -----> Press 5" << endl;
            cout << "To Print Employee by DID     -----> Press 6" << endl;
            cout << "To Print Department by ID    -----> Press 7" << endl;
            cout << "To Print Department by name  -----> Press 8" << endl;
            cout << "To Write a Query             -----> Press 9" << endl;
            cout << "To Exit                      -----> Press 0" << endl;
            cout << "\nEnter number you want: ";
            cin >> choice;
            switch (choice) {

                case 1:
                    employee.writeToFileEmpolyee();
                    break;

                case 2:
                    department.writeToFileDepartment();
                    break;

                case 3:
                    cout << "Enter the ID of employee you need to delete: ";
                    cin >> ID;
                    employee.removeEmp(ID);
                    break;

                case 4:
                    cout << "Enter the ID of Department you need to delete: ";
                    cin >> ID;
                    department.removeEDepartment(ID);
                    break;

                case 5:
                    cout << "Enter the ID of employee: ";
                    cin >> ID;
                    employee.searchbyid(ID);
                    break;

                case 6:
                    cout << "Enter the DID of employee: ";
                    cin >> ID;
                    employee.searchbydid(ID);
                    break;

                case 7:
                    cout << "Enter the ID of department: ";
                    cin >> ID;
                    department.searchbyid(ID);
                    break;

                case 8:
                    cout << "Enter the name of department: ";
                    cin >> ID;
                    department.searchbyName(ID);
                    break;

                case 9:
                    cin.ignore();
                    getline(cin ,ID);
                    cout << "Enter the Query: ";
                    takeQuery(ID, department, employee);
                    break;

                default:
                    cout << "enter the correct choice" << endl;
            }

        } while (choice != 0);

        //takeQuery("select all from Employee where Employee_ID = �20200490�",department,employee);

    return 0;
}
