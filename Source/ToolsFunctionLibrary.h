// See GPL-v3.0 LICENSE in GitHub repo.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolsFunctionLibrary.generated.h"

/**
 * Tools blueprint function library
 */
UCLASS()
class PMUEINTEGRATION_API UToolsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		UFUNCTION(BlueprintCallable)
		static int32 NumberOfAtoms(FString Filename, bool OnlyAtom = true);

		UFUNCTION(BlueprintCallable)	
		static TSet<FString> GetUniqueElements(FString Filename);

		UFUNCTION(BlueprintCallable)
		FVector GetElementColourRGB(FString AtomSymbol);

		UFUNCTION(BlueprintCallable)
		double GetVDWRadius(FString AtomSymbol);
};
