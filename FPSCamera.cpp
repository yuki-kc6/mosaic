#include "FPSCamera.h"
#include "Engine/Input.h"

FPSCamera::FPSCamera()
{
	centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
	centerY = GetSystemMetrics(SM_CYSCREEN) / 2;
	ShowCursor(FALSE);
}

FPSCamera::~FPSCamera()
{
}

void FPSCamera::Update()
{
	currentMousePos=Input::GetMousePosition();

	DeltaX = currentMousePos.x - centerX;
	DeltaY = currentMousePos.y - centerY;


	SetCursorPos(centerX, centerY);


}
