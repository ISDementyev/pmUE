// See GPL-v3.0 LICENSE in GitHub repo.

#include "MainHUD.h"
#include "ToolsFunctionLibrary.h"
#include "FileData.h"

void AMainHUD::DrawHUD()
{
	Super::DrawHUD();

	FileData FD;

	// draw (write) PDB filename
	DrawText(FileNameDisplay + FD.FileName, FLinearColor::Black, HorizontalDeviation, TopDeviation, HudFont);
}
