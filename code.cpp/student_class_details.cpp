
//425492 Alaattin Uysal
//425476 Abdulkerim Çiçek

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

class PersonType
{
    public:
    PersonType(){
        Name="";
    }
    void setName(string name){
        Name=name;
    }
    string getName(){
        return Name;
    }
    void print(){
        cout<<Name<<" ";
    }
    private:
    string Name;
};

class CourseType
{
    public:
    CourseType(){
        courseName="";
        courseCredits=0;
        courseGrade='x';
        courseNo="";
    }
    CourseType(string name,string no, int credit,char grade);
    void setCourseInfo(string name,string no, int credit,char grade){
        courseName=name;
        courseNo=no;
        courseCredits=credit;
        courseGrade=grade;
    }

    string getName(){
        return courseName;
    }
    int getCredits(){
        return courseCredits;
    }
    string getCourseNo(){
        return courseNo;
    }
    char getCourseGrade(){
        return courseGrade;
    }

    private:
    string courseName;
    string courseNo;
    int courseCredits;
    char courseGrade;
};

CourseType::CourseType(string name,string no, int credit,char grade) 
{
    courseName=name;
    courseNo=no;
    courseCredits=credit;
    courseGrade=grade;
} 



class StudentType
{
    public:
    StudentType(int size);
    ~StudentType();
    void print();
    void print(string a);
    void readInput(string fileName);//inputu okuyup kaydeder
    void getHoursEnrolled();///ogrenci toplam kaç kredilik derse kayıtlı onu hesaplıyo totalHours'a yazıyor
    void getGPA();//ortalama hesaplıyo GPA'ye yazıyor
    void billingAmmount();//harç tutarı bill'e yazıyor

    private:
    string* Id;
    char* isTuitonPaid;
    int* numberOfCourses;//öğrencinin kaç kurs aldığını tutuyor
    string* CourseNames;//aldığı tüm derlerin ismini tutuyor
    string* CourseGrades;//course names sırasına göre tüm notları tutuyor
    double* GPA;//ortalama
    int* totalHours;//toplam kaç kredi hesaplıyor.
    int* bill;//harç tutarı
    PersonType* person;//isim tutmak için --> composition
    CourseType** course;//öğrencinin aldığı kursları tutmak için  --> composition
    int Ucret;//Toplam ödenecek harç 
    int Size;//kaç öğrenci var
};

StudentType::StudentType(int size){
    Id=new string[size];
    isTuitonPaid=new char[size];
    numberOfCourses=new int[size];
    CourseNames = new string[size];
    CourseGrades = new string[size];
    GPA=new double[size];
    totalHours=new int[size];
    bill=new int[size];

    person = new PersonType[size];
    course= new CourseType*[size];

    Size=size;

    for (int i=0;i<size;i++){
        person[i].setName("");
        Id[i]="";
        isTuitonPaid[i]='-';
        numberOfCourses[i]=0;
        CourseNames[i]="";
        CourseGrades[i]="";
        GPA[i]=0;
        totalHours[i]=0;
        bill[i]=0;
    }
}

StudentType::~StudentType(){
    delete[] Id;
    delete[] isTuitonPaid;
    delete[] numberOfCourses;
    delete[] CourseNames;
    delete[] CourseGrades;
    delete[] totalHours;
    delete[] GPA;
    delete[] bill;
    delete[] person;
    for (int i = 0 i < Size; i++) {
        delete[] course[i];
    }
    delete[] course;
}


void StudentType::readInput(string filename){
    ifstream input(filename);
    while(!input.is_open()){
        cout<<"Dosya açılamadı: "<<filename<<endl;
        return;
    }

    string text;
    int ogrenciSayisi,ucret;
    int index=0;
    getline(input,text);
    stringstream ss(text);
    ss>>ogrenciSayisi;
    ss>>ucret;
    Ucret=ucret;//inputun ilk satırındeki öğrenci sayısını ve ücreti okur
    while(getline(input,text)){
        int dersSayisi;
        string surname,temp;
                                                                       
        stringstream line(text); //                                     Lisa Miller 890238 Y 4
        getline(line,temp,' ');  //     Lisa
        getline(line,surname,' '); //   Miller
        temp=temp+" "+surname;  // temp = Lisa Miller
        person[index].setName(temp);
        getline(line,temp,' ');  // 890238
        Id[index]=temp;  
        getline(line,temp,' ');
        isTuitonPaid[index]=temp[0];
        getline(line,temp);
        dersSayisi=stoi(temp);

        numberOfCourses[index]=dersSayisi;


        course[index]=new CourseType[dersSayisi];
        
        for (int i=0;i<dersSayisi;i++){
            getline(input,text);
            string cId,cHour,cGrade;
            stringstream line(text);

            getline(line,temp,' ');                                                                                     C
            CourseNames[index]+=temp+" ";
            getline(line,cId,' ');
            getline(line,cHour,' ');
            getline(line,cGrade,' ');
            CourseGrades[index]+=cGrade;
            course[index][i].setCourseInfo(temp,cId,stoi(cHour),cGrade[0]);
        }
        index++;
    }
}

