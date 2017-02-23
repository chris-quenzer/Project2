#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <algorithm>

#include <vector>

#define ALL_FILES 0
#define DEBUG 1 //turn this on for !ALL_FILES

enum letter_index 
{ 
	DELETE = 1, 
	A = 2, 
	T = 3, 
	G = 4, 
	C = 5 
};

using namespace std;

int edit_dist(string &str1, string &str2, int m, int n, char **cost_arr_in);
void align(string &str1, string &str2, int m, int n, vector<vector<int>> table, int cost);
int det_cost(char comp1, char comp2, char **cost_arr_in);
void create_cost_arr(char **&cost_arr_in);
int get_min_cost(int x, int y, int z);

int edit_dist(string &str1, string &str2, int m, int n, char **cost_arr_in)
{
	int output = -1;
	vector<vector<int>> table;

	if (m > n)
	{
		string str1_temp = str1;
		string str2_temp = str2;
		int m_temp = m;
		int n_temp = n;

		str1 = str2_temp;
		str2 = str1_temp;
		m = n_temp;
		n = m_temp;
	}

	// Create table to hold subproblems
	if (m <= n)
	{
		table.resize(m + 1, vector<int>(n + 1));
	}
	else
	{
		table.resize(n + 1, vector<int>(m + 1));
	}

	str1 = '-' + str1;
	str2 = '-' + str2;

	//Base cases
	table[0][0] = 0;
	for (int i = 1; i < m + 1; i++)
	{
		table[i][0] = table[i - 1][0] + det_cost(str1[i], '-', cost_arr_in);
	}
	for (int j = 1; j < n + 1; j++)
	{
		table[0][j] = table[0][j - 1] + det_cost(str2[j], '-', cost_arr_in);
	}

	for (int i = 1; i < m + 1; i++)
	{
		for (int j = 1; j < n + 1; j++)
		{
			if (str1[i] == str2[j])
			{
				table[i][j] = table[i - 1][j - 1];
				output = table[i][j];
			}
			else
			{
				table[i][j] = get_min_cost(table[i - 1][j]/* + det_cost(str1[i], '-', cost_arr_in)*/, //Delete
											table[i][j - 1]/* + det_cost(str2[j], '-', cost_arr_in)*/, //Insert
											table[i - 1][j - 1] + det_cost(str1[i], str2[j], cost_arr_in)); //Align
				output = table[i][j];
			}

		}
	}

	align(str1, str2, m, n, table, output);
	
	return output;
}

void align(string &str1, string &str2, int m, int n, vector<vector<int>> table, int cost)
{
	string str1_new = "", str2_new = "";
	int i, j, min_cost;

	if (m > n)
	{
		string str1_temp = str1;
		string str2_temp = str2;
		int m_temp = m;
		int n_temp = n;

		str1 = str2_temp;
		str2 = str1_temp;
		m = n_temp;
		n = m_temp;
	}

	i = 1;
	j = 1;

	while (i < m && j < n)
	{
		min_cost = get_min_cost(table[i + 1][j], table[i + 1][j + 1], table[i][j + 1]);

		if (min_cost == table[i + 1][j]) //Delete
		{
			str1_new = str1_new + "-";
			str2_new = str2_new + str2[j];
			i++;
		}

		else if (min_cost == table[i + 1][j + 1]) //Align
		{
			str1_new = str1_new + str1[i];
			str2_new = str2_new + str2[j];
			i++;
			j++;
		}

		else if (min_cost == table[i][j + 1]) //Insert
		{
			str1_new = str1_new + str1[i];
			str2_new = str2_new + "-";
			j++;
		}

		//i--;
		//j--;
	}
	/*if (j < n)
	{
		str1_new = str1.substr(0, j) + str1_new;

		for (int k = 0; k < j; k++)
		{
			str2_new = "-" + str2_new;
		}

		str2_new = str2_new + str2_new;
	}
	
	if (i < m)
	{
		for (int k = 0; k < i; k++)
		{
			str1_new = "-" + str1_new;
		}

		str1_new = str1_new + str1_new;

		str2_new = str2.substr(0, i) + str2_new;
	}*/
	
	if (DEBUG)//#####################################################################################
	{
		ofstream outfile;
		outfile.open("Chris_Table.txt");

		outfile << "Input:" << endl;
		outfile << str1 << endl;
		outfile << str2 << endl << endl;

		outfile << "Str 1 Len: " << str1.length() << endl;
		outfile << "Str 2 Len: " << str2.length() << endl << endl;

		outfile << "Output: (Alignment is still wrong)" << endl;
		outfile << str1_new << "," << endl << str2_new << ":" << cost << endl << endl;

		for (int i = 0; i < m + 1; i++)
		{
			if (i == 0)
			{
				outfile << "\t ";
				for (int j = 0; j < n + 1; j++)
				{
					outfile << setw(5) << j + 1;
				}
				outfile << endl;
				outfile << "\t ";
				for (int j = 0; j < n + 1; j++)
				{
					outfile << setw(5) << "_____";
				}
				outfile << endl;
			}
			
			outfile << setw(4) << i + 1 << " [";
			for (int j = 0; j < n + 1; j++)
			{
				outfile << setw(4) << table[i][j];
				if (j < (n))
				{
					outfile << ",";
				}
			}
			outfile << "]";
			outfile << endl;
		}
		outfile << endl;
		outfile.close();

		//ofstream outfile;
		outfile.open("example.txt");
		outfile << str1_new << "," << str2_new << ":" << cost << endl;
		outfile.close();
		
		cout << str1_new << "," << endl;
		cout << str2_new << ":" << cost << endl;
	}//###################################################################################################

	if (m > n)
	{
		string str1_temp = str1;
		string str2_temp = str2;
		str1 = str2_temp;
		str2 = str1_temp;
	}
	else
	{
		str1 = str1_new;
		str2 = str2_new;
	}
}

