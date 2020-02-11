#include "System.h"
#include "MatrixDisplay.h"

int main(void)
{
	System::Init();

	MatrixDisplay* matrix = MatrixDisplay::GetInstance();

	matrix->DisplayInt(34);

	while (1)
	{
		matrix->Refresh();
	}

	return 0;
}