void StudentType::getHoursEnrolled(){
    for(int i=0;i<Size;i++){
        for(int j=0;j<numberOfCourses[i];j++){
            totalHours[i]+=course[i][j].getCredits();
        }
    }

}

void StudentType::getGPA(){
    for(int i=0;i<Size;i++){
        double gpa=0;
        for(int k=0;k<CourseGrades[i].length();k++){
            if(CourseGrades[i][k]=='A')gpa+=4;
            else if(CourseGrades[i][k]=='B')gpa+=3;
            else if(CourseGrades[i][k]=='C')gpa+=2;
            else if(CourseGrades[i][k]=='D')gpa+=1;
        }
        gpa=gpa/numberOfCourses[i];
        GPA[i]=gpa;
    }
}

void StudentType::billingAmmount(){
    for(int i=0;i<Size;i++){
        bill[i]=totalHours[i]*Ucret;
    }
}

void StudentType::print(){
    for (int i=0;i<Size;i++){
        cout<<"Student Name : "<<person[i].getName()<<endl;
        cout<<"Student ID : "<<Id[i]<<endl;
        cout<<"Number of Course Enrolled : "<<numberOfCourses[i]<<endl;
        cout<<"Course No Course Name Credits Grade"<<endl;
        for(int j=0;j<numberOfCourses[i];j++){
            cout<<course[i][j].getCourseNo()<<"  "<<course[i][j].getName()<<"  "<<course[i][j].getCredits()<<"  ";
            if(isTuitonPaid[i]=='Y'){
                cout<<course[i][j].getCourseGrade()<<endl;
            }
            else if (isTuitonPaid[i]=='N'){
                cout<<"-"<<endl;
            }
        }
        cout<<"Total Number of Credits : "<<totalHours[i]<<endl;
        if(isTuitonPaid[i]=='Y'){
            cout<<"Mid-Semester GPA : "<<setprecision(3)<<GPA[i]<<endl<<endl<<endl;
        }
        else if (isTuitonPaid[i]=='N')
        {
            cout<<"Mid-Semester GPA : "<<"  Harç ücretiniz henüz ödenmedi!"<<endl<<endl<<endl;
        }
        
    }
}

void StudentType::print(string a){
    ofstream Dosya(a);
        while(!Dosya.is_open()){
        cout<<"Dosya açılamadı: "<<a<<endl;
        return;
    }
    for (int i=0;i<Size;i++){
        Dosya<<"Student Name : "<<person[i].getName()<<endl;
        Dosya<<"Student ID : "<<Id[i]<<endl;
        Dosya<<"Number of Course Enrolled : "<<numberOfCourses[i]<<endl;
        Dosya<<"Course No Course Name Credits Grade"<<endl;
        for(int j=0;j<numberOfCourses[i];j++){
            Dosya<<course[i][j].getCourseNo()<<"  "<<course[i][j].getName()<<"  "<<course[i][j].getCredits()<<"  ";
            if(isTuitonPaid[i]=='Y'){
                Dosya<<course[i][j].getCourseGrade()<<endl;
            }
            else if (isTuitonPaid[i]=='N')
            {
                Dosya<<"-"<<endl;
            }
        }
        Dosya<<"Total Number of Credits : "<<totalHours[i]<<endl;
        if(isTuitonPaid[i]=='Y'){
            Dosya<<"Mid-Semester GPA : "<<setprecision(3)<<GPA[i]<<endl<<endl<<endl;
        }
        else if (isTuitonPaid[i]=='N')
        {
            Dosya<<"Mid-Semester GPA : "<<"  Harç ücretiniz henüz ödenmedi!"<<endl<<endl<<endl;
        }
        
    }
    Dosya.close();
}

int checkSize(string filename){
    ifstream input(filename);
    while(!input.is_open()){
        cout<<"Dosya açılamadı: "<<filename<<endl;
        return 0;
    }
    int ogrenciSayisi;
    string text;
    getline(input,text);
    stringstream ss(text);
    ss>>ogrenciSayisi;
    input.close();
    return ogrenciSayisi;
}

main(){
    StudentType student(checkSize("input.txt"));
    student.readInput("input.txt");
    student.getHoursEnrolled();
    student.getGPA();
    student.billingAmmount();
    student.print();
    student.print("output.txt");
}