int det_cost(char comp1, char comp2, char **cost_arr_in)
{
	letter_index comp1_ind;
	letter_index comp2_ind;
	int cost = -1;
	
	switch (comp1)
	{
		case '-':
			comp1_ind = DELETE;
			break;
		case 'A':
			comp1_ind = A;
			break;
		case 'T':
			comp1_ind = T;
			break;
		case 'G':
			comp1_ind = G;
			break;
		case 'C':
			comp1_ind = C;
			break;
	}
	switch (comp2)
	{
		case '-':
			comp2_ind = DELETE;
			break;
		case 'A':
			comp2_ind = A;
			break;
		case 'T':
			comp2_ind = T;
			break;
		case 'G':
			comp2_ind = G;
			break;
		case 'C':
			comp2_ind = C;
			break;
	}

	cost = cost_arr_in[comp1_ind][comp2_ind] - '0'; //convert to correct int from ASCII number
	
	return cost;
}

void create_cost_arr(char **&cost_arr_in)
{
	string line;
	ifstream file;
	int count = 0;

	file.open("imp2cost.txt");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			if (DEBUG && !ALL_FILES)//#########################
			{
				cout << line << endl;
			}//##################################
				
			//strip commas
			for (int i = 0; i < 6; i++)
			{
				if (line[i] == ',')
				{
					line.erase(i,1);
				}
			}
			//assign to 2D array by row
			for (int i = 0; i < 6; i++)
			{
				cost_arr_in[count][i] = line[i];
			}
			count++;
		}
		cout << endl;
		
		file.close();
	}

	else cout << "Unable to open file";

	file.close();
}

int get_min_cost(int x, int y, int z)
{
	return min(min(x, y), z);
}

int main()
{
	
	if (ALL_FILES)
	{
		string line;
		ifstream file;
		ofstream outfile;

		int count = 0;

		char **cost_arr = new char *[6];
		for (int i = 0; i < 7; i++)
		{
			cost_arr[i] = new char[6];
		}

		file.open("imp2input.txt");
		outfile.open("imp2output.txt");
		if (file.is_open() && outfile.is_open())
		{
			while (getline(file, line))
			{
				int delim = line.find(',');
				string str1 = line.substr(0, delim);
				string str2 = line.substr(delim + 1, line.length());

				int cost = 0;

				int m = str1.length(), n = str2.length();
			
				create_cost_arr(cost_arr);
				cost = edit_dist(str1, str2, m, n, cost_arr);

				outfile << str1 << "," << str2 << ":" << cost << endl;
				outfile.flush();
			}
			file.close();
			outfile.close();
			
		}
		else cout << "Unable to open files";

		
	}
	else
	{
		string str1 = "CCACTGACATAAGCCTCTGCGTTC";
		string str2 = "TCATGACATGTGCCGCTCACCGTGACTATAGACTCTCAGCAATGGTTAAAGAATCAGCCCGCCAATAGGGGCGGCAAATAATACCGCTGC";

		int cost = 0;

		char **cost_arr = new char *[6];
		for (int i = 0; i < 7; i++)
		{
			cost_arr[i] = new char[6];
		}

		int m = str1.length(), n = str2.length();

		create_cost_arr(cost_arr);

		if (DEBUG)//#########################
		{
			cout << "Parsed array:\n";
			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					cout << cost_arr[i][j];
				}
				cout << endl;
			}
			cout << endl;
		}//#################################

		cost = edit_dist(str1, str2, m, n, cost_arr);

		if (DEBUG)//#################################
		{
			cout << endl << "Cost: " << cost << endl << endl;
		}//##########################################
	}

	return 0;
}