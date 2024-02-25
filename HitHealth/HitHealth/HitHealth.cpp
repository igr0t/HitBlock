#include <iostream>
#include <windows.h>

using std::cout;
using std::endl;

int main()
{
	int health = 100;
	cout << "Player esta com 100 de life , Aperte J para dar um hit de 5 de dano!\n";

	while (1)
	{
		if (GetAsyncKeyState('J') & 0x0001)
		{
			health = health - 5;
			cout << "life " << health << endl;
		}
		if (health <= 0) {
			cout << "O player foi de F" << endl;
			return 0;
		}
	}




	return 0;
}