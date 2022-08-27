// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * A HUD Class
 */
UCLASS()
class PMUEDEVELOPMENTTWO_API AMainHUD : public AHUD
{
	GENERATED_BODY()

private:
	const int HorizontalDeviation{70};
	const int TopDeviation{60};
	const FString FileNameDisplay{ "Filename: " };

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,
		Category = HUDFont)
		UFont* HudFont;

	/**
	 * Draws the HUD
	*/
	virtual void DrawHUD() override;
};
