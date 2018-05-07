#include<iostream>
#include<fstream>
#include<string>
#include<random>
#include<chrono>
#include<ctime>
using namespace std;
using namespace std::chrono;

string DAYS[7] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
string *PLANES;

struct Flight{

	string Aircraft;
	int Pilot;
	int Route;
	int Conflicts;
};

void CHECK_FITNESS(Flight ***&Schedule, int Days, int Runways, int Slots){

	int i, j, k, a, b, c, Day_Count, Week_Count, Consecutive_Count,Same_Slot_Count;

	for (i = 0; i < Days; i++){
		for (j = 0; j < Runways; j++){
			for (k = 0; k < Slots; k++){
				Schedule[i][j][k].Conflicts = 0;
			}
		}
	}

	for (i = 0; i < Days; i++){
		for (j = 0; j < Runways; j++){
			for (k = 0; k < Slots; k++){
				if (Schedule[i][j][k].Pilot != -1){

					Week_Count = 1;
					
					
					for (a = 0; a < Days; a++){
						Day_Count = 0;
						Same_Slot_Count = 0;
						Consecutive_Count = 0;
						for (b = 0; b < Runways; b++){
							for (c = 0; c < Slots; c++){
								if (!(a == i && b == j && c == k)){
									if (Schedule[i][j][k].Pilot == Schedule[a][b][c].Pilot){
										Week_Count++;						
										
										if (a == i)
											Day_Count++;
										if (a==i && c == k)
											Same_Slot_Count++;
										
										if (a==i && b==j && abs(c-k)==1)
											Consecutive_Count++;
									}
								}
							}
						}
					}
					if (Day_Count > 2)
						Schedule[i][j][k].Conflicts++;

					if (Week_Count > 3)
						Schedule[i][j][k].Conflicts++;
			
					if (Consecutive_Count > 0)
						Schedule[i][j][k].Conflicts++;

					if (Same_Slot_Count > 0)
						Schedule[i][j][k].Conflicts++;
				}
			}
		}
		
	}
}

int CHECK_CONFLICTS(Flight ***Schedule, int Days, int Runways, int Slots){

	int x = 0;
	for (int i = 0; i < Days; i++){
		for (int j = 0; j < Runways; j++){
			for (int k = 0; k < Slots; k++){
				x += Schedule[i][j][k].Conflicts;					
			}
		}
	}
	return x;
}

void CROSS_OVER(Flight ***&Schedule, int Days, int Runways, int Slots){

	int a, b, c;
	for (int i = 0; i < Days; i++){
		for (int j = 0; j < Runways; j++){
			for (int k = 0; k < Slots; k++){

				if (Schedule[i][j][k].Conflicts>0){
					
					a = rand() % Days;
					b = rand() % Runways;
					c = rand() % Slots;
					if (Schedule[a][b][c].Pilot != -1 && Schedule[a][b][c].Conflicts == 0){
						Schedule[i][j][k].Pilot = Schedule[a][b][c].Pilot;
						Schedule[a][b][c].Aircraft = Schedule[i][j][k].Aircraft;
						swap(Schedule[i][j][k].Route, Schedule[a][b][c].Route);
					}
				}
			}
		}
	}
}

void MUTATION(Flight ***&Schedule, int Days, int Runways, int Slots,int Pilots){

	int P;
	for (int i = 0; i < Days; i++){
		for (int j = 0; j < Runways; j++){
			for (int k = 0; k < Slots; k++){

				if (Schedule[i][j][k].Conflicts>0){
					P = rand() % Pilots;
					while (P == Schedule[i][j][k].Pilot)
						P = rand() % Pilots + 1;

					Schedule[i][j][k].Pilot = P;

				}
			}
		}
	}
}

void PRINT(Flight ***S, int Days, int Runways, int Slots, ofstream &fout){

	
	for (int i = 0; i < Days; i++){
		fout << endl << DAYS[i] << endl << endl << endl;
		for (int s = 1; s <= Slots; s++)
			fout << "           S" << s;
		fout<< endl << endl;
		for (int j = 0; j < Runways; j++){
			fout << "R" << j + 1;
			for (int k = 0; k < Slots; k++){
				if (S[i][j][k].Pilot != -1){
					fout << "  " << S[i][j][k].Aircraft << ", P" << S[i][j][k].Pilot << ", R" << S[i][j][k].Route;
				}
				else{
					fout << "              ";
				}
			}
			fout << endl<<endl;
		}
		fout << endl << endl;
	}
}

int main(){

	Flight ***Schedule;
	int Planes, Days, Routes,Slots, Pilots, Runways, R, S, z, Route,count, i, j, k, a, b, c, Day_Count, Week_Count;

	ifstream fin("Input.txt");
	fin >> Planes;
	PLANES = new string[Planes];

	for (int i = 0; i < Planes; i++)
		fin >> PLANES[i];

	fin >> Runways;
	fin >> Pilots;
	fin >> Routes;
	fin >> Days;
	fin >> Slots;
	
	Schedule = new Flight**[Days];

	for (i = 0; i < Days; i++){
		Schedule[i] = new Flight*[Runways];
		for ( j = 0; j < Runways; j++)
			Schedule[i][j] = new Flight[Slots];
	}

	for (i = 0; i < Days; i++){
		for (j = 0; j < Runways; j++){
			for ( k = 0; k < Slots; k++){
				Schedule[i][j][k].Aircraft ="X";
				Schedule[i][j][k].Pilot = -1;
				Schedule[i][j][k].Route = -1;
				Schedule[i][j][k].Conflicts = 0;
			}
		}
	}				
	
	z = (Routes/Days)+(Routes%Days);
	Route = 1;
	for (i = 0; i < Days; i++){
		for (j = 0; j < z;){

			mt19937 gen(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
			uniform_int_distribution<> RD(0, Runways-1);
			R = RD(gen);
			
			uniform_int_distribution<> SD(0, Slots-1);
			S = SD(gen);
					
			if (Schedule[i][R][S].Pilot == -1){
				Schedule[i][R][S].Aircraft = PLANES[rand() % Planes];
				Schedule[i][R][S].Pilot = rand() % Pilots + 1;
				Schedule[i][R][S].Route = Route++;
				Schedule[i][R][S].Conflicts = 0;
				j++;
			}
		}
		z = (Routes / Days);
	}				
	
	ofstream fout("Genetic Algorithm Result.txt");
	
	fout << "\t\t\t***** ACTUAL SCHEDULE *****\n\n\n";
	PRINT(Schedule, Days, Runways, Slots,fout);
	CHECK_FITNESS(Schedule, Days, Runways, Slots);

	count = 1;
	while (CHECK_CONFLICTS(Schedule, Days, Runways, Slots)!=0){
		
		CROSS_OVER(Schedule, Days, Runways, Slots);
		if (++count >= 3){
			count = 0;
			MUTATION(Schedule, Days, Runways, Slots, Pilots);
		}
		CHECK_FITNESS(Schedule, Days, Runways, Slots);
	}
		
		
	fout << "\n\n\n\n\n\n\t\t***** SCHEDULE SOLVED BY GENETIC ALGORITHM *****\n\n\n";
	PRINT(Schedule, Days, Runways, Slots,fout);

	system("pause");
	return 0;
}