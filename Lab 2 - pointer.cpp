#include <iostream>
#include <iomanip>
using namespace std;

void selectionSort(int * const, const int);
void swap(int * const, int * const);

int main()
{
        const int arraySize = 10;
        int a[ arraySize ] = { 2, 6, 4, 8, 10, 12, 89, 68, 45, 37 };

        cout << "Data items in original order\n";

        for( int i = 0; i < arraySize; i++)
                cout << setw( 4 ) << a[ i ];

        selectionSort( a, arraySize);

        cout << "\nData items in ascending order\n";

        for( int j = 0; j < arraySize; j++)
                cout << setw( 4 ) << a[j];

        cout << endl;
}

void selectionSort( int * const array, const int size)
{
        int *smallest;


        for( int *i = array; i != (array + size); i++) {
                smallest = i;

                for( int *index = i + 1; index != (array + size); index++)
                        if (*index < *smallest)
                                smallest = index;

                swap(i, smallest);
        }
}

void swap( int * const element1Ptr, int * const element2Ptr )
{
        int hold = *element1Ptr;
        *element1Ptr = *element2Ptr;
        *element2Ptr = hold;
}
