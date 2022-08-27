// See GPL-v3.0 LICENSE in GitHub repo.

#include "MainHUD.h"
#include "ToolsFunctionLibrary.h"

void AMainHUD::DrawHUD()
{
	Super::DrawHUD();

	FString PDBFilename{ "Methane.pdb" };

	// draw (write) PDB filename
	DrawText(FileNameDisplay + PDBFilename, FLinearColor::Black, HorizontalDeviation, TopDeviation, HudFont);
}
